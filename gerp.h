/*
* gerp.h
* 12/5/22
* John Berg, Tomer Wenderow
* Purpose: Interface for the gerp class.
*
*/

#ifndef __GERP_H__
#define __GERP_H__

#include "hashTable.h"
#include "FSTree.h"

class gerp {

public:
    
    gerp();
    
    ~gerp();

    void run(std::string inputFile, std::string outputFile);

private:

    void search(bool sensitive, std::string &query, std::ostream &output);

    void query(std::string fileName);

    void FSTreeTraversal(DirNode *tree, std::string path);

    void traverseFile(std::string &pathName);

    void traverseLine(std::string &sentence, int &lineNum, std::string *pathPt);

    hashTable insensitiveTable;
    hashTable sensitiveTable;

    void stripNonAlphaNum(std::string &input);

    bool isAlphaNum(char c);

    std::string toLower(std::string &key); 
    //keep track of allocated memory
    std::vector<pathData*> memory;
    std::vector<std::string*> memoryPath;

    template<typename streamtype>
    void open_or_die(streamtype &stream, std::string file_name);
    
};

#endif
