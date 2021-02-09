#ifndef ZIPLIST_H
#define ZIPLIST_H

#include <iostream>

#include "mvote.h"
#include "hashTable.h"

using namespace std;

// Linked list within the zip code list that contains voter entries; inherited from the original HashLinkedList as their functionality is similar
class ZipVoterLinkedList : public HashLinkedList {
	private:
		LLNode* head;
		int entryNum = 0; // How many entries in the linked list		
};

// Linked list that holds zip codes and another linked list that holds voter data
class ZipLLNode {
	private:
		int zipCode;
		ZipVoterLinkedList* value;
		ZipLLNode* next;

		friend class ZipLinkedList;

	public:
		~ZipLLNode() { // Need a separate destructor because we have dynamically allocated data in ZipLLNodes
			cout << "In ZipLLNode destructor." << endl;
			delete value;
		}
};

class ZipLinkedList {
	public:
		ZipLinkedList();
		~ZipLinkedList();
		bool empty() const;
		int& front() const;
		void addFront(const int& e, const Voter& f);
		void removeFront();
		void remove(int rin, int zip);
		void displayAll();
		int getEntryNum();
		bool findEntry(int zip);
		bool insertEntry(int zip, Voter voter);
		void printZipEntries(int zip);
		void getZipVoterTotals();
	private:
		ZipLLNode* head;
		int entryNum = 0; // How many entries in the linked list
};

#endif