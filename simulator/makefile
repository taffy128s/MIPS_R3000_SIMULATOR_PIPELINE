CC = gcc -Wall -O3
SRCS = ./*.c
OBJS = ./*.o

pipeline: SRC
	$(CC) -pthread -o $@ $(OBJS)

SRC: $(SRCS)
	$(CC) -c $(SRCS)

clean: $(OBJS)
	rm $(OBJS) pipeline
