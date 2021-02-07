#include<iostream>
#include<fstream>
#include<cstring> // The string data structure itself is not used, only functions that work with char arrays

using namespace std;

int main() {

	char* input;
	char* command;
	char* param1;
	char* param2;
	char* param3;
	char* param4;
	char* extraInput;
	bool lookupFlag = false;
	input = (char*)malloc(100*sizeof(char));
	command = (char*)malloc(30*sizeof(char));
	param1 = (char*)malloc(30*sizeof(char));
	param2 = (char*)malloc(30*sizeof(char));
	param3 = (char*)malloc(30*sizeof(char));
	param4 = (char*)malloc(30*sizeof(char));
	extraInput = (char*)malloc(30*sizeof(char));

	while(true) {

		cout << "Enter input: " << endl;

		cin.getline(input, 100); // Turns out, you don't need strings to use getline! Found in the reference: http://www.cplusplus.com/reference/istream/istream/getline/

		command = strtok(input, " ");
		if(command == NULL) {
			cerr << "Input cannot be empty." << endl;
			//continue;
			exit(-1);
		}
		cout << "Command: " << command << endl;

		param1 = strtok(NULL, " ");
		if(param1 == NULL) {
			continue;
		}
		cout << "First parameter: " << param1 << endl;

		param2 = strtok(NULL, " ");
		if(param2 == NULL) {

			if(strcmp(command, "l") == 0) {
				char* endptr;
				cout << "Lookup entered." << endl;
				long rin = strtol(param1, &endptr, 10);
				cout << "RIN: " << rin << endl;
				lookupFlag = true;

			}
			continue;
		}
		cout << "Second parameter: " << param2 << endl;		

		if(param2 != NULL) {


			param3 = strtok(NULL, " ");
			if(param3 == NULL) {
				continue;
			}
			cout << "Third parameter: " << param3 << endl;
		}

		if(param3 != NULL) {

		
			param4 = strtok(NULL, " ");
			if(param4 == NULL) {
				continue;
			}
			cout << "Fourth parameter: " << param4 << endl;
		}

/*		if(extraInput != NULL) {
			cout << "Extra input: |" << extraInput << "|" << endl;
			cout << typeid(extraInput).name() << endl;
			cerr << "Too many parameters. Please try again." << endl;
			continue;
		}*/


	}

}