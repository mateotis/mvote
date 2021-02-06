#include<iostream>
#include<fstream>
#include<cstring> // For strcpy and strcmp; the actual string container is not used as per requirements

#include "mvote.h"
#include "hashTable.h"

using namespace std;

int main(int argc, char* args[]) {

	char* inputFile;
	inputFile = (char*)malloc(30*sizeof(char));
	for(int i = 0; i < argc; i++) {
		cout << typeid(args[i]).name() << endl;
		if (strcmp(args[i], "-f") == 0) { // Can't use a simple == operator as args[i] elements have the weird "Pc" type
			inputFile = args[i+1];
		}
	}

	if(inputFile == NULL) { // We need something to open!
		cerr << "Please specify an input file." << endl;
		return -1;
	}

	cout << inputFile << endl;
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
		//cout << rin << ' ' << firstName << ' ' << lastName << ' ' << zipCode << '\n';
	}

	hashTable.scanTable();

/*	while(true) {
		int searchedRIN;
		cout << "Enter RIN: ";
		cin >> searchedRIN;
		hashTable.lookup(searchedRIN);

	}*/

	while(true) {
		int commandChoice;

		cout << "What command? 1. is insertion, 2. is search. 4. is register, 6. is total voter number, 7. is voter percentage." << endl;
		cin >> commandChoice;

		if (commandChoice == 1) {
			char firstName[30], lastName[30];
			int rin, zipCode;
			bool voted = false;

			cout << "Enter voter info: ";
			cin >> rin >> firstName >> lastName >> zipCode;

			char* firstName2;
			char* lastName2;

			firstName2 = (char*)malloc(30*sizeof(char));
			lastName2 = (char*)malloc(30*sizeof(char));

			strncpy(firstName2, firstName, sizeof(firstName2));
			strncpy(lastName2, lastName, sizeof(lastName2));

			cout << firstName2 << ' ' << lastName2 << endl;
			Voter voter(rin, firstName2, lastName2, zipCode, voted);

			bool insertSuccess = false;

			if(hashTable.lookup(rin, 0) == 1) {
				cerr << "Error: voter already in system." << endl;
			}
			else {
				hashTable.insert(rin, voter, insertSuccess);	
				cout << "Voter inserted!" << endl;				
			}

		}

		else if(commandChoice == 2) {
			int searchedRIN;
			cout << "Enter RIN: ";
			cin >> searchedRIN;
			hashTable.lookup(searchedRIN, 0);	
		}

		else if(commandChoice == 4) {
			int searchedRIN;
			cout << "Enter RIN: ";
			cin >> searchedRIN;
			hashTable.lookup(searchedRIN, 1); // Finds the voter and changes its status to having voted

			cout << "Voter status changed!" << endl;
		}

		else if(commandChoice == 6) {
			cout << hashTable.getVotedNum() << " people in the hash table have voted." << endl;
		}

		else if(commandChoice == 7) {
			cout << hashTable.calcPercVoted() << " percent of voters in the hash table have voted." << endl;
		}

	}


	return 0;

}