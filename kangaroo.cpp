class TKangaroo {
    private:
        unsigned long  zTimestamps[cMaxNumOfEvents];
        unsigned long  zDatas[cMaxNumOfEvents];
        unsigned short zDataIDs[cMaxNumOfEvents];
        unsigned short zTrigIDs[cMaxNumOfEvents];
        unsigned short zIDs[cMaxNumOfEvents];
        unsigned long  zTimestampBuffer;
        unsigned long  zNumOfEvents;
        unsigned long zMaxPeriodeLength;
        THisto* zHisto;
    public:
        TKangaroo();
        ~TKangaroo();
        void set_timestamp_buffer(
         unsigned long pTimestampBuffer);
        void append_event(unsigned long long pRumPraline);
        void print_first_lines();
        void set_histo(THisto* pHisto);
        void determine_max_periode_length();
        void set_max_periode_length(
         unsigned long pMaxPeriodeLength); 
        void fill_histo();
};

TKangaroo::TKangaroo() {
    zNumOfEvents = 0;
    zTimestampBuffer = 0;
}

TKangaroo::~TKangaroo() {

}

void TKangaroo::set_timestamp_buffer(
 unsigned long pTimestampBuffer) {
    zTimestampBuffer = pTimestampBuffer;
}

void TKangaroo::append_event(
 unsigned long long pRumPraline) {
   unsigned long timestamp_event;
   unsigned long data;
   unsigned short data_id;
   unsigned short trig_id;
   unsigned short id;

   timestamp_event = pRumPraline % 524288;
   pRumPraline -= timestamp_event;
   pRumPraline /= 524288;
   zTimestamps[zNumOfEvents] = 
    zTimestampBuffer + timestamp_event;

   data = pRumPraline % 2097152;
   pRumPraline -= data;
   pRumPraline /= 2097152;
   zDatas[zNumOfEvents] = data;

   data_id = pRumPraline % 16;
   pRumPraline -= data_id;
   pRumPraline /= 16;
   zDataIDs[zNumOfEvents] = data_id;

   trig_id = pRumPraline % 8;
   pRumPraline -= trig_id;
   pRumPraline /= 8;
   zTrigIDs[zNumOfEvents] = trig_id;

   id = pRumPraline;
   zIDs[zNumOfEvents] = id;

   zNumOfEvents++;
}


void TKangaroo::print_first_lines() {
    int i;
    for(i = 0; i < 20; i++) {
        cout << zIDs[i] << " " 
            << zTrigIDs[i] << " "
            << zDataIDs[i] << " "
            << zDatas[i] << " "
            << zTimestamps[i] << endl;
    }
}

void TKangaroo::set_histo(THisto* pHisto) {
    zHisto = pHisto;
}

void TKangaroo::determine_max_periode_length() {
    unsigned long i;
    unsigned long istart;
    unsigned long previous_timestamp;
    unsigned long max;
    max = 0;
    istart = 0;
    while(zDataIDs[istart] != cTriggerChannel) {
        istart++;
    }
    previous_timestamp = zTimestamps[istart];
    for(i = istart + 1; i < zNumOfEvents; i++) {
        if ( zDataIDs[i] == cTriggerChannel) {
            if( (zTimestamps[i] - previous_timestamp)
             > max) {
                max = zTimestamps[i] - previous_timestamp;
                previous_timestamp = zTimestamps[i];
            }
        }
    }
    zMaxPeriodeLength = max;
}

void TKangaroo::set_max_periode_length(
 unsigned long pMaxPeriodeLength) {
    zMaxPeriodeLength = pMaxPeriodeLength;
}

void TKangaroo::fill_histo() {
    unsigned long i;
    unsigned long istart;
    unsigned long previous_timestamp;


    zHisto->set_first_left_edge(0.0);
    zHisto->set_last_right_edge((float)zMaxPeriodeLength);
    zHisto->place_bins();

    istart = 0;
    while(zDataIDs[istart] != cTriggerChannel) {
        istart++;
    }
    previous_timestamp = zTimestamps[istart];
    for(i = istart + 1; i < zNumOfEvents; i++) {
        if (zDataIDs[i] == cTriggerChannel) {
            previous_timestamp = zTimestamps[i];
        }
        else {
            zHisto->fill((float)(  zTimestamps[i]
                                 - previous_timestamp ));
        }
    }

}
