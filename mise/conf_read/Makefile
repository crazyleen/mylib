

CC=gcc
Flags=-g -Wall

all: read

#-static 
read: read_conf.o
	$(CC) $(Flags) -o $@  $^ 

%%c:   %%c
	$(CC) $(Flags) -o $@ -c $< 


clean:
	rm -f *.o read

