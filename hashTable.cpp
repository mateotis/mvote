// Defines all of the major functions for the hash table and its accompanying linked list

#include "hashTable.h"

using namespace std;

void HashLinkedList::addFront(const Voter& e) { // Standard pointer realignment procedures
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

void HashLinkedList::remove(int rin) { // Remove a voter with the given RIN from the hash table; as with the zip list, we have to be careful here
	LLNode* v = head;
	LLNode* temp = head; // Temp variable to help keep list structure intact while removing; initially set to the head as that's the first case we look at

	if(v->voter.getRIN() == rin) { // If this triggers, that means the very first entry (the head) is what we need to delete
		head = temp->next; // We move the head to the next entry, then delete the old head
		delete temp;
		entryNum--; // Keeping accurate track of the number of entries in the table

		if(entryNum == 0) { // If we're out of entries now, just delete the head as well to make cleaning up the linked list easier
			delete head;
			head = NULL;
		}
		return;
	}

	while(v != NULL) {
		if(v->next->voter.getRIN() == rin) { // Always looking one step ahead, since we can't iterate backwards (this being a singly linked list)
			LLNode* newNext = v->next->next; // Save the to-be-deleted node's next
			delete v->next; // Delete the offending node
			v->next = newNext; // Connect the current node to the next valid one
			entryNum--;

			if(entryNum == 0) { // If we're out of entries now, just delete the head as well to make cleaning up the linked list easier
				delete head;
				head = NULL;
			}
			return;
		}

		v = v->next;
	}

	cerr << "Voter not found!" << endl;
	return;
	
}

void HashLinkedList::displayAll() { // Similar display function to ZipLinkedList's
	LLNode* v = head;
	while(v != NULL) {
		v->voter.getVoterInfo();
		v = v->next;
	}
}

bool HashLinkedList::findEntry(int rin, int lookupMode) { // As the name says, this function finds a specific voter by RIN and either prints it (for lookup) or registers it
	LLNode* v = head;
	while(v != NULL) {
		if(v->voter.getRIN() == rin) {
			v->voter.getVoterInfo();
			if(lookupMode == 1) { // Register the voter as having voted
				v->voter.setVoted(1);
			}
			return 1;			
		}

		v = v->next;
	}
	return 0;	
}

Voter HashLinkedList::getVoter(int rin) { // Retrieves the voter
	LLNode* v = head;
	while(v != NULL) {
		if(v->voter.getRIN() == rin) {
			return v->voter;	
		}

		v = v->next;
	}

	cerr << "Could not delete voter as it was not found in the database." << endl; // This should never occur in normal operation, but if it does, it's bad form to return nothing (as this isn't a void function) - so we declare a dummy voter object that is checked against
	char dummyFN[30] = "DUMMY";
	char dummyLN[30] = "VOTER";
	Voter dummyVoter(0, dummyFN, dummyLN, 0, 0); 
	return dummyVoter;
}


int HashTable::hashCode(const int key) // Very simple hashing function
{

	return key % capacity; // To ensure we stay within bounds of the table
}

void HashTable::insert(const int key, Voter value) // Inserting voter into the table through separate chaining
{
	int hash = hashCode(key); // Calculate the hash
	while(true) {
		if(nodeArray[hash] == nullptr) { // If we have an empty space, put the node there
			nodeArray[hash] = new HashNode(hash, value);
			size++;
			return;
		}
		else if(nodeArray[hash] != nullptr) { // If there is already a node at that hash, append to the linked list within
			nodeArray[hash]->insertVoter(value);
			size++;
			return;
		}	
	}

}

bool HashTable::lookup(const int key, int lookupMode) // A powerful function that actually handles three things: simple lookup, registration, and even deletion; its mode given to it by the lookupMode variable
{
	int hash = hashCode(key);
	if(nodeArray[hash] != nullptr) {

		// lookupMode: making use of an int to make the lookup() function achieve different things - 0 is find, 1 is find and register, 2 is find and delete
		if(lookupMode == 2) { // Deletion
			if(getVoter(key).getVoted() == 1) { // Only subtract from voted count if that voter has actually voted
				votedNum--;
			}
			
			if(nodeArray[hash]->removeVoter(key) == 0) { // Attempts removal - if unsuccessful, returns 0, if successful, we're good
				return 0;
			}
			else {
				size--;

				if(nodeArray[hash]->getListEntryNum() == 0) { // If this deletion empties the list, we can delete the whole node
					delete nodeArray[hash];
					nodeArray[hash] = nullptr;
				}
			}
		}
		else if(lookupMode == 1) { // Register
			if(getVoter(key).getVoted() == 1) { // This prevents voter fraud (one person voting multiple times) :)
				cerr << "This voter has already voted!" << endl;
				return 0;
			}

			if(nodeArray[hash]->findVoter(key, lookupMode) == 0) { // If we could not register the voter, return 0, otherwise return 1
				return 0;
			}
			else {
				votedNum++; // Keeping track of the voter count for easy check
				return 1;			
			}

		}
		else { // Lookup
			if(nodeArray[hash]->findVoter(key, lookupMode) == 0) {
				return 0;
			}
			else {
				return 1;
			}
		}
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

	if(nodeArray[hash] == nullptr) { // As mentioned before, this shouldn't happen, but it might, so we have to be prepared for it
		cerr << "Could not delete voter as it was not found in the database." << endl;
		char dummyFN[30] = "DUMMY";
		char dummyLN[30] = "VOTER";
		Voter dummyVoter(0, dummyFN, dummyLN, 0, 0);
		return dummyVoter;
	}
	else { // Otherwise, proceed to retrieving the voter from the linked list
		return nodeArray[hash]->getVoter(rin);
	}
	
}