#include "global.hpp"
#include "load.hpp"
#include "semanticsimilarity.hpp"

int main(int argc, char* argv[]) {
    try {
        Auxiliary::update_working_dir(argv[0]);
    } catch (...) {
        cerr << "Problem @ trying to update working directory!\n";
    }
    load();
    cout << "loaded.\n\n";
    // --------- demonstration ------------
    cout << "Similarity between `marker' and `pen' is "     << semanticsimilarity::similarity_2_word("marker", "pen") << endl;
    cout << "Similarity between `marker' and `sticker' is " << semanticsimilarity::similarity_2_word("marker", "sticker") << endl;
    cout << "Similarity between `potato' and `tomato' is "  << semanticsimilarity::similarity_2_word("potato", "tomato") << endl;
    cout << "Similarity between `wine' and `beer' is "      << semanticsimilarity::similarity_2_word("wine", "beer") << endl;
    cout << "Similarity between `car' and `driver' is "     << semanticsimilarity::similarity_2_word("car", "driver") << endl;
    cout << "Similarity between `man' and `lion' is "       << semanticsimilarity::similarity_2_word("man", "lion") << endl;
    // --------- using function ------------
    // function semanticsimilarity::similarity_2_word(string1, string2) se tra ve 1 so thuc thuoc [0.0; 1.0] la do tuong dong giua 2 tu string1 va string2
}   