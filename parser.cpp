// For TParser time variables are given in units of
// 100 nano seconds.

class TParser {

    private:
    long zIndex;
    char zFilename[512];
    TKangaroo* zKangaroo;
    public:
        TParser();
        ~TParser();
        void hallowelt();
        void set_kangaroo(TKangaroo* pKangaroo);
        void set_filename(char* pFilename);
        void parse();
        void read_header(FILE* pFile);
        void read_line(FILE* pFile);
        void read_separator(FILE* pFile);
        void read_buffer(FILE* pFile);
        int read_word(FILE* pFile);
        unsigned long long read_48bits(FILE* pFile);
        void read_event(FILE* pFile);
};

TParser::TParser() {
    zIndex = 0;
    strcpy(zFilename, "");
}

TParser::~TParser() {

}

void TParser::hallowelt() {
    cout << "hallo welt!" << endl;
}

void TParser::set_kangaroo(TKangaroo* pKangaroo) {
    zKangaroo = pKangaroo;
}

void TParser::set_filename(char* pFilename) {
    strcpy(zFilename, pFilename);
}

void TParser::parse() {
    FILE* file;
    file = fopen(zFilename, "rb");
    read_header(file);
    read_separator(file);
    while(!feof(file)) {
        read_buffer(file);
    }
    fclose(file);
}

void TParser::read_header(FILE* pFile) {
    int i;
    for(i = 0; i < 2; i++) {
        read_line(pFile);
    }
}

void TParser::read_line(FILE* pFile) {
    char k;
    k = (char)0;
    while(k != (char)10) {
        fread(&k, 1, 1, pFile);
    }
}

void TParser::read_separator(FILE* pFile) {
    int k = 0;
    int i;
    for(i = 0; i < 8; i++){
        fread(&k, 1, 1, pFile);
    }
}

void TParser::read_buffer(FILE* pFile) {
    int buffer_length;
    int i;
    int k;
    unsigned long long timestamp_buffer_100ns;
    buffer_length = read_word(pFile);
    if (buffer_length == 65535) {
        read_word(pFile);
        read_word(pFile);
        read_word(pFile);
        fread(&k, 1, 1, pFile);
    }
    else {
        for(i = 0; i < 5; i++) {
            read_word(pFile);
        }
        timestamp_buffer_100ns = read_48bits(pFile);
        zKangaroo->set_timestamp_buffer_100ns(
         timestamp_buffer_100ns);
        //zKangaroo->append_event(1099511627776);
        for(i = 0; i < 4; i++) {
            read_48bits(pFile);
        }
        for(i = 0; i < (buffer_length - 21) / 3; i++) {
            read_event(pFile);
        }
        read_separator(pFile);
    }
}

int TParser::read_word(FILE* pFile) {
    int erste_ziffer = 0;
    int zweite_ziffer = 0;
    fread(&erste_ziffer, 1, 1, pFile);
    fread(&zweite_ziffer, 1, 1, pFile);
    return 256 * erste_ziffer + zweite_ziffer;
}

unsigned long long TParser::read_48bits(FILE* pFile) {
    int ziffer_lo, ziffer_mid, ziffer_hi;
    ziffer_lo = read_word(pFile);
    ziffer_mid = read_word(pFile);
    ziffer_hi = read_word(pFile);
    return ziffer_lo
         + ziffer_mid * 65536
         + ziffer_hi * 4294967296;
}

void TParser::read_event(FILE* pFile) {
    unsigned long long rum_praline;
    rum_praline = read_48bits(pFile);
    zKangaroo->append_event(rum_praline);
}