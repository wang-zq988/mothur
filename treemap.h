#ifndef TREEMAP_H
#define TREEMAP_H
/*
 *  treemap.h
 *  Mothur
 *
 *  Created by Sarah Westcott on 1/26/09.
 *  Copyright 2009 Schloss Lab UMASS Amherst. All rights reserved.
 *
 */

#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include "utilities.hpp"

/* This class is used by the read.tree command to build the tree container. */

struct GroupIndex {
	string	groupname;
	int		vectorIndex;
};



class TreeMap {
public:
	TreeMap() {};
	TreeMap(string);
	~TreeMap();
	void readMap();
	int getNumGroups();
	int getNumSeqs();
	void setIndex(string, int);  //sequencename, index
	int getIndex(string);		//returns vector index of sequence
	bool isValidGroup(string);  //return true if string is a valid group
	string getGroup(string);
	vector<string> namesOfGroups;
	vector<string> namesOfSeqs;
    map<string,int> seqsPerGroup;	//groupname, number of seqs in that group.
	map<string, GroupIndex> treemap; //sequence name and <groupname, vector index>
	void print(ostream&);
	
private:
	ifstream fileHandle;
	string groupFileName;
	int numGroups;
	map<string, GroupIndex>::iterator it;
	map<string, int>::iterator it2;
	void setNamesOfGroups(string); 
	
};

#endif