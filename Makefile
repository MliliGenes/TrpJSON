# Get current date and time
DATE := $(shell date +"%Y-%m-%d %H:%M:%S")

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -ggdb -std=c++98 -Iinclude

SRCDIR = src
TARGET = TrpJSON

# Get all .cpp files from root and src directory
ROOT_SRC = $(wildcard *.cpp)
SRCDIR_SRC = $(wildcard $(SRCDIR)/*.cpp)
CORE_SRC = $(wildcard $(SRCDIR)/core/*.cpp)
PARSER_SRC = $(wildcard $(SRCDIR)/parser/*.cpp)
VALUES_SRC = $(wildcard $(SRCDIR)/values/*.cpp)

SRC = $(ROOT_SRC) $(SRCDIR_SRC) $(CORE_SRC) $(PARSER_SRC) $(VALUES_SRC)

OBJDIR = build

# Maintain directory hierarchy in build dir
# Root .cpp files go to build/*.o
# src/*.cpp files go to build/src/*.o  
OBJ = $(patsubst %.cpp,$(OBJDIR)/%.o,$(ROOT_SRC)) \
    $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/$(SRCDIR)/%.o,$(SRCDIR_SRC)) \
    $(patsubst $(SRCDIR)/core/%.cpp,$(OBJDIR)/$(SRCDIR)/core/%.o,$(CORE_SRC)) \
    $(patsubst $(SRCDIR)/parser/%.cpp,$(OBJDIR)/$(SRCDIR)/parser/%.o,$(PARSER_SRC)) \
    $(patsubst $(SRCDIR)/values/%.cpp,$(OBJDIR)/$(SRCDIR)/values/%.o,$(VALUES_SRC))

all: $(TARGET)

$(TARGET): $(OBJ)
	@echo "[$(DATE)] [Linking] $@"
	@$(CXX) $(CXXFLAGS) $(OBJ) -o $@
	@echo "[$(DATE)] [Built] $@ - 100% complete"

# Calculate total number of source files for percentage
TOTAL_FILES := $(words $(SRC))

# Initialize counter for progress tracking
CURRENT_FILE = 0

define print_progress
$(eval CURRENT_FILE=$(shell echo $$(($(CURRENT_FILE) + 1))))
$(eval PERCENT=$(shell echo $$(($(CURRENT_FILE) * 100 / $(TOTAL_FILES)))))
echo "[$(DATE)] [Compiling] $< → $@ - $(PERCENT)% complete"
endef

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(OBJDIR)
	@$(call print_progress)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/$(SRCDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)/$(SRCDIR)
	@$(call print_progress)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/$(SRCDIR)/core/%.o: $(SRCDIR)/core/%.cpp
	@mkdir -p $(OBJDIR)/$(SRCDIR)/core
	@$(call print_progress)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/$(SRCDIR)/parser/%.o: $(SRCDIR)/parser/%.cpp
	@mkdir -p $(OBJDIR)/$(SRCDIR)/parser
	@$(call print_progress)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/$(SRCDIR)/values/%.o: $(SRCDIR)/values/%.cpp
	@mkdir -p $(OBJDIR)/$(SRCDIR)/values
	@$(call print_progress)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

re: fclean all

clean:
	@echo "[$(DATE)] [Cleaning] removing object files"
	@rm -rf $(OBJDIR)

fclean: clean
	@echo "[$(DATE)] [Cleaning] removing binary $(TARGET)"
	@rm -f $(TARGET)

.PHONY: all re clean fclean
