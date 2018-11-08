# Compiler
CC = gcc

#Flags
dbFLAGS = -g -Wall -Wextra
RFlags = -03

#Program names
TARGET = cracker
DEBUG = dbcracker
ENCRYPT = encrypt

$(TARGET): cracker.o DA.o BFA.o
	$(CC) $(RFLAGS) -o $(TARGET) cracker.o DA.o BFA.o -lcrypt -fopenmp

$(DEBUG): cracker.o DA.o BFA.o
	$(CC) $(dbFLAGS) -o $(DEBUG) cracker.o DA.o BFA.o -lcrypt -fopenmp

cracker.o: cracker.c
	$(CC) $(RFLAGS) -c -o cracker.o cracker.c

DA.o: DA.c
	$(CC) $(RFLAGS) -c -o DA.o DA.c

BFA.o: BFA.c
	$(CC) $(RFLAGS) -c -o BFA.o BFA.c

$(ENCRYPT): crypto.c
	$(CC) crypto.c -o $(ENCRYPT) -lcrypt

clean:
	rm *.o $(TARGET)
	rm -f $(TARGET)

cleandb:
	rm *.o $(DEBUG)
	rm -f $(DEBUG)

cleanencrypt: 
	rm -f $(ENCRYPT)

remake: 
	clean $(TARGET)