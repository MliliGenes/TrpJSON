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

TRP_SRC = $(SRCDIR_SRC) $(CORE_SRC) $(PARSER_SRC) $(VALUES_SRC)
SRC = $(ROOT_SRC) $(TRP_SRC)

# Library sources (exclude main.cpp for static library)
LIB_SRC = $(TRP_SRC)
STATIC_LIB = libtrpjson.a

OBJDIR = build

# Maintain directory hierarchy in build dir
# Root .cpp files go to build/*.o
# src/*.cpp files go to build/src/*.o  
OBJ = $(patsubst %.cpp,$(OBJDIR)/%.o,$(ROOT_SRC)) \
    $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/$(SRCDIR)/%.o,$(SRCDIR_SRC)) \
    $(patsubst $(SRCDIR)/core/%.cpp,$(OBJDIR)/$(SRCDIR)/core/%.o,$(CORE_SRC)) \
    $(patsubst $(SRCDIR)/parser/%.cpp,$(OBJDIR)/$(SRCDIR)/parser/%.o,$(PARSER_SRC)) \
    $(patsubst $(SRCDIR)/values/%.cpp,$(OBJDIR)/$(SRCDIR)/values/%.o,$(VALUES_SRC))

# Library object files (exclude main.o)
LIB_OBJ = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/$(SRCDIR)/%.o,$(SRCDIR_SRC)) \
    $(patsubst $(SRCDIR)/core/%.cpp,$(OBJDIR)/$(SRCDIR)/core/%.o,$(CORE_SRC)) \
    $(patsubst $(SRCDIR)/parser/%.cpp,$(OBJDIR)/$(SRCDIR)/parser/%.o,$(PARSER_SRC)) \
    $(patsubst $(SRCDIR)/values/%.cpp,$(OBJDIR)/$(SRCDIR)/values/%.o,$(VALUES_SRC))

all: $(TARGET)

$(TARGET): $(OBJ)
	@echo "[$(DATE)] [Linking] $@"
	@$(CXX) $(CXXFLAGS) $(OBJ) -o $@
	@echo "[$(DATE)] [Built] $@ - 100% complete"

lib: $(STATIC_LIB)

$(STATIC_LIB): $(LIB_OBJ)
	@echo "[$(DATE)] [Creating Static Library] $@"
	@ar rcs $@ $(LIB_OBJ)
	@echo "[$(DATE)] [Built] $@ - Static library ready for use!"
	@echo ""
	@echo "Library Usage Instructions:"
	@echo "   Include: -I$(PWD)/include"
	@echo "   Link:    -L$(PWD) -ltrpjson"
	@echo "   Example: g++ -std=c++98 -I$(PWD)/include your_file.cpp -L$(PWD) -ltrpjson"
	@echo ""

TOTAL_FILES := $(words $(SRC))

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
	@rm -f $(STATIC_LIB)

libclean:
	@echo "[$(DATE)] [Cleaning] removing static library $(STATIC_LIB)"
	@rm -f $(STATIC_LIB)

libfclean: clean libclean

install: $(STATIC_LIB)
	@echo "[$(DATE)] [Installing] TrpJSON library to system"
	@sudo mkdir -p /usr/local/lib
	@sudo mkdir -p /usr/local/include/trpjson
	@sudo cp $(STATIC_LIB) /usr/local/lib/
	@sudo cp -r include/* /usr/local/include/trpjson/
	@echo "[$(DATE)] [Installed] TrpJSON library to /usr/local/"
	@echo "   Use: g++ -std=c++98 -ltrpjson your_file.cpp"
	@echo "   Include: #include <trpjson/parser/TrpJsonParser.hpp>"

uninstall:
	@echo "[$(DATE)] [Uninstalling] TrpJSON library from system"
	@sudo rm -f /usr/local/lib/$(STATIC_LIB)
	@sudo rm -rf /usr/local/include/trpjson
	@echo "[$(DATE)] [Uninstalled] TrpJSON library removed"

.PHONY: all lib re clean fclean libclean libfclean install uninstall
