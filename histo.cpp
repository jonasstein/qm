// In THisto all time variables are given in nano seconds.

class THisto {
    private:
        unsigned long zNumOfBins;
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
        void set_num_of_bins(unsigned long pNumOfBins);
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
//        cout << zLeftEdges[index] << " < " <<
//              pValue << " < " <<
//              zRightEdges[index] << endl;
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
    if (zFlipperOn) {
        cout << "# flipper : ON" << endl;
    }
    else {
        cout << "# flipper: OFF" << endl;
    }
    cout << "# left edge (ns) | mid point (ns) | "
         << "right egde (ns) | counts | error counts" << endl;
    for(i = 0; i < zNumOfBins; i++) {
/*        cout << zLeftEdges[i] << " "
             << zMidPoints[i] << " "
             << zRightEdges[i] << " "
             << zCounts[i] << endl; */
        printf("%12.3f, %12.3f, %12.3f, %12lli, %12.3f \n", zLeftEdges[i], zMidPoints[i], zRightEdges[i], zCounts[i], zErrorCounts[i]);
    }
}
