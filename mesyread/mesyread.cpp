#include <iostream>
#include <fstream>
#include <cstring>
#include <math.h>
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
    char filename[512];
    char histofilename[512];
    float temperature;
    float voltage;
    int num_of_bins;
    unsigned char byte;
    TParser* parser;
    TKangaroo* kangaroo;
    THisto* histo;
    unsigned long long period_length_1ns;

    if (argc < 2) {
    help();
    }
	       

    num_of_bins = str2int(argv[1]);
    period_length_1ns = str2int(argv[2]);
    strcpy(filename, argv[3]);

    strcpy(histofilename, filename);
    temperature = 0;
    voltage     = 0;
    

    parser = new TParser();
    kangaroo = new TKangaroo();
    histo = new THisto();

    strcpy(histo->zParameters.filename, filename);
    histo->zParameters.temperature = temperature;
    histo->zParameters.voltage     = voltage;

    parser->set_kangaroo(kangaroo);
    
    kangaroo->set_histo(histo);
    kangaroo->set_period_length_1ns(period_length_1ns);

    histo->set_num_of_bins(num_of_bins);
    kangaroo->init_histo();

    histo->set_filename(histofilename);


    while (!parser->end_of_file()) {
        byte = std::getchar();
        parser->add_byte(byte);
    }
    fprintf(stderr, "EOF\n");    


    delete(histo);
    delete(kangaroo);
    delete(parser);
}

