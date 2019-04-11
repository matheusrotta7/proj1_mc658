#include<bits/stdc++.h>

#define INF numeric_limits<int>::max();

using namespace std;

typedef struct job {
    int d1; //duração da tarefa na máquina 1
    int d2; // ----------------------------2
    // bool inM;
} job;

typedef struct node {
    vector<int> jobs_in_m;
    vector<int> chosen_jobs;
    int classif; //valor que a função classificadora dá para o nó (lim. dual)

} node;


typedef struct answer {
    long long int limitante_primal;
    long long int limitante_dual;
    long long int nodes_explored;
    float tempo_primal;
    float tempo_dual;
    float tempo_total;
} answer;

struct Comp {
    Comp(vector<job> paramA) { this->paramA = paramA; }
    bool operator () (int i, int j) {

        return paramA[i].d1 < paramA[j].d1;

    }

    vector<job> paramA;
};

struct Comp2 {
    Comp2(vector<job> paramA) { this->paramA = paramA; }
    bool operator () (int i, int j) {

        return paramA[i].d2 < paramA[j].d2;

    }

    vector<job> paramA;
};


int s1(node &cur_node, vector<job> &jobs, int n);
int calc_bound(node &cur_node, vector<job> jobs, int n);
long long int calc_end_time_m2(vector<job> &jobs, vector<int> &chosen_jobs);
long long int branch(long long int limitante_primal, vector<node> &active_nodes, int min_pos, int n, vector<job> jobs);
answer bnb(vector<job> jobs, int n, vector<node> active_nodes, long long int max_nodes, long long int max_time);

/* https://hal.archives-ouvertes.fr/hal-00680452v1/document
 *
 *
 * */

/* relaxação 1: overlapping permitido na máquina 2
 * cada job começa logo após o seu término na máquina 1
 * */
int s2(node &cur_node, vector<job> &jobs, int n) {

    //ordenar duração de r+1 até n dos jobs em M1 em ordem crescente
    int f1tr = 0; //f1tr: fim de tr na máquina 1 (otimizar)

    int cj_size = cur_node.chosen_jobs.size();
    for (int i = 0; i < cj_size; i++) {
        int cur_job = cur_node.chosen_jobs[i];
        f1tr += jobs[cur_job].d1; //vai somando as durações dos jobs na máquina 1 na ordem dada por chosen_jobs, isso é o próprio f1tr
    }

    // vector<int> aux = cur_node.jobs_in_m
    sort(cur_node.jobs_in_m.begin(), cur_node.jobs_in_m.end(), Comp2(jobs)); //increasing order of job duration on machine 2
    int m_size = cur_node.jobs_in_m.size();
    int sum = 0;
    // cout << cj_size << "\n\n\n";
    int r;
    if (cj_size != 0)
       r = cur_node.chosen_jobs.back();
    else
       r = 0;
    int f2tr = f1tr + jobs[r].d2;
    int min1 = INF;
    int min_index;
    for (int i = 0; i < m_size; i++) {
        int cur_job = cur_node.jobs_in_m[i];
        if (min1 > jobs[cur_job].d1) {
            min1 = jobs[cur_job].d1;
            min_index = i;
        }
    }
    int special_value = max(f2tr, f1tr + min1);
    for (int j = 0, i = cj_size+1; j < m_size; i++, j++) {
       int cur_job = cur_node.jobs_in_m[j];
       sum += special_value;
       sum += (n-i+1)*jobs[cur_job].d2; //n foi passado como parâmetro
    }
    // cout << "s2 heuristic returns " << sum << '\n';
    return sum;
}


/* relaxação 1: overlapping permitido na máquina 2
* cada job começa logo após o seu término na máquina 1
* */
int s1(node &cur_node, vector<job> &jobs, int n) {

  //ordenar duração de r+1 até n dos jobs em M1 em ordem crescente
  int f1tr = 0; //f1tr: fim de tr na máquina 1 (otimizar)

  int cj_size = cur_node.chosen_jobs.size();
  for (int i = 0; i < cj_size; i++) {
      int cur_job = cur_node.chosen_jobs[i];
      f1tr += jobs[cur_job].d1; //vai somando as durações dos jobs na máquina 1 na ordem dada por chosen_jobs, isso é o próprio f1tr
  }

  // vector<int> aux = cur_node.jobs_in_m
  sort(cur_node.jobs_in_m.begin(), cur_node.jobs_in_m.end(), Comp(jobs));
  int m_size = cur_node.jobs_in_m.size();
  int sum = 0;
  for (int j = 0, i = cj_size+1; j < m_size; i++, j++) {
      int cur_job = cur_node.jobs_in_m[j];
      sum += f1tr;
      sum += (n-i+1)*jobs[cur_job].d1; //passar n como parâmetro ou deixar global
      sum += jobs[cur_job].d2;
  }
  // cout << "s1 heuristic returns " << sum << '\n';

  return sum;
}

