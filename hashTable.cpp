// Defines all of the linked list and the major hash table functions

#include <iostream>

#include "hashTable.h"

using namespace std;

HashLinkedList::HashLinkedList() : head(NULL) {};

HashLinkedList::~HashLinkedList() {
	while (!empty()) removeFront(); 
}

bool HashLinkedList::empty() const {
	return head == NULL;
}

Voter& HashLinkedList::front() const {
	return head->voter;
}

void HashLinkedList::addFront(const Voter& e) {
	LLNode* v = new LLNode;
	v->voter = e;
	v->next = head;
	head = v;
	entryNum++;
}

void HashLinkedList::removeFront() {
	LLNode* old = head;
	head = old->next;
	delete old;
	entryNum--;
}

void HashLinkedList::remove(int rin) {
	LLNode* v = head;
	LLNode* temp = head; // Temp variable to help keep list structure intact while removing; initially set to the head as that's the first case we look at

	if(v->voter.getRIN() == rin) {
		cout << "Found voter to delete at the head!" << endl;
		head = temp->next;
		//v->next->voter.deleteChars(); // Delete the Voter object
		//delete temp->voter;
		delete temp;
		entryNum--;

		if(entryNum == 0) { // If we're out of entries now, just delete the head as well to make cleaning up the linked list easier
			cout << "Head deleted." << endl;
			delete head;
			head = NULL;
		}
		return;
	}

	while(v != NULL) {
		if(v->next->voter.getRIN() == rin) { // Always looking one step ahead, since we can't iterate backwards (this being a singly linked list)
			cout << "Found voter to delete!" << endl;
			LLNode* newNext = v->next->next; // Save the to-be-deleted node's next
			//v->next->voter.deleteChars(); // Delete the Voter object
			//delete v->next->voter;
			delete v->next; // Delete the offending node
			v->next = newNext; // Connect the current node to the next valid one
			entryNum--;

			if(entryNum == 0) { // If we're out of entries now, just delete the head as well to make cleaning up the linked list easier
				cout << "Head deleted." << endl;
				delete head;
				head = NULL;
			}
			return;
		}

		v = v->next;
	}

	cout << "Voter not found in LL!" << endl;
	return;
	
}

void HashLinkedList::displayAll() {
	LLNode* v = head;
	while(v != NULL) {
		v->voter.getVoterInfo();
		v = v->next;
	}
}

void HashLinkedList::findEntry(int rin, int lookupMode) {
	LLNode* v = head;
	while(v != NULL) {
		if(v->voter.getRIN() == rin) {
			cout << "Voter found in LL!" << endl;
			v->voter.getVoterInfo();
			if(lookupMode == 1) {
				v->voter.setVoted(1);
			}
			return;			
		}

		v = v->next;
	}
	cout << "Voter not found in LL!" << endl;
	return;	
}

Voter HashLinkedList::getVoter(int rin) {
	LLNode* v = head;
	while(v != NULL) {
		if(v->voter.getRIN() == rin) {
			cout << "getVoter() found the voter!" << endl;
			return v->voter;	
		}

		v = v->next;
	}

	cerr << "Could not delete voter as it was not found in the database." << endl;
	char dummyFN[30] = "DUMMY";
	char dummyLN[30] = "VOTER";
	Voter dummyVoter(0, dummyFN, dummyLN, 0, 0); // In case the user tries to delete an entry that's not in the table, return a dummy to prevent crashing
	return dummyVoter;
}

int HashLinkedList::getEntryNum() {
	return entryNum;
}

int HashTable::hashCode(const int key) // Very simple hashing function
{

	return key % capacity; // To ensure we stay within bounds of the table
}

void HashTable::insert(const int key, Voter value, bool& insertSuccess) // Insertion through separate chaining
{
	int hash = hashCode(key);
	//cout << "Insertion hash: " << hash << endl;
	int count = 0;
	while(true) {
		count++;
		if(nodeArray[hash] == nullptr) { // If we have an empty space, put the node there
			nodeArray[hash] = new HashNode(hash, value);
			//cout << "Comparisons made in table: " << count << endl;
			size++;
			insertSuccess = true;
			//cout << "Inserted into new hash ";
			nodeArray[hash]->getVoterInfo();
			cout << "Linked list in hash node " << hash << " has " << nodeArray[hash]->getListEntryNum() << " entries." << endl;
			return;
		}
		else if(nodeArray[hash] != nullptr) {
			nodeArray[hash]->insertVoter(value);
			//cout << "Comparisons made in table: " << count << endl;
			size++;
			insertSuccess = true;
			//cout << "Inserted at existing hash ";
			nodeArray[hash]->getVoterInfo();
			cout << "Linked list in hash node " << hash << " has " << nodeArray[hash]->getListEntryNum() << " entries." << endl;
			return;
		}	
	}

}

bool HashTable::lookup(const int key, int lookupMode) // Returns boolean whether the voter was found or not; also handles vote registering with another handy boolean
{
	int hash = hashCode(key);
	if(nodeArray[hash] != nullptr) {
		cout << "Currently searched node: " << nodeArray[hash]->getKey() << endl;
		cout << "Current number of voters: " << votedNum << endl;

		// lookupMode: making use of an int to make the lookup() function achieve different things - 0 is find, 1 is find and register, 2 is find and delete
		if(lookupMode == 2) {
			nodeArray[hash]->removeVoter(key);
			votedNum--;
			cout << votedNum << " people have now voted." << endl;

			cout << "Number of voters in node: " << nodeArray[hash]->getListEntryNum() << endl;

			if(nodeArray[hash]->getListEntryNum() == 0) { // If this deletion empties the list, we can delete the whole node
				delete nodeArray[hash];
				nodeArray[hash] = nullptr;
				cout << "Node deleted." << endl;
			}
		}
		else if(lookupMode == 1) {
			nodeArray[hash]->findVoter(key, lookupMode);
			votedNum++;
			cout << votedNum << " people have now voted." << endl;
		}
		else {
			nodeArray[hash]->findVoter(key, lookupMode);
		}
		return 1;
	}
	else {
		cout << "Voter not found!" << endl;
		return 0;
	}
	cout << "Voter not found!" << endl;
	return 0;
}

Voter HashTable::getVoter(const int rin) { // Gets a specific Voter object, needed (ironically) for zip code list handling
	int hash = hashCode(rin);

	if(nodeArray[hash] == nullptr) {
		cerr << "Could not delete voter as it was not found in the database." << endl;
		char dummyFN[30] = "DUMMY";
		char dummyLN[30] = "VOTER";
		Voter dummyVoter(0, dummyFN, dummyLN, 0, 0); // In case the user tries to delete an entry that's not in the table, return a dummy to prevent crashing
		return dummyVoter;
	}
	else {
		return nodeArray[hash]->getVoter(rin);
	}
	
}