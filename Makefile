CC=gcc
INCLUDES = -I/usr/include
LIBS = -L/usr/lib

CCFLAGS = -g
OBJS = obj/b_tree.o obj/rb_tree.o obj/binary_tree.o obj/avl_tree.o obj/queue.o obj/memory_pool.o obj/print_binary_tree_ascii.o

all: obj bin bin/tree

obj:
	mkdir -p obj
bin:
	mkdir -p bin
bin/tree: obj/main.o ${OBJS}
	$(CC) obj/main.o ${OBJS} $(CCFLAGS) -o $@ $(INCLUDES) $(LIBS)

obj/main.o: main.c
	${CC} -c main.c $(CCFLAGS) -o $@ $(INCLUDES)
obj/b_tree.o: b_tree.c
	${CC} -c b_tree.c $(CCFLAGS) -o $@ $(INCLUDES)
obj/avl_tree.o: avl_tree.c
	${CC} -c avl_tree.c $(CCFLAGS) -o $@ $(INCLUDES)	
obj/rb_tree.o: rb_tree.c
	${CC} -c rb_tree.c $(CCFLAGS) -o $@ $(INCLUDES)
obj/binary_tree.o: binary_tree.c
	$(CC) -c binary_tree.c $(CCFLAGS) -o $@ $(INCLUDES)
obj/queue.o: ./util/queue.c
	$(CC) -c ./util/queue.c $(CCFLAGS) -o $@ $(INCLUDES)
obj/memory_pool.o: ./util/memory_pool.c
	$(CC) -c ./util/memory_pool.c $(CCFLAGS) -o $@ $(INCLUDES)
obj/print_binary_tree_ascii.o: ./util/print_binary_tree_ascii.c
	$(CC) -c ./util/print_binary_tree_ascii.c $(CCFLAGS) -o $@ $(INCLUDES)

clean:
	rm -rf bin
	rm -rf obj