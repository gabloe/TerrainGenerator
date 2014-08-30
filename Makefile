CXX = g++ 
CXXFLAGS = -Wall
LDLIBS =-lglfw -lm -lGLEW -lGL -lGLU -lXrandr -lXi

<<<<<<< HEAD
OBJ_DIR=bin
ifeq ($(OS),Windows_NT)
LIB_DIR=-L./lib
INC_DIR=-I./includes
else
LIB_DIR=-L/usr/local/lib
INC_DIR=-I/usr/local/include
endif
=======
OBJ_DIR = bin
LIB_DIR = -L/usr/lib64:/usr/lib:/usr/local/lib
INC_DIR = -I.:/usr/local/include:./Includes
>>>>>>> cbf25dac8f6d7d9bddfa3e0d5539d6d0a4986255

SOURCE=main.cpp
OBJECTS=${SOURCE:%.cpp=$(OBJ_DIR)/%.o}
EXECUTABLE=main

<<<<<<< HEAD
all: $(OBJECTS) $(EXECUTABLE)
=======

all: init math renderer $(OBJECTS) $(EXECUTABLE)
>>>>>>> cbf25dac8f6d7d9bddfa3e0d5539d6d0a4986255

$(EXECUTABLE):
	$(CXX) $(CXXFLAGS) $(LD_FLAGS) $(LIB_DIR) -o $@ $(OBJECTS) bin/math.a bin/renderer.a $(LDLIBS)

$(OBJ_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INC_DIR) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR) $(EXECUTABLE)
	$(MAKE) -C math clean
	$(MAKE) -C renderer clean

test:
	cd tests
	make
	cd ..


.PHONY:	math

.PHONY: renderer


renderer:
	$(MAKE) -C renderer

math:
	$(MAKE) -C math

