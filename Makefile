INCLUDE =-I include/
CC = gcc

auto: main.o socketHandler.o tftpProtocol.o
	$(CC) -o tftpServ build/main.o build/socketHandler.o build/tftpProtocol.o
	mv tftpServ bin/
main.o: src/main.c
	$(CC) -c -Wall $(INCLUDE) src/main.c 
	mv main.o build/
socketHandler.o: src/socketHandler.c
	$(CC) -c -Wall $(INCLUDE) src/socketHandler.c 
	mv socketHandler.o build/
tftpProtocol.o: src/tftpProtocol.c
	$(CC) -c -Wall $(INCLUDE) src/tftpProtocol.c 
	mv tftpProtocol.o build/

build: main.o socketHandler.o tftpProtocol.o
	gcc -o tftpServ build/main.o build/socketHandler.o build/tftpProtocol.o
	mv tftpServ bin/
start: 
	bin/tftpServ
clean:
	rm bin/*
	rm build/*