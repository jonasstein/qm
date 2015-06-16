#include <iostream>
#include <fstream>
#include <cstring>
#include <math.h>
//#include <stdlib>
using namespace std;

#include "glob.cpp"
#include "histo.cpp"
#include "kangaroo.cpp"
#include "parser.cpp"

void help()
    {
      printf("Syntax: time ./mesyread  FILE.mdat 1 30.9 4000. 100");
    }


int main(int argc, char **argv) {
    char filename[512];
    int flipper;
    float temperature;
    float voltage;
    int num_of_bins;
    TParser* parser;
    TKangaroo* kangaroo;
    THisto* histo;


    if (argc < 5)
	       {
		 help();
	       }
	       

    strcpy(filename, argv[1]);
    flipper = str2int(argv[2]);
    temperature = str2float(argv[3]);
    voltage = str2float(argv[4]);
    num_of_bins = str2int(argv[5]);

    printf("# ----------------------------------\n"
	   "# filename:     %s \n"
	   "# flipper:      %d \n"
  	   "# temperature:  %1.3f \n"
   	   "# voltage:      %1.0f \n"
   	   "# num of bins:  %d \n"
	   "# ----------------------------------\n"
	   "# \n",
	   filename, flipper, temperature, voltage, num_of_bins);

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

