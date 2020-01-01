#pragma once

// =================================== class Ptr ===================================
// ---------------------- prototype --------------------------------------------
class Ptr {
    public:
        std::string     pointer_symbol;
        int             synset_offset;
        char            pos;

        std::string     source_target_hex;
        short           source_word_id;
        short           target_word_id;

        void input(stringstream&);
        void show() const;
};
// ---------------------- definition --------------------------------------------
void Ptr::show() const {
    std::cout << "    pointer_symbol: " << pointer_symbol << '\n';
    printf(      "    synset_offset: %d\n", synset_offset);
    printf(      "    pos: %c\n", pos);
    std::cout << "    source_target: " << source_target_hex << '\n';
    printf(      "    source_word_id: %d | target_word_id: %d\n", source_word_id, target_word_id);
}

void Ptr::input(stringstream& ss) {
    ss >> this->pointer_symbol >> this->synset_offset >> this->pos >> this->source_target_hex;
    //
    this->source_word_id = Ultility::hex_to_dec(source_target_hex, 0, 2);
    this->target_word_id = Ultility::hex_to_dec(source_target_hex, 2, 4);
}

// =================================== class Index ===================================
// ---------------------- prototype --------------------------------------------
class Index {
    public:
        std::string                 word;
        char                        pos;
        
        int                         synset_cnt;
        int                         p_cnt;
        std::vector<std::string>    ptr_symbol;

        int                         sense_cnt;      // redundant, noted from documentaion
        int                         tagsense_cnt;   // honestly dont know what this does
        
        std::vector<int>            synset_offset;

        int                         line_number;

        void input(const string&);
        void show();
        void __set(){};                             // compatibility, does nothing at all.
        const int idx() const {return this->line_number - 1;}
};
// ---------------------- definition -------------------------------------------
void Index::input(const string& S) {
    stringstream ss (S);
    // -----------------------------------
    ss >> this->word >> this->pos >> synset_cnt >> p_cnt;
    
    this->ptr_symbol.resize(this->p_cnt);
    for(int i = 0; i < this->p_cnt; i++)
        ss >> this->ptr_symbol[i];
    
    ss >> this->sense_cnt >> this->tagsense_cnt;

    this->synset_offset.resize(this->synset_cnt);
    for(int i = 0; i < this->synset_cnt; i++) 
        ss >> this->synset_offset[i];
}

void Index::show() {
    cout << " word =  " << word << '\n';
    printf(" pos = %c\n", pos);
    printf(" synset_cnt = %d\n", synset_cnt);
    printf(" p_cnt = %d\n", p_cnt);

    printf(" ptr_symbols:\n");
    for(int i = 0; i < p_cnt; i++) cout << " " << ptr_symbol[i] ;
    cout << '\n';

    printf(" sense_cnt = %d\n", sense_cnt);
    printf(" tagsense_cnt = %d\n", tagsense_cnt);

    printf(" synset_offset: \n");
    for(int i = 0; i < synset_cnt; i++) cout << " " << synset_offset[i];
    cout << '\n';

    printf("This is line: %d\n", line_number);
}

// =================================== class Data ===================================
// ---------------------- prototype --------------------------------------------
class Data {
        typedef std::unordered_map< char, std::unordered_map<int, int> > __synset_to_index_table;
    private:
        // global within class member
        static  __synset_to_index_table     __index;
        static  __synset_to_index_table     __index_make();
    public:
        // instance member
        int                                 synset_offset;
        int                                 lex_filenum;
        char                                ss_type = 'z';

        std::string                         w_cnt_hex;
        short                               w_cnt;

        std::vector<std::string>            word;
        //std::vector<int>                  lex_id_hex; // an extra vector<>, no. processed when load.
        std::vector<int>                    lex_id;

        int                                 p_cnt;
        std::vector<Ptr>                    ptr;

        std::vector<string>                 frames;
        std::string                         gloss;

        int                                 line_number = 0;

