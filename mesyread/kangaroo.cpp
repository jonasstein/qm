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
        unsigned long long zTimestamps_1ns[cMaxNumOfEvents];
        unsigned long  zDatas[cMaxNumOfEvents];
        unsigned short zDataIDs[cMaxNumOfEvents];
        unsigned short zTrigIDs[cMaxNumOfEvents];
        unsigned short zIDs[cMaxNumOfEvents];
        unsigned long long zTimestampBuffer_100ns;
        unsigned long zNumOfEvents;
        unsigned long zNumOfNeutrons;
        unsigned long long zMaxPeriodeLength_1ns;
        unsigned long zNumOfTriggers;
        unsigned long long zFirstTimestampTrigger_1ns;
        unsigned long long zLastTimestampTrigger_1ns;
        THisto* zHisto;
    public:
        TKangaroo();
        ~TKangaroo();
        void set_timestamp_buffer_100ns(unsigned long long pTimestampBuffer_100ns);
        void append_event(unsigned long long pRumPraline);
        void print_first_lines();
        void set_histo(THisto* pHisto);
        void determine_max_periode_length_1ns();
        void set_max_periode_length_1ns(unsigned long long  pMaxPeriodeLength_1ns);
        void determine_first_and_last_timestamp_trigger_1ns();
        void fill_histo();
        void write_out();
};

TKangaroo::TKangaroo() {
  // constructor
    zNumOfEvents = 0;
    zTimestampBuffer_100ns = 0;
    zMaxPeriodeLength_1ns = 0;
    zNumOfTriggers = 0;
    zFirstTimestampTrigger_1ns = 0;
    zLastTimestampTrigger_1ns = 0;
    zNumOfNeutrons = 0;
}

TKangaroo::~TKangaroo() {
  // destructor
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
   zTimestamps_1ns[zNumOfEvents] = (zTimestampBuffer_100ns + timestamp_event_100ns) * 100;

   data = pRumPraline & ((1 << 21) - 1);
   pRumPraline >>= 21;
   zDatas[zNumOfEvents] = data;

   data_id = pRumPraline & ((1 << 4) - 1);
   pRumPraline >>= 4;
   zDataIDs[zNumOfEvents] = data_id;   
   
   if (data_id == cNeutronChannel) {
       zNumOfNeutrons++;
   }
   else if (data_id == cTriggerChannel) {
       zNumOfTriggers++;
   }
   trig_id = pRumPraline & ((1 << 3) - 1);
   pRumPraline >>= 3;
   zTrigIDs[zNumOfEvents] = trig_id;

   id = pRumPraline;
   zIDs[zNumOfEvents] = id;

   zNumOfEvents++;
}


void TKangaroo::print_first_lines() {
  // TODO: rewrite this to print all lines
    int i;
    for(i = 0; i < 20000; i++) {
      printf("%d, %12d, %12d, %lu, %llu",
      zIDs[i], zTrigIDs[i], zDataIDs[i], zDatas[i], zTimestamps_1ns[i]);
    }
}

void TKangaroo::set_histo(THisto* pHisto) {
    zHisto = pHisto;
}

void TKangaroo::determine_max_periode_length_1ns() {
    unsigned long long i;
    unsigned long long istart;
    unsigned long long previous_timestamp_1ns;
    unsigned long long max;
    if (zNumOfTriggers >= 2) {
        max = 0;
        istart = 0;
        while(zDataIDs[istart] != cTriggerChannel) {
            istart++;
        }
        previous_timestamp_1ns = zTimestamps_1ns[istart];
        istart++;
        for(i = istart; i < zNumOfEvents; i++) {
            if ( zDataIDs[i] == cTriggerChannel) {
                if( (zTimestamps_1ns[i] - previous_timestamp_1ns) > max){
                    max = zTimestamps_1ns[i] - previous_timestamp_1ns;
                }
                previous_timestamp_1ns = zTimestamps_1ns[i];
            }
        }
        zMaxPeriodeLength_1ns = max;
    }
    else {
        zMaxPeriodeLength_1ns = zLastTimestampTrigger_1ns
                              - zFirstTimestampTrigger_1ns;
    }

}

void TKangaroo::set_max_periode_length_1ns(
 unsigned long long pMaxPeriodeLength_1ns) {
    zMaxPeriodeLength_1ns = pMaxPeriodeLength_1ns;
}

void TKangaroo::determine_first_and_last_timestamp_trigger_1ns() {
    unsigned long i;
    int first_trigger_was_there;
    if (zNumOfTriggers >= 2) {
        first_trigger_was_there = 0;
        for(i = 0; i < zNumOfEvents; i++) {
            if (zDataIDs[i] == cTriggerChannel) {
                if (first_trigger_was_there == 0) {
                    zFirstTimestampTrigger_1ns = zTimestamps_1ns[i];
                    first_trigger_was_there++;
                }
//                cout << zTimestamps_1ns[i] << endl;
                zLastTimestampTrigger_1ns = zTimestamps_1ns[i];
            }
        }
    }
    else {
        zFirstTimestampTrigger_1ns = zTimestamps_1ns[0];
        zLastTimestampTrigger_1ns = zTimestamps_1ns[zNumOfEvents - 1];
    }
}

void TKangaroo::fill_histo() {
    unsigned long i;
    unsigned long istart;
    unsigned long long previous_timestamp_1ns;


    zHisto->set_first_left_edge(0.0);
    zHisto->set_last_right_edge(
     (float)zMaxPeriodeLength_1ns);
    zHisto->place_bins();
    zHisto->set_flipper_off();
    istart = 0;
    if (zNumOfTriggers >= 2) {
        while(zDataIDs[istart] != cTriggerChannel) {
            istart++;
        }
    }
    previous_timestamp_1ns = zFirstTimestampTrigger_1ns;
    for(i = istart + 1; i < zNumOfEvents; i++) {
        if (zDataIDs[i] == cTriggerChannel) {
            previous_timestamp_1ns = zTimestamps_1ns[i];
        }
        else if (zDataIDs[i] == cFlipperChannel) {
            zHisto->set_flipper_on();
        }
        else if (zDataIDs[i] == cNeutronChannel) {
            if ( (zTimestamps_1ns[i] > zFirstTimestampTrigger_1ns)
              && (zTimestamps_1ns[i] < zLastTimestampTrigger_1ns) ){
                zHisto->fill((float)(  zTimestamps_1ns[i]
                                 - previous_timestamp_1ns ));
            }
        }
    }

}

void TKangaroo::write_out() {
  //print header
         zHisto->zParameters.first_time_stamp_1ns
             = zTimestamps_1ns[0];
         zHisto->zParameters.last_time_stamp_1ns
             = zTimestamps_1ns[zNumOfEvents - 1];
         zHisto->zParameters.complete_duration_1ns
             = zTimestamps_1ns[zNumOfEvents - 1] - zTimestamps_1ns[0];
         zHisto->zParameters.max_period_length_1ns
             = zMaxPeriodeLength_1ns;
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
