// Defines the Voter class

#ifndef MVOTE_H
#define MVOTE_H

#include<iostream>
#include<cstring> // For strcpy, strcmp, strtok, and strtol; the actual string container is not used as per requirements

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

		Voter(int inputRIN, char inputFN[30], char inputLN[30], int inputZip, bool inputVoted) {

			// After days of struggle, I finally figured out the overwriting error - I didn't allocate memory for my char arrays properly
			this->firstName = new char[30];
			this->lastName = new char[30];

			strncpy(this->firstName, inputFN, sizeof(this->firstName)); // Copy the content into the container char arrays; this ensures that they are all properly present
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
		void getVoterInfo() { // Prints out all the voter info in one line
			cout << this->getRIN() << ' ' << this->getFirstName() << ' ' << this->getLastName() << ' ' << this->getZipCode() << ' '<< this->getVoted() << '\n';
			return;
		}
		void setVoted(bool votedChange) { // Change registration status
			this->voted = votedChange;
			return;
		}

		~Voter() {}

};

#endif