GREEN=\033[1;32m
YELLOW=\033[1;33m
BLUE=\033[1;34m
RED=\033[1;31m
RESET=\033[0m

CXX = g++

CXXFLAGS = -Wall -Wextra -Wpedantic -std=c++98 -Iinclude

SRCDIR = src
TARGET = TrpJSON

# Get all .cpp files from root and src directory
ROOT_SRC = $(wildcard *.cpp)
SRCDIR_SRC = $(wildcard $(SRCDIR)/*.cpp)

SRC = $(ROOT_SRC) $(SRCDIR_SRC)

OBJDIR = build

# Maintain directory hierarchy in build dir
# Root .cpp files go to build/*.o
# src/*.cpp files go to build/src/*.o  
OBJ = $(patsubst %.cpp,$(OBJDIR)/%.o,$(ROOT_SRC)) \
    	$(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/$(SRCDIR)/%.o,$(SRCDIR_SRC))

all: $(TARGET)

$(TARGET): $(OBJ)
	@echo "$(BLUE)[Linking]$(RESET) $@"
	@$(CXX) $(CXXFLAGS) $(OBJ) -o $@
	@echo "$(GREEN)[Built]$(RESET) $@ ✅"

# Root .cpp -> build/*.o
$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(OBJDIR)
	@echo "$(YELLOW)[Compiling]$(RESET) $< → $@"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# src/*.cpp -> build/src/*.o
$(OBJDIR)/$(SRCDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)/$(SRCDIR)
	@echo "$(YELLOW)[Compiling]$(RESET) $< → $@"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

re: fclean all

clean:
	@echo "$(RED)[Cleaning]$(RESET) removing object files"
	@rm -rf $(OBJDIR)

fclean: clean
	@echo "$(RED)[Cleaning]$(RESET) removing binary $(TARGET)"
	@rm -f $(TARGET)

.PHONY: all re clean fclean