###
### Makefile for Gerp Project
### 
###
### Author:  John Berg & Tomer Wenderow

MAKEFLAGS += -L 

CXX      = clang++
CXXFLAGS = -g3 -Wall -Wextra -Wpedantic -Wshadow
LDFLAGS  = -g3 

gerp: main.o gerp.o hashTable.o FSTree.o DirNode.o 
	$(CXX) $(CXXFLAGS) -o gerp main.o gerp.o hashTable.o FSTree.o \
	DirNode.o -O2

main.o: main.cpp gerp.h hashTable.h FSTree.h DirNode.h
	$(CXX) $(CXXFLAGS) -c main.cpp

gerp.o: gerp.cpp hashTable.h FSTree.h DirNode.h
	$(CXX) $(CXXFLAGS) -c gerp.cpp

hashTable.o: hashTable.cpp hashTable.h
	$(CXX) $(CXXFLAGS) -c hashTable.cpp

# treeTraversal: FSTreeTraversal.o FSTree.o DirNode.o
# 	$(CXX) $(CXXFLAGS) -o treeTraversal FSTreeTraversal.o FSTree.o DirNode.o


# FSTreeTraversal.o: FSTreeTraversal.cpp FSTree.h DirNode.h
# 	$(CXX) $(CXXFLAGS) -c FSTreeTraversal.cpp


# stringProcessing: stringProcessing.o 
# 	$(CXX) $(CXXFLAGS) -o stringProcessing stringProcessing.o 

# stringProcessing.o: stringProcessing.cpp stringProcessing.h 
# 	$(CXX) $(CXXFLAGS) -c stringProcessing.cpp