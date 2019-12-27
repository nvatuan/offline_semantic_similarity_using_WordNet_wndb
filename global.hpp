#include <stdio.h>       /* defines FILENAME_MAX */
#include <string.h>      /* defines strlen() */
#include <math.h>        /* defines exp() */
#include <limits.h>      /* defines numerical limits */

#include <sstream>       /* defines stringstream */
#include <iostream>

#include <vector>
#include <string>
#include <queue>
#include <unordered_map> 
#include <map>

#include <fstream>
using namespace std;

/*** --------------------------- something experimental --------------------------- ***/
#ifdef WINDOWS
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
#endif

/*** --------------------------- GLOBALLY DEFINED --------------------------- ***/
std::string PATH;
std::string DATASUFFIX   = "\\dict\\data.";
//ifstream f_data_noun, f_data_verb, f_data_adj, f_data_adv;
std::string INDEXSUFFIX  = "\\dict\\index.";
//ifstream f_index_noun, f_index_verb, f_index_adj, f_index_adv;
std::vector<std::string> POS  = {"noun", "verb", "adj", "adv"};

#define POS_NOUN    0
#define POS_VERB    1
#define POS_ADJ     2
#define POS_ADV     3

/*** --------------------------- the Auxiliary namespace --------------------------- ***/
/*** contains support functions that are not important but are needed ***/
namespace Auxiliary {
    // -- get working dir
    void update_working_dir(char *exePath) {
        char buffer[FILENAME_MAX];
        if (!GetCurrentDir(buffer, sizeof(buffer))) {
            throw;
        }
        PATH = std::string(buffer) + "\\"; 
        int lastdash = strlen(exePath);
        while(exePath[lastdash] != '\\' || lastdash >= 0) lastdash--;
        for(int i = 0; i < lastdash; i++) PATH += exePath[i];
        PATH += "\\";
    }
    // -- miscellaneous
    std::unordered_map<char, short> hexadec = {
        {'0', 0}, {'4', 4}, {'8', 8},  {'c', 12}, {'A', 10}, {'E', 14}, 
        {'1', 1}, {'5', 5}, {'9', 9},  {'d', 13}, {'B', 11}, {'F', 15}, 
        {'2', 2}, {'6', 6}, {'a', 10}, {'e', 14}, {'C', 12}, 
        {'3', 3}, {'7', 7}, {'b', 11}, {'f', 15}, {'D', 13}
    };
    // --
    int hex_to_dec(const std::string& hex, int i = -1, int j = -1) {
        if(i == -1 || j == -1) i = 0, j = hex.length();
        int __res;
        // --
        __res = 0;
        for(int x = i; x < j; x++) __res = __res*16 + (Auxiliary::hexadec[hex[x]]);
        // --
        return __res;
    }
};

/*** --------------------------- DATABASE RELATED --------------------------- ***/
#include "synset.hpp"

Data ROOT;
// ROOT.ss_type     = 'z';
// ROOT.line_number = -1;
// -- Data-struct vector
std::vector<Data> data_noun, data_verb, data_adj, data_adv;
// -- Index-struct vector
std::vector<Index> index_noun, index_verb, index_adj, index_adv;

// -- Data-struct lookup function
Data& seek(char pos, int synset_offset);
// -- Index-struct lookup function
void __binsearch(const std::string&, const vector<Index>&, vector<Data>&);
vector<Data> subsets_that_contain(std::string);

// functions are defined here in this below header file
#include "search.hpp"
