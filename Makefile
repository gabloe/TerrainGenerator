CXX = clang++
CXXFLAGS = -Wall
LDLIBS = -lglfw3 -lm -lGL -lGLU -lXrandr -lXi

OBJ_DIR = bin
LIB_DIR = -L/usr/local/lib
INC_DIR = -I/usr/local/include

SOURCE = main.cpp
OBJECTS = ${SOURCE:%.cpp=$(OBJ_DIR)/%.o}
EXECUTABLE = main

all: init $(OBJECTS) $(EXECUTABLE)

$(EXECUTABLE):
	$(CXX) $(LD_FLAGS) $(LIB_DIR) -o $@ $(OBJECTS) $(LDLIBS)

$(OBJ_DIR)/%.o: %.cpp
	$(CXX) $(INC_DIR) -c $< -o $@

init:
	@mkdir -p "$(OBJ_DIR)"

clean:
	@rm -rf $(OBJ_DIR) $(EXECUTABLE)
