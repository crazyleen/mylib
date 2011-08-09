

CC=gcc
Flags=-o2 -g -Wall

all:	test

#-static 
test: test.o md5.o packet_eap.o pcap_dev.o blog.o
	$(CC) $(Flags) -o $@  $^   -lnet -lpcap -lpthread -lc

%%c:   %%c
	$(CC) $(Flags) -o $@ -c $< 


clean:
	rm -f *.o test

