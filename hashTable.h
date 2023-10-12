/*
* HashTable.h
* 12/5/22
* John Berg, Tomer Wenderow
* Purpose: Interface for hashTable class.
*
*/

#include <vector>
#include <functional>

#ifndef __hashTable_H__
#define __hashTable_H__

struct pathData {
    int *lineNum;
    std::string *pathName;
    std::string *sentence;
 };

class hashTable {
public:

    hashTable();
	~hashTable();

    bool access(std::string &key, std::ostream &output);

    void insert(std::string &key, pathData *value); 

private:
    
    struct wordNode {
        //the word itself
		std::string key;
        //vector of the path of each occurence of this word
        std::vector<pathData*> allPaths;
	};

    static const int INITIAL_TABLE_SIZE = 1000;

    wordNode **table;
    int capacity;
    int numItems;
    
    void printNode(wordNode *node, std::ostream &output);

    bool diffLine(pathData *a, pathData *b);

    void expand();  

    void loadFactor();

    size_t hash_function(std::string &key);

};

#endif
