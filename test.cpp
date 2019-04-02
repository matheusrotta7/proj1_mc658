#include<bits/stdc++.h>

using namespace std;

int main() {

    vector<int> first;
    for (int i = 0; i < 10; i++) {
        first.push_back(i);
    }

    vector<int> second;
    first.erase( first.end()-4);
    second = first;

    for (int i = 0; i < second.size(); i++) {
        cout << i << ' ';
    }
    cout << '\n';

    return 0;
}
