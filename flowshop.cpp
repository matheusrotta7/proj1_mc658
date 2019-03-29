#include<bits/stdc++.h>

typedef struct job {
    int d1; //duração da tarefa na máquina 1
    int d2; // ----------------------------2
} job;

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

    return 0;
}
