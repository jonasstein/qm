// For TParser time variables are given in units of
// 100 nano seconds.

class TSchnibbler {

    private:
    long zIndex;
    char zFilename[512];
    FILE* zOutputfile;
    unsigned char zBuffer[1500];
    int zBufferLength;
    unsigned int zRunNumber;
    public:
        TSchnibbler();
        ~TSchnibbler();
        void open_outputfile();
        void close_outputfile();
        void add_byte(unsigned char pByte);
        void add_byte_to_buffer(unsigned char pByte);
        int end_of_buffer();
        void clear_buffer();
        int read_buffer();
        unsigned long long read_48bits(FILE* pFile);
        int is_super(unsigned long long pRumPraline);
};

TSchnibbler::TSchnibbler() {
    zIndex = 0;
    strcpy(zFilename, "");
    zBufferLength = 0;
    zRunNumber = 0;
    open_outputfile();
}

TSchnibbler::~TSchnibbler() {
    close_outputfile();
}


void TSchnibbler::open_outputfile() {
    char outputfilename[128];
    char nummer[16];
    sprintf(nummer, "%06i", zRunNumber);
    strcpy(outputfilename, "/tmp/run_");
    strcat(outputfilename, nummer);
    zOutputfile = fopen(outputfilename, "wb");
}

void TSchnibbler::close_outputfile() {
    fclose(zOutputfile);
}

void TSchnibbler::add_byte(unsigned char pByte) {
    add_byte_to_buffer(pByte);
    if (end_of_buffer()) {
        read_buffer();
        if (read_buffer()) {
            close_outputfile();
            zRunNumber++;
            open_outputfile();
        }
        clear_buffer();
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
        // In this case it is not a buffer: the word FFFF is the first word
        // of the closing signature, which was read in as buffer_length though
        // it is not.
    }
    else {
        // In this case it is really a buffer and buffer_length is really
        // the length of the buffer in words.

        // Buffer Type, Header Length, Buffer Number, Run-ID, MCPD-ID and
        // Status are skipped. Those would be bytes 2-11.

        // The time stamp of the buffer is read in:
        timestamp_buffer_100ns = zBuffer[16];
        timestamp_buffer_100ns = (timestamp_buffer_100ns << 8) + zBuffer[17];
        timestamp_buffer_100ns = (timestamp_buffer_100ns << 8) + zBuffer[14];
        timestamp_buffer_100ns = (timestamp_buffer_100ns << 8) + zBuffer[15];
        timestamp_buffer_100ns = (timestamp_buffer_100ns << 8) + zBuffer[12];
        timestamp_buffer_100ns = (timestamp_buffer_100ns << 8) + zBuffer[13];

        cout << timestamp_buffer_100ns << " " << endl;

        // The four Parameters Parameter 0..3 are skipped.
        // Those would be bytes 19-42.

        // The events are read in:
        for(i = 0; i < (buffer_length - 21) / 3; i++) {
            j = i * 6 + 43;
            rum_praline = zBuffer[j + 5];
            rum_praline = (rum_praline << 8) + zBuffer[j + 4];
            rum_praline = (rum_praline << 8) + zBuffer[j + 3];
            rum_praline = (rum_praline << 8) + zBuffer[j + 2];
            rum_praline = (rum_praline << 8) + zBuffer[j + 1];
            rum_praline = (rum_praline << 8) + zBuffer[j];
            cout << "Event: " << is_super(rum_praline) << endl;
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
    data_id = pRumPraline & ((1 << 4) - 1);
    return (data_id == cSuperChannel);
}
