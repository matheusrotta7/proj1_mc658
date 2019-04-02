#include<bits/stdc++.h>

using namespace std;

#define INF 999999

typedef struct job {
    int d1; //duração da tarefa na máquina 1
    int d2; // ----------------------------2
    // bool inM;
} job;

typedef struct node {
    vector<int> jobs_in_m;
    vector<int> chosen_ones;
    int classif; //valor que a função classificadora dá para o nó (lim. dual)

} node;

int calc_bound(&node n, vector<job> jobs) {
    return n.classif = max(s1(n, jobs), s2(n, jobs));
}

void branch (&vector<node> active_nodes, int min_pos) {
    int num_of_jobs = active_nodes[min_pos].jobs_in_m.size();
    if (num_of_jobs == 0) {
        //we reached a leaf
        //calculate real value of flowshop
        real_value(----)
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

            new_n.classif = calc_bound(new_n, jobs);
            active_nodes.push_back(new_n);

            // int num_total_jobs = jobs.size();
            // for(int i = 0; i < num_total_jobs; i++) {
            //     if(jobs[i] not in new_n.jobs_in_m) {
            //         new_n.chosen_ones.push_back(jobs[i]);
            //     }
            // }
        }


    }

}


int bnb(vector<job> jobs, int n, vector<node> active_nodes) {

    //temos duas divisões em conjuntos:
    /*nós ativos e não ativos (nós ativos são nós
     para os quais calculamos a função classificadora)*/
    //jobs em M e jobs fora de M (em M já decididos, fora ainda a explorar)

    while (!active_nodes.empty()) {
        int an_size = active_nodes.size();
        int min_classif = +INF;
        int min_pos = -1; //because we believe that +INF is large enough, so min_pos will be overwritten
        for (int i = 0; i < an_size; i++) { //theta(an)
            if (min_classif > active_nodes[i].classif) {
                min_classif = active_nodes[i].classif;
                min_pos = i;
            }
        }

        branch (active_nodes, min_pos);
    }



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
    initial.classif = calc_bound(initial, jobs); //não faz sentido aplicar a função classificadora para o primeiro nó
    active_nodes.push_back(initial);
    /*a gente tem que implementar um algoritmo bruteforce que faz bfs no
    espaço de busca e faz bound pra melhor opção dada a função classificadora*/
    int sft;
    sft = bnb(jobs, n, active_nodes);
    cout << sft << '\n';

    return 0;
}
