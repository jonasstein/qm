#include <iostream>
#include <fstream>
#include <cstring>
#include <math.h>
#include <boost/property_tree/ini_parser.hpp>
//#include <stdlib>
using namespace std;

#include "glob.cpp"
#include "histo.cpp"
#include "kangaroo.cpp"
#include "parser.cpp"

void help()
    {
      printf("Syntax: time ./mesyread  FILE.mdat 100");
    }


int main(int argc, char **argv) {
    char filenametrunc[512];
    char inifilename[512];
    char histofilename[512];
    FILE* inifile;
    boost::property_tree::ptree pt;
    float temperature;
    float voltage;
    int num_of_bins;
    unsigned char byte;
    TParser* parser;
    TKangaroo* kangaroo;
    THisto* histo;
    unsigned long long period_length_1ns;
    int count;


    if (argc < 2) {
    help();
    }
	       

    num_of_bins = str2int(argv[1]);
    period_length_1ns = str2ull(argv[2]);
    strcpy(filenametrunc, argv[3]);

    strcpy(histofilename, filenametrunc);
    strcat(histofilename, ".csv");
    strcpy(inifilename, filenametrunc);
    strcat(inifilename, ".ini");
    inifile = fopen(inifilename, "r");
    if (inifile != NULL) {
        fclose(inifile);
        read_ini(inifilename, pt);
        temperature = pt.get<float>("T");
        voltage = pt.get<float>("U");
    }
    else {
         temperature = 0;
         voltage     = 0;
    }

    parser = new TParser();
    kangaroo = new TKangaroo();
    histo = new THisto();

    strcpy(histo->zParameters.filename, filenametrunc);
    histo->zParameters.temperature = temperature;
    histo->zParameters.voltage     = voltage;

    parser->set_kangaroo(kangaroo);
    
    kangaroo->set_histo(histo);
    kangaroo->set_period_length_1ns(period_length_1ns);

    histo->set_num_of_bins(num_of_bins);
    kangaroo->set_filenametrunc(filenametrunc);
    kangaroo->init_histo();


    count = 0;
    while (!parser->end_of_file()) {
        byte = std::getchar();
        parser->add_byte(byte);
        count++;
        if (count == cWriteHistoEveryNBytes) {
            kangaroo->write_out();
            histo->write_out();
            count = 0;
        }
    }
//    fprintf(stderr, "EOF\n");    
//    kangaroo->write_out();
//    histo->write_out();


    delete(histo);
    delete(kangaroo);
    delete(parser);
}

