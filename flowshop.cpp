#include<bits/stdc++.h>

#define INF 999999

typedef struct job {
    int d1; //duração da tarefa na máquina 1
    int d2; // ----------------------------2
    // bool inM;
} job;

typedef struct node {
    vector<int> jobsinM;
    int classif; //valor que a função classificadora dá para o nó (lim. dual)

} node;



int bnb(vector<job> jobs, int n) {

    //temos duas divisões em conjuntos:
    /*nós ativos e não ativos (nós ativos são nós
     para os quais calculamos a função classificadora)*/
    //jobs em M e jobs fora de M (em M já decididos, fora ainda a explorar)

    while (!active_nodes.empty()) {
        int an_size = active_nodes.size();
        for (int i = 0; i < an_size; i++) {
            
        }
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
        initial.jobsinM.push_back(i);
    }
    initial.classif = -INF; //não faz sentido aplicar a função classificadora para o primeiro nó

    /*a gente tem que implementar um algoritmo bruteforce que faz bfs no
    espaço de busca e faz bound pra melhor opção dada a função classificadora*/
    int sft;
    sft = bnb(jobs, n, active_nodes);
    cout << sft << '\n';

    return 0;
}
