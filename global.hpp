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
#include <unordered_set>

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
std::string PATHPREFIX   = "\\dict\\";
std::string DATAPREFIX   = "\\dict\\data.";
//ifstream f_data_noun, f_data_verb, f_data_adj, f_data_adv;
std::string INDEXPREFIX  = "\\dict\\index.";
//ifstream f_index_noun, f_index_verb, f_index_adj, f_index_adv;

std::vector<std::string> POS  = {"noun", "verb", "adj", "adv"};
std::string EXCEPTIONSUFFIX = ".exc";

#define POS_NOUN    0
#define POS_VERB    1
#define POS_ADJ     2
#define POS_ADV     3

/*** --------------------------- namespaces --------------------------- ***/
#include "namespaces.hpp"
/*** --------------------------- DATABASE RELATED --------------------------- ***/
#include "synset.hpp"

// Data ROOT;
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
