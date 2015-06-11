class TKangaroo {
    private:
        unsigned long  zTimestamps[cMaxNumOfEvents];
        unsigned long  zDatas[cMaxNumOfEvents];
        unsigned short zDataIDs[cMaxNumOfEvents];
        unsigned short zTrigIDs[cMaxNumOfEvents];
        unsigned short zIDs[cMaxNumOfEvents];
        unsigned long  zTimestampBuffer;
        unsigned long  zNumOfEvents;
    public:
        TKangaroo();
        ~TKangaroo();
        void set_timestamp_buffer(
         unsigned long pTimestampBuffer);
        void append_event(unsigned long long pRumpraline);
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
 unsigned long long pRumpraline) {
   unsigned long timestamp_event;
   unsigned long data;
   unsigned short data_id;
   unsigned short trig_id;
   unsigned short id;

   timestamp_event = pRumpraline % 524288;
   pRumpraline -= timestamp_event;
   pRumpraline /= 524288;
   zTimestamps[zNumOfEvents] = 
    zTimestampBuffer + timestamp_event;

   data = pRumpraline % 2097152;
   pRumpraline -= data;
   pRumpraline /= 2097152;
   zDatas[zNumOfEvents] = data;

   data_id = pRumpraline % 16;
   pRumpraline -= data_id;
   pRumpraline /= 16;
   zDataIDs[zNumOfEvents] = data_id;

   trig_id = pRumpraline % 8;
   pRumpraline -= trig_id;
   pRumpraline /= 8;
   zTrigIDs[zNumOfEvents] = trig_id;

   id = pRumpraline;
   zIDs[zNumOfEvents] = id;

/*   cout << zIDs[zNumOfEvents] << " " 
        << zTrigIDs[zNumOfEvents] << " "
        << zDataIDs[zNumOfEvents] << " "
        << zDatas[zNumOfEvents] << " "
        << zTimestamps[zNumOfEvents] << endl;
*/
   zNumOfEvents++;
}


