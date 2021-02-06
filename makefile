mvote: mvote.o hashTable.o zipList.o
	g++ mvote.o hashTable.o zipList.o -o mvote
zipList.o: zipList.cpp zipList.h
	g++ -c zipList.cpp
hashTable.o: hashTable.cpp hashTable.h
	g++ -c hashTable.cpp
mvote.o: mvote.cpp mvote.h
	g++ -c mvote.cpp
clean:
	rm *.o mvote