#this target will compile all the file 
OBJS = file1 file2 file3 
CC = gcc


all: file1 file2 file3  
#create a target

file1:
	$(CC) client.c -o client -lpthread

file2:
	$(CC) -Wall -w -c server.c -o server.o
	$(CC) -Wall -w -c server_unix_action.c -o server_unix_action.o
	$(CC) -Wall -w -c server_unix_setup.c -o server_unix_setup.o
	$(CC) -Wall -w -c extraction.c -o extraction.o
	$(CC) -Wall -w -c packet_generator.c -o packet_generator.o
	$(CC) -Wall -w -c session_db.c -o session_entry.o
	$(CC) -Wall -w -c socket_db.c -o session_db.o
file3: 
	$(CC) -o ServerTest server.o server_unix_action.o server_unix_setup.o extraction.o \
	packet_generator.o session_entry.o session_db.o



clean:
	rm -rf $(OBJS)
