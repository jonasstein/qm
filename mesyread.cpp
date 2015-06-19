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
      printf("Syntax: time ./mesyread  FILE.mdat 30.9 4000. 100");
    }


int main(int argc, char **argv) {
    char filename[512];
    char inifilename[512];
    FILE* inifile;
    boost::property_tree::ptree pt;
    float temperature;
    float voltage;
    int num_of_bins;
    TParser* parser;
    TKangaroo* kangaroo;
    THisto* histo;


    if (argc < 4)
	       {
		 help();
	       }
	       

    strcpy(filename, argv[1]);
    num_of_bins = str2int(argv[2]);

    
    strcpy(inifilename, filename);
    strcat(inifilename, ".ini");
    inifile = fopen(inifilename, "r");
    printf("# filename:               %s \n", filename);
    if (inifile != NULL) {
        fclose(inifile);
        read_ini(inifilename, pt);
        temperature = pt.get<float>("T");
        voltage = pt.get<float>("V");
        printf("# temperature:            %1.3f \n", temperature);
        printf("# voltage:                %1.0f \n", voltage);

    }
    else {
        cout << "# temperature:            unknown" << endl;
        cout << "# voltage:                unknown" << endl;
    }


    printf("# num of bins:            %d \n", num_of_bins);

    parser = new TParser();
    kangaroo = new TKangaroo();
    histo = new THisto();

    parser->set_kangaroo(kangaroo);
    parser->set_filename(filename);
    parser->parse();

//    kangaroo->print_first_lines(); // for debug

    kangaroo->set_histo(histo);
    histo->set_num_of_bins(num_of_bins);
    kangaroo->determine_max_periode_length_1ns();
//    kangaroo->set_max_periode_length_1ns(40050000);
    kangaroo->fill_histo();
    histo->calculate_errors();

    kangaroo->write_out();


    histo->write_out();

    delete(histo);
    delete(kangaroo);
    delete(parser);
}

