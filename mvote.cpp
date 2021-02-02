#include<iostream>
#include<fstream>

#include "mvote.h"

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

	char firstName[30], lastName[30]; // Generous estimates as to how long the names can get, since we have no way of telling beforehand
	int rin, zipCode;
	while (fin >> rin >> firstName >> lastName >> zipCode) // Since the format of each line is the same, we can tell the program exactly what is what
	{ // Got the idea to use this from the ifstream documentation: http://www.cplusplus.com/reference/istream/istream/operator%3E%3E/
		bool voted = false;
		Voter voter(rin, firstName, lastName, zipCode, voted);
		cout << voter.getRIN() << '\n';
		//cout << rin << ' ' << firstName << ' ' << lastName << ' ' << zipCode << '\n';
	}

}