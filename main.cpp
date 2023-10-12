/*
* main.cpp
* 12/5/22
* John Berg, Tomer Wenderow
* Purpose: implementation for the gerp class, runs the gerp program
*
*/

#include <iostream>

#include "gerp.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: ./gerp inputDirectory outputFile" << std::endl;
        return 1;
    } 
    gerp g;
    g.run(argv[1], argv[2]);

    return 0;
}