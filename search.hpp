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

void __binsearch(const std::string& word, const vector<Index>& here, vector<Data>& join) {
    int L = 0, R = here.size();
    while(L <= R) {
        int mid = (L + R) / 2;
        // -- debug
        // printf("L = %d | R = %d -> word@mid = ", L, R);
        // cout << here[mid].word << endl;
        // --
        if(here[mid].word == word) {
            // cout << "!! Found! " << here[mid].word << " == " << word << endl;
            for(int i = 0; i < here[mid].synset_cnt; i++) {
                join.push_back(seek(here[mid].pos, here[mid].synset_offset[i]));
            }
            return;
        }
        // --
        if(here[mid].word > word)
            R = mid - 1;
        else
            L = mid + 1;
    }
}

vector<Data> subsets_that_contain(std::string word) {
    vector<Data> Ds;
    // --
    __binsearch(word, index_noun, Ds);
    __binsearch(word, index_verb, Ds);
    __binsearch(word, index_adj, Ds);
    __binsearch(word, index_adv, Ds);
    //
    return Ds;
}