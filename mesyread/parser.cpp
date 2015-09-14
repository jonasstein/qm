// For TParser time variables are given in units of
// 100 nano seconds.

class TParser {

    private:
    TKangaroo* zKangaroo;
    unsigned char zBuffer[1500];
    int zBufferLength;
    unsigned long long zTimestampBuffer_100ns;
    int zStillInHeader;
    public:
        TParser();
        ~TParser();
        void set_kangaroo(TKangaroo* pKangaroo);
        void add_byte(unsigned char pByte);
        void add_byte_to_buffer(unsigned char pByte);
        int end_of_buffer();
        int end_of_header();
        int end_of_file();
        void clear_buffer();
        void read_buffer();
};

TParser::TParser() {
    zBufferLength = 0;
    zTimestampBuffer_100ns = 0;
    zStillInHeader = 1;
}

TParser::~TParser() {

}

void TParser::set_kangaroo(TKangaroo* pKangaroo) {
    zKangaroo = pKangaroo;
}

void TParser::add_byte(unsigned char pByte) {
    add_byte_to_buffer(pByte);
    if (zStillInHeader) {
        if (end_of_header()) {
            clear_buffer();
            zStillInHeader = 0;
        }
    }
    else {
        if (end_of_buffer()) {
            read_buffer();
            clear_buffer();
        }
    }
}

void TParser::add_byte_to_buffer(unsigned char pByte) {
    zBuffer[zBufferLength] = pByte;
    zBufferLength++;
}

void TParser::clear_buffer() {
    zBufferLength = 0;
}

int TParser::end_of_buffer() {
    if (zBufferLength < 4) {
        return 0;
    }
    if (zBuffer[zBufferLength-1] != (unsigned char)-86) { // AA
        return 0;
    }
    if (zBuffer[zBufferLength-2] != (unsigned char)-86) { // AA
        return 0;
    }
    if (zBuffer[zBufferLength-3] != (unsigned char)85) { // 55
        return 0;
    }
    if (zBuffer[zBufferLength-4] != (unsigned char)85) { // 55
        return 0;
    }
    if (zBuffer[zBufferLength-5] != (unsigned char)-1) { // FF
        return 0;
    }
    if (zBuffer[zBufferLength-6] != (unsigned char)-1) { // FF
        return 0;
    }
    if (zBuffer[zBufferLength-7] != (unsigned char)0) { // 00
        return 0;
    }
    if (zBuffer[zBufferLength-8] != (unsigned char)0) { // 00
        return 0;
    }
    return 1;
}

int TParser::end_of_header() {
    if (zBufferLength < 4) {
        return 0;
    }
    if (zBuffer[zBufferLength-8] != (unsigned char)0) { // 00
        return 0;
    }
    if (zBuffer[zBufferLength-8] != (unsigned char)0) { // 00
        return 0;
    }
    if (zBuffer[zBufferLength-5] != (unsigned char)85) { // 55
        return 0;
    }
    if (zBuffer[zBufferLength-6] != (unsigned char)85) { // 55
        return 0;
    }
    if (zBuffer[zBufferLength-3] != (unsigned char)-86) { // AA
        return 0;
    }
    if (zBuffer[zBufferLength-4] != (unsigned char)-86) { // AA
        return 0;
    }
    if (zBuffer[zBufferLength-1] != (unsigned char)-1) { // FF
        return 0;
    }
    if (zBuffer[zBufferLength-2] != (unsigned char)-1) { // FF
        return 0;
    }
    return 1;
}

int TParser::end_of_file() {
    if (zBufferLength < 8) {
        return 0;
    }
    if (zBuffer[zBufferLength-8] != (unsigned char)-1) { // FF
        return 0;
    }
    if (zBuffer[zBufferLength-7] != (unsigned char)-1) { // FF
        return 0;
    }
    if (zBuffer[zBufferLength-6] != (unsigned char)-86) { // AA
        return 0;
    }
    if (zBuffer[zBufferLength-5] != (unsigned char)-86) { // AA
        return 0;
    }
    if (zBuffer[zBufferLength-4] != (unsigned char)85) { // 55
        return 0;
    }
    if (zBuffer[zBufferLength-3] != (unsigned char)85) { // 55
        return 0;
    }
    if (zBuffer[zBufferLength-2] != (unsigned char)0) { // 00
        return 0;
    }
    if (zBuffer[zBufferLength-1] != (unsigned char)0) { // 00
        return 0;
    }
    return 1;
}

void TParser::read_buffer() {
    int buffer_length;
    int i;
    tWord buffer_type;
    int j;
    unsigned long long rum_praline;
    
    
    buffer_length = (zBuffer[0] << 8) + zBuffer[1]; // bytes 0, 1
    if (buffer_length == ((1 << 16) - 1)) {
        // In this case it is not a buffer: The word FFFF is the first word
        // of the closing signature, which was read in as buffer_length though
        // it is not.
    }
    else {
        // In this case it is really a buffer and buffer_length is really
        // the length of the buffer in words.

        buffer_type = chars2word(zBuffer[2], zBuffer[3]); // bytes 2, 3
        
        // Header Length, Buffer Number, Run-ID, MCPD-ID and Status are 
        // skipped. Those would be bytes 4-11.

        // The time stamp of the buffer is read in:

        zTimestampBuffer_100ns = threeword2ull(
            words2threeword(
                chars2word(zBuffer[12], zBuffer[13]), // lo
                chars2word(zBuffer[14], zBuffer[15]), // mid
                chars2word(zBuffer[16], zBuffer[17])  // hi
            )
        );

        zKangaroo->set_timestamp_buffer_100ns(zTimestampBuffer_100ns);

        // The four Parameters Parameter 0..3 are skipped.
        // Those would be bytes 19-42.

        // The events are read in:
        for(i = 0; i < (buffer_length*2 - 42) / 6; i++) {
            j = i * 6 + 42;
            rum_praline = threeword2ull(
                words2threeword(
                    chars2word(zBuffer[j    ], zBuffer[j + 1]), // lo
                    chars2word(zBuffer[j + 2], zBuffer[j + 3]), // mid
                    chars2word(zBuffer[j + 4], zBuffer[j + 5])  // hi
                )
            );
            zKangaroo->append_event(rum_praline);
        }
        // Then there is the buffer seperator.
    }
}


