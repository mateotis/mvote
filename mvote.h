#ifndef MVOTE_H
#define MVOTE_H

#include<iostream>
#include<string>
#include<algorithm>

using namespace std;

class Voter {
	private:
		int rin;
		string firstName;
		string lastName;
		int zipCode;
		bool voted;
	public:
		Voter(int inputRIN, char* inputFN, char* inputLN, int inputZip, bool inputVoted) : rin(inputRIN),firstName(inputFN),lastName(inputLN),zipCode(inputZip),voted(inputVoted) {} // Constructor for a voter with all the info
		int getRIN() { // For regular insertion/access
			return rin;
		}
		int getZip() { // For zip code based queries
			return zipCode;
		}
};

#endif