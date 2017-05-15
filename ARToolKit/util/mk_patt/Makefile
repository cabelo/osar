INC_DIR= ../../include
LIB_DIR= ../../lib
BIN_DIR= ../../bin

LDFLAG=-lgstreamer-0.10 -lgobject-2.0 -lgmodule-2.0 -pthread -lgthread-2.0 -pthread -lglib-2.0 -lxml2 -L/usr/X11R6/lib -L/usr/local/lib -L$(LIB_DIR)
LIBS= -lARgsub_lite -lARvideo -lAR -lpthread -lglut -lGLU -lGL -lXi -lX11 -lm
CFLAG= -O -pthread -I/usr/include/gstreamer-0.10 -I/usr/include/glib-2.0 -I/usr/lib64/glib-2.0/include -I/usr/include/libxml2 -I/usr/X11R6/include -I$(INC_DIR)


all: $(BIN_DIR)/mk_patt


$(BIN_DIR)/mk_patt: mk_patt.c
	cc -o $(BIN_DIR)/mk_patt $(CFLAG) mk_patt.c\
	   $(LDFLAG) $(LIBS)

clean:
	rm -f $(BIN_DIR)/mk_patt

allclean:
	rm -f $(BIN_DIR)/mk_patt
	rm -f Makefile
