
class THisto {
    private:
        unsigned long zNumOfBins;
        float zFirstLeftEdge;
        float zLastRightEdge;
        float zLeftEdges[cMaxNumOfBins];
        float zMidPoints[cMaxNumOfBins];
        float zRightEdges[cMaxNumOfBins];
        float zBinWidth;
        unsigned long long zCounts[cMaxNumOfBins];
    public:
        THisto();
        ~THisto();
        void set_num_of_bins(unsigned long pNumOfBins);
        void set_first_left_edge(float pFirstLeftEdge);
        void set_last_right_edge(float pLastRightEdge);
        void place_bins();
        void fill(float pValue);
        void write_out();
};


THisto::THisto() {
    int i;
    for(i = 0; i < cMaxNumOfBins; i++) {
        zCounts[i] = 0;
    }
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

void THisto::write_out() {
    int i;
    cout << "#  --------" << endl;
    cout << "# | Histo: |" << endl;
    cout << "#  --------" << endl;
    cout << "# left edge | mid point | "
         << "right egde | counts " << endl;
    for(i = 0; i < zNumOfBins; i++) {
      //        cout << zLeftEdges[i] << " "
      //       << zMidPoints[i] << " "
      //        << zRightEdges[i] << " "
      //       << zCounts[i] << endl;
	printf("%f, %f, %f, %llu\n", zLeftEdges[i], zMidPoints[i], zRightEdges[i], zCounts[i]);

    }
}
