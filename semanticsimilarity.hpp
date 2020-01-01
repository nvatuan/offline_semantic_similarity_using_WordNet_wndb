#pragma once

class SemanticSimilarity {
public:
    // == Some dynamic programming
    bool            __ss_computed = 0;
    double     ssimilarity   = 0.0;

    static std::unordered_map< std::string, double > __hash_sim_2_word;

    // -- original sentences
    std::string sentence1;
    std::string sentence2;
    // -- list of words that appear in the sentences
    std::vector<std::string> ws1;
    std::vector<std::string> ws2;
    // -- hash map for checking if a word exists in the sentence
    std::unordered_set<std::string> exist1;
    std::unordered_set<std::string> exist2;
    // -- the JOINT word vector
    std::vector<std::string> JOINT;
    // -- smoothing constants
    double $ALPHA; 
    double $BETA;
    // -- 
    std::vector<double> SeSim1;
    std::vector<double> OrdSim1;

    std::vector<double> SeSim2;
    std::vector<double> OrdSim2;
    // =============================================================================
public:
    SemanticSimilarity (std::string s1, std::string s2, double alpha = 0.2, double beta = 0.45) {
        this->sentence1 = s1;
        this->sentence2 = s2;
        this->$ALPHA    = alpha;
        this->$BETA     = beta;
        //
        this->ws1 = Ultility::string_to_list_of_words(this->sentence1);
        this->ws2 = Ultility::string_to_list_of_words(this->sentence2);
        //
        JOINT.push_back("");
        Ultility::update_joint_list(JOINT, ws1);
        Ultility::update_joint_list(JOINT, ws2);
        //
        for(unsigned i = 0; i < ws1.size(); i++) exist1.insert(ws1[i]);
        for(unsigned i = 0; i < ws2.size(); i++) exist2.insert(ws2[i]);
    }
//private:
    // =============================== SUPPLEMENT ===================================
    // -- return a Standardized for argument `word'
    std::string standardized_word(std::string word);

    // ======================= Traverse the Datastructure ===========================
    // -- return shortest_path between two synset `D1' and `D2'
    unsigned shortest_path_same_ss_type(const Data& D1, const Data& D2);

    // -- ancestor approach for serving the same purpose as the above
    void list_ancestors_same_ss_type(const Data& D, std::vector< std::pair<Data, unsigned> >& ancs, char& pos, unsigned depth = 1);

    // ============== Calculate arguments contribute to Similarity ==================
    // -- this function will returns a pair of unsigned number, is accordingly L then H
    // corresponding to LCA-SHORTEST-PATH and LOWEST-COMMON-SUBSUMER-DEPTH
    std::pair<unsigned, unsigned> lca_shortest_path_same_ss_type(const Data& D1, const Data& D2);
    
    // ========================= COMPUTE SIMILARITY VALUE ===========================
    // -- Compute a value in [0.0 -> 1.0] is the similarity value of two synsets
    double similarity_2_synset_same_ss_type(const Data& D1, const Data& D2);

    // -- Compute a value in [0.0 -> 1.0] is the similarity value of two words
    double similarity_2_word(std::string w1, std::string w2);

    // ==============================================================================
    //  calculating semantic similarity of two sentences 
    //  this function is used to calculate the semantic similarity value of each words in a sentence 
    //  it returns a vector of these values
    std::vector<double> semanticProc(std::vector<std::string>s, std::unordered_set<std::string> exist);

