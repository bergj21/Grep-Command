/*
* stringProcessing.cpp
* 11/30/22
* John Berg & Tomer Wenderow
* Purpose: strips non alphanumeric characters from a string
*
*/

#include <iostream>
#include "stringProcessing.h"

// int main() {
//     std::string word = "";
//     std::string result = stripNonAlphaNum(word);
//     std::cout << result << std::endl;

//     word = "@##!!#!@!#COMP-15!!!!!!!";
//     result = stripNonAlphaNum(word);
//     std::cout << result << std::endl;

//     word = "!!Hello.";
//     result = stripNonAlphaNum(word);
//     std::cout << result << std::endl;

//     word = "!!Hello..9/";
//     result = stripNonAlphaNum(word);
//     std::cout << result << std::endl;

//     word = "!!!Hello..0//";
//     result = stripNonAlphaNum(word);
//     std::cout << result << std::endl;

//     word = "!!Hello.#$%^&";
//     result = stripNonAlphaNum(word);
//     std::cout << result << std::endl;
// }

std::string stripNonAlphaNum(std::string input) {
    if (input == "") { return input; }
    while (not isAlphaNum(input.at(0))) {
        input = input.substr(1, input.length() - 1);
    }
    while (not isAlphaNum(input.at(input.length() - 1))) {
            input.pop_back();
    }
    return input;
}

bool isAlphaNum(char c) {
    if ((c >= 48 and c <= 57) or (c >= 65 and c <= 90) or (c >= 97 and c <= 122)) {
        return true;
    }
    return false;
}