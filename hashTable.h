#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>

#include "mvote.h"

using namespace std;

// Linked list that the hash nodes point to, contains voter data - based on implementations in Data Structures labs
class LLNode {
private:
	Voter voter;
	LLNode* next;

	friend class HashLinkedList;
};

class HashLinkedList {
	public:
		HashLinkedList();
		~HashLinkedList();
		bool empty() const;
		Voter& front() const;
		void addFront(const Voter& e);
		void removeFront();
		void displayAll();
		int getEntryNum();
		void findEntry(int rin);
	private:
		LLNode* head;
		int entryNum; // How many entries in the linked list
};

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

void HashLinkedList::displayAll() {
	LLNode* v = head;
	while(v != NULL) {
		v->voter.getVoterInfo();
		v = v->next;
	}
}

void HashLinkedList::findEntry(int rin) {
	LLNode* v = head;
	while(v != NULL) {
		if(v->voter.getRIN() == rin) {
			cout << "Voter found in LL!" << endl;
			v->voter.getVoterInfo();
			return;			
		}

		v = v->next;
	}
	cout << "Voter not found in LL!" << endl;
	return;	
}

int HashLinkedList::getEntryNum() {
	return entryNum;
}


class HashNode
{
	private:
		int key; // The hash code of the node
		HashLinkedList* value;

	public:
		HashNode(int key, Voter voter) // Node constructor
		{
			this->key = key;
			this->value = new HashLinkedList();
			this->value->addFront(voter);
		}
		int getKey()
		{
			return this->key;
		}
/*		HashLinkedList getValue()
		{
			return this->value;
		}*/
		void getVoterInfo() {
			this->value->displayAll();
			return;
		}
		void insertVoter(Voter voter) {
			this->value->addFront(voter);
		}
		int getListEntryNum() {
			return this->value->getEntryNum();
		}
		void findVoter(int rin) {
			this->value->findEntry(rin);
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
		void insert(const int key, Voter value, bool& insertSuccess);
		void lookup(const int key);
		//void remove(const int key);

		int getSize()
		{
			return this->size;
		}

		void scanTable() {
			int cnt = 0;
			for(int i = 0; i < capacity; i++) {
				if(nodeArray[i] != nullptr) { // Finds every node
					nodeArray[i]->getVoterInfo();
					cnt++;
				}
			}
			cout << "Scanned " << cnt << " nodes." << endl;
			return;	
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

void HashTable::insert(const int key, Voter value, bool& insertSuccess) // Insertion through open addressing
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
/*		else if(nodeArray[hash]->getKey() == key) { // If we run into an entry with the same key, then we don't need to add it again
			cerr << key << " already in database." << endl;
			cout << "Comparisons made in table: " << count << endl;
			return;
		}*/
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

void HashTable::lookup(const int key)
{
	int hash = hashCode(key);
	cout << "Searching hash: " << hash << endl;
	cout << "Looking for: " << key << endl;
	int count = 0;
	if(nodeArray[hash] != nullptr) {
		count++;
		cout << "Searching..." << endl;
		cout << "Currently searched node: " << nodeArray[hash]->getKey() << endl;

		nodeArray[hash]->findVoter(key);
		return;
/*		if(nodeArray[hash]->getKey() == key) {
			cout << "Comparisons made in table: " << count << endl;
			nodeArray[hash]->getVoterInfo();
			cout << "Voter found!" << endl;
			return;
		}*/	
	}
	else {
		cout << "Voter not found!" << endl;
		return;
	}
	cout << "Voter not found!" << endl;
	return;
}

#endif