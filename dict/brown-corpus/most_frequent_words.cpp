#include <bits/stdc++.h>
using namespace std;

int main() {
    ifstream F ("words_freq.txt", ios::in);
    string S;
    //
    getline(F, S);

    //
    vector< pair<int, string > > V;
    int n;
    string s;
    // --
    for(int i = 0; i < 46758 ; i++) {
        F >> s >> n;
        V.push_back( {n, s} );
    }
    // --
    sort(V.begin(), V.end(), greater< pair<int, string> >() );
    // --
    ofstream f ("words_popular.txt", ios::out);
    for(auto P : V) {
        f << P.second << '\n';
    }
    //
}