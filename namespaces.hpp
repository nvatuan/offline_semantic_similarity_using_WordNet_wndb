// ======================================= ... =======================================
/*** contains support functions that are not important but are needed ***/
namespace Ultility {
    // ============= get working dir =============
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

    // ============= hexadecimal processing =============
    std::unordered_map<char, short> hexadec = {
        {'0', 0}, {'4', 4}, {'8', 8},  {'c', 12}, {'A', 10}, {'E', 14}, 
        {'1', 1}, {'5', 5}, {'9', 9},  {'d', 13}, {'B', 11}, {'F', 15}, 
        {'2', 2}, {'6', 6}, {'a', 10}, {'e', 14}, {'C', 12}, 
        {'3', 3}, {'7', 7}, {'b', 11}, {'f', 15}, {'D', 13}
    };
    //
    int hex_to_dec(const std::string& hex, int i = -1, int j = -1) {
        if(i == -1 || j == -1) i = 0, j = hex.length();
        int __res;
        // --
        __res = 0;
        for(int x = i; x < j; x++) __res = __res*16 + (Ultility::hexadec[hex[x]]);
        // --
        return __res;
    }

    bool validchar(const char& C) {
        return (isalpha(C) || (C == '-') || (C == '_') || (C == '\'') || (C == '`'));
    }

    // ============= sentences, words processing =============
    std::vector<std::string> string_to_list_of_words(const std::string& s)
    {
        std::string tempstr; 
        std::vector<std::string> words;
        //  skip index #0
        words.push_back("");  
        //
        unsigned i=0; 
        while (i<s.length())
        {
            tempstr = ""; 
            //  detach a word from a sentence
            for ( ; validchar(s[i]) && i<s.length() ; i++)
            {
                tempstr += tolower(s[i]); 
            }
            words.push_back(tempstr); 
            //  skip if the character is non-alphabetical
            if ( !validchar(s[i]) ) i++; 
        } 
        //
        return words;   
    }

    void update_joint_list(std::vector<std::string>& JOINT, std::vector<std::string>& V, bool renew = false) {
        std::unordered_set <std::string> CHECK;
        if(renew) JOINT.resize(0);
        //
        for (unsigned i = 0; i < JOINT.size(); i++) CHECK.insert(JOINT[i]);
        for (unsigned i = 0; i < V.size(); i++) {
            if( CHECK.find(V[i]) == CHECK.end() ) {
                CHECK.insert(V[i]);
                JOINT.push_back(V[i]);
            }
        }
        //
    }
};

// ================================ Brown Corpus Statistic =================================
namespace BrownCorpus {
    unsigned DISTINCT_WORDS = 0;
    unsigned TOTAL_WORDS    = 0;
    bool     RAN = 0;
    std::map<std::string, unsigned> _freq;
    // --
    std::string filename = "brown-corpus\\words_freq.txt";
    // --
    void load() {
        ifstream F (PATH + PATHPREFIX + filename);
        if(!F.is_open()) {
            cerr << "Could not open Brown Corpus Statistic file.\n";
            cerr << "File path: " << PATH + PATHPREFIX + filename << "\n";
            return;
        }
        // -------------------------------------------------------
        std::string word;
        unsigned    num;
        F >> word >> BrownCorpus::DISTINCT_WORDS;
        F >> word >> BrownCorpus::TOTAL_WORDS;
        // --
        while(!F.eof()) {
            F >> word >> num;
            BrownCorpus::_freq[word] = num;
        }
    }
    // --
    unsigned freq(const std::string& S) {
        if(!RAN) {
            RAN = 1;
            BrownCorpus::load();
        }
        return _freq[S];
    }
}

// ======================================= MORPHOLOGY =======================================
namespace Morphology {
    // -- Morphological Exceptions Hashmap
    std::unordered_map<std::string, std::string> noun_exc, verb_exc, adj_exc, adv_exc;

    // -- check if s1 possibly is inflected-form of s2
    // this has to be after checking for morphological exceptions
    typedef std::pair<std::string, std::string> DetachAttach;
    std::vector<DetachAttach> detach_noun {
        {"s", ""},      {"ses", "s"},   {"xes", "x"},   {"zes", "z"}, 
        {"ches", "ch"}, {"shes", "sh"}, {"men", "man"}, {"ies", "y"}
    };
    std::vector<DetachAttach> detach_verb {
        {"s", ""},      {"ies", "y"},   {"es", "e"},    {"es", ""},
        {"ed", "e"},    {"ed", ""},     {"ing", "e"},   {"ing", ""}
    };
    std::vector<DetachAttach> detach_adj {
        {"er", ""},     {"est", ""},    {"er", "e"},    {"est", "e"}
    };
    std::vector<DetachAttach> detach_adv;
    
    // -- update ref
    std::vector<DetachAttach>& _inflected_pos_rule (const char& pos) {
        switch(pos) {
            case 'n': return detach_noun;
            case 'v': return detach_verb;
            case 's': 
            case 'a': return detach_adj;
            case 'r': return detach_adv;
        }
        throw std::invalid_argument(std::string("Unexpected `pos` to be ") + pos);
    }

    // -- update ref    
    std::unordered_map<std::string, std::string>& _inflected_pos_exception (const char& pos) {
        switch(pos) {
            case 'n': return noun_exc;
            case 'v': return verb_exc;
            case 's': 
            case 'a': return adj_exc;
            case 'r': return adv_exc;
        }
        throw std::invalid_argument(std::string("Unexpected `pos` to be ") + pos);
    }

    // -- functions
    std::vector<std::string> base_form(std::string inflected_word, const char& pos) {
        std::vector<DetachAttach>& V                     = _inflected_pos_rule(pos);
        std::unordered_map<std::string, std::string>& Hm = _inflected_pos_exception(pos);
        //
        int _Size                                        = int(V.size());
        int _SuffixLength;
        std::string                     original;
        std::vector<std::string>        base_forms;

        std::unordered_set<std::string> existed;

        // -- Process
        // if none is found on exceptions list, proceed to follow detachment rules
        std::unordered_map<std::string, std::string>::const_iterator search;
        search = Hm.find(inflected_word);
        //
        if(search != Hm.end()) {    
            base_forms.push_back(search->second);
        } else {
            base_forms.push_back(inflected_word);
            //
            for(int i = 0; i < _Size; i++) {
                _SuffixLength = V[i].first.length();
                // --
                if(std::string(inflected_word.end() - _SuffixLength, inflected_word.end()) == V[i].first) {
                    original      = std::string(inflected_word.begin(), inflected_word.end() - _SuffixLength);
                    original     += V[i].second;
                    //
                    if(existed.find(original) == existed.end()) {
                        base_forms.push_back(original);
                        existed.insert(original);
                    }
                }
            }
        }
        // --
        return base_forms;
    }
};