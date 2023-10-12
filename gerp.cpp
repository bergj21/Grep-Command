/*
* gerp.cpp
* 12/5/22
* John Berg, Tomer Wenderow
* Purpose: Implementation for the gerp class. Handles the traversal of the 
*       directory, fills the hash table, runs the command loop that queries 
*       for user input, and the printing to an output file.
*       
*
*/
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "FSTree.h"
#include "gerp.h"

/*
 * name:      constructor
 * purpose:   creates an instance of the gerp class
 * arguments: none
 * returns:   none
 * effects:   no need to initialize or create anything
 */
gerp::gerp(){}

/*
 * name:      destructor
 * purpose:   recycle allocated memory
 * arguments: none
 * returns:   none
 * effects:   memory vectors point to heap allocated memory.
              Since both tables' nodes point to the same values, the vectors
              make sure that allocated memory is freed only time
 */
gerp::~gerp(){ 
    for(size_t i = 0; i < memory.size(); i++) {
        delete memory.at(i)->lineNum;
        delete memory.at(i)->sentence;
        delete memory.at(i);
    }
    for(size_t i = 0; i < memoryPath.size(); i++) {
        delete memoryPath.at(i);
    }
}

/*
 * name:      run
 * purpose:   runs the gerp program 
 * arguments: strings input file, output file
 * returns:   none
 * effects:   creates directory tree, traverses tree, inserts into the hash
              tables, and runs query command loop
 */
void gerp::run(std::string inputFile, std::string outputFile) {
    std::string dirName = inputFile;
    FSTree *dir = new FSTree(dirName); // create pointer to the directory tree
    DirNode *tree = dir->getRoot(); // gets root of the directory tree
    FSTreeTraversal(tree, ""); // traverses the directory and fills hash table
    query(outputFile); // queries for user input and prints to output file
    dir->burnTree(); // deallocates memory
    delete dir;
}

/*
 * name:      query
 * purpose:   handles the query loop
 * arguments: a string of a filename 
 * returns:   none
 * effects:   calls search helper function to distinguish between
              sensitive and insensitive search
 */
void gerp::query(std::string fileName) {
    std::ofstream output;
    open_or_die(output, fileName);
    std::string query, str = "";
    bool quit = false;
    std::cout << "Query? ";
    while (quit == false and std::cin >> query) {
        if (query == "@i" or query == "@insensitive"){ // insensitive words
            std::cin >> str;
            search(false, str, output);
        } else if (query == "@q" or query == "@quit"){ quit = true; } 
        else if (query == "@f") { // new output file
            std::cin >> str;
            output.close();
            open_or_die(output, str);
            std::cout << "Query? ";
        } else { //sensitive words
            search(true, query, output);
            }
        }
    std::cout << "Goodbye! Thank you and have a nice day." << std::endl;
    output.close();
}

/*
 * name:      search
 * purpose:   properly handle which table to access from
 * arguments: a boolean, a string, and an output stream
 * returns:   none
 * effects:   print to an output stream
 */
void gerp::search(bool sensitive, std::string &query, std::ostream &output) {
    if(not sensitive) {
        stripNonAlphaNum(query);
        query = toLower(query);
        if (not insensitiveTable.access(query, output)){
            output << query << " Not Found." << std::endl;
        }
    } else {
        stripNonAlphaNum(query);
        if (not sensitiveTable.access(query, output)) {
            output << query << " Not Found. Try with @insensitive or @i.";
            output << std::endl;
        }
    }
    std::cout << "Query? ";
}

/*
 * name:      FSTreeTraversal
 * purpose:   traverse tree and print paths to leaves
 * arguments: pointer to root, string 
 * returns:   none
 * effects:   prints the paths
 */
