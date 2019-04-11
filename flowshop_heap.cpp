#include<bits/stdc++.h>

#define INF numeric_limits<int>::max();

using namespace std;

int ac_nodes_size = 0;

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
long long int bnb(vector<job> jobs, int n, vector<node> active_nodes);

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
     cout << "s2 heuristic returns " << sum << '\n';
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
      cout << "s1 heuristic returns " << sum << '\n';

      return sum;
  }

/*int s2(node &n, vector<job> &jobs, int n) {

}*/

int calc_bound(node &cur_node, vector<job> jobs, int n) {
    int chosen_acc = calc_end_time_m2(jobs, cur_node.chosen_jobs);
    return cur_node.classif = chosen_acc + max(s1(cur_node, jobs, n), s2(cur_node, jobs, n));

    /*
 *    SO FAR WE DO NOT HAVE S2
 * */
    //return cur_node.classif = chosen_acc + max(s1(cur_node, jobs), s2(cur_node, jobs));
}

long long int calc_end_time_m2(vector<job> &jobs, vector<int> &chosen_jobs) {

    /* vector containing endtime for machine 1 (m1) */
    vector<int>  end_time_1;

    /* adding dummy node to calculate end_time for m1 */
    end_time_1.push_back(0);

    /* vector containing endtime for machine 2 (m2) */
    vector<int>  end_time_2;

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
        long long int sum = 0;

        //calcular para a folha
        //(já está tudo definido)
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
        cout << "FOLHAAAAA\n";
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

long long int bnb(vector<job> jobs, int n, vector<node> active_nodes) {

    //temos duas divisões em conjuntos:
    /*nós ativos e não ativos (nós ativos são nós
     para os quais calculamos a função classificadora)*/
    //jobs em M e jobs fora de M (em M já decididos, fora ainda a explorar)

    long long int limitante_primal = INF;

    /*  TODO: esse while pode ter outra condição ne? a gente nao precisa ficar abrindo
     *  mais nos depois que limitante_primal = limitante_dual
     *  TODO: a gente ta deixando de adicionar nos por limitantes, otimalidade?
     * */
    while (!active_nodes.empty()) {
        int an_size = active_nodes.size();
        int min_classif = INF;
        int min_pos = -1; //because we believe that +INF is large enough, so min_pos will be overwritten
        for (int i = 0; i < an_size; i++) { //theta(an)
            if (min_classif > active_nodes[i].classif) {
                min_classif = active_nodes[i].classif;
                min_pos = i;
            }
        }

        long long int old_limitante_primal = limitante_primal;
        limitante_primal = min(limitante_primal, branch(limitante_primal, active_nodes, min_pos, n, jobs));
        active_nodes.erase(active_nodes.begin() + min_pos);

        /* Check if limitante_primal was updated by above min() */
        /*
 *         TODO: the below procedure gets REALLY faster using PriorityQueue
 *      */
        if (old_limitante_primal != limitante_primal) {
            an_size -= 1;
            for(int i = 0; i < an_size; i++) {
                if(limitante_primal <= active_nodes[i].classif) {
                    active_nodes.erase(active_nodes.begin() + i);
                }
            }

        }
    }

    return limitante_primal;
}

int main() {

    int n; //número de tarefas
    cin >> n;

    vector<job> jobs(n);

    for (int i = 0; i < n; i++) {
        int a, b;
        cin >> a >> b;
        jobs[i].d1 = a;
        jobs[i].d2 = b;
    }

    vector<node> active_nodes; //size = 0;
    priority_queue <pair<int, int>, vector<pair<int,int>>, greater<pair<int, int>>> pq;
    node initial;
    for (int i = 0; i < n; i++) {
        initial.jobs_in_m.push_back(i);
    }
    initial.classif = calc_bound(initial, jobs, n);
    pq.push(pair<int,int>(initial.classif, ac_nodes_size++));
    active_nodes.push_back(initial);
    /*a gente tem que implementar um algoritmo bruteforce que faz bfs no
    espaço de busca e faz bound pra melhor opção dada a função classificadora*/
    long long int sft; //sft := sum of finishing times (m2)

    sft = bnb(jobs, n, active_nodes);
    cout << sft << '\n';

    return 0;
}
