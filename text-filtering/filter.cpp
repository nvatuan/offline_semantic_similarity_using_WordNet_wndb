#include <bits/stdc++.h>
using namespace std;

#include "..\global.hpp"
// #include "..\namespaces.hpp"

double how_often(std::string w) {
    return (BrownCorpus::freq(w) * 1.0 / BrownCorpus::TOTAL_WORDS);
}

int main(int argc, char* argv[]) {
    Ultility::update_working_dir(argv[0]);

    ifstream pure ("list.txt", ios::in);
    vector< pair<double, string> > V;
    set< vector<string> > SSS;
    // --
    string S;
    while( !pure.eof() ) {
        getline(pure, S);

        
        S = string(S.begin(), S.begin() + (S.find(" - ") == string::npos ? S.length() : S.find(" - ")));

        auto SV = Ultility::string_to_list_of_words(S);
        if(SV.size() == 0) continue;
        if(SSS.find(SV) == SSS.end()) {
            SSS.insert(SV);
        } else continue;

        V.push_back( {0.0, S} );

        for(string& st : SV) {
            V.back().first += how_often(st);
        }
        V.back().first /= SV.size();

        cout << V.size() << "\n";
        cout << S << "\n";
        S = "";
    }
    pure.close();
    // --
    sort(V.begin(), V.end(), greater< pair<double, string> >() );
    // --
    ofstream filtered ("list_filtered.txt", ios::out);
    const int Take = V.size();

    for(int i = 0; i < Take; i++) {
        filtered << V[i].second << "\n";
    }
    filtered.close();
}