CXX = g++ 
CXXFLAGS = -DGLEW_STATIC -Wall --std=c++0x

OBJ_DIR=bin
ifeq ($(OS),Windows_NT)
LIB_DIR=-Llib/Win32/
INC_DIR=-Iincludes
LDLIBS =-lglfw3 -lm -lglew32 -lopengl32 -lglu32
else
LIB_DIR=-L/usr/local/lib
INC_DIR=-I/usr/local/include
LDLIBS =-lglfw -lm -lGLEW -lGL -lGLU -lXrandr -lXi
endif

SOURCE=main.cpp
OBJECTS=${SOURCE:%.cpp=$(OBJ_DIR)/%.o}
EXECUTABLE=main.exe

all: math renderer $(OBJECTS) $(OBJ_BIN)/$(EXECUTABLE)

$(OBJ_BIN)/$(EXECUTABLE):
	$(CXX) $(CXXFLAGS) $(LD_FLAGS) $(LIB_DIR) -o $@ $(OBJECTS) bin/math.a bin/renderer.a $(LDLIBS)

$(OBJ_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INC_DIR) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR) $(EXECUTABLE)/*.o main
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