void gerp::FSTreeTraversal(DirNode *tree, std::string path) {
    // base case - traverse files
    if (tree->hasFiles()) {
        int numFiles = tree->numFiles();
        for (int i = 0; i < numFiles; i++) {
            std::string pathName = path + tree->getName() + 
                                   "/" + tree->getFile(i);
            traverseFile(pathName);
        }
    } // recursive case - traverse sub directories
    if (tree->hasSubDir()) {
        for (int i = 0; i < tree->numSubDirs(); i++) {
            FSTreeTraversal(tree->getSubDir(i), path + tree->getName() + "/");
        }
    }  
}

/*
 * name:      traverseFile
 * purpose:   traverse a file 
 * arguments: the address of a string
 * returns:   none
 * effects:   traverses a file and passes along arguements to traverseLine
 */
void gerp::traverseFile(std::string &pathName) {
    std::string *pathPtr = new std::string;
    *pathPtr = pathName;
    memoryPath.push_back(pathPtr);
    //open text file
    std::ifstream inputFile;
    open_or_die(inputFile, pathName);
    std::string sentence;
    int lineNum = 0;
    // loop through each line of a file
    while (getline(inputFile, sentence)){
        lineNum++;
        traverseLine(sentence, lineNum, pathPtr);
    }
    inputFile.close();
}

/*
 * name:      traverseLine
 * purpose:   traverses a file line and inserts each word into hash table
 * arguments: a string sentence, the line number, and the string path name
 * returns:   none
 * effects:   inserts each word of a sentence into both hash tables 
 */
void gerp::traverseLine(std::string &sentence, int &lineNum, 
                        std::string *pathPtr) {
                            
    int *linePtr = new int(lineNum);
    std::string *sentencePtr = new std::string(sentence);

    std::stringstream ss;
    std::string word;
    //reads each word from inputted line
    ss << sentence;
    pathData *value = new pathData{linePtr, pathPtr, sentencePtr};
    memory.push_back(value);

    while (ss >> word) {
        stripNonAlphaNum(word);
        if(word != "") { //insert into tables
           // all words in this line point to same pathData value
            sensitiveTable.insert(word, value); // sensitive insertion
            std::string insensitized = toLower(word);
            insensitiveTable.insert(insensitized, value);//insensitive insert
        }
    }
}

/*
 * name:      toLower
 * purpose:   makes a string into all lowercase 
 * arguments: the adress of a string 
 * returns:   the lowercase version of a string
 * effects:   none
 */
std::string gerp::toLower(std::string &key) {
    std::string word = "";
    for(size_t i = 0; i < key.length(); i++) {
        word.push_back(tolower(key[i]));
    }
    return word;
}

/*
 * name:      stripNonAlphaNum
 * purpose:   strips all non alpha numeric characters from the front and back
 *          of a string
 * arguments: the address of a string
 * returns: a string with the alpha numerics stripped
 * effects: none
 */
void gerp::stripNonAlphaNum(std::string &input) {
    if (input == "") { return; }
    while (input != "" and not isAlphaNum(input.at(0))) {
        input = input.substr(1, input.length() - 1);
    }
    while (input != "" and not isAlphaNum(input.at(input.length() - 1))) {
        input.pop_back();
    }
}

/*
 * name:      isAlphaNum
 * purpose:   determines whether a char is alpha numeric 
 * arguments: a character
 * returns:   a boolean value depending on whether the char is alpha numeric
 * effects:   none
 */
bool gerp::isAlphaNum(char c) {
    if ((c >= 48 and c <= 57) or (c >= 65 and c <= 90) 
        or (c >= 97 and c <= 122)) {
        return true;
    }
    return false;
}

/*
 * name:      open_or_die
 * purpose:   make sure that the provided files are able to open
 * arguments: streamtype stream and a string filename
 * returns:   none
 * effects:   throws error if unable to open file
 */
template<typename streamtype>
void gerp::open_or_die(streamtype &stream, std::string file_name) {
    stream.open(file_name);
    if (not stream.is_open())
        throw std::runtime_error("Unable to open file " + file_name);
}