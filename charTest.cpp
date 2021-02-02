#include<iostream>
#include<fstream>
#include<cstring> // The string data structure itself is not used, only functions that work with char arrays

using namespace std;

// Char array functions (strncpy, strtol, strtok) learned from cplusplus.com and the C++ reference pages

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

	char input[30];
	long lineCount = 0;

	while(!fin.eof()) { // Gets number of entries; this is how much data we'll load into our hash table
		lineCount++;
	}

	fin.seekg(0, fin.beg); // Reset file iterator to the top

	fin.close();

	char voter[] = "258217 FLORENTINO GARRETT 4006";
	char* rinChar = (char*)malloc(7); // +1 for the null terminator; C++ also mandates type-casting the malloc 
	strncpy(rinChar, voter, 6);
	char *endptr1; // Used for the strtol function
	long rin = strtol(rinChar, &endptr1, 10); // Converting the RIN string to long

	size_t voterLength = strlen(voter); // Length of the entire string
	char* zipCodeChar = (char*)malloc(5);
	strncpy(zipCodeChar, voter+voterLength-4, 4); // We know that the zip code is always the last four characters of the string
	char *endptr2;
	long zipCode = strtol(zipCodeChar, &endptr2, 10); // Same as with the RIN

	char* fullName = (char*)malloc(voterLength-12); // The name is always length-12 characters long (6 characters for the RIN, 4 for the zip code, plus 2 spaces)
	strncpy(fullName, voter+7, voterLength-12); // The 7th character in any voter string is the first char of the name

	char* firstName = strtok(fullName, " "); // Tokenise, aka split the full name into first and last based on the space inbetween
	char* lastName = strtok(NULL, " ");

	cout << rin << '\n' << voterLength << '\n' << zipCode << '\n' << firstName << '\n' << lastName << '\n';
}