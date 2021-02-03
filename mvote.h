#ifndef MVOTE_H
#define MVOTE_H

#include<iostream>
#include<cstring> // For strcpy and strcat
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
		Voter(int inputRIN, char* inputFN, char* inputLN, int inputZip, bool inputVoted) : rin(inputRIN),firstName(inputFN),lastName(inputLN),zipCode(inputZip),voted(inputVoted) {} // Constructor for a voter with all the info
		int getRIN() { // For regular insertion/access
			return rin;
		}
		int getZipCode() { // For zip code based queries
			return zipCode;
		}
		char* getFirstName() {
			return firstName;
		}
		char* getLastName() {
			return lastName;
		}
		bool getVoted() {
			return voted;
		}
		void getVoterInfo() {
			cout << getRIN() << ' ' << getFirstName() << ' ' << getLastName() << ' ' << getZipCode() << '\n';
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


class HashNode
{
	private:
		int key; // The RIN or the zip code, depending on the table
		Voter value;

	public:
		HashNode(int key, Voter value) // Node constructor
		{
			this->key = key;
			this->value = value;
		}
		int getKey()
		{
			return this->key;
		}
		Voter getValue()
		{
			return value;
		}
		void getVoterInfo() {
			this->value.getVoterInfo();
			return;
		}
/*		void getVoterInfo() {
			cout << this->value.getRIN() << ' ' << this->value.getFirstName() << ' ' << this->value.getLastName() << ' ' << this->value.getZipCode() << '\n';
			return;
		}*/

};

class HashTable
{
	private:
		HashNode **nodeArray; // The container is an array of node pointers
		int size;
		int capacity;
	public:
		HashTable(int capacity)
		{
			nodeArray = new HashNode*[capacity];
			this->capacity = capacity;
			this->size = 0;
		} 
		int hashCode(const int key);
		void insert(const int key, const Voter value, bool& insertSuccess);
		void lookup(const int key);
		//void remove(const int key);

		int getSize()
		{
			return this->size;
		}

		~HashTable() // Destructor, releases all dynamically allocated memory
		{
			int cnt = 0;
			for(int i = 0; i < capacity; i++) {
				if(nodeArray[i] != nullptr) { // Finds every node
					delete nodeArray[i]; // Deletes whatever is there
					nodeArray[i] = NULL; // Replaces it with nothing
					cnt++;
				}
			}
			delete[] this->nodeArray; // Then finally we delete the array itself
		}
};


int HashTable::hashCode(const int key) // Very simple hashing function
{

	return key % capacity; // To ensure we stay within bounds of the table
}

void HashTable::insert(const int key, const Voter value, bool& insertSuccess) // Insertion through open addressing
{
	int hash = hashCode(key);
	//cout << "Insertion hash: " << hash << endl;
	int count = 0;
	while(true) {
		count++;
		if(nodeArray[hash] == nullptr) { // If we have an empty space, put the node there
			nodeArray[hash] = new HashNode(key, value);
			//cout << "Comparisons made in table: " << count << endl;
			size++;
			insertSuccess = true;
			//cout << "Successful insert!" << endl;
			return;
		}
		else if(nodeArray[hash]->getKey() == key) { // If we run into an entry with the same key, then we don't need to add it again
			cerr << key << " already in database." << endl;
			cout << "Comparisons made in table: " << count << endl;
			return;
		}
		else { 

			if(hash < capacity - 1) {
				hash++; // Linear open addressing
			}
			else if(hash == capacity - 1 && size != capacity) { // If we reached the end but there's still space in the array, start looking for it from the beginning
				hash = 0;
			}
			else { // If we're at capacity, then we obviously can't insert it
				cerr << "No place in array for element " << key << endl;
				cout << "Comparisons made in hotel table: " << count << endl;
				return;
			}
		}	
	}

}

void HashTable::lookup(const int key)
{
	int hash = hashCode(key);
	cout << "Searching hash: " << hash << endl;
	cout << "Looking for: " << key << endl;
	int count = 0;
	bool wrappedAround = false; // Check if we've wrapped around the array once already
	while(nodeArray[hash] != nullptr) { // Uses the same open addressing scheme as insertion, stops looking upon finding the first empty space
		count++;
		cout << "Searching..." << endl;
		cout << "Currently searched node: " << nodeArray[hash]->getKey() << endl;
		if(nodeArray[hash]->getKey() == key) {
			cout << "Comparisons made in table: " << count << endl;
			nodeArray[hash]->getVoterInfo();
			cout << "Voter found!" << endl;
			return;
		}
		else {
			if(hash < capacity - 1) {
				hash++;
			}
			else if(hash == capacity - 1 && wrappedAround == false) { // Only wrap around once - if we still can't find it, then it really isn't there
				hash = 0;
				wrappedAround = true;
			}
			else {
				cout << "Voter not found!" << endl;
				return;
			}
		}	
	}
	cout << "Voter not found!" << endl;
	return;
}

#endif