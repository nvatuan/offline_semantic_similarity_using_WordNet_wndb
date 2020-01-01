#pragma once

Data& seek(char pos, int synset_offset) {
    //return data[__index[pos][synset_offset];
    switch(pos) {
        case 'n': return data_noun[Data::__index[pos][synset_offset]];
        case 'v': return data_verb[Data::__index[pos][synset_offset]];
        case 'a': return data_adj [Data::__index[pos][synset_offset]];
        case 's': return data_adj [Data::__index[pos][synset_offset]];
        case 'r': return data_adv [Data::__index[pos][synset_offset]];
    };
    throw std::invalid_argument("Unexpected 'pos' character.");
}

void __binsearch(std::string word, vector<Data>& join, char pos, const vector<Index>& here) {
    // -- binary search
    int L = 0, R = here.size();
    while(L <= R) {
        int mid = (L + R) / 2;
        // -- debug
        // printf("L = %d | R = %d -> word@mid = ", L, R);
        // cout << here[mid].word << endl;
        // --
        if( (here[mid].word == word) ) {
            // cout << "!! Found! " << here[mid].word << " == " << word << endl;
            for(int i = 0; i < here[mid].synset_cnt; i++) {
                join.push_back(seek(here[mid].pos, here[mid].synset_offset[i]));
            }
            return;
        }
        // 
        if(here[mid].word > word)
            R = mid - 1;
        else
            L = mid + 1;
    }
}

std::vector<Data> subsets_that_contain(std::string word) {
    std::vector<Data> Ds;
    // --
    // the search has 2 possible results:
    // 1: `word` is found in the data structure.
    // 2: `word` is not found
    //     when this happens, there is a chance that searched `word` is in inflected forms
    //
    //     The idea is to generate its base forms. So we have a list of all its alledgely base_forms and the word itself
    //     What is left to do is to search for the word in datastructure.
    std::vector<std::string> _possible_baseform [4] {
        Morphology::base_form(word, 'n'),
        Morphology::base_form(word, 'v'),
        Morphology::base_form(word, 'a'),
        Morphology::base_form(word, 'r')
    };
    //
    for(int pos = 0; pos < 4; pos++) {
        for(int iw = 0; iw < int(_possible_baseform[pos].size()); iw++) {
            word = _possible_baseform[pos][iw];
            // cout << pos << " " << word << endl;
            switch(pos) {
                case POS_NOUN:
                    __binsearch(word, Ds, 'n', index_noun);
                    break;
                case POS_VERB:
                    __binsearch(word, Ds, 'v', index_verb);
                    break;
                case POS_ADJ:
                    __binsearch(word, Ds, 'a', index_adj);
                    break;
                case POS_ADV:
                    __binsearch(word, Ds, 'r', index_adv);
                    break;
            }
        }
    }
    //
    return Ds;
}