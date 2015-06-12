#include <iostream>
#include <fstream>
#include <cstring>
//#include <stdlib>
using namespace std;

#include "glob.cpp"
#include "histo.cpp"
#include "kangaroo.cpp"
#include "parser.cpp"

int main(int argc, char **argv) {
    char filename[512];
    int flipper;
    float temperature;
    float voltage;
    int num_of_bins;
    TParser* parser;
    TKangaroo* kangaroo;
    THisto* histo;



    strcpy(filename, argv[1]);
    flipper = str2int(argv[2]);
    temperature = str2float(argv[3]);
    voltage = str2float(argv[4]);
    num_of_bins = str2int(argv[5]);
    cout << "#----------------------------------" << endl;
    cout << "#filename:    " << filename << endl;
    cout << "#flipper:     " << flipper << endl;
    cout << "#temperature: " << temperature << endl;
    cout << "#voltage:     " << voltage << endl;
    cout << "#num of bins: " << num_of_bins << endl;
    cout << "#----------------------------------" << endl;
    cout << "#" << endl;

    parser = new TParser();
    kangaroo = new TKangaroo();
    histo = new THisto();

    parser->set_kangaroo(kangaroo);
    parser->set_filename(filename);
    parser->parse();

    kangaroo->print_first_lines();

    kangaroo->set_histo(histo);
    histo->set_num_of_bins(num_of_bins);
    kangaroo->determine_max_periode_length_1ns();
//    kangaroo->set_max_periode_length_1ns(40050000);
    kangaroo->fill_histo();

    kangaroo->write_out();

    histo->write_out();

    delete(histo);
    delete(kangaroo);
    delete(parser);
}