int calc_bound(node &cur_node, vector<job> jobs, int n) {
    int chosen_acc = calc_end_time_m2(jobs, cur_node.chosen_jobs);
    return cur_node.classif = chosen_acc + max(s1(cur_node, jobs, n), s2(cur_node, jobs, n));

}

long long int calc_end_time_m2(vector<job> &jobs, vector<int> &chosen_jobs) {

    /* vector containing endtime for machine 1 (m1) */ 
    vector<int> end_time_1; 

    /* adding dummy node to calculate end_time for m1 */
    end_time_1.push_back(0);
    
    /* vector containing endtime for machine 2 (m2) */ 
    vector<int> end_time_2; 

    /* adding dummy node to calculate end_time for m2 */
    end_time_2.push_back(0);

    /* get number of chosen jobs for a leaf */
    int num_of_jobs = chosen_jobs.size();

    /* create a vector with end times (m1) for each chosen job in its order */
    for(int i = 0; i < num_of_jobs; i++) {
        end_time_1.push_back(jobs[chosen_jobs[i]].d1 + end_time_1.back());
    }

    /* removing dummy node used to easilly calculate above function */
    end_time_1.erase(end_time_1.begin());

    long long int sum = 0;
    for(int i = 0; i < num_of_jobs; i++) {
        if(end_time_1[i] >= end_time_2.back()) {
            /* enter here menas that machine 2 is ready to operate now */
            end_time_2.push_back(end_time_1[i] + jobs[chosen_jobs[i]].d2);
        } else {
            end_time_2.push_back(end_time_2.back() + jobs[chosen_jobs[i]].d2);
        }
        sum += end_time_2.back();
    }

    // This erase is only necessary if you want to check end_time_2 values
    //end_time_2.erase(end_time_2.begin());
    return sum;

}

long long int branch(long long int limitante_primal, vector<node> &active_nodes, int min_pos, int n, vector<job> jobs) {
    int num_of_jobs = active_nodes[min_pos].jobs_in_m.size();
    if (num_of_jobs == 0) {
        /*
 *         TODO: project decision: eliminate all the active nodes with classif
 *         higher than limitante_primal calculated everytime we get here.
 *         Or only remove active nodes when limitante_primal is updated with
 *         a smaller value.
 *
 *         Taken decision: only eliminate active nodes when limitante_primal is
 *         updated, because it means we found a good new value. If it wasnt't
 *         update we may not cut many active nodes and cut nodes that will take
 *         longer to be open (wouldnt even be open maybe).
 *      */
        cout << "Folha\n";
        return calc_end_time_m2(jobs, active_nodes[min_pos].chosen_jobs); 

    }
    else {

        for (int i = 0; i < num_of_jobs; i++) {
            int cur_job;
            cur_job = active_nodes[min_pos].jobs_in_m[i];
            node new_n;
            for (int j = 0; j < num_of_jobs; j++) {
                if (active_nodes[min_pos].jobs_in_m[j] != cur_job) {
                    new_n.jobs_in_m.push_back(active_nodes[min_pos].jobs_in_m[j]);
                }
            }

            int num_of_chosen = active_nodes[min_pos].chosen_jobs.size();
            for(int j = 0; j < num_of_chosen; j++) {
                new_n.chosen_jobs.push_back(active_nodes[min_pos].chosen_jobs[j]);
            }

            new_n.chosen_jobs.push_back(cur_job);
            new_n.classif = calc_bound(new_n, jobs, n);

            /* if we already have something equal or better than this node is
            * promessing, we do not need to consider it */
            /*
 *             TODO: a possible optimization is to find fast a limitante_primal
 *             so whenever you try to open a new node you check if that node
 *             deserves to be inserted. A better optimization is to find fast
 *             a really low limitante_primal to this problem of minimization.
 *      */

            if(new_n.classif >= limitante_primal) {
                continue;
            }

            active_nodes.push_back(new_n);

        }

    }
    return INF;

}

