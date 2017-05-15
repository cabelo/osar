INC_DIR= ../../include
LIB_DIR= ../../lib
BIN_DIR= ../../bin

LDFLAG=-lgstreamer-0.10 -lgobject-2.0 -lgmodule-2.0 -pthread -lgthread-2.0 -pthread -lglib-2.0 -lxml2 -L/usr/X11R6/lib -L/usr/local/lib -L$(LIB_DIR)
LIBS= -lARgsub -lARvideo -lAR -lpthread -lglut -lGLU -lGL -lXi -lX11 -lm
CFLAG= -O -pthread -I/usr/include/gstreamer-0.10 -I/usr/include/glib-2.0 -I/usr/lib64/glib-2.0/include -I/usr/include/libxml2 -I/usr/X11R6/include -I$(INC_DIR)


OBJS = object.o draw_object.o
HEADDERS = object.h draw_object.h

all: $(BIN_DIR)/modeTest

$(BIN_DIR)/modeTest: modeTest.o $(OBJS)
	cc -o $(BIN_DIR)/modeTest modeTest.o $(OBJS) $(LDFLAG) $(LIBS)

modeTest.o: modeTest.c $(HEADDERS)
	cc -c $(CFLAG) modeTest.c

object.o: object.c $(HEADDERS)
	cc -c $(CFLAG) object.c

draw_object.o: draw_object.c $(HEADDERS)
	cc -c $(CFLAG) draw_object.c

clean:
	rm -f *.o
	rm -f $(BIN_DIR)/modeTest

allclean:
	rm -f *.o
	rm -f $(BIN_DIR)/modeTest
	rm -f Makefile
