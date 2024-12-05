TARGET_1 = run
SRCS_1 = list.c main.c my_spinlock.c

CC=gcc
RM=rm
CFLAGS= -g -Wall
INCLUDE_DIR="."

all: ${TARGET_1}

${TARGET_1}: list.h my_spinlock.h ${SRCS_1}
	${CC} ${CFLAGS} -I${INCLUDE_DIR} ${SRCS_1} -o ${TARGET_1}

clean:
	${RM} -f *.o ${TARGET_1}