#include <iostream>
#include <fstream>
//#include <stdlib>
using namespace std;

#include "parser.cpp"

int main() {
    TParser* parser;

    


    parser = new TParser();
//    parser->hallowelt();

    parser->parse();
    delete(parser);
}

