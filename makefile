OBJS_S = server.o
OBJS_C = client.o
CC = g++
CFLAGS = -g -O
server : $(OBJS_S)
	$(CC) $^ -o $@ -luv
$(OBJS_S) : uv_server_multi_echo.cpp
	$(CC) $(CFLAGS) -c $< -o $@ -luv

client : $(OBJS_C)
	$(CC) -pthread $^ -o $@ -lev
$(OBJS_C) : client.cpp
	$(CC) $(CFLAGS) -pthread -c $< -o $@ -lev
