// For TParser time variables are given in units of
// 100 nano seconds.

class TSchnibbler {

    private:
    char zOutputfileTrunc[512];
    FILE* zOutputfile;
    unsigned char zBuffer[1500];
    int zBufferLength;
    unsigned int zRunNumber;
    unsigned long long zTimestampBuffer_100ns;
    int zStillInHeader;
    public:
        TSchnibbler();
        ~TSchnibbler();
        void set_outputfile_trunc(char* pOutputfileTrunc);
        void open_outputfile();
        void close_outputfile();
        void write_header();
        void write_closing_signature();
        void add_byte(unsigned char pByte);
        void add_byte_to_buffer(unsigned char pByte);
        int end_of_buffer();
        int end_of_header();
        void clear_buffer();
        int read_buffer();
        int is_super(unsigned long long pRumPraline);
};

TSchnibbler::TSchnibbler() {
    zBufferLength = 0;
    zRunNumber = 0;
    zTimestampBuffer_100ns = 0;
    zStillInHeader = 1;
}

TSchnibbler::~TSchnibbler() {
}

void TSchnibbler::set_outputfile_trunc(char* pOutputfileTrunc) {
    strcpy(zOutputfileTrunc, pOutputfileTrunc);
}

void TSchnibbler::open_outputfile() {
    char outputfilename[128];
    char nummer[16];
    sprintf(nummer, "%06i", zRunNumber);
    strcpy(outputfilename, zOutputfileTrunc);
    strcat(outputfilename, "_RUN");
    strcat(outputfilename, nummer);
    zOutputfile = fopen(outputfilename, "wb");
}

void TSchnibbler::close_outputfile() {
    write_closing_signature();
    fclose(zOutputfile);
}

void TSchnibbler::write_header() {
    fputs("This file was created by mesyschnibbler.\n"
          "The header consists of two lines.\n", zOutputfile);
    // Here comes the header seperator:
    fputc((char)0, zOutputfile);   // 00
    fputc((char)0, zOutputfile);   // 00
    fputc((char)85, zOutputfile);  // 55
    fputc((char)85, zOutputfile);  // 55
    fputc((char)-86, zOutputfile); // AA
    fputc((char)-86, zOutputfile); // AA
    fputc((char)-1, zOutputfile);  // FF
    fputc((char)-1, zOutputfile);  // FF
}

void TSchnibbler::write_closing_signature() {
    fputc((char)-1, zOutputfile);  // FF
    fputc((char)-1, zOutputfile);  // FF
    fputc((char)-86, zOutputfile); // AA
    fputc((char)-86, zOutputfile); // AA
    fputc((char)85, zOutputfile);  // 55
    fputc((char)85, zOutputfile);  // 55
    fputc((char)0, zOutputfile);   // 00
    fputc((char)0, zOutputfile);   // 00
}

void TSchnibbler::add_byte(unsigned char pByte) {
    char timefile_name[128];
    FILE* timefile;


    add_byte_to_buffer(pByte);
    fputc(pByte, zOutputfile);

    if (zStillInHeader) {
        if (end_of_header()) {
            clear_buffer();
            zStillInHeader = 0;
        }
    }
    else {
        if (end_of_buffer()) {
            if (read_buffer()) {
                close_outputfile();
                zRunNumber++;                
                strcpy(timefile_name, zOutputfileTrunc);
                strcat(timefile_name, "_TIME");
                timefile = fopen(timefile_name, "a");
                fprintf(timefile,
                    "RUN %06i TIME %015llu \n",
                    zRunNumber, zTimestampBuffer_100ns);
                fclose(timefile);
                open_outputfile();
                write_header();
            }
            clear_buffer();
        }
    }
}

void TSchnibbler::add_byte_to_buffer(unsigned char pByte) {
    zBuffer[zBufferLength] = pByte;
    zBufferLength++;
}

void TSchnibbler::clear_buffer() {
    zBufferLength = 0;
}

int TSchnibbler::end_of_buffer() {
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

int TSchnibbler::end_of_header() {
    if (zBufferLength < 4) {
        return 0;
    }
    if (zBuffer[zBufferLength-1] != (unsigned char)-1) { // FF
        return 0;
    }
    if (zBuffer[zBufferLength-2] != (unsigned char)-1) { // FF
        return 0;
    }
    if (zBuffer[zBufferLength-3] != (unsigned char)-86) { // AA
        return 0;
    }
    if (zBuffer[zBufferLength-4] != (unsigned char)-86) { // AA
        return 0;
    }
    if (zBuffer[zBufferLength-5] != (unsigned char)85) { // 55
        return 0;
    }
    if (zBuffer[zBufferLength-6] != (unsigned char)85) { // 55
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

int TSchnibbler::read_buffer() {
    int buffer_length;
    int i;
    unsigned long long timestamp_buffer_100ns;
    int j;
    unsigned long long rum_praline;
    int buffer_contains_supersignal;

    buffer_contains_supersignal = 0;

    buffer_length = (zBuffer[0] << 8) + zBuffer[1]; // bytes 0, 1
    if (buffer_length == ((1 << 16) - 1)) {
        // In this case it is not a buffer: The word FFFF is the first word
        // of the closing signature, which was read in as buffer_length though
        // it is not.
    }
    else {
        // In this case it is really a buffer and buffer_length is really
        // the length of the buffer in words.

        // Buffer Type, Header Length, Buffer Number, Run-ID, MCPD-ID and
        // Status are skipped. Those would be bytes 2-11.

        // The time stamp of the buffer is read in:

        timestamp_buffer_100ns = threeword2ull(
            words2threeword(
                chars2word(zBuffer[12], zBuffer[13]), // lo
                chars2word(zBuffer[14], zBuffer[15]), // mid
                chars2word(zBuffer[16], zBuffer[17])  // hi
            )
        );


        zTimestampBuffer_100ns = timestamp_buffer_100ns;

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
            if (is_super(rum_praline)) {
                buffer_contains_supersignal = 1;
            }
        }

        // Then there is the buffer seperator.

    }
    return buffer_contains_supersignal;
}

int TSchnibbler::is_super(unsigned long long pRumPraline) {
    unsigned short data_id;

    pRumPraline >>= 40;
    data_id = pRumPraline & (((unsigned long)1 << 4) - 1);
    return (data_id == cSuperChannel);
}
