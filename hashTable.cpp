/*
* hashTable.cpp
* Purpose: implement the hasTable class which makes a viable hash table
*          for gerp
* 12/8/22
* Jack Berg, Tomer Wenderow
*/
#include <iostream>
#include <vector>
#include <functional>
#include <cstring>
#include "hashTable.h"

/*
* name:      constructor
* purpose:   initializes all values of table and the table
* arguments: none
* returns:   none
* effects:   none
*/
hashTable::hashTable(){
    //initialize values
    capacity = INITIAL_TABLE_SIZE;
    numItems = 0;
    table = new wordNode *[capacity]{nullptr};
}

/* 
* name:      destructor
* purpose:   recycles memory
* arguments: none
* returns:   none
* effects:   none
*/
hashTable::~hashTable(){
    //delete all wordNodes and the table itself
    for(int i = 0; i < capacity; i++) {
        if(table[i] != nullptr) {
            delete table[i];
        }
    }
    delete [] table;
}

/*
* name:      hash_function
* purpose:   hashes the key and returns the hashed value
* arguments: string key
* returns:   size_t of hashed value
* effects:   none
*/
size_t hashTable::hash_function(std::string &key){
    return std::hash<std::string>{}(key);
}

/*
* name:      access
* purpose:   accesses values of hash table
* arguments: string key, ostream output
* returns:   none
* effects:   outputs the necessary contents of key to output stream
*/
bool hashTable::access(std::string &key, std::ostream &output) {
    int attempt = 0;
    bool found = false;
    bool loop = true;
    while (not found and loop) {
        size_t hash_value = (hash_function(key) + 
                            attempt * attempt) % capacity;
        if(table[hash_value] == nullptr or 
                (attempt > capacity)) {
                loop = false;
        }
        else if(table[hash_value]->key == key) {
                //if goes here then it is found and print the node
                printNode(table[hash_value], output);
                found = true;
        }
        attempt++;
    }
    return found;
}

/*
* name:      insert
* purpose:   inserts a pathData value into the hash table
* arguments: string key, pathData value
* returns:   none
* effects:   creates a word node and inserts into array
*/
void hashTable::insert(std::string &key, pathData *value)
{
    //load factor checks if expansion is needed
    loadFactor();
    int attempt = 0;
    bool found = false;
    while (not found) {
        size_t hash_value = (hash_function(key) + 
                        attempt * attempt) % capacity;
        if (table[hash_value] == nullptr) {
            //makes new word node
            wordNode *node = new wordNode;
            node->key = key;
            node->allPaths.push_back(value);
            table[hash_value] = node;
            found = true;
        } else if (table[hash_value] != nullptr and 
            table[hash_value]->key == key) {
            //adds word to existing word node
            if (diffLine(table[hash_value]->allPaths.back(), value)) {
                    table[hash_value]->allPaths.push_back(value);
            } 
            found = true;
        } 
        attempt++;
    }
    numItems++;
}

/*
 * name:      loadFactor
 * purpose:   determines whether or not the array must be expanded
 * arguments: none
 * returns:   none
 * effects:   calculates load factor and expands if >= 0.7 
 */
void hashTable::loadFactor() {
    double num = numItems;
    double cap = capacity;
    if (num / cap >= 0.7) { expand(); }
}

/*
 * name:      expand
 * purpose:   doubles the size of the table
 * arguments: none
 * returns:   none
 * effects:   creates a new array double the size and rehashes elements from
 *              old array and inserts into the new one. 
 */
 void hashTable::expand() {
    int currCapacity = capacity;
    capacity = capacity * 2;
    //new array with double capacity
    wordNode **newTable = new wordNode *[capacity]{nullptr};
    for (int i = 0; i < currCapacity; i++) {
        int attempt = 0;
        bool found = false;
        while (table[i] != nullptr and not found) {
            //rehash anc copy contents from old to new array
            std::string key = table[i]->key;
            size_t hash_value = (hash_function(key) + 
            (attempt * attempt)) % capacity;
            if (newTable[hash_value] == nullptr) {
                newTable[hash_value] = table[i];
                table[i] = nullptr;
                found = true;
            }
            attempt++;
        }
        if(table[i] != nullptr) {
            delete table[i];
        } 
    }
    delete [] table;
    table = newTable;
    newTable = nullptr;
}

/*
 * name:      diffLine
 * purpose:   checks if paths are on the same line
 * arguments: two paths
 * returns:   true if on same line, otherwise false
 * effects:   none
 */
bool hashTable::diffLine(pathData *a, pathData *b) {
    if (a->lineNum != b->lineNum or a->pathName != b->pathName) {
        return true;
    }
    return false;
}

/*
 * name:      printNode
 * purpose:   prints all instances of a word
 * arguments: the node, output stream 
 * returns:   none
 * effects:   prints the path, line number, and the line
 */
void hashTable::printNode(wordNode *node, std::ostream &output){
    for (size_t i = 0; i < node->allPaths.size(); i++) {
        output << *node->allPaths.at(i)->pathName << ":";
        output << *node->allPaths.at(i)->lineNum << ": ";
        output << *node->allPaths.at(i)->sentence << std::endl;
    }
}