    std::vector<double> orderProc(std::vector<string> s);
public:
    double compute_semantic_similarity();
    // =========================== operator <, >, =================================== 
    bool operator<(SemanticSimilarity& D) {
        return this->compute_semantic_similarity() < D.compute_semantic_similarity();
    }
    bool operator>(SemanticSimilarity& D) {
        return this->compute_semantic_similarity() > D.compute_semantic_similarity();
    }
    // ================================ DEBUG ======================================= 
    void debug() {
        cout << "Keep in mind that vectors are 1-indexed in this context.\n";
        //
        cout << "__ss_computed = " <<  __ss_computed << "\n";
        cout << "ssimilarity = " <<  ssimilarity << "\n";

        cout << "Sentence 1: "      << sentence1 << '\n';
        cout << "Sentence 2: "      << sentence2 << "\n\n";
        //
        cout << "List of words for sentence 1:\n";
        for(std::string& s : ws1) cout << s << " | ";
        cout << "\n";
        cout << "List of words for sentence 2:\n";
        for(std::string& s : ws2) cout << s << " | ";
        cout << "\n\n";

        cout << "Skip hashmap debug()\n\n";

        cout << "List of words for Joint vector:\n";
        for(std::string& s : JOINT) cout << s << " | ";
        cout << "\n\n";

        cout << "$ALPHA = " << $ALPHA  << "\n";
        cout << "$BETA = "  << $BETA << "\n\n";

        cout << "Semantic Vector 1:\n";
        for(double& ld : SeSim1) cout << ld << " ";
        cout << "\n";
        cout << "Semantic Vector 2:\n";
        for(double& ld : SeSim2) cout << ld << " ";
        cout << "\n\n";

        cout << "Order Vector 1:\n";
        for(double& ld : OrdSim1) cout << ld << " ";
        cout << "\n";
        cout << "Order Vector 2:\n";
        for(double& ld : OrdSim2) cout << ld << " ";
        cout << "\n\n";
    }
};

// =================================== DEFINITION ===================================
std::unordered_map< std::string, double > SemanticSimilarity::__hash_sim_2_word;
// =================================== DEFINITION ===================================

std::string SemanticSimilarity::standardized_word(std::string word) {
    for(int i = 0; i < int(word.length()); i++)
        if(word[i] == ' ')  word[i] = '_';
        else                word[i] = std::tolower(word[i]);
    return word;
}

unsigned SemanticSimilarity::shortest_path_same_ss_type(const Data& D1, const Data& D2) {
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
            return step;
            break;
        }
        // --
        if(Vis[now.idx()]) continue;
        Vis[now.idx()] = 1;
        // --
        for(int i = 0; i < now.p_cnt; i++) {
            if(now.ptr[i].pos == pos)
                Q.push( {seek(now.ptr[i].pos, now.ptr[i].synset_offset), step+1} );
        }
    }
    //
    return UINT_MAX;
}

// ------------------------------------

