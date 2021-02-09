// Main file: takes command line arguments, parses files, processes user inputs

#include<iostream>
#include<fstream>
#include<cstring> // For strcpy and strcmp; the actual string container is not used as per requirements

#include "mvote.h"
#include "hashTable.h"
#include "zipList.h"

using namespace std;

int main(int argc, char* args[]) {

	// PARSING COMMAND LINE ARGUMENTS
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

	char firstName[30], lastName[30]; // Generous estimates as to how long the names can get, since we have no way of telling beforehand
	int rin, zipCode;

	// Running over the file once front to back to find how many lines there are
	int lineCount = 0;
	while (fin >> rin >> firstName >> lastName >> zipCode) { // We don't actually do anything with our variable assignments here, however this is the simplest/smoothest way to iterate over a standard format file
		lineCount++;
	}
	cout << "File has " << lineCount << " lines." << endl;
	fin.close();

	// READING DATA FROM SPECIFIED FILE INTO OUR DATA STRUCTURES
	fin.open(inputFile);

	// Initialising our main data structures
	HashTable* hashTable = new HashTable(lineCount * 2); // Dynamically generate hash table: double the size of the input file should give us enough space to work with
	ZipLinkedList* zipList = new ZipLinkedList();

	while (fin >> rin >> firstName >> lastName >> zipCode) // Since the format of each line is the same, we can tell the program exactly what is what
	{ // Got the idea to use this from the ifstream documentation: http://www.cplusplus.com/reference/istream/istream/operator%3E%3E/
		bool voted = false;		

/*		char* firstName2; // The actual name containers
		char* lastName2;

		// After days of struggle, I finally figured out the overwriting error - I didn't allocate memory for my char arrays properly
		firstName2 = (char*)malloc(30*sizeof(char));
		lastName2 = (char*)malloc(30*sizeof(char));

		strncpy(firstName2, firstName, sizeof(firstName2));
		strncpy(lastName2, lastName, sizeof(lastName2));*/

		//cout << firstName2 << ' ' << lastName2 << endl;
		Voter voter(rin, firstName, lastName, zipCode, voted);
		cout << "right after voter object init" << endl;

		bool insertSuccess = false;
		hashTable->insert(rin, voter, insertSuccess);
		cout << "right after insertion" << endl;
	}

	fin.close();

	hashTable->scanTable();

	// PROCESSING USER INPUT AND EXECUTING COMMANDS
	// Initialise and allocate memory for all potential user inputs: the longest command will have four parametres, anything extra should be dealt with appropriately
	char* input;
	char* command;
	char* param1;
	char* param2;
	char* param3;
	char* param4;
	char* extraInput;
	input = (char*)malloc(100*sizeof(char));
	command = (char*)malloc(30*sizeof(char));
	param1 = (char*)malloc(30*sizeof(char));
	param2 = (char*)malloc(30*sizeof(char));
	param3 = (char*)malloc(30*sizeof(char));
	param4 = (char*)malloc(30*sizeof(char));
	extraInput = (char*)malloc(30*sizeof(char));

	while(true) {

		cout << "Enter input: " << endl;

		cin.getline(input, 100); // Turns out, you don't need strings to use getline! Found in the reference: http://www.cplusplus.com/reference/istream/istream/getline/

		// Starting the tokenisation process: we go word by word, figuring out exactly how many params the user input and handling each use case appropriately
		command = strtok(input, " ");
		if(command == NULL) { // If at any point the next token is a null, it means we've reached the end of the input and we should stop tokenising and start executing
			cerr << "Input cannot be empty." << endl;
			continue;
		}
		cout << "Command: " << command << endl;

		param1 = strtok(NULL, " ");
		if(param1 == NULL) { // Only one command - used for paramaterless inputs, like v or perc
			if(strcmp(command, "exit") == 0) { // TO DO - release all memory before exiting
				delete hashTable;
				exit(0);
			}
			if(strcmp(command, "v") == 0) { // Display how many people in total have voted
				cout << hashTable->getVotedNum() << " people in the hash table have voted." << endl;
			}
			else if(strcmp(command, "perc") == 0) { // Display the percentage of people who have voted
				cout << hashTable->calcPercVoted() << " percent of voters in the hash table have voted." << endl;
			}
			else if(strcmp(command, "o") == 0) {
				zipList->getZipVoterTotals();
			}
			continue;
		}
		cout << "First parameter: " << param1 << endl;

		param2 = strtok(NULL, " ");
		if(param2 == NULL) { // One parameter - used for entry lookup, deletion, and voter registration

			if(strcmp(command, "l") == 0) { // Lookup entry based on unique RIN
				char* endptr; // Only used for the strtol command below which converts the char array param to a long, which we then immediately convert to int
				int searchedRIN = int(strtol(param1, &endptr, 10)); // The 10 is the base of the number system
				int lookupM = 0; // Have to define it as an int because for some reason, the compiler thinks this 0 is a boolean if passed directly
				hashTable->lookup(searchedRIN, lookupM);

			}
			else if(strcmp(command, "r") == 0) { // Register voter as having voted
				char* endptr;
				int searchedRIN = int(strtol(param1, &endptr, 10));
				hashTable->lookup(searchedRIN, 1); // Finds the voter and changes its status to having voted (the boolean handles the latter)
				cout << "Voter status changed!" << endl;

				// After we registered the voter in the main hash table, we have to add them to the zip code list as well
				Voter currentVoter = hashTable->getVoter(searchedRIN); // Get the voter we're supposed to add
				int currentZipCode = currentVoter.getZipCode(); // Get its zip code, which will be the key of insertion

				if(zipList->findEntry(currentZipCode) == 0) { // Look for the zip code first - if it's not in the list, add it and create a new embedded LL with the voter inside
						zipList->addFront(currentZipCode, currentVoter);
						cout << "Added to zip list!" << endl;
				}
				else { // If the zip code is already present, insert the voter into the existing embedded LL
					zipList->insertEntry(currentZipCode, currentVoter);
					cout << "Added to zip list!" << endl;
				}
				cout << "Current entries in zip list: " << zipList->getEntryNum() << endl;

				cout << "Zip code list:" << endl;
				zipList->displayAll();				
			}
			else if(strcmp(command, "d") == 0) { // Delete specified voter
				char* endptr;
				int searchedRIN = int(strtol(param1, &endptr, 10));				
				hashTable->lookup(searchedRIN, 2); // Calls lookup with lookupMode 2 - search & destroy
			}
			else if(strcmp(command, "z") == 0) { // Display all entries in the zip list under the specified zip code
				char* endptr;
				int searchedZip = int(strtol(param1, &endptr, 10));				
				zipList->printZipEntries(searchedZip);			
			}
			else if(strcmp(command, "bv") == 0) { // One of the more complex commands - open specified file and register every RIN in there
				int rinEntry;

				ifstream fin;
				fin.open(param1); // Fortunately, we can just pass the first (and only) param as the file name
				if(!fin) {
					cerr << "Error: file could not be opened." << endl;
					return -1;
				}

				while (fin >> rinEntry)	 { // Just execute lookup commands for every line in the file
					hashTable->lookup(rinEntry, 1);
					cout << "Voter status changed!" << endl;

					Voter currentVoter = hashTable->getVoter(rinEntry);
					int currentZipCode = currentVoter.getZipCode();

					if(zipList->findEntry(currentZipCode) == 0) { // Look for the zip code first - if it's not in the list, add it and create a new embedded LL with the voter inside
							zipList->addFront(currentZipCode, currentVoter);
							cout << "Added to zip list!" << endl;
					}
					else { // If the zip code is already present, insert the voter into the existing embedded LL
						zipList->insertEntry(currentZipCode, currentVoter);
						cout << "Added to zip list!" << endl;
					}
					cout << "Current entries in zip list: " << zipList->getEntryNum() << endl;
				}

				fin.close();				
			}

			continue;
		}
		cout << "Second parameter: " << param2 << endl;	

		// There are actually no commands with two or three parametres, but this makes for a consistent and predictable tokenising process, making the program run smoother and stabler
		if(param2 != NULL) {
			param3 = strtok(NULL, " ");
			if(param3 == NULL) {
				continue;
			}
			cout << "Third parameter: " << param3 << endl;
		}

		if(param3 != NULL) {
			param4 = strtok(NULL, " ");
			if(param4 == NULL) {
				continue;
			}
			cout << "Fourth parameter: " << param4 << endl;
		}

		if(param4 != NULL) {
			extraInput = strtok(NULL, "");
			if(extraInput == NULL) { // The big one: insertion. Uses all four parameters, which are cast into the appropriate types before they are passed to the hash table

				char* endptr;
				int rin = int(strtol(param1, &endptr, 10));
				int zipCode = int(strtol(param4, &endptr, 10));	

				char* firstName;
				char* lastName;

				firstName = (char*)malloc(30*sizeof(char)); // Same setup as in the initial file parsing
				lastName = (char*)malloc(30*sizeof(char));

				strncpy(firstName, param2, sizeof(firstName));
				strncpy(lastName, param3, sizeof(lastName));		

				bool voted = false;

				Voter voter(rin, firstName, lastName, zipCode, voted);

				bool insertSuccess = false;

				if(hashTable->lookup(rin, 0) == 1) {
					cerr << "Error: voter already in system." << endl;
				}
				else {
					hashTable->insert(rin, voter, insertSuccess);	
					cout << "Voter inserted!" << endl;				
				}				
			}
			else { // Can't have more than four parametres
				cout << "Extra input: |" << extraInput << "|" << endl;
				cerr << "Too many parameters. Please try again." << endl;
				continue;				
			}
		}


	}

	return 0;
}