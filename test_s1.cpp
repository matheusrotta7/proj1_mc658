#include <bits/stdc++.h>

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

struct Comp {
    Comp(vector<job> paramA) { this->paramA = paramA; }
    bool operator () (int i, int j) {

        return paramA[i].d1 < paramA[j].d1;

    }

    vector<job> paramA;
};

// sort(v.begin(), v.end(), Comp(paramA));

// bool comp_1 (int a, int b, vector<job> jobs) {
//
// }


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

    return sum;
}

int main () {

    int n; //número de tarefas
    cin >> n;

    vector<job> jobs(n);

    for (int i = 0; i < n; i++) {
        int a, b;
        cin >> a >> b;
        jobs[i].d1 = a;
        jobs[i].d2 = b;
    }

    // vector<node> active_nodes;

    node test;
    test.jobs_in_m.push_back(1);
    test.jobs_in_m.push_back(0);
    test.chosen_jobs.push_back(2);
    cout << "s1 heuristic returns: " << s1(test, jobs, n) << '\n';
    /*a gente tem que implementar um algoritmo bruteforce que faz bfs no
    espaço de busca e faz bound pra melhor opção dada a função classificadora*/
    // long long int sft; //sft := sum of finishing times (m2)



    // sft = bnb(jobs, n, active_nodes);
    // cout << sft << '\n';




    return 0;
}