void SemanticSimilarity::list_ancestors_same_ss_type(const Data& D, std::vector< std::pair<Data, unsigned> >& ancs, char& pos, unsigned depth) {
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

// ------------------------------------

std::pair<unsigned, unsigned> SemanticSimilarity::lca_shortest_path_same_ss_type(const Data& D1, const Data& D2) {
    // -- some dynamic-programming to reduce complexity
    static std::vector< std::pair<Data, unsigned> > ancs1, ancs2;
    static std::unordered_map< std::string, unsigned > depthHashmap;
    static Data last_run1, last_run2;
    // --
    char __placeholder = '!';
    if( !(last_run1 == D1) ) {
        // -- update last_run object;
        last_run1 = D1;
        // -- get new <ancestor;depth> list
        ancs1.resize(0);
        list_ancestors_same_ss_type(D1, ancs1, __placeholder);
        // -- generate new hashmap
        depthHashmap.clear();
        for(int i = 0; i < (int)(ancs1.size()); i++) depthHashmap[ancs1[i].first()] = ancs1[i].second;
    }
    if( !(last_run2 == D2) ) {
        // -- update last_run object;
        last_run2 = D2;
        // -- get new <ancestor;depth> list
        ancs2.resize(0);
        list_ancestors_same_ss_type(D2, ancs2, __placeholder);
    }
    // -- compute similarity necessary values
    unsigned __inverted_depth = std::min(ancs1.back().second, ancs2.back().second);
    unsigned lcs_depth        = 0;
    unsigned lcs_shortestpath = UINT_MAX;
    //        
    for(int i = 0; i < (int)(ancs2.size()); i++)
        if(depthHashmap[ancs2[i].first()]) {
            // result = path1 + path2 - 2 because both depths are 1-indexed
            unsigned sub_lcs_shortestpath = ancs2[i].second + depthHashmap[ancs2[i].first()] - 2;
            if(lcs_shortestpath > sub_lcs_shortestpath) {
                lcs_shortestpath = sub_lcs_shortestpath;
                lcs_depth        = __inverted_depth - std::min(ancs2[i].second, depthHashmap[ancs2[i].first()]);
            }
        }
    return {lcs_shortestpath, lcs_depth};
    // if they share a non-common root returns infinity (very large num)
}

// ------------------------------------

double SemanticSimilarity::similarity_2_synset_same_ss_type(const Data& D1, const Data& D2) {
    // From the paper, we only consider if two synset are of same type
    if(D1 == D2) {
        return 1.0;
    }
    if(D1.ss_type != D2.ss_type) {
        return 0.0;
    }
    
    // One major drawback is even two synsets of type adjective and adverb 
    // always has similarity of 0.0, this is because wordnet_db does not have
    // IS-A hierarchy for these word type.

    // -- slow algorithm
    // unsigned L = shortest_path_same_ss_type(D1, D2); 
    // return exp(-1 * $ALPHA * L);

    // -- faster, improved algorithm
    std::pair<unsigned, unsigned> Pair_L_H = lca_shortest_path_same_ss_type(D1, D2);
    const unsigned& L = Pair_L_H.first;
    const unsigned& H = Pair_L_H.second;
    return exp(-1 * $ALPHA * L) * (exp($BETA * H) - exp(-$BETA * H)) / (exp($BETA * H) + exp(- $BETA * H));
}

// ------------------------------------

double SemanticSimilarity::similarity_2_word(std::string w1, std::string w2) {
    const double threshold = 0.2;
    //
    if(w1 == w2) return 1.0;
    if(w1 > w2) std::swap(w1, w2);
    std::string __st_hash = w1 + " " + w2;
    if(__hash_sim_2_word.find(__st_hash) != __hash_sim_2_word.end()) {
        return __hash_sim_2_word[__st_hash];
    }
    // --
    w1 = standardized_word(w1);
    w2 = standardized_word(w2);
    // --
    std::vector<Data> V1 = subsets_that_contain(w1);
    std::vector<Data> V2 = subsets_that_contain(w2);
    double _calculated_sim = 0.0;

    for(int i = 0; i < int(V1.size()); i++) {
        for(int j = 0; j < int(V2.size()); j++) {
            // cout << i << " " << j << " " << similarity_2_synset_same_type(V1[i], V2[j]) << endl;
            _calculated_sim = std::max(_calculated_sim, similarity_2_synset_same_ss_type(V1[i], V2[j]));
        }
    }
    // --
    if(_calculated_sim < threshold) _calculated_sim = 0.0;
    __hash_sim_2_word[__st_hash] = _calculated_sim;
    return _calculated_sim;
}

// -------------------------------------

std::vector<double> SemanticSimilarity::semanticProc(std::vector<std::string>s, std::unordered_set<std::string> exist)
{
    vector<double> S(JOINT.size(), 0); 
    //  a threshold used to determined if the words in semantic similar or not
    const double threshold = 0.1; 
    //
    double MaxW, temp; 
    string chosenword = "";
    //
    bool error_announce = 0;
    //
    for (unsigned i=1; i<JOINT.size(); i++)
    {
        MaxW = 0.0;
        temp = 0.0;
        //  if the word exists in the sentence, then w[i] = 1
        if (exist.find(JOINT[i]) != exist.end()) {
            S[i] = 1.0;
            //  if JOINT[i] also exist in the sentence, the associated word is the same as JOINT[i]
            chosenword = JOINT[i];
        } else {
            //  if the word does not exist in the sentence, 
            //  the semantic similarity value is calculated between JOINT[i] 
            //  and each words in the sentence
            for (unsigned j = 1; j<s.size(); j++)
            {
                temp = SemanticSimilarity::similarity_2_word(JOINT[i],s[j]); 
                //  choose the most similar word in the sentence to JOINT[i]
                if (MaxW<temp) 
                {
                    //  choosing associated word
                    chosenword = s[j]; 
                    MaxW = temp; 
                }
            }
            //  if the value exceeds a preset threshold, then w[i] = MaxW, 
            //  otherwise, w[i] = 0 ; 
            if (MaxW>threshold)  S[i] = MaxW; 
            else S[i] = 0.0;
        }
        //  calculate the information content based on formulas of Information content of a word in the corpus 
        
        // --- Information Content
        //  ICw_joint is the IC of the current word int joint vector
        double ICw_joint = 0.0;
        //  ICw_chosen is the IC of the chosen word (most similar)
        double ICw_chosen = 0.0;

        //  frequency of associated word in the sentence 
        //  double freq = exist[chosenword] + 1; 
        unsigned int freq_joint  = BrownCorpus::freq(JOINT[i])   + 1;
        unsigned int freq_chosen = BrownCorpus::freq(chosenword) + 1;
        unsigned int N           = BrownCorpus::TOTAL_WORDS      + 1;
        //
        if(N == 1) {
            if(!error_announce) {
                cerr << "Division by Zero. Total words cannot equal to 0, this might because of data file for ``Brown Corpus'' is not loaded or is missing.\n";
                cerr << "Program will continue running. This may lead to unexpected results.\n";
                error_announce = 1;
            }
        } else {
            // 
            ICw_joint   = 1 - log(freq_joint) /log(N);
            ICw_chosen  = 1 - log(freq_chosen)/log(N);

            //  S[i] is the semantic value of each words in the JOINT set, compared with the input sentence
            S[i] = S[i] * ICw_joint * ICw_chosen;    
        }
    }
    return S; 
}  


std::vector<double> SemanticSimilarity::orderProc(std::vector<std::string> s)
{
    std::unordered_map<std::string, int> __index;
    for(unsigned i = 1; i < s.size(); i++) {
        if(!__index[s[i]]) __index[s[i]] = i;
    }
    // ---------------------------------------------------------------------------------------
    vector<double> O (JOINT.size(), 0); 
    const double threshold = 0.2; 
    
    for (unsigned i = 1; i<JOINT.size(); i++)
    {
        //  if the word in the JOINT vector appears in the sentence
        //  we fill the entry in O with the corresponding index number (of the word) from the sentence
        O[i] = __index[JOINT[i]];
        //  otherwise, we try to find the most similar word in the sentence;  
        if ( O[i] == 0 ) {  // this only happens when the current word doesn't exist
            double MaxS = 0.0;

            //If the similarity between these two words is greater than a preset threshold, 
            //the entry of word in the JOINT vector in O is filled with the index number of most similar word from the sentence
            for (unsigned j = 1; j<s.size(); j++)
            {
                double temp = SemanticSimilarity::similarity_2_word(JOINT[i], s[j]); 
                if (temp > MaxS)
                {
                    MaxS = temp; 
                    O[i] = j;
                }
            }
            //If the above two searches fail, the entry of word in the JOINT vector in O is 0.
            if (MaxS<=threshold) O[i] = 0;             
        }
    }
    return O;
}

double SemanticSimilarity::compute_semantic_similarity()
{
    if(this->__ss_computed) {
        return ssimilarity;
    }
    this->__ss_computed = true;
    // -----------------------------------------
    this->SeSim1 = this->semanticProc(ws1, exist1);
    std::vector<double>& s1 = this->SeSim1;

    this->SeSim2 = this->semanticProc(ws2, exist2);
    std::vector<double>& s2 = this->SeSim2;

    this->OrdSim1 = this->orderProc(ws1);
    std::vector<double>& r1 = this->OrdSim1;

    this->OrdSim2 = this->orderProc(ws2);
    std::vector<double>& r2 = this->OrdSim2;

    // The semantic similarity between two sentences is defined as the cosine coefficient between the two vectors
    // norm1 and norm2 is the variables which contains length of a vector
    double norm1 = 0.0, norm2 = 0.0;
    double semanticRes = 0.0; 
    //Because s1 and s2 have the same size
    for (unsigned i=0; i<s1.size(); i++)
    {
        semanticRes += s1[i]*s2[i]; 
        norm1 += s1[i]*s1[i];
        norm2 += s2[i]*s2[i]; 
    }
    semanticRes /= (sqrt(norm1)*sqrt(norm2)); 

    //Calcualte the word order similarity of two sentences 
    double ordMinus = 0.0, ordAdd = 0.0; 
    for (unsigned i=1; i<s1.size(); i++)
    {
        ordMinus += (r1[i]-r2[i])*(r1[i]-r2[i]); 
        ordAdd   += (r1[i]+r2[i])*(r1[i]+r2[i]);         
    }
    double orderRes = 1 - (sqrt(ordMinus))/(sqrt(ordAdd)); 

    //the overall sentence similarity is defined as a combination of semantic similarity and word order similarity
    //delta decides the relative contributions of semantic and word order information to the overall similarity computation
    //through experimenting, choose delta = 0.85
    const double delta = 0.85; 
    //
    double overallRes = delta*semanticRes + (1.0-delta)*orderRes; 
    //
    this->ssimilarity = overallRes;
    return overallRes; 
}