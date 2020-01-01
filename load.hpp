template<typename T>
void _load_from_dict_to_container(const std::string _FILEPATH, std::vector<T>& V) {
    ifstream __F;
    __F.open((_FILEPATH.c_str()), ios::in);

    if(!__F.is_open()) {
        throw std::pair<std::string, int> {(std::string("Couldn't open the following data file:\n") + _FILEPATH + '\n'), -1};
    }
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

void _load_from_dict_to_hashmap(const std::string _FILEPATH, std::unordered_map<std::string, std::string>& Hm) {
    ifstream __F;
    __F.open((_FILEPATH.c_str()), ios::in);

    if(!__F.is_open()) {
        throw std::pair<std::string, int> {(std::string("Couldn't open the following data file:\n") + _FILEPATH + '\n'), 0};
    }
    // --
    std::string S;
    std::string key;
    std::string value;
    //
    while(!__F.eof()) {
        getline(__F, S);
        if(S.length() < 3) continue;
        
        unsigned space = S.find(' ');
        //
        if(space == std::string::npos) {
            cout << S << endl;
            throw std::invalid_argument("exc files are ill-formated");
        }
        key     = std::string(S.begin(), S.begin() + space);
        value   = std::string(S.begin() + space + 1, S.end());
        //
        Hm[key] = value;
    }
}

void load() {
    try {
        // data.files
        _load_from_dict_to_container(PATH + DATAPREFIX + POS[POS_NOUN], data_noun);
        _load_from_dict_to_container(PATH + DATAPREFIX + POS[POS_VERB], data_verb);
        _load_from_dict_to_container(PATH + DATAPREFIX + POS[POS_ADJ],  data_adj);
        _load_from_dict_to_container(PATH + DATAPREFIX + POS[POS_ADV],  data_adv);
        // index.files
        _load_from_dict_to_container(PATH + INDEXPREFIX + POS[POS_NOUN], index_noun);
        _load_from_dict_to_container(PATH + INDEXPREFIX + POS[POS_VERB], index_verb);
        _load_from_dict_to_container(PATH + INDEXPREFIX + POS[POS_ADJ],  index_adj);
        _load_from_dict_to_container(PATH + INDEXPREFIX + POS[POS_ADV],  index_adv);
        // .exc files
        _load_from_dict_to_hashmap(PATH + PATHPREFIX + POS[POS_NOUN] + EXCEPTIONSUFFIX, Morphology::noun_exc);
        _load_from_dict_to_hashmap(PATH + PATHPREFIX + POS[POS_VERB] + EXCEPTIONSUFFIX, Morphology::verb_exc);
        _load_from_dict_to_hashmap(PATH + PATHPREFIX + POS[POS_ADJ] + EXCEPTIONSUFFIX,  Morphology::adj_exc);
        _load_from_dict_to_hashmap(PATH + PATHPREFIX + POS[POS_ADV] + EXCEPTIONSUFFIX,  Morphology::adv_exc);
    } catch (std::pair<std::string, int> eP) {
        cerr << eP.first << '\n';
        throw eP.second;
    }
}