answer bnb(vector<job> jobs, int n, vector<node> active_nodes, long long int max_nodes, long long int max_time) {

    //temos duas divisões em conjuntos:
    /*nós ativos e não ativos (nós ativos são nós
     para os quais calculamos a função classificadora)*/
    //jobs em M e jobs fora de M (em M já decididos, fora ainda a explorar)

    answer ans;

    ans.limitante_primal = INF;
    ans.limitante_dual = INF;
    ans.nodes_explored = 0;
    ans.tempo_primal = 0;
    ans.tempo_dual = 0;
    ans.tempo_total = 0;

    /*  TODO: esse while pode ter outra condição ne? a gente nao precisa ficar abrindo
     *  mais nos depois que limitante_primal = limitante_dual
     * */

    auto start = chrono::steady_clock::now();
    auto end = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

    while (!active_nodes.empty() && ans.nodes_explored < max_nodes) {
        int an_size = active_nodes.size();
        long long int min_classif = INF;
        int min_pos = -1; //because we believe that +INF is large enough, so min_pos will be overwritten
        for (int i = 0; i < an_size; i++) { //theta(an)
            if (min_classif > active_nodes[i].classif) {
                min_classif = active_nodes[i].classif;
                min_pos = i;
            }
        }
    
        /* Check if we got a new lower dual bound  */ 
        if(min_classif < ans.limitante_dual) {
            end = chrono::steady_clock::now();
            duration = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
            ans.tempo_dual = duration; 
            ans.limitante_dual = min_classif;
        }

        /* After some block of instructions check if we exceeded the proposed time limiter */ 
        end = chrono::steady_clock::now();
        duration = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
            if(max_time <= (float)duration/1000000000) {
            /* time limit exceeded */
            ans.tempo_total = duration;
            break;
        }

        ans.nodes_explored += 1;
        long long int old_limitante_primal = ans.limitante_primal;

        /* Check if we got a new lower primal bound  */ 
        long long int branch_return = branch(ans.limitante_primal, active_nodes, min_pos, n, jobs);
        if(branch_return < ans.limitante_primal) {
            end = chrono::steady_clock::now();
            duration = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
            ans.tempo_primal = duration; 
            ans.limitante_primal = branch_return;
        }

        active_nodes.erase(active_nodes.begin() + min_pos);

        /* After some block of instructions check if we exceeded the proposed time limiter */ 
        end = chrono::steady_clock::now();
        duration = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
        if(max_time <= (float)duration/1000000000) {
            /* time limit exceeded */
            ans.tempo_total = duration;
            break;
        }

        if (old_limitante_primal != ans.limitante_primal) {
            an_size -= 1;
            for(int i = 0; i < an_size; i++) {
                if(ans.limitante_primal <= active_nodes[i].classif) {
                    active_nodes.erase(active_nodes.begin() + i);
                }
            }
        }
        
        /* After some block of instructions check if we exceeded the proposed time limiter */ 
        end = chrono::steady_clock::now();
        duration = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
        if(max_time <= (float)duration/1000000000) {
            /* time limit exceeded */
            ans.tempo_total = duration;
            break;
        }
    }

    end = chrono::steady_clock::now();
    ans.tempo_total = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    return ans;
}

int main(int argc, char *argv[]) {

    if (argc < 3) {
        printf("ERROR! Less than 2 parameters were passed to bnb-fs\n");
        return 0;
    }

    ifstream input_file, param_file;
    input_file.open(argv[1]);
    param_file.open(argv[2]);

    long long int max_nodes, max_time;
    
    param_file >> max_nodes;
    param_file >> max_time;
    
    int n; //número de tarefas
    input_file >> n;

    vector<job> jobs(n);

    for (int i = 0; i < n; i++) {
        int a, b;
        input_file >> a >> b;
        jobs[i].d1 = a;
        jobs[i].d2 = b;
    }

    input_file.close();
    param_file.close();

    vector<node> active_nodes;
    node initial;
    for (int i = 0; i < n; i++) {
        initial.jobs_in_m.push_back(i);
    }
    initial.classif = calc_bound(initial, jobs, n);
    active_nodes.push_back(initial);
    /*a gente tem que implementar um algoritmo bruteforce que faz bfs no
    espaço de busca e faz bound pra melhor opção dada a função classificadora*/
    long long int sft; //sft := sum of finishing times (m2)

    answer ans = bnb(jobs, n, active_nodes, max_nodes, max_time);
    cout << argv[1] << ',';
    cout << ans.limitante_primal << ',';
    cout << ans.limitante_dual << ',';
    cout << ans.nodes_explored << ',';
    cout << ans.tempo_primal << ',';  
    cout << ans.tempo_dual << ',';  
    cout << ans.tempo_total << '\n';

    return 0;
}
