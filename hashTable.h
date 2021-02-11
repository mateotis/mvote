// Declares the hash table and all its related classes: the hash node, the linked list to be put in the nodes, and the linked list node

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "mvote.h"

using namespace std;

// Linked list (and node) that the hash nodes point to, contains voter data - based on implementations in Data Structures labs
class LLNode {
	private:
		Voter voter;
		LLNode* next;

		friend class HashLinkedList;
		friend class ZipVoterLinkedList; // Since ZipVoterLinkedList inherits from HashLinkedList, we make the node compatible too
};

class HashLinkedList {
	private:
		LLNode* head;
		int entryNum = 0; // How many entries in the linked list
	public:
		void addFront(const Voter& e);
		void removeFront();
		void remove(int rin);
		void displayAll();
		bool findEntry(int rin, int lookupMode); // Methods explained at their definition
		Voter getVoter(int rin);

		HashLinkedList() : head(NULL) {};

		~HashLinkedList() {
			while (!empty()) removeFront(); 
		}

		bool empty() const {
			return head == NULL;
		}

		Voter& front() const {
			return head->voter;
		}

		int getEntryNum() {
			return entryNum;
		}
};

// Hash table node, holds the aforementioned linked lists, their key is the hash code
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
		void insertVoter(Voter voter) { // As can be seen, HashNode methods generally just pass the work to the linked lists which contain the voter data; this ensures modularity
			this->value->addFront(voter);
		}
		int getListEntryNum() {
			return this->value->getEntryNum();
		}
		bool findVoter(int rin, int lookupMode) { // Used to ensure an entry is present in the hash table and to sometimes register it
			if(this->value->findEntry(rin, lookupMode) == 1) {
				return 1;
			}
			else {
				return 0;
			}
		}
		Voter getVoter(int rin) { // Returns the actual Voter object, used to get information from a specific voter
			return this->value->getVoter(rin);
		}
		bool removeVoter(int rin) {
			if(this->value->empty() == false && this->value->findEntry(rin, 2) == 1) { // Only proceed to removal if the voter is actually there
				this->value->remove(rin);
				return 1;
			}
			else {
				cerr << "Could not remove voter as it is not in the database." << endl;
				return 0;
			}
			
		}

		~HashNode() {

			delete this->value; // HashLinkedList has a destructor which empties out the list before deletion; it also automatically calls the Voter destructors - so we can safely call delete here
		}

};

// The main hash table class, does much of the program's heavy lifting
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
		void insert(const int key, Voter value);
		bool lookup(const int key, int lookupMode);
		Voter getVoter(const int rin);

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