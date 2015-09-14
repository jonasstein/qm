#include <iostream>
#include <fstream>
#include <cstring>
#include <math.h>
using namespace std;

#include "glob.cpp"
#include "schnibbler.cpp"

void help()
    {
      printf("Syntax: time ./mesyread  FILE.mdat 100");
    }


int main(int argc, char** argv) {
    char outputfile_trunc[512];
    unsigned char byte;
    TSchnibbler* schnibbler;

    if (argc < 1) {
      printf("usage: mesycut PathToListModeFile.mdat\n");
    }
    else {
        strcpy(outputfile_trunc, argv[1]);

        schnibbler = new TSchnibbler();

        schnibbler->set_outputfile_trunc(outputfile_trunc);

        schnibbler->open_outputfile();
        
        while (!schnibbler->end_of_file()) {
            byte = std::getchar();
            schnibbler->add_byte(byte);
        }

        schnibbler->close_outputfile();

        fprintf(stderr, "EOF \n");

        delete(schnibbler);
    }
}

