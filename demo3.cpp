#include "global.hpp"
#include "load.hpp"
#include "semanticsimilarity.hpp"

#include <algorithm>

#include "time.h"
clock_t now;
void time_since_now() {
    cout << "Time = " << (clock() - now) * 1.0 / CLOCKS_PER_SEC << "s\n";
}

int main(int argc, char* argv[]) {
    try {
        Ultility::update_working_dir(argv[0]);
    } catch (...) {
        cerr << "Critical error! Unexpected, uncatched problem while trying to update working directory!\n";
    }

    now = clock();

    try {
        load();    
    } catch (int e) {
        if(e == -1) {
            cerr << "Cannot load necessary dictionary files, program will terminate.\n";
            return -1;
        }
        if(e == 0) {
            cerr << "Cannot load morphological exceptions files, this leads to unable to process certain morphological exceptions in English language\n";
            cerr << "For example: (bad -> worse) is an exception. (fast -> faster) is not an exception.\n";
            cerr << "However, program can still run, and shall continue to.\n";
        } 
    }
    cout << "LOADED.\n";
    time_since_now();
    now = clock();
    // ------------------------------------------- demonstration -------------------------------------------
    SemanticSimilarity ss ("", "");

    ifstream fw ("\\dict\\brown-corpus\\words_ordered.txt", ios::in);
    const int prerun = 0;

    std::vector<std::string> prerunword (prerun);
    for(int i = 0; i < prerun; i++) {
        fw >> prerunword[i];
    }
    fw.close();

    for(int i = 0; i < prerun; i++) {
        for(int j = i+1; j < prerun; j++) {
            ss.similarity_2_word(prerunword[i], prerunword[j]);
        }
    }
    cout << "Prerun finished.\n";
    // --------------------------------------------------------------------
    time_since_now();
    now = clock();
    int     sentences = 0;
    const int MAX_LINES   = 1000;
    
    ifstream FileIn;
    
    FileIn.open("LIST.txt", ios::in);
    string line;
    while(!FileIn.eof() && sentences <= MAX_LINES) {
        getline(FileIn, line);
        // --
        if(line.length() > 1) {
            sentences++;
        }
    }
    FileIn.close();
    // -----------------------
    while(1) {
        vector< SemanticSimilarity > objects;
        string sentc;

        fflush(stdin);
        cout << "\nSentence = "; getline(cin, sentc);

        now = clock();

        FileIn.open("LIST.txt", ios::in);

        int currentline = 0;
        while(!FileIn.eof() && currentline <= MAX_LINES) {
            cout << "Completed = " << currentline*1.0/sentences << endl;
            getline(FileIn, line);
            // --
            if(line.length() > 1) {
                SemanticSimilarity ss (sentc, line);
                ss.compute_semantic_similarity();
                objects.push_back(ss);
                currentline++;
            }
        }

        std::sort(objects.begin(), objects.end());

        time_since_now();

        cout << "Similar to " << sentc << "\n";
        cout << "Currently showing you top 10:\n";
        for(int i = objects.size() - 1; i >= int(objects.size()) - 10; i-- ) {
            cout << objects[i].ssimilarity << " " << objects[i].sentence2 << '\n';
        }

        ofstream F ("ranking.txt", ios::out);
        F << "Similar to " << sentc << "\n";
        for(int i = objects.size() - 1; i >= 0; i-- ) {
            F << objects[i].ssimilarity << " " << objects[i].sentence2 << '\n';
        }
        // --
        FileIn.close();
        F.close();
    }
    // cout << ss2.compute_semantic_similarity();
    // cout << endl;
    // ss2.debug();
}