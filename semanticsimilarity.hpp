#pragma once

namespace semanticsimilarity {
    std::string standardized_word(std::string word) {
        for(int i = 0; i < int(word.length()); i++)
            if(word[i] == ' ')  word[i] = '_';
            else                word[i] = std::tolower(word[i]);
        return word;
    }
    // -----------------
    long double alpha = 0.2; 
    long double beta  = 0.45;
    // --
    unsigned int $SHORTEST_PATH = 0;
    // --
    unsigned shortest_path_same_ss_type(const Data& D1, const Data& D2) {
        $SHORTEST_PATH = 0;
        //
        char pos = D2.ss_type;
        if(pos == 's') pos = 'a';
        //
        std::unordered_map< int, int >   Vis;
        std::queue< std::pair<Data, int> > Q;
        //
        Q.push( {D1, 0} );
        // --        
        while(!Q.empty()) {
            Data now     = Q.front().first;
            int  step    = Q.front().second;
            Q.pop();
            // --
            if(now.ss_type == D2.ss_type && now.idx() == D2.idx()) {
                $SHORTEST_PATH = step;
                break;
            }
            // --
            if(Vis[now.idx()]) continue;
            Vis[now.idx()] = 1;
            // --
            for(int i = 0; i < now.p_cnt; i++) {
                if(now.ptr[i].pointer_symbol == "@" || now.ptr[i].pointer_symbol == "~")
                    if(now.ptr[i].pos == pos)
                        Q.push( {seek(now.ptr[i].pos, now.ptr[i].synset_offset), step+1} );
            }
        }
        //
        return $SHORTEST_PATH;
    }
    // -- ancestor approach
    //
    void list_ancestors_same_ss_type(const Data& D, std::vector< std::pair<Data, unsigned> >& ancs, char& pos, unsigned depth = 1) {
        if(depth == 1) {    // uninitilized state,
            pos = (D.ss_type == 's' ? 'a' : D.ss_type);
            ancs.push_back({D, 1});
        }
        // --
        std::vector<Data> _parents;
        for(int i = 0; i < D.p_cnt; i++) {
            if(D.ptr[i].pointer_symbol == "@") {
                if(D.ptr[i].pos != pos) continue;
                _parents.push_back( seek(D.ptr[i].pos, D.ptr[i].synset_offset) );
            }
        }

        for(int i = 0; i < int(_parents.size()); i++) {
            // the existence of this loop is sorely because 
            // I want the `ancs' vector to follow order of `depth' that is beening pushed in,
            ancs.push_back( {_parents[i], depth + 1} ); 
        }
        for(int i = 0; i < int(_parents.size()); i++) {
            list_ancestors_same_ss_type(_parents[i], ancs, pos, depth + 1);
        }

    }
    //
    unsigned lca_shortest_path_same_ss_type(const Data& D1, const Data& D2) {
        std::vector< std::pair<Data, unsigned> > ancs1, ancs2;
        // --
        char __placeholder = '!';
        list_ancestors_same_ss_type(D1, ancs1, __placeholder);
        list_ancestors_same_ss_type(D2, ancs2, __placeholder);
        //
        std::unordered_map< std::string, unsigned > depthHashmap;
        //
        unsigned minDist = UINT_MAX;
        //
        for(int i = 0; i < (int)(ancs1.size()); i++) depthHashmap[ancs1[i].first()] = ancs1[i].second;
        for(int i = 0; i < (int)(ancs2.size()); i++)
            if(depthHashmap[ancs2[i].first()]) 
                minDist = std::min(minDist, ancs2[i].second + depthHashmap[ancs1[i].first()] - 2);
            // minus 2 because two depth starts at 1
        return minDist;
        // if they share a non-common root returns infinity (very large num)
    }
    // --
    long double similarity_2_synset_same_ss_type(const Data& D1, const Data& D2) {
        if(D1.ss_type != D2.ss_type) return 0.0;
        // now for simplicity, we only consider if two synset are of same type
        // NO HEIGHT! we also discard Height (or depth) for now.

        // unsigned L = shortest_path_same_ss_type(D1, D2);
        unsigned L = lca_shortest_path_same_ss_type(D1, D2);
        return exp(-1 * alpha * L);
    }

    long double similarity_2_word(std::string w1, std::string w2) {
        w1 = standardized_word(w1);
        w2 = standardized_word(w2);
        // --
        std::vector<Data> V1 = subsets_that_contain(w1);
        std::vector<Data> V2 = subsets_that_contain(w2);
        long double _calculated_sim = 0.0;

        for(int i = 0; i < int(V1.size()); i++) {
            for(int j = 0; j < int(V2.size()); j++) {
                // cout << i << " " << j << " " << similarity_2_synset_same_type(V1[i], V2[j]) << endl;
                _calculated_sim = std::max(_calculated_sim, similarity_2_synset_same_ss_type(V1[i], V2[j]));
            }
        }

        return _calculated_sim;
    }
    // --
}