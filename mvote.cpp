// Main file: takes command line arguments, parses files, processes user inputs

#include<fstream>

#include "mvote.h"
#include "hashTable.h"
#include "zipList.h"

using namespace std;

int main(int argc, char* args[]) {

	// PARSING COMMAND LINE ARGUMENTS
	char* inputFile = new char[30];
	for(int i = 0; i < argc; i++) {
		if (strcmp(args[i], "-f") == 0) { // Can't use a simple == operator as args[i] elements have the weird "Pc" type
			inputFile = args[i+1];
		}
	}

	if(inputFile == NULL) { // We need something to open!
		cerr << "Please specify an input file." << endl;
		return -1;
	}

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
	HashTable* hashTable = new HashTable(lineCount * 2); // Dynamically generate hash table: double the size of the input file should give us enough space to work with as it reduces the load factor of the table
	ZipLinkedList* zipList = new ZipLinkedList();

	while (fin >> rin >> firstName >> lastName >> zipCode) // Since the format of each line is the same, we can tell the program exactly what is what
	{ // Got the idea to use this from the ifstream documentation: http://www.cplusplus.com/reference/istream/istream/operator%3E%3E/
		bool voted = false;		

		Voter voter(rin, firstName, lastName, zipCode, voted);

		hashTable->insert(rin, voter);
	}

	fin.close();

	cout << hashTable->getSize() << " voters successfully inserted into hash table." << endl;

	// PROCESSING USER INPUT AND EXECUTING COMMANDS
	// Initialise and allocate memory for all potential user inputs: the longest command will have four parametres, anything extra should be dealt with appropriately
	char* input = new char[100];
	char* command = new char[30];
	char* param1 = new char[30];
	char* param2 = new char[30];
	char* param3 = new char[30];
	char* param4 = new char[30];
	char* extraInput = new char[30];

	while(true) {

		cout << "Enter input: " << endl;

		cin.getline(input, 100); // Turns out, you don't need strings to use getline! Found in the reference: http://www.cplusplus.com/reference/istream/istream/getline/

		// Starting the tokenisation process: we go word by word, figuring out exactly how many params the user input and handling each use case appropriately
		command = strtok(input, " ");
		if(command == NULL) { // If at any point the next token is a null, it means we've reached the end of the input and we should stop tokenising and start executing
			cerr << "Input cannot be empty." << endl;
			continue;
		}

		param1 = strtok(NULL, " ");
		if(param1 == NULL) { // Only one command - used for parameterless inputs, like v or perc
			if(strcmp(command, "exit") == 0) {
				delete hashTable;
				delete zipList;
				delete[] input; // This is the only char array in this segment that always needs to be deleted - the rest sometimes cause a segfault if deleted manually
/*				delete[] command;
				delete[] param1;
				delete[] param2;
				delete[] param3;
				delete[] param4;
				delete[] extraInput;*/
				exit(0);
			}
			if(strcmp(command, "v") == 0) { // Display how many people in total have voted
				cout << hashTable->getVotedNum() << " people in the hash table have voted." << endl;
			}
			else if(strcmp(command, "perc") == 0) { // Display the percentage of people who have voted
				cout << hashTable->calcPercVoted() << " percent of voters in the hash table have voted." << endl;
			}
			else if(strcmp(command, "o") == 0) { // Print a list of zip codes in descending order based on the number of voters in each 
				zipList->getZipVoterTotals();
			}
			continue;
		}

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
				if(hashTable->lookup(searchedRIN, 0) == 0 || hashTable->lookup(searchedRIN, 1) == 0) { // Check if the person we're trying to register is in fact NOT in the database OR has already voted
					cerr << "Voter not found!" << endl;
					continue;
				} 
				cout << "Voter status changed!" << endl;

				// After we registered the voter in the main hash table, we have to add them to the zip code list as well
				Voter currentVoter = hashTable->getVoter(searchedRIN); // Get the voter we're supposed to add
				int currentZipCode = currentVoter.getZipCode(); // Get its zip code, which will be the key of insertion

				if(zipList->findEntry(currentZipCode) == 0) { // Look for the zip code first - if it's not in the list, add it and create a new embedded LL with the voter inside
						zipList->addFront(currentZipCode, currentVoter);
				}
				else { // If the zip code is already present, insert the voter into the existing embedded LL
					zipList->insertEntry(currentZipCode, currentVoter);
				}
			}
			else if(strcmp(command, "d") == 0) { // Delete specified voter
				char* endptr;
				int searchedRIN = int(strtol(param1, &endptr, 10));
				Voter currentVoter = hashTable->getVoter(searchedRIN); // Fetch the voter before we remove it so we can get its zip

				if(currentVoter.getRIN() == 0 && currentVoter.getZipCode() == 0) { // If we get a dummy voter return from getVoter() with these values, that means the voter wasn't found 
					cerr << "Voter not found!" << endl;
					continue;
				}

				int currentZipCode = currentVoter.getZipCode(); // Get the zip

				hashTable->lookup(searchedRIN, 2); // Calls lookup with lookupMode 2 - search & destroy

				if(zipList->findVoterInZip(searchedRIN, currentZipCode) == 0) { // First look for the voter in the zip list and only delete if they're actually there
					continue;
				}
				else {
					zipList->remove(searchedRIN, currentZipCode);					
				}


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

				while (fin >> rinEntry)	 { // Just execute lookup commands in register mode for every line in the file

					if(hashTable->lookup(rinEntry, 0) == 0 || hashTable->lookup(rinEntry, 1) == 0) { // Check if the person we're trying to register is in fact NOT in the database OR has already voted
						cout << "Voter not found!" << endl;
						continue;
					} 

					Voter currentVoter = hashTable->getVoter(rinEntry);
					int currentZipCode = currentVoter.getZipCode();

					if(zipList->findEntry(currentZipCode) == 0) { // Look for the zip code first - if it's not in the list, add it and create a new embedded LL with the voter inside
							zipList->addFront(currentZipCode, currentVoter);
					}
					else { // If the zip code is already present, insert the voter into the existing embedded LL
						zipList->insertEntry(currentZipCode, currentVoter);
					}
				}

				fin.close();				
			}

			continue;
		}

		// There are actually no commands with two or three parametres, but this makes for a consistent and predictable tokenising process, making the program run smoother and stabler
		if(param2 != NULL) {
			param3 = strtok(NULL, " ");
			if(param3 == NULL) {
				continue;
			}
		}

		if(param3 != NULL) {
			param4 = strtok(NULL, " ");
			if(param4 == NULL) {
				continue;
			}
		}

		if(param4 != NULL) {
			extraInput = strtok(NULL, "");
			if(extraInput == NULL) { // The big one: insertion. Uses all four parameters, which are cast into the appropriate types before they are passed to the hash table

				char* endptr;
				int rin = int(strtol(param1, &endptr, 10)); // Cast these two into ints
				int zipCode = int(strtol(param4, &endptr, 10));	

				bool voted = false;

				Voter voter(rin, param2, param3, zipCode, voted); // We can pass the char arrays straight from input

				if(hashTable->lookup(rin, 0) == 1) {
					cerr << "Error: voter already in system." << endl;
				}
				else {
					hashTable->insert(rin, voter);	
					cout << "Voter inserted!" << endl;				
				}				
			}
			else { // Can't have more than four parametres
				cerr << "Too many parameters. Please try again." << endl;
				continue;				
			}
		}


	}

	return 0;
}