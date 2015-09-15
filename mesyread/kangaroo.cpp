// The communist kangaroo gets time varia
// the parser which are given in 100 nano seconds,
// converts it somewhere to time variables in
// nano seconds, and gives them to the histo in
// nano seconds.
// The kangaroo's time variables given in
// units of 100 nano seconds end with *_100ns, the ones
// given in units of nano seconds end with *_1ns.

class TKangaroo {
    private:
        unsigned long long zTimestamp_1ns;
        unsigned long  zData;
        unsigned short zDataID;
        unsigned short zTrigID;
        unsigned short zID;
        unsigned long long zTimestampBuffer_100ns;
        unsigned long zNumOfEvents;
        unsigned long zNumOfNeutrons;
        unsigned long long zPeriodeLength_1ns;
        unsigned long zNumOfTriggers;
        unsigned long long zFirstTimestamp_1ns;
        unsigned long long zFirstTimestampTrigger_1ns;
        unsigned long long zLastTimestampTrigger_1ns;
        unsigned long zRun;
        char zFilenametrunc[512];
        THisto* zHisto;
    public:
        TKangaroo();
        ~TKangaroo();
        void set_filenametrunc(char* pFilenametrunc);
        void set_timestamp_buffer_100ns(unsigned long long pTimestampBuffer_100ns);
        void append_event(unsigned long long pRumPraline);
        void set_histo(THisto* pHisto);
        void set_period_length_1ns(unsigned long long  pPeriodeLength_1ns);
        void init_histo();
        void fill_histo();
        void write_out();
};

TKangaroo::TKangaroo() {
  // constructor
    zNumOfEvents = 0;
    zTimestampBuffer_100ns = 0;
    zPeriodeLength_1ns = 0;
    zNumOfTriggers = 0;
    zFirstTimestampTrigger_1ns = 0;
    zLastTimestampTrigger_1ns = 0;
    zNumOfNeutrons = 0;
    zRun = 0;
}

TKangaroo::~TKangaroo() {
  // destructor
}

void TKangaroo::set_filenametrunc(char* pFilenametrunc) {
    strcpy(zFilenametrunc, pFilenametrunc);
}

void TKangaroo::set_timestamp_buffer_100ns(
 unsigned long long pTimestampBuffer_100ns) {
    zTimestampBuffer_100ns = pTimestampBuffer_100ns;
}

void TKangaroo::append_event(
 unsigned long long pRumPraline) {
    unsigned long long timestamp_event_100ns;
    unsigned long data;
    unsigned short data_id;
    unsigned short trig_id;
    unsigned short id;

    timestamp_event_100ns = pRumPraline & (((unsigned long long)1 << 19) - (unsigned long long)1); 
    pRumPraline >>= 19;
    zTimestamp_1ns = (zTimestampBuffer_100ns + timestamp_event_100ns) * 100;
    if (zNumOfEvents == 0) {
        zFirstTimestamp_1ns = (zTimestampBuffer_100ns + timestamp_event_100ns) * 100;
    }

    data = pRumPraline & ((1 << 21) - 1);
    pRumPraline >>= 21;
    zData = data;

    data_id = pRumPraline & ((1 << 4) - 1);
    pRumPraline >>= 4;
    zDataID = data_id;   
    
    if (zDataID == cTriggerChannel) {
        zNumOfTriggers++;
        if (zNumOfTriggers == 1) {
            zFirstTimestampTrigger_1ns = zTimestamp_1ns;
        }
        zLastTimestampTrigger_1ns = zTimestamp_1ns;
        zHisto->pop();
    }
    trig_id = pRumPraline & ((1 << 3) - 1);
    pRumPraline >>= 3;
    zTrigID = trig_id;
 
    id = pRumPraline;
    zID = id;


    if (zNumOfTriggers > 0) {
        if (zDataID == cFlipperChannel) {
            zHisto->set_flipper_on();
        }
        else if (zDataID == cNeutronChannel) {
            zNumOfNeutrons++;
            zHisto->fill(
                (float)(zTimestamp_1ns - zLastTimestampTrigger_1ns)
            );
        }
    }

    if (zDataID == cSuperChannel) {
        zRun++;
        zHisto->pop();
        write_out();
        zHisto->write_out();
        fprintf(stderr, "%lu \n", zRun);
        init_histo();
    }
    zNumOfEvents++;

}

void TKangaroo::set_histo(THisto* pHisto) {
    zHisto = pHisto;
}

void TKangaroo::set_period_length_1ns(
 unsigned long long pPeriodeLength_1ns) {
    zPeriodeLength_1ns = pPeriodeLength_1ns;
}

void TKangaroo::init_histo() {
    char filename[512];
    char endung[32];
    sprintf(endung, "_RUN%06lu", zRun);
    strcpy(filename, zFilenametrunc);
    strcat(filename, endung);
    zHisto->set_filename(filename);
    zHisto->set_first_left_edge(0.0);
    zHisto->set_last_right_edge(
     (double)zPeriodeLength_1ns);
    zHisto->place_bins();
    zHisto->set_flipper_off();
}

void TKangaroo::write_out() {
  //print header
         zHisto->zParameters.first_time_stamp_1ns
             = zFirstTimestamp_1ns;
         zHisto->zParameters.last_time_stamp_1ns
             = zTimestamp_1ns;
         zHisto->zParameters.complete_duration_1ns
             = zTimestamp_1ns - zFirstTimestamp_1ns;
         zHisto->zParameters.period_length_1ns
             = zPeriodeLength_1ns;
         zHisto->zParameters.num_of_neutrons
             = zNumOfNeutrons;
         zHisto->zParameters.num_of_triggers
             = zNumOfTriggers;
         zHisto->zParameters.first_trigger_time_stamp_1ns
             = zFirstTimestampTrigger_1ns;
         zHisto->zParameters.last_trigger_time_stamp_1ns
             = zLastTimestampTrigger_1ns;
         zHisto->zParameters.triggered_duration_1ns
             = zLastTimestampTrigger_1ns - zFirstTimestampTrigger_1ns;
}
