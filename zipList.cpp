// Defines ZipLinkedList methods

//#include <iostream>

#include "zipList.h"

using namespace std;

void ZipLinkedList::addFront(const int& e, const Voter& f) { // Add a new element to the zip list, which is actually three things: a new zip node, a new linked list WITHIN that zip node, and the voter WITHIN that linked list
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

void ZipLinkedList::displayAll() { // Displays all entries in the zip list, used for debugging and printZipEntries()
	ZipLLNode* v = head;
	while(v != NULL) {
		cout << v->zipCode << endl;
		v->value->displayAll();
		v = v->next;
	}
}

bool ZipLinkedList::findEntry(int zip) { // Checks whether zip code is in the list, used to determine method of insertion
	ZipLLNode* v = head;
	while(v != NULL) {
		if(v->zipCode == zip) {
			return 1;			
		}

		v = v->next;
	}
	return 0;	
}

bool ZipLinkedList::findVoterInZip(int rin, int zip) { // Finds voter in zip list from zip code and RIN
	ZipLLNode* v = head;
	while(v != NULL) {
		if(v->zipCode == zip) { // Same process as findEntry() so far but we go a step further 
			if(v->value->findEntry(rin, 2) == 1) { // Only return 1 if we actually found the voter under that zip
				return 1;
			}		
		}

		v = v->next;
	}
	return 0;	
}

bool ZipLinkedList::insertEntry(int zip, Voter voter) { // Voter insertion into existing zip node - addFront() is used for creating new nodes
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
	cerr << "Insertion unsuccessful!" << endl;
	return 0;	
}

void ZipLinkedList::remove(int rin, int zip) { // Remove voter from zip list - a somewhat complicated method that has to account for a lot of things
	ZipLLNode* v = head;
	while(v != NULL) {
		if(v->zipCode == zip && v == head) { // If we find it right away at the head, it's a special procedure
			v->value->remove(rin);

			if(v->value->empty() == true) { // If this removal empties the embedded linked list, we have to delete it too, which means going through the whole pointer realignment procedure
				ZipLLNode* temp = head;
				head = temp->next;
				delete temp;
				entryNum--;
			}
			return;
		}

		else if(v->next->zipCode == zip) { // We have to stay one step ahead, because we might have to delete the entire embedded linked list
			v->next->value->remove(rin); // ZipVoterLinkedList inherits from HashLinkedList, so this remove function is identical to the other
			entryNum--;

			if(v->next->value->empty() == true) { // If this removal empties the embedded linked list, we have to delete it too, which means going through the whole pointer realignment procedure
				ZipLLNode* newNext = v->next->next; // Save the to-be-deleted node's next
				delete v->next; // Delete the offending node
				v->next = newNext; // Connect the current node to the next valid one			

			}
			return;
		}

		v = v->next;
	}
	cerr << "Zip code not found in LL!" << endl;
	return;		
}

void ZipLinkedList::printZipEntries(int zip) { // Prints out entries for one specific zip code
	ZipLLNode* v = head;
	while(v != NULL) {
		if(v->zipCode == zip) {
			v->value->displayAll();
			return;
		}

		v = v->next;
	}
	cout << "No entries for this zip code." << endl; // Unless of course we don't have any
	return;
}

void ZipLinkedList::getZipVoterTotals() { // Used for the o command - gets the zip codes in the list and the number of voters who have voted in each, then sorts them
	if(entryNum == 0) { // If there's nothing in the big linked list, there's no use diving into the embedded ones
		cout << "There are currently no entries in the zip list." << endl;
		return;
	}

	ZipLLNode* v = head;
	int zipListStats[entryNum][2] = {0}; // 2D array to store all the zip codes and their # of voters - we know its max size in advance, which is very helpful; oh and we also initialise all its elements to 0 at the start
	int cnt = 0;
	while(v != NULL) { // If we do have something, iterate over every node and query every embedded linked list for its number of entries
		zipListStats[cnt][0] = v->zipCode; // We save two things: the zip code and the number of voters in that zip code
		zipListStats[cnt][1] = v->value->getEntryNum();
		cnt++;
		v = v->next;
	}

	// Now that we have a 2D array with all our zip codes and voters, it's time to sort it!
	int sortedZipListStats[entryNum][2] = {0};
	int currentMaxZip = 0;
	int currentMaxNum = 0;
	int currentMaxLoc = 0; // This saves the position of the highest found number/zip code pair in the 2D array, because we set the number to 0 after the inner loop is over to avoid counting it again on the next iteration

	for(unsigned int i = 0; i < sizeof(zipListStats)/sizeof(zipListStats[0]); i++) { // Selection sort algorithm written from scratch - it's not particularly time-efficient (O(n^2)), but it's simple and it gets the job done
		for(unsigned int j = 0; j < sizeof(zipListStats)/sizeof(zipListStats[0]); j++) { // Casting these as unsigned to match the sizeof() number, plus they can never be negative anyway - caught this thanks to g++ with -Wall
			if(zipListStats[j][1] > currentMaxNum) { // Update current maximum number of voters and the zip code they're in
				currentMaxZip = zipListStats[j][0];
				currentMaxNum = zipListStats[j][1];
				currentMaxLoc = j;
			}
		}
		sortedZipListStats[i][0] = currentMaxZip; // At the end of each loop, we set the earliest available point to the maxes
		sortedZipListStats[i][1] = currentMaxNum;

		zipListStats[currentMaxLoc][1] = 0; // Set what we just found to 0 in the original array; ensures we won't find it again next time - I think it's a pretty elegant solution!

		currentMaxZip = 0;
		currentMaxNum = 0;
	}

	// Finally, we just print the sorted list in order, since we constructed it to be descending!
	cout << "SORTED ZIP LIST STATS" << endl;
	for(int i = 0; i < entryNum; i++) {
		for(int j = 0; j < 2; j++) {
			if(sortedZipListStats[i][0] != 0) { // Ignore any extra 0s that might have remained after filling up the array
				cout << sortedZipListStats[i][j] << " ";
			}
		}
		if(zipListStats[i][0] != 0) {
			cout << endl;
		}
	}

	return;

}

