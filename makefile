mvote: mvote.o hashTable.o zipList.o
	g++ mvote.o hashTable.o zipList.o -o mvote
zipList.o: zipList.cpp zipList.h
	g++ -g -O1 -c zipList.cpp
hashTable.o: hashTable.cpp hashTable.h
	g++ -g -O1 -c hashTable.cpp
mvote.o: mvote.cpp mvote.h
	g++ -g -O1 -c mvote.cpp
clean:
	rm *.o mvote