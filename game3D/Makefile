.DEFAULT_GOAL 	:= run
MAKEFLAGS 		+= -j16
CXX 			:= g++
OUTPUT 			:= game3d.exe
OS 				:= $(shell uname -o)
SRC_DIR     	:= ./src
BUILD_DIR   	:= ./build

ifeq ($(OS), Msys)
	CXX_FLAGS 		:= -Wall
	INCLUDES 		:= -I$(SRC_DIR)
	LDFLAGS 		:= -lglfw3 -lopengl32
endif

OBJ_FILES 		:= $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(wildcard $(SRC_DIR)/*.cpp)) 
OBJ_FILES      	+= $(patsubst $(SRC_DIR)/glad/%.cpp,$(BUILD_DIR)/glad/%.o,$(wildcard $(SRC_DIR)/glad/*.cpp))
H_FILES			:= $(wildcard $(SRC_DIR)/*.h) $(wildcard $(SRC_DIR)/shader/*.h)

all: $(OUTPUT)

$(OUTPUT): $(OBJ_FILES)
	$(CXX) $(OBJ_FILES) $(LDFLAGS) -o ./bin/$@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(H_FILES)
	$(CXX) -c $(CXX_FLAGS) $(INCLUDES) $< -o $@

run: $(OUTPUT)
	cd bin && ./$(OUTPUT) && cd ..

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)/*.o ./bin/$(OUTPUT) 
