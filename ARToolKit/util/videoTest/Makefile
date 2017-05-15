INC_DIR= ../../include
LIB_DIR= ../../lib
BIN_DIR= ../../bin

LDFLAG=-lgstreamer-0.10 -lgobject-2.0 -lgmodule-2.0 -pthread -lgthread-2.0 -pthread -lglib-2.0 -lxml2 -L/usr/X11R6/lib -L/usr/local/lib -L$(LIB_DIR)
LIBS= -lARgsub -lARvideo -lAR -lpthread -lglut -lGLU -lGL -lXi -lX11 -lm
CFLAG= -O -pthread -I/usr/include/gstreamer-0.10 -I/usr/include/glib-2.0 -I/usr/lib64/glib-2.0/include -I/usr/include/libxml2 -I/usr/X11R6/include -I$(INC_DIR)

OBJS =
HEADDERS =

all: $(BIN_DIR)/videoTest

$(BIN_DIR)/videoTest: videoTest.o $(OBJS)
	cc -o $(BIN_DIR)/videoTest videoTest.o $(OBJS) $(LDFLAG) $(LIBS)

videoTest.o: videoTest.c $(HEADDERS)
	cc -c $(CFLAG) videoTest.c

clean:
	rm -f *.o
	rm -f $(BIN_DIR)/videoTest

allclean:
	rm -f *.o
	rm -f $(BIN_DIR)/videoTest
	rm -f Makefile
