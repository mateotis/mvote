mvote: mvote.o hashTable.o
	g++ mvote.o hashTable.o -o mvote
hashTable.o: hashTable.cpp hashTable.h
	g++ -c hashTable.cpp
mvote.o: mvote.cpp mvote.h
	g++ -c mvote.cpp
clean:
	rm *.o mvote