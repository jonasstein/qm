#include <iostream>
#include <fstream>
#include <cstring>
#include <math.h>
//#include <ios>
//#include <stdlib.h>
//#include <stdio.h>
using namespace std;

#include "glob.cpp"
#include "histo.cpp"
#include "kangaroo.cpp"
#include "schnibbler.cpp"

void help()
    {
      printf("Syntax: time ./mesyread  FILE.mdat 100");
    }


int main() {
    char filename[512];
    unsigned char byte;
    TSchnibbler* schnibbler;

    strcpy(filename, "180sec_2chan_6kHz_pm_4kHz_FM_4Hz_Trigger1Hz.mdat");

    schnibbler = new TSchnibbler();

    // schnibbler->set_filename(filename);
    //schnibbler->open_file();
    //schnibbler->schnibbel();
    //schnibbler->close_file();

    
    while (1) {
        byte = std::getchar();
        schnibbler->add_byte(byte);
    }




    delete(schnibbler);
}

