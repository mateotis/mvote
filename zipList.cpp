#include <iostream>

#include "zipList.h"

using namespace std;

ZipLinkedList::ZipLinkedList() : head(NULL) {};

ZipLinkedList::~ZipLinkedList() {
	while (!empty()) removeFront(); 
}

bool ZipLinkedList::empty() const {
	return head == NULL;
}

int& ZipLinkedList::front() const {
	return head->zipCode;
}

void ZipLinkedList::addFront(const int& e, const Voter& f) {
	ZipLLNode* v = new ZipLLNode;
	v->zipCode = e;
	v->value = new ZipVoterLinkedList;
	v->value->addFront(f); // Add the voter to the newly created zip voter linked list node, which is just an LLNode
	v->next = head;
	head = v;
	entryNum++;
}

void ZipLinkedList::removeFront() {
	ZipLLNode* old = head;
	head = old->next;
	delete old;
	entryNum--;
}

void ZipLinkedList::displayAll() {
	ZipLLNode* v = head;
	while(v != NULL) {
		cout << v->zipCode << endl;
		v->value->displayAll();
		v = v->next;
	}
}

int ZipLinkedList::getEntryNum() {
	return entryNum;
}

bool ZipLinkedList::findEntry(int zip) {
	ZipLLNode* v = head;
	while(v != NULL) {
		if(v->zipCode == zip) {
			cout << "Zip code " << zip << " found in LL!" << endl;
			return 1;			
		}

		v = v->next;
	}
	cout << "Zip code not found in LL!" << endl;
	return 0;	
}

bool ZipLinkedList::insertEntry(int zip, Voter voter) {
	ZipLLNode* v = head;
	while(v != NULL) {
		if(v->zipCode == zip) { // First we find the right node
			v->value->addFront(voter); // Then we insert into the ZipVoterLinkedList which is in the ZipLLNode
			cout << "Voter inserted into zip code " << zip << "!" << endl;
			return 1;
		}

		v = v->next;
	}
	cout << "Insertion unsuccessful!" << endl;
	return 0;	
}

void ZipLinkedList::printZipEntries(int zip) {
	ZipLLNode* v = head;
	while(v != NULL) {
		if(v->zipCode == zip) {
			v->value->displayAll();
			return;
		}

		v = v->next;
	}
	cout << "No entries for this zip code." << endl;
	return;
}