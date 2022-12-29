
all:: test

OBJS += $(patsubst %.c,%,$(wildcard *.c))

${OBJS}: %: %.c ../argparse.c
	$(CC) -I../ -o $@ ${DEP_LIBS} $^
