

CC=gcc
Flags=-o2 -g -Wall

TARGET=test

all:	$(TARGET)

#-static 
$(TARGET): $(TARGET).o mynotify.o
	$(CC) $(Flags) -o $@  $^ `pkg-config --cflags --libs gtk+-2.0`    -lnet -lpcap -lpthread -lc   -l notify

mynotify.o: mynotify.c 
	$(CC) $(Flags) -c mynotify.c -o mynotify.o `pkg-config --cflags --libs gtk+-2.0` -l notify
	
%%c:   %%c
	$(CC) $(Flags) -o $@ -c $<  `pkg-config --cflags --libs gtk+-2.0` -l notify


clean:
	rm -f *.o test

