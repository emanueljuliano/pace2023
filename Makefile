CC = g++
CXXFLAGS = -std=c++17 -g -Wall -fsanitize=address,undefined -fno-omit-frame-pointer

INCLUDE_FOLDER = ./include/
OBJ_FOLDER = ./obj/
SRC_FOLDER = ./src/

TARGET = a
SRC = $(wildcard $(SRC_FOLDER)*.cpp)
OBJ = $(patsubst $(SRC_FOLDER)%.cpp, $(OBJ_FOLDER)%.o, $(SRC))

$(OBJ_FOLDER)%.o: $(SRC_FOLDER)%.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@ -I$(INCLUDE_FOLDER)

all: $(OBJ) cadical
	$(CC) $(CXXFLAGS) -o ./$(TARGET) $(OBJ)

cadical:
	cd cadical && ./configure && $(MAKE)

clean:
	@rm -rf $(OBJ_FOLDER)*
