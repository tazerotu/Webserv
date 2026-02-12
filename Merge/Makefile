CC = c++

TARGET = webserv

SRC_DIR = src/
OBJ_DIR = obj/

SRC_FILES = $(shell find $(SRC_DIR) -name "*.cpp")
#SRC_FILES = $(wildcard $(SRC_DIR)*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)%.cpp,$(OBJ_DIR)%.o,$(SRC_FILES))

# -Wall -Werror -Wextra
CFLAGS = -std=c++98 -g
SANITIZER_FLAGS = #-fsanitize=address

LIBS = 

all : $(TARGET)
	@echo "default rule"

# Rule to build the target
$(TARGET): $(OBJ_FILES)
	@echo "\033[1;34mCreating executable...\033[0m"
	$(CC) $(CFLAGS) $(SANITIZER_FLAGS) $(OBJ_FILES) -o $(TARGET)
	@echo "Executable name: \033[33m$(TARGET)\033[0m"

# Rule to compile the object files
$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp #| $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@echo "\033[1;34mCompiling $<...\033[0m"
	$(CC) $(CFLAGS) $(SANITIZER_FLAGS) -c $< -o $@

# Rule to create the object directory if it doesn't exist
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# Clean object files
clean :
	@echo "\033[1;31mCleaning file objects...\033[0m"
	rm -rf $(OBJ_DIR)

# Clean all including the library and executable
fclean : clean
	@echo "\033[1;31mComplete cleaning (executable and objects)...\033[0m"
	rm -rf $(TARGET)

# Rebuild everything
re : fclean all

.PHONY : all clean fclean re
