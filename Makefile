OBJS_S = main.o error.o server_conn_instance.o server.o TimerWatcher.o connect_info.o
OBJS_SMP = main_mp.o error.o multi_process_server.o TimerWatcher.o connect_info.o worker_instance.o
OBJS_SS = sub_main.o error.o connect_info.o
OBJS_C = client.o
CC = g++
CFLAGS = -Wall -ggdb -std=c++11

server : $(OBJS_S)
	$(CC) $(CFLAGS) $^ -o $@ -luv

#server_mp : $(OBJS_SMP)
#	$(CC) $(CFLAGS) $^ -o $@ -luv

sub_main : $(OBJS_SS)
	$(CC) $(CFLAGS) $^ -o $@ -luv

%.o    : %.cpp
	$(CC) $(CFLAGS) -c $< -o $@ -luv
	
client : $(OBJS_C)
	$(CC) -pthread $^ -o $@ -lev

$(OBJS_C) : client.cpp
	$(CC) $(CFLAGS) -pthread -c $< -o $@ -lev

clear  :
	rm -f *.o; 
	rm -f server rm client;
	rm sub_main rm server_mp
