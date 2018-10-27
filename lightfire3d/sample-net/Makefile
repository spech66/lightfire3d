progname = sample2
OBJECTS = main.o
CXX= g++
DEBUG= #-ggdb
CCFLAGS= -ldl -Wl -export-dynamic
INCLUDES = -I/usr/local/include `sdl-config --cflags`

LIBS= -L/usr/local/lib -L/usr/X11R6/lib -L../src -lGLU -lGL \
	-LIBS=`sdl-config --libs` -llf3d -LIBS=`audiere-config --libs`

$(progname) : $(OBJECTS)
	$(CXX) $(DEBUG) $(CCFLAGS) -o $(progname) $(OBJECTS) $(LIBS)

%.o: %.cpp %.h
	$(CXX) -c $< $(INCLUDES)

.PHONY : clean
clean :
	rm -f $(progname) $(OBJECTS)
