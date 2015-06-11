#include <iostream>
#include <fstream>
//#include <stdlib>
using namespace std;

#include "parser.cpp"

int main(int argc, char **argv) {
    TParser* parser;

    // argc = number of parameters
    // argv[1] contains first parameter


    parser = new TParser();
//    parser->hallowelt();

    parser->parse();
    delete(parser);
}

