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
			entryNum++;
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

void ZipLinkedList::getZipVoterTotals() { // Used for the o command - gets the zip codes in the list and the number of voters who have voted in each
	if(entryNum == 0) { // If there's nothing in the big linked list, there's no use diving into the embedded ones
		cout << "There are currently no entries in the zip list." << endl;
		return;
	}

	ZipLLNode* v = head;
	int zipListStats[entryNum][2] = {0}; // 2D array to store all the zip codes and their # of voters - we know its max size in advance, which is very helpful
	int cnt = 0;
	while(v != NULL) { // Otherwise, iterate over every node and query every embedded linked list for its number of entries
		cout << v->zipCode << " has this many voters: " << v->value->getEntryNum() << endl;
		zipListStats[cnt][0] = v->zipCode;
		zipListStats[cnt][1] = v->value->getEntryNum();
		cnt++;
		v = v->next;
	}

	cout << "ZIP LIST STATS" << endl;
	for(int i = 0; i < entryNum; i++) {
		for(int j = 0; j < 2; j++) {
			if(zipListStats[i][0] != 0) { // If there is more than one voter in any given zip code, zipListStats will have rows with 0s as it's set to total entryNum size; we can ignore those 0 lines
				cout << zipListStats[i][j] << " ";
			}
		}
		if(zipListStats[i][0] != 0) {
			cout << endl;
		}
	}

	int sortedZipListStats[entryNum][2] = {0};
	int currentMaxZip = 0;
	int currentMaxNum = 0;
	int currentMaxLoc = 0; // This saves the position of the highest found number/zip code pair in the 2D array, because we set the number to 0 after the inner loop is over to avoid counting it again on the next iteration

	for(int i = 0; i < sizeof(zipListStats)/sizeof(zipListStats[0]); i++) { // Selection sort algorithm written from scratch - it's not particularly time-efficient (O(n^2)), but it's simple and it gets the job done
		for(int j = 0; j < sizeof(zipListStats)/sizeof(zipListStats[0]); j++) {
			if(zipListStats[j][1] > currentMaxNum) { // Update current maximum number of voters and the zip code they're in
				currentMaxZip = zipListStats[j][0];
				currentMaxNum = zipListStats[j][1];
				currentMaxLoc = j;
			}
		}
		sortedZipListStats[i][0] = currentMaxZip;
		sortedZipListStats[i][1] = currentMaxNum;

		zipListStats[currentMaxLoc][1] = 0; // Set what we just found to 0 in the original array; ensures we won't find it again next time - I think it's a pretty elegant solution!

		currentMaxZip = 0;
		currentMaxNum = 0;
	}


	cout << "SORTED ZIP LIST STATS" << endl;
	for(int i = 0; i < entryNum; i++) {
		for(int j = 0; j < 2; j++) {
			if(sortedZipListStats[i][0] != 0) {
				cout << sortedZipListStats[i][j] << " ";
			}
		}
		if(zipListStats[i][0] != 0) {
			cout << endl;
		}
	}

	return;

}

