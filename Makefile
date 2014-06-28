CXX = g++ 
CXXFLAGS = -Wall
LDLIBS =-lglfw -lm -lGLEW -lGL -lGLU -lXrandr -lXi

OBJ_DIR = bin
LIB_DIR = -L/usr/lib64:/usr/lib:/usr/local/lib
INC_DIR = -I.:/usr/local/include:./Includes

SOURCE = main.cpp
OBJECTS = ${SOURCE:%.cpp=$(OBJ_DIR)/%.o}
EXECUTABLE = main

all: init $(OBJECTS) $(EXECUTABLE)

$(EXECUTABLE):
	$(CXX) $(CXXFLAGS) $(LD_FLAGS) $(LIB_DIR) -o $@ $(OBJECTS) bin/math.a bin/renderer.a $(LDLIBS)

$(OBJ_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INC_DIR) -c $< -o $@

init:
	@mkdir -p "$(OBJ_DIR)"
	make math
clean:
	@rm -rf $(OBJ_DIR) $(EXECUTABLE)

test:
	cd tests
	make
	cd ..
