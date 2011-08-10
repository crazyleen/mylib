


CC=gcc



obj=mylog.o mylogd.o test.o
ALL=mylogd test

all: $(ALL)

$(obj): %.o:%.c
	$(CC) -c  $< -o $@

mylogd: mylogd.o mylog.h mylog.o
	$(CC) $^ -o $@

test: test.o mylog.o
	$(CC) $^ -o $@

.PHONY: clean
clean:
	rm -rf mylogd *.o


