BJS_S = error.o server_conn_instance.o server.o TimerWatcher.o connect_info.o main.o
OBJS_C = client.o
CC = g++
CFLAGS = -Wall -ggdb -O -std=c++11
server : $(OBJS_S)
	$(CC) $^ -o $@ -luv
%.o    : %.cpp
	$(CC) $(CFLAGS) -c $< -o $@ -luv
client : $(OBJS_C)
	$(CC) -pthread $^ -o $@ -lev
$(OBJS_C) : client.cpp
	$(CC) $(CFLAGS) -pthread -c $< -o $@ -lev
clear  : rm *.o rm server rm client
