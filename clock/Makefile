PROGRAM = clock
OBJECTS = main.o
CXX = g++
LDDFLAGS = -Wl,-rpath=. -ldl -lGLU -lGL -llf3d
CXXFLAGS = -g -export-dynamic `sdl-config --cflags` `audiere-config --cxxflags`
INCLUDES = -I/usr/X11R6/include
LIBS = -L../../Programmieren/lightfire3d/core `sdl-config --libs` `audiere-config --libs`

all: $(PROGRAM)
$(PROGRAM) : $(OBJECTS)
	$(CXX) $(LDDFLAGS) -o $@ $(LIBS) $(OBJECTS)

%.o: %.cpp %.h
	$(CXX) -c $< -o $@ $(INCLUDES) $(CXXFLAGS)

.PHONY : clean
clean :
	rm -f $(PROGRAM) $(OBJECTS)