// In THisto all time variables are given in nano seconds.

class THisto {
    private:
        unsigned long zNumOfBins;
        char zFileName[512];
        double zFirstLeftEdge;
        double zLastRightEdge;
        double zLeftEdges[cMaxNumOfBins];
        double zMidPoints[cMaxNumOfBins];
        double zRightEdges[cMaxNumOfBins];
        double zBinWidth;
        unsigned long long zCounts[cMaxNumOfBins];
        double zErrorCounts[cMaxNumOfBins];
        unsigned short zFlipperOn;
    public:
        THisto();
        ~THisto();
        tParameters zParameters;
        void set_num_of_bins(unsigned long pNumOfBins);
        void set_filename(char* pFileName);
        void set_temperature(long pTemperature);
        void set_voltage(long pVoltage);
        void set_first_left_edge(float pFirstLeftEdge);
        void set_last_right_edge(float pLastRightEdge);
        void place_bins();
        void fill(float pValue);
        void set_flipper_on();
        void set_flipper_off();
        void calculate_errors();
        void write_out();
};


THisto::THisto() {
    int i;
    for(i = 0; i < cMaxNumOfBins; i++) {
        zCounts[i] = 0;
    }
    zFlipperOn = 0;
}

THisto::~THisto() {

}

void THisto::set_num_of_bins(unsigned long pNumOfBins) {
    zNumOfBins = pNumOfBins;
}

void THisto::set_filename(char* pFileName) {
    strcpy(zFileName, pFileName);
}

void THisto::set_first_left_edge(float pFirstLeftEdge) {
    zFirstLeftEdge = pFirstLeftEdge;
}

void THisto::set_last_right_edge(float pLastRightEdge) {
    zLastRightEdge = pLastRightEdge;
}

void THisto::place_bins() {
    unsigned long i;
    float edge;
    zBinWidth = (zLastRightEdge - zFirstLeftEdge) /
                (float)zNumOfBins;
    zLeftEdges[0] = zFirstLeftEdge;
    for(i = 1; i < zNumOfBins; i++) {
        edge = zFirstLeftEdge + i * zBinWidth;
        zLeftEdges[i] = edge;
        zRightEdges[i - 1] = edge;
        zMidPoints[i - 1] = (edge + zLeftEdges[i - 1]) / 2;
    }
    zRightEdges[zNumOfBins - 1] = zLastRightEdge;
    zMidPoints[zNumOfBins - 1] =
        (zLeftEdges[zNumOfBins - 1]
         + zRightEdges[zNumOfBins - 1]) / 2;
}

void THisto::fill(float pValue) {
    unsigned long index;
    index = (unsigned long)
            ((pValue - zFirstLeftEdge) / zBinWidth);
    if (index < zNumOfBins) {
        zCounts[index]++;
    }
}

void THisto::set_flipper_on() {
    zFlipperOn = 1;
}

void THisto::set_flipper_off() {
    zFlipperOn = 0;
}

void THisto::calculate_errors() {
    unsigned long i;
    for(i = 0; i < zNumOfBins; i++) {
        zErrorCounts[i] = sqrt(zCounts[i]);
    }
}

void THisto::write_out() {
    unsigned long i;
    FILE* file;
    
    file = fopen(zFileName, "w");
    fprintf(file,
           "# filename:                      %s \n"
           "# num of bins:                   %lu \n"
           "# temperature:                   %f \n"
           "# voltage:                       %f \n"
           "# first  time stamp (ns):        %llu \n"
           "# last   time stamp (ns):        %llu \n"
           "# complete duration (ns):        %llu \n"
           "# max period length (ns):        %llu \n"
           "# num of neutrons:               %lu \n"
           "# num of triggers:               %lu \n"
           "# first trigger time stamp (ns): %llu \n"
           "# last trigger time stamp (ns):  %llu \n"
           "# triggered duration (ns):       %llu \n",
           zParameters.filename,
           zNumOfBins,
           zParameters.temperature,
           zParameters.voltage,
           zParameters.first_time_stamp_1ns,
           zParameters.last_time_stamp_1ns,
           zParameters.complete_duration_1ns,
           zParameters.max_period_length_1ns,
           zParameters.num_of_neutrons,
           zParameters.num_of_triggers,
           zParameters.first_trigger_time_stamp_1ns,
           zParameters.last_trigger_time_stamp_1ns,
           zParameters.triggered_duration_1ns);
  

    if (zFlipperOn) {
        fprintf(file, "# flipper:                       ON\n");
    }
    else {
        fprintf(file, "# flipper:                       OFF\n");
    }
    fprintf(file, "# --------------------------------------------\n");
    fprintf(file, "            left,              mid,            right,             CNTS,             SQRT\n");

    for(i = 0; i < zNumOfBins; i++) {
        fprintf(file, "%16.1f, %16.1f, %16.1f, %16lli, %16.1f \n", zLeftEdges[i], zMidPoints[i], zRightEdges[i], zCounts[i], zErrorCounts[i]);
    }
    fclose(file);
}
