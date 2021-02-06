#include<iostream>
#include<fstream>
#include<cstring> // For strcpy; the actual string container is not used as per requirements

#include "mvote.h"
#include "hashTable.h"

using namespace std;

int main(int argc, char* args[]) {

	string inputFile = "";
	for(int i = 0; i < argc; i++) {
		string param(args[i]);
		if (param == "-f") {
			string inputFileStr(args[i+1]); // Get the input file name
			inputFile = inputFileStr; // Then take it out of the loop
		}
	}

	if(inputFile == "") { // We need something to open!
		cerr << "Please specify an input file." << endl;
		return -1;
	}

	ifstream fin;
	fin.open(inputFile);
	if(!fin) { // Fallback in case we can't open the file for whatever reason
		cerr << "Error: file could not be opened." << endl;
		return -1;
	}

	HashTable hashTable(100);

	char firstName[30], lastName[30]; // Generous estimates as to how long the names can get, since we have no way of telling beforehand
	int rin, zipCode;
	while (fin >> rin >> firstName >> lastName >> zipCode) // Since the format of each line is the same, we can tell the program exactly what is what
	{ // Got the idea to use this from the ifstream documentation: http://www.cplusplus.com/reference/istream/istream/operator%3E%3E/
		bool voted = false;		

		char* firstName2; // The actual name containers
		char* lastName2;

		// After days of struggle, I finally figured out the overwriting error - I didn't allocate memory for my char arrays properly
		firstName2 = (char*)malloc(30*sizeof(char));
		lastName2 = (char*)malloc(30*sizeof(char));

		strncpy(firstName2, firstName, sizeof(firstName2));
		strncpy(lastName2, lastName, sizeof(lastName2));

		cout << firstName2 << ' ' << lastName2 << endl;
		Voter voter(rin, firstName2, lastName2, zipCode, voted);

		bool insertSuccess = false;
		hashTable.insert(rin, voter, insertSuccess);
		//hashTable.scanTable();
		cout << "Voter info: ";
		voter.getVoterInfo();
		//cout << rin << ' ' << firstName << ' ' << lastName << ' ' << zipCode << '\n';
	}

	hashTable.scanTable();

	while(true) {
		int searchedRIN;
		cout << "Enter RIN: ";
		cin >> searchedRIN;
		hashTable.lookup(searchedRIN);

	}

}