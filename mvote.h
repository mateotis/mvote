// Defines the Voter class

#ifndef MVOTE_H
#define MVOTE_H

#include<iostream>
#include<cstdlib>
#include<cstring>

using namespace std;

class Voter {
	private:
		int rin;
		char* firstName;
		char* lastName;
		int zipCode;
		bool voted;
	public:
		Voter() { // Default constructor, filled with dummy data
			this->rin = 0;
			this->firstName;
			this->lastName;
			this->zipCode = 0;
			this->voted = false;
		}
		//Voter(int inputRIN, char inputFN[30], char inputLN[30], int inputZip, bool inputVoted) : rin(inputRIN),firstName(inputFN),lastName(inputLN),zipCode(inputZip),voted(inputVoted) {} // Constructor for a voter with all the info

		Voter(int inputRIN, char inputFN[30], char inputLN[30], int inputZip, bool inputVoted) {

			// After days of struggle, I finally figured out the overwriting error - I didn't allocate memory for my char arrays properly
			//this->firstName = (char*)malloc(30*sizeof(char));
			//this->lastName = (char*)malloc(30*sizeof(char));

			this->firstName = new char[30];
			this->lastName = new char[30];

			//free(this->firstName);
			//free(this->lastName);

			strncpy(this->firstName, inputFN, sizeof(this->firstName));
			strncpy(this->lastName, inputLN, sizeof(this->lastName));

			this->rin = inputRIN;
			this->zipCode = inputZip;
			this->voted = inputVoted;
		}
		int getRIN() { // For regular insertion/access
			return this->rin;
		}
		int getZipCode() { // For zip code based queries
			return this->zipCode;
		}
		char* getFirstName() {
			return this->firstName;
		}
		char* getLastName() {
			return this->lastName;
		}
		bool getVoted() {
			return this->voted;
		}
		void getVoterInfo() {
			cout << this->getRIN() << ' ' << this->getFirstName() << ' ' << this->getLastName() << ' ' << this->getZipCode() << ' '<< this->getVoted() << '\n';
			return;
		}
		void setVoted(bool votedChange) {
			this->voted = votedChange;
			return;
		}

/*		void deleteChars() { // Using a separate method to free up the malloc'd class variables (called when we're deleting the class object as a whole) because the destructor is called automatically upon insertion, and this is a handy work-around
			cout << "Deleting class char: " << this->firstName << endl;
			cout << "Deleting class char: " << this->lastName << endl;
			delete[] this->firstName;
			delete[] this->lastName;
		}*/

		~Voter() {
			cout << "Calling Voter destructor on RIN and zip: " << rin << " " << zipCode << endl;
			//delete[] this->firstName;
			//delete[] this->lastName;			
		}

};

#endif