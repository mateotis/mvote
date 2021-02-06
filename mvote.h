// Defines the Voter class

#ifndef MVOTE_H
#define MVOTE_H

#include<iostream>
#include<cstdlib>

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
		Voter(int inputRIN, char inputFN[30], char inputLN[30], int inputZip, bool inputVoted) : rin(inputRIN),firstName(inputFN),lastName(inputLN),zipCode(inputZip),voted(inputVoted) {} // Constructor for a voter with all the info
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

/*		char* getVoterInfo() {
			char voterInfo[30];
			strcpy(voterInfo, to_string(rin).c_str()); // Workaround to transform an int to a char array without using the unsupported itoa()
			strcat(voterInfo, firstName);
			strcat(voterInfo, lastName);
			strcat(voterInfo, to_string(zipCode).c_str());

			cout << voterInfo << "\n";

			return voterInfo;
		}*/
};

#endif