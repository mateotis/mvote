mvote: mvote.o hashTable.o
	g++ -std=c++11 mvote.o hashTable.o -o mvote
hashTable.o: hashTable.cpp hashTable.h
	g++ -std=c++11 -c hashTable.cpp
mvote.o: mvote.cpp mvote.h
	g++ -std=c++11 -c mvote.cpp
clean:
	rm *.o mvote