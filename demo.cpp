#include "global.hpp"
#include "load.hpp"
#include "SemanticSimilarity.hpp"

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
    
    cout << "LOADED.";
    // ------------------------------------------- demonstration -------------------------------------------

    ofstream F ("wordnet_noun.txt", ios::out);
    for(auto& D : data_noun) {
        F << D.
    }


    SemanticSimilarity ss ("", "", 0.15);
    cout << "\n\nTEST: noun vs noun\n\n";

    cout << "Similarity between `juice' and `cider' is "         << ss.similarity_2_word("juice", "cider") << endl;
    cout << "Similarity between `cider' and `apple' is "         << ss.similarity_2_word("cider", "apple") << endl;
    cout << "Similarity between `marker' and `sticker' is "     << ss.similarity_2_word("marker", "sticker") << endl;
    cout << "Similarity between `potato' and `tomato' is "      << ss.similarity_2_word("potato", "tomato") << endl;
    cout << "Similarity between `wine' and `beer' is "          << ss.similarity_2_word("wine", "beer") << endl;
    cout << "Similarity between `car' and `driver' is "         << ss.similarity_2_word("car", "driver") << endl;
    cout << "Similarity between `man' and `lion' is "           << ss.similarity_2_word("man", "lion") << endl;
    // cout << "\n\nMorph testing\n\n";
    // cout << "Similarity between `markers' and `pens' is "         << ss.similarity_2_word("markers", "pens") << endl;
    // cout << "Similarity between `markers' and `stickers' is "     << ss.similarity_2_word("markers", "stickers") << endl;
    // cout << "Similarity between `potatoes' and `tomatoes' is "    << ss.similarity_2_word("potatoes", "tomatoes") << endl;
    // cout << "Similarity between `wines' and `beers' is "          << ss.similarity_2_word("wines", "beers") << endl;
    // cout << "Similarity between `cars' and `drivers' is "         << ss.similarity_2_word("cars", "drivers") << endl;
    // cout << "Similarity between `mans' and `lions' is "           << ss.similarity_2_word("mans", "lions") << endl;

    // cout << "\n\nTEST: verb vs verb\n\n";

    // cout << "Similarity between `look' and `observe' is "       << ss.similarity_2_word("look", "observe") << endl;
    // cout << "Similarity between `walk' and `teleport' is "      << ss.similarity_2_word("walk", "teleport") << endl;
    // cout << "Similarity between `operate' and `control' is "    << ss.similarity_2_word("operate", "control") << endl;
    // cout << "Similarity between `disgust' and `hate' is "       << ss.similarity_2_word("disgust", "hate") << endl;
    // cout << "Similarity between `fight' and `love' is "         << ss.similarity_2_word("fight", "love") << endl;
    // cout << "\n\nMorph testing\n\n";
    // cout << "Similarity between `looked' and `observe' is "     << ss.similarity_2_word("looked", "observe") << endl;
    // cout << "Similarity between `walked' and `teleport' is "    << ss.similarity_2_word("walked", "teleport") << endl;
    // cout << "Similarity between `operated' and `control' is "   << ss.similarity_2_word("operated", "control") << endl;
    // cout << "Similarity between `disgusting' and `hated' is "   << ss.similarity_2_word("disgusting", "hated") << endl;
    // cout << "Similarity between `fought' and `love' is "        << ss.similarity_2_word("fought", "loving") << endl;

    // cout << "\n\nTEST: adjective vs adjective\n\n";
    // cout << "Similarity between `hot' and `cold' is "           << ss.similarity_2_word("hot", "cold") << endl;
    // cout << "Similarity between `absurd' and `strange' is "     << ss.similarity_2_word("absurd", "strange") << endl;
    // cout << "Similarity between `logical' and `reasonable' is " << ss.similarity_2_word("logical", "reasonable") << endl;
    // cout << "Similarity between `mundane' and `happy' is "      << ss.similarity_2_word("mundane", "happy") << endl;

    // string w1 = "antenna", w2;
    // do {
    //     cin >> w2;
    //     cout << "Similarity between `" << w1 << "' and `" << w2 << "' is "   << ss.similarity_2_word(w1, w2) << endl;
    //     w1 = w2;
    // } while(1);
    // --------- using function ------------
    // function ss.similarity_2_word(string1, string2) se tra ve 1 so thuc thuoc [0.0; 1.0] la do tuong dong giua 2 tu string1 va string2
}   