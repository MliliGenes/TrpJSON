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
    $(LIB_OBJ)

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

fclean: clean clean-benchmark
	@echo "[$(DATE)] [Cleaning] removing binary $(TARGET)"
	@rm -f $(TARGET)
	@rm -f $(STATIC_LIB)
	@rm -f $(BENCHMARK_TARGET)

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

BENCHMARK_DIR = benchmark
BENCHMARK_SRC = $(BENCHMARK_DIR)/benchmark.cpp
BENCHMARK_TARGET = $(BENCHMARK_DIR)/benchmark
BENCHMARK_OBJ = $(OBJDIR)/$(BENCHMARK_DIR)/benchmark.o

benchmark: $(BENCHMARK_TARGET)

$(BENCHMARK_TARGET): $(BENCHMARK_OBJ) $(LIB_OBJ)
	@echo "[$(DATE)] [Linking Benchmark] $@"
	@$(CXX) $(CXXFLAGS) $(BENCHMARK_OBJ) $(LIB_OBJ) -o $@
	@echo "[$(DATE)] [Built] Benchmark executable ready!"
	@echo ""
	@echo "Usage:"
	@echo "   ./benchmark/benchmark          - Run basic benchmarks"
	@echo "   ./benchmark/run_benchmarks.sh  - Run comprehensive analysis with valgrind"

$(OBJDIR)/$(BENCHMARK_DIR)/%.o: $(BENCHMARK_DIR)/%.cpp
	@mkdir -p $(OBJDIR)/$(BENCHMARK_DIR)
	@echo "[$(DATE)] [Compiling Benchmark] $< → $@"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

run-benchmarks: benchmark
	@echo "[$(DATE)] [Running] Comprehensive benchmark suite..."
	@cd $(BENCHMARK_DIR) && ./run_benchmarks.sh

clean-benchmark:
	@echo "[$(DATE)] [Cleaning] benchmark objects and results"
	@rm -f $(BENCHMARK_TARGET)
	@rm -f $(BENCHMARK_OBJ)
	@rm -rf $(BENCHMARK_DIR)/results

.PHONY: all lib benchmark run-benchmarks clean-benchmark re clean fclean libclean libfclean install uninstall
