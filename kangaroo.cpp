// The communist kangaroo gets time variables from
// the parser which are given in 100 nano seconds,
// converts it somewhere to time variables in
// nano seconds, and gives them to the histo in
// nano seconds.
// The kangaroo's time variables given in
// units of 100 nano seconds end with *_100ns, the ones
// given in units of nano seconds end with *_1ns.

class TKangaroo {
    private:
        unsigned long long
         zTimestamps_1ns[cMaxNumOfEvents];
        unsigned long  zDatas[cMaxNumOfEvents];
        unsigned short zDataIDs[cMaxNumOfEvents];
        unsigned short zTrigIDs[cMaxNumOfEvents];
        unsigned short zIDs[cMaxNumOfEvents];
        unsigned long long
         zTimestampBuffer_100ns;
        unsigned long  zNumOfEvents;
        unsigned long long zMaxPeriodeLength_1ns;
        THisto* zHisto;
    public:
        TKangaroo();
        ~TKangaroo();
        void set_timestamp_buffer_100ns(
         unsigned long long pTimestampBuffer_100ns);
        void append_event(unsigned long long pRumPraline);
        void print_first_lines();
        void set_histo(THisto* pHisto);
        void determine_max_periode_length_1ns();
        void set_max_periode_length_1ns(
         unsigned long long  pMaxPeriodeLength_1ns); 
        void fill_histo();
        void write_out();
};

TKangaroo::TKangaroo() {
    zNumOfEvents = 0;
    zTimestampBuffer_100ns = 0;
}

TKangaroo::~TKangaroo() {

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

   timestamp_event_100ns = pRumPraline & ((0b1 << 19) - 1); 
   pRumPraline >>= 19;
   zTimestamps_1ns[zNumOfEvents] = 
    (zTimestampBuffer_100ns + timestamp_event_100ns)*100;

   data = pRumPraline & ((0b1 << 21) - 1);
   pRumPraline >>= 21;
   zDatas[zNumOfEvents] = data;

   data_id = pRumPraline & ((0b1 << 4) - 1);
   pRumPraline >>= 4;
   zDataIDs[zNumOfEvents] = data_id;

   trig_id = pRumPraline & ((0b1 << 3) - 1);
   pRumPraline >>= 3;
   zTrigIDs[zNumOfEvents] = trig_id;

   id = pRumPraline;
   zIDs[zNumOfEvents] = id;

   zNumOfEvents++;
}


void TKangaroo::print_first_lines() {
    int i;
    for(i = 0; i < 20000; i++) {
        cout << zIDs[i] << " " 
            << zTrigIDs[i] << " "
            << zDataIDs[i] << " "
            << zDatas[i] << " "
            << zTimestamps_1ns[i] << endl;
    }
}

void TKangaroo::set_histo(THisto* pHisto) {
    zHisto = pHisto;
}

void TKangaroo::determine_max_periode_length_1ns() {
    unsigned long i;
    unsigned long istart;
    unsigned long long previous_timestamp_1ns;
    unsigned long long max;
    max = 0;
    istart = 0;
    while(zDataIDs[istart] != cTriggerChannel) {
        istart++;
    }
    previous_timestamp_1ns = zTimestamps_1ns[istart];
    for(i = istart + 1; i < zNumOfEvents; i++) {
        if ( zDataIDs[i] == cTriggerChannel) {
            if( (zTimestamps_1ns[i]
             - previous_timestamp_1ns) > max) {
                max = zTimestamps_1ns[i]
                    - previous_timestamp_1ns;
                previous_timestamp_1ns =
                 zTimestamps_1ns[i];
            }
        }
    }
    zMaxPeriodeLength_1ns = max;
}

void TKangaroo::set_max_periode_length_1ns(
 unsigned long long pMaxPeriodeLength_1ns) {
    zMaxPeriodeLength_1ns = pMaxPeriodeLength_1ns;
}

void TKangaroo::fill_histo() {
    unsigned long i;
    unsigned long istart;
    unsigned long long previous_timestamp_1ns;


    zHisto->set_first_left_edge(0.0);
    zHisto->set_last_right_edge(
     (float)zMaxPeriodeLength_1ns);
    zHisto->place_bins();

    istart = 0;
    while(zDataIDs[istart] != cTriggerChannel) {
        istart++;
    }
    previous_timestamp_1ns = zTimestamps_1ns[istart];
    for(i = istart + 1; i < zNumOfEvents; i++) {
        if (zDataIDs[i] == cTriggerChannel) {
            previous_timestamp_1ns = zTimestamps_1ns[i];
        }
        else {
            zHisto->fill((float)(  zTimestamps_1ns[i]
                             - previous_timestamp_1ns ));
        }
    }

}

void TKangaroo::write_out() {
    cout << "# first time stamp (ns): " <<
     zTimestamps_1ns[0] << endl;
    cout << "#  last time stamp (ns): " <<
     zTimestamps_1ns[zNumOfEvents-1] << endl;
    cout << "# => lenth of measurement (ns): " <<
     zTimestamps_1ns[zNumOfEvents-1]
     -zTimestamps_1ns[0] << endl;
}
