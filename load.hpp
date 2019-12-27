#include "synset.hpp"

template<typename T>
void _load_from_dict_to_container(const std::string _FILEPATH, std::vector<T>& V) {
    ifstream __F;
    __F.open((_FILEPATH.c_str()), ios::in);

    if(!__F.is_open()) 
        throw (std::string("Couldn't open the following data file:\n") + _FILEPATH);
    // --
    string S;
    T Instance;
    //
    int line = 0;
    while(!__F.eof()) {
        getline(__F, S);
        if(S[0] == ' ') continue; // license notice lines start with 2 empty spaces
        // --
        line++;
        Instance.input(S);
        Instance.line_number = line;
        Instance.__set();
        // -- finish parsing current Data line
        V.push_back(Instance);
    }
}

void load() {
    try {
        // data.files
        _load_from_dict_to_container(PATH + DATASUFFIX + POS[POS_NOUN], data_noun);
        _load_from_dict_to_container(PATH + DATASUFFIX + POS[POS_VERB], data_verb);
        _load_from_dict_to_container(PATH + DATASUFFIX + POS[POS_ADJ],  data_adj);
        _load_from_dict_to_container(PATH + DATASUFFIX + POS[POS_ADV],  data_adv);
        // index.files
        _load_from_dict_to_container(PATH + INDEXSUFFIX + POS[POS_NOUN], index_noun);
        _load_from_dict_to_container(PATH + INDEXSUFFIX + POS[POS_VERB], index_verb);
        _load_from_dict_to_container(PATH + INDEXSUFFIX + POS[POS_ADJ],  index_adj);
        _load_from_dict_to_container(PATH + INDEXSUFFIX + POS[POS_ADV],  index_adv);
    } catch (std::string e) {
        cerr << e << '\n';
        throw;
    }
}