progname = liblf3d
OBJECTS = Main.o LogFile.o FontLoader.o ImageLoader.o TerrainLoader.o ModelLoader.o GraphicDevice.o AudioDevice.o CameraDevice.o InputDevice.o SceneGraph.o SceneNode.o SceneNodeModel.o SceneNodeOctree.o Helper.o Types.o NetworkServer.o NetworkClient.o
CXX?= g++
CXXFLAGS?= #-g

all: $(progname).a $(progname).so

$(progname).so : $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS) -shared -Wl,-soname=$(progname).so

$(progname).a : $(OBJECTS)
	ar rcs $(progname).a $(OBJECTS) 

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -DPIC -fpic -fPIC -c $< -o $@ -I/usr/X11R6/include `sdl-config --cflags` `audiere-config --cxxflags` 

.PHONY : clean
clean :
	rm -f $(progname).a $(progname).so $(OBJECTS)
