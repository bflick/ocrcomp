CC=g++ -Wall -O2 
FLAGS=-I /aux/brian/local/include -L /aux/brian/local/lib -ltesseract -llept
UP=Utilities/
SC=Scanner
SL=ScanLine
UT=Utils

main: ReadImage.cpp Compare.cpp Utils.o $(SC).o $(SL).o makefile 
	$(CC) $(FLAGS) -o readimg ReadImage.cpp Utils.o
	$(CC) -o comp Compare.cpp $(SC).o $(UT).o $(SL).o

Utils.o: $(UP)$(UT).cpp $(UP)$(UT).h
	$(CC) -c $(UP)$(UT).cpp

Scanner.o: $(UP)$(SC).cpp $(UP)$(SC).h
	$(CC) -c $(UP)$(SC).cpp

ScanLine.o: $(UP)$(SL).cpp $(UP)$(SL).h
	$(CC) -c $(UP)$(SL).cpp

clean:
	rm -vf readimg *.o *~
