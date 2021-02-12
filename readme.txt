===MVOTE WRITE-UP===
Máté Hekfusz

This is a write-up/readme for the mvote program. I have spent more time on this project than perhaps any other CS assignment - for two weeks, I worked on it consistently. There were plenty of frustrations and the most inexplicable issues throughout, but after some time, I took them more as a challenge than an annoyance. Along the way, I learned a lot about the intricacies of both C and C++, as while the program is written in C++, the limitations meant that I had to learn and adopt some C practices as well.

The result of this long learning process, I hope, speaks for itself.

==FILE STRUCTURE==

mvote is made up of seven internal files: three headers, three cpp files, and one makefile pulling them all together. The user is also supposed to supply an input file from which the program can read voter data.

Additionally, I have included in my submission a sample batchVoting.csv file which contains a few hundred RINs from the voters100000.csv file, to be used for testing the bv command.

1. mvote.cpp is the main file and handles input file parsing and user command execution.
2. mvote.h defines the Voter class, the central structure of the program.
3. hashTable.cpp defines the main functions of the hash table and the linked list that accompanies it.
4. hashTable.h declares all functions of the table, the list, and their nodes, and defines some of the simpler ones for modularity.
5. zipList.cpp defines the main functions of the zip code-based linked list.
6. zipList.h declares/defines the functions of the aforementioned linked list, as well as its node and a third linked list which inherits from the hash table's linked list.
7. The makefile uses separate compilation to link all six of these files together.

==DATA STRUCTURES==

There are several dynamic data structures in mvote, all of them interconnected to some level.

There is the main hash table which is built on an array of hash nodes. The hash nodes all contain HashLinkedLists, which are linked lists that store Voter objects. The hash nodes are created based on their hash value, which is based on voter RIN. All RINs (and accompanying voter data) with the same hash code are placed in the same hash node and the same linked list, which can extend indefinitely, making insertion/removal of voter dynamic.

The hash table's size is determined on runtime: the program scans the given file top to bottom, counting the number of lines, then the hash table is created at twice the number of lines. This greatly reduces load on the table at any input size, allowing for fast access. As the hash table uses separate chaining instead of open addressing, there is no penalty in having a large hash table: no iteration happens in the table itself, it only happens in the linked lists.

The hash linked list (HashLinkedList and LLNode classes) implementation, as mentioned in the code, is based on our work back in Data Structures labs, although almost every function is modified and a bunch of new ones are added (such as Voter object retrieval and any-node-removal) to cover every requirement. The actual voter data is stored in the nodes of these linked lists.

The zip-based linked list (ZipLinkedList and ZipLLNode classes) is another linked list which stores data based on zip codes. Each node contains a *third* linked list, called ZipVoterLinkedList, which inherits all of its functionality from HashLinkedList, as it also handles voter data. The flow of data will be explained later, which will hopefully clear up any confusion.

==PROGRAM FLOW==

This section will walk you through the execution of mvote and explain a few more design choices on the way.

The program is called with one parameter, -f, after which the input file name should be specified. After determining the size of the file, the program reads it line by line, parsing the uniformly-formatted input in a while loop and inserts it one-by-one into the hash table. The hash table calculates the hash code of the RIN and either creates a new node and linked list to store it in, or passes the voter data to an existing node's linked list to add to.

The zip linked list is also created here, but for now it remains empty, as we only need to store people who have already voted inside it, and every voter starts out as not having voted.

After parsing is finished, the user is given an introduction and prompted to enter a command. I included a handy "man" command that displays all valid commands and their parameters for your convenience.

User input is processed by tokenising it word by word and handling each functionality as it comes up. If the command is just one word, the program detects that and finds which of the one-word commands it is. If it's two words, the program does the same and handles the second word (the parameter) appropriately for each use case. The maximum number of "words" is five: the command and four parameters for the insertion command. If a command includes more parameters than four, the program handles that too and prompts the user to enter a different one.

The zip linked list comes into play when the register command is used. mvote first gets the specified voter from the hash table, then creates a new Voter object with the same data, which is then passed to the zip list. The zip list first searches for the zip code: if a node with that zip already exists, the voter is passed to it, if not, it creates a new one. The zip list node then passes to its embedded (and RIN-based) ZipVoterLinkedList which finally stores the voter. Access is done with the same logic: first we find the zip list node with the right zip code, then we find the right embedded list node with the right RIN.

Each command that can affect both the hash table and the zip list (insertion, removal, registration) also ensures synchronicity between the two. As there are distinct voter objects in each, this is essential, and it ensures that data in these two main data structures match.

Upon exiting, the program frees up all dynamically allocated memory, including the hash table and the zip list. Each of these has a defined destructor which also cleans out all its embedded data, ensuring that there are no leaks.

==FINAL NOTES==

As per requirements, mvote is as self-sufficient as possible. I wrote each of the aforementioned data structures myself, and there is almost no reliance on external libraries. Only three libraries are used throughout the entirety of the program: iostream for cin and cout, fstream for file I/O, and cstring, not for the string, but for char array methods such as strncpy and strcmp that are used to accurately process voter names and to parse user input, respectively. The entire program is free of strings and all STL data structures like vectors and lists. This was one of the hardest things to accomplish, but I'm quite proud that I was able to.

I also spent a lot of time trying to make mvote robust and reliable, fixing bugs and implementing sanity checks wherever I could. As a result, it can handle all kinds of wonky inputs: missing parameters, wrong parameters, deletion/registration of already deleted/registered voters, etc. It's not perfect on that front, but I think it's a decent effort.

Along the journey, I maintained a Github repository where you can see all my progress as it happened. This repo can be found here: https://github.com/mateotis/mvote/ It's currently private, but I will make it public after the assignment due date has passed, to avoid any complications.

The program is also extensively commented; I did my best to explain and contextualise everything I was doing in each file. If you have any questions after reading this readme, I think you will find the answers to them among the code comments. And if not, feel free to reach out to me any time. :)

--Máté