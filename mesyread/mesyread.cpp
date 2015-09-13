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
    char filename[512];
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
    int count;

    if (argc < 1) {
    help();
    }
	       

    num_of_bins = str2int(argv[1]);
    strcpy(filename, argv[2]);

    strcpy(histofilename, filename);
    strcat(histofilename, ".csv");
    strcpy(inifilename, filename);
    strcat(inifilename, ".ini");
    inifile = fopen(inifilename, "r");
    if (inifile != NULL) {
        fclose(inifile);
        read_ini(inifilename, pt);
        temperature = pt.get<float>("T");
        voltage = pt.get<float>("U");
//        printf("# temperature:             %1.3f \n" 
//               "# voltage (Vp):            %1.0f \n", temperature, voltage);
    }
    else {
         temperature = 0;
         voltage     = 0;
//        cout << "# temperature:                   unknown" << endl;
//        cout << "# voltage:                       unknown" << endl;
    }

    


    parser = new TParser();
    kangaroo = new TKangaroo();
    histo = new THisto();

    strcpy(histo->zParameters.filename, filename);
    histo->zParameters.temperature = temperature;
    histo->zParameters.voltage     = voltage;

    parser->set_kangaroo(kangaroo);
    
    kangaroo->set_histo(histo);

    histo->set_num_of_bins(num_of_bins);
    histo->set_filename(histofilename);

    count = 0;
    while (1) {
        byte = std::getchar();
        parser->add_byte(byte);
        count++;
        if (count == 10000) {
            kangaroo->determine_first_and_last_timestamp_trigger_1ns();
            kangaroo->determine_max_periode_length_1ns();
            kangaroo->fill_histo();
            histo->calculate_errors();
            kangaroo->write_out();
            histo->write_out();
            count = 0;
        }
    }

    delete(histo);
    delete(kangaroo);
    delete(parser);
}