        void input(const std::string& S);
        void show(bool = false) const;

        friend Data& seek(char, int);
        void  __set() {
            char pos = this->ss_type; if(pos == 's') pos = 'a';
            // the previous line is to prevent a case where
            // the synset type is 's' which means 'satellite adjective'
            // different from 'head adjective', but __index[][] table
            // cares about WORD TYPE, not synset_type. So we group
            // ss_type of 's' and 'a' to one POS which is 'a'
            __index[pos][this->synset_offset] = this->line_number - 1;
        }


        // -- Data::operator() is a hash function for its instance, return a std::string-typed hash value
        std::string operator()() const {
            std::string ss_type_st = std::to_string(this->line_number);
            ss_type_st += this->ss_type;
            return ss_type_st;
        }

        // -- custom comparision to make things faster
        bool operator==(const Data& rhs) const {
            // compare between two hashes produces by Data::operator()
            return ((*this)()) == (rhs());
        }

        // -- index of current instance in datastructure
        const int idx() const {return this->line_number - 1;}
};
// ---------------------- definition -------------------------------------------
Data::__synset_to_index_table Data::__index_make() {
    Data::__synset_to_index_table tmp;
    tmp['n'] = tmp['v'] = tmp['a'] = tmp['s'] = tmp['r'] = {};
    return tmp;
}
Data::__synset_to_index_table Data::__index = Data::__index_make();

void Data::input(const std::string& S) {
    stringstream ss;
    ss.str(S);                  // copy string S to stringstream ss
    //
    ss >> this->synset_offset;
    ss >> this->lex_filenum;
    ss >> this->ss_type;
    ss >> this->w_cnt_hex;

    this->w_cnt = Ultility::hex_to_dec(this->w_cnt_hex);

    this->word.resize(this->w_cnt);
    this->lex_id.resize(this->w_cnt);
    for(int i = 0; i < this->w_cnt; i++) {
        char lex_id_hex;
        ss >> this->word[i] >> lex_id_hex;
        this->lex_id[i] = Ultility::hexadec[lex_id_hex];
    }

    ss >> this->p_cnt;
    this->ptr.resize(this->p_cnt, Ptr());
    for(int i = 0; i < this->p_cnt; i++) {
        // ss >> (this->ptr[i]).pointer_symbol >> (this->ptr[i]).synset_offset >> (this->ptr[i]).pos >> (this->ptr[i]).source_target;
        this->ptr[i].input(ss);
    }
    
    // our purpose does not include these extra information 
    // that what we are about to exclude has, so we move 
    // the content from the sstream back to string to
    // process filter it out
    // [frames] are what we getting rid of
    int gloss_start = S.find('|');
    this->gloss = std::string(S.begin() + gloss_start, S.end());
    for(unsigned i = 0; i + 1 < this->gloss.size(); i++)
        if(this->gloss[i] == ';') this->gloss[i+1] = '\n';
}

void Data::show(bool wordsonly) const {
    if(!wordsonly) {
        printf("This synset is on line number: %d\n", line_number);
        printf("Synset_offset: %d\n", synset_offset);
        printf("lex_filenum: %d\n", lex_filenum);
        printf("ss_type: %c\n", ss_type);
        printf("w_cnt: %d\n", w_cnt);
        
        printf("WORDS | LEX_ID:\n");
        for(int i = 0; i < w_cnt; i++) 
            cout << "  " << word[i] << " " << lex_id[i] << '\n';
        
        printf("p_cnt: %d\n", p_cnt);
        for(int i = 0; i < p_cnt; i++) {
            cout << "  Pointer #" << i << "\n";
            ptr[i].show();
        }

        for(int i = 0; i < int(frames.size()); i++)
            cout << frames[i] << '\n';
        cout << "Gloss: " << gloss << '\n';
    } else {
        for(int i = 0; i < w_cnt; i++) 
            cout << word[i] << " | ";
        cout << '\n';
    }
}