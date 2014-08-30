CXX = g++
CXXFLAGS = -Wall
LDLIBS = -lglfw3 -lm -lGL -lGLU -lXrandr -lXi

OBJ_DIR=bin
ifeq ($(OS),Windows_NT)
LIB_DIR=-L./lib
INC_DIR=-I./includes
else
LIB_DIR=-L/usr/local/lib
INC_DIR=-I/usr/local/include
endif

SOURCE=main.cpp
OBJECTS=${SOURCE:%.cpp=$(OBJ_DIR)/%.o}
EXECUTABLE=main

all: $(OBJECTS) $(EXECUTABLE)

$(EXECUTABLE):
	$(CXX) $(LD_FLAGS) $(LIB_DIR) -o $@ $(OBJECTS) $(LDLIBS)

$(OBJ_DIR)/%.o: %.cpp
	$(CXX) $(INC_DIR) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR) $(EXECUTABLE)

test:
	cd tests
	make
	cd ..