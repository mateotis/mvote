// Declares the hash table and all its related classes: the hash node, the linked list to be put in the nodes, and the linked list node

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
	friend class ZipVoterLinkedList;
};

class HashLinkedList {
	public:
		HashLinkedList();
		~HashLinkedList();
		bool empty() const;
		Voter& front() const;
		void addFront(const Voter& e);
		void removeFront();
		void remove(int rin);
		void displayAll();
		int getEntryNum();
		void findEntry(int rin, int lookupMode);
		Voter getVoter(int rin);
	private:
		LLNode* head;
		int entryNum = 0; // How many entries in the linked list
};


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
		void findVoter(int rin, int lookupMode) {
			this->value->findEntry(rin, lookupMode);
		}
		Voter getVoter(int rin) {
			return this->value->getVoter(rin);
		}
		void removeVoter(int rin) {
			if(this->value->empty() == false) {
				this->value->remove(rin);
			}
			else {
				cerr << "Could not remove voter as it is not in the database." << endl;
			}
			
		}

		~HashNode() {
			if(this->value->empty() == false) {
				cerr << "Error: should not be deleting node with non-empty linked list!" << endl;
			}
			else { // Since we should've already cleaned up the entire linked list before we get to this pont, we can safely delete the list itself
				delete this->value;
			}
		}

};

class HashTable
{
	private:
		HashNode **nodeArray; // The container is an array of node pointers
		int size; // How many voters there currently are in the table
		int capacity; // Total capacity of table
		int votedNum = 0; // How many people have voted in the hashtable
	public:
		HashTable(int capacity)
		{
			nodeArray = new HashNode*[capacity];
			this->capacity = capacity;
			this->size = 0;
		} 
		int hashCode(const int key);
		void insert(const int key, Voter value, bool& insertSuccess);
		bool lookup(const int key, int lookupMode);
		Voter getVoter(const int rin);
		//void remove(const int key);

		int getSize()
		{
			return this->size;
		}

		int getVotedNum() {
			return this->votedNum;
		}

		void scanTable() { // Goes over the entire table and prints every node; used for debugging
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

		float calcPercVoted() { // Returns percentage of people who voted
			if(votedNum != 0) { // Sanity check
				return float(votedNum) / float(size) * 100.0;
			}
			return 0.0;
		}

		~HashTable() // Destructor, releases all dynamically allocated memory
		{
			cout << "Hash table destructor called." << endl;
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

#endif