#include "global.hpp"
#include "load.hpp"
#include "semanticsimilarity.hpp"

#include "time.h"

int main(int argc, char* argv[]) {
    try {
        Ultility::update_working_dir(argv[0]);
    } catch (...) {
        cerr << "Critical error! Unexpected, uncatched problem while trying to update working directory!\n";
    }

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
    // ------------------------------------------- demonstration -------------------------------------------
    // SemanticSimilarity ss ("RAM keeps things being worked with.", "The CPU uses RAM as a short-term memory store.");
    // SemanticSimilarity ss ("Straws will be made from cardboard from now on.", "To reduce the amount of plastic, straws are banned.");
    // SemanticSimilarity ss ("Mango trees are planted near school.", "Flag was raised on this island long ago.");
    // SemanticSimilarity ss ("It is a dog", "It is a log");
    SemanticSimilarity ss ("Dog is a man's best friend", "What's this?");
    
    cout << ss.compute_semantic_similarity();
    cout << endl;
    ss.debug();

    // cout << endl;
    // cout << endl;

    // cout << endl;
    // clock_t start = clock();
    // cout << ss2.compute_semantic_similarity();

    // cout << "Time = " << (clock() - start) * 1.0 / CLOCKS_PER_SEC << endl;
    // ss2.debug();

    // ofstream F ("Board.txt", ios::out);
    // std::vector< std::vector<std::string> > board ( ss2.ws1.size(), std::vector<std::string> (ss2.JOINT.size()) );
    // for( unsigned i = 1; i < ss2.JOINT.size(); i++ ) {
    //     board[0][i] = ss2.JOINT[i];
    // }
    // for( unsigned i = 1; i < ss2.ws1.size(); i++ ) {
    //     board[i][0]    = ss2.ws1[i];
    // }
    // for( unsigned i = 1; i < ss2.ws1.size(); i++ ) {
    //     for( unsigned j = 1; j < ss2.JOINT.size(); j++ ) {
    //         board[i][j] = std::to_string( ss2.similarity_2_word(ss2.ws1[i], ss2.JOINT[j]) );
    //     }
    // }

    //
    // board.push_back(std::vector<std::string> (ss2.JOINT.size()));
    // board.back()[0] = "$MAX";
    // for(unsigned i = 1; i < ss2.JOINT.size(); i++) {
    //     for(unsigned j = 1; j < ss2.ws1.size(); j++) {
    //         board.back()[i] = std::max(board.back()[i], board[j][i]);
    //     }
    // }
    // // ---
    // for( unsigned i = 0; i < ss2.ws1.size() + 1; i++ ) {
    //     for( unsigned j = 0; j < ss2.JOINT.size(); j++ ) {
    //         F << std::string( board[i][j] + std::string(10 - board[i][j].length(), ' ') ) << " ";
    //     }
    //     F << endl;
    // }
    // F.close();
}