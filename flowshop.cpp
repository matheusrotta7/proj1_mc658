#include<bits/stdc++.h>

using namespace std;

/* https://hal.archives-ouvertes.fr/hal-00680452v1/document
 *
 *
 * */

//#define INF numeric_limits<float>::infinity();
#define INF numeric_limits<int>::max();

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


/* S1 = sum of (jobs in jobs_in_m of node) 
 *
 * */
int s1(&node n, vector<job> jobs) {

}

int s2(&node n, vector<job> jobs) {

}

int calc_bound(&node n, vector<job> jobs) {
    return n.classif = max(s1(n, jobs), s2(n, jobs));
}

long long int branch (&vector<node> active_nodes, int min_pos) {
    int num_of_jobs = active_nodes[min_pos].jobs_in_m.size();
    if (num_of_jobs == 0) {
        long long int sum = 0;

        return 
        for(vector<int>::iterator it = vector.begin(); it != vector.end(); ++it)
            sum += *it;

        return sum;
        
    }
    else {

        for (int i = 0; i < num_of_jobs; i++) {
            int cur_job;
            cur_job = active_nodes[min_pos].jobs_in_m[i];
            node new_n;
            for (int j = 0; j < num_of_jobs; j++) {
                if (j != cur_job) {
                    new_n.jobs_in_m.push_back(j);
                }
            }

            int num_of_chosen = active_nodes[min_pos].chosen_jobs.size();
            for(int j = 0; j < num_of_chosen; j++) {
                new_n.chosen_jobs.push_back(j); 
            } 
           
            new_n.chosen_jobs.push_back(cur_job); 
            new_n.classif = calc_bound(new_n, jobs);
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

        limitante_primal = min(limitante_primal, branch (active_nodes, min_pos));
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

    vector<node> active_nodes;
    node initial;
    for (int i = 0; i < n; i++) {
        initial.jobs_in_m.push_back(i);
    }
    initial.classif = calc_bound(initial, jobs);
    active_nodes.push_back(initial);
    /*a gente tem que implementar um algoritmo bruteforce que faz bfs no
    espaço de busca e faz bound pra melhor opção dada a função classificadora*/
    long long int sft;
    /*
 *      sft := sum of finishing times (m2)
 * */
    sft = bnb(jobs, n, active_nodes);
    cout << sft << '\n';

    return 0;
}
