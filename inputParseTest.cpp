#include<iostream>
#include<fstream>
#include<cstring> // The string data structure itself is not used, only functions that work with char arrays

using namespace std;

int main() {

	while(true) {

		cout << "boop" << endl;
		char* command;
		command = (char*)malloc(10*sizeof(char));
		cout << "Enter input: " << endl;
		cin >> command;
		cout << "beep" << endl;
		
		if(strcmp(command, "l") == 0) {
			cout << "Lookup entered." << endl;

		}
		else if(strcmp(command, "perc") == 0) {
			cout << "Percentage entered." << endl;
		}
		else {
			cout << "Unrecognised command: " << command << endl;
		}
	}

}