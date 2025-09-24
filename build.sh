#!/bin/bash

# Define the project name
PROJECT_NAME="."

# Define directories
DIRS=("${PROJECT_NAME}" "${PROJECT_NAME}/include" "${PROJECT_NAME}/src")

# Define header and source files
HEADERS=(
  "TrpLexer.hpp"
  "TrpParser.hpp"
  "TrpValue.hpp"
  "TrpNumber.hpp"
  "TrpString.hpp"
  "TrpBool.hpp"
  "TrpNull.hpp"
  "TrpObject.hpp"
  "TrpArray.hpp"
)

# Create the directories
for dir in "${DIRS[@]}"; do
  mkdir -p "$dir"
  echo "Created directory: $dir"
done

# Create header files in the include directory
for header in "${HEADERS[@]}"; do
  touch "${PROJECT_NAME}/include/$header"
  echo "Created file: ${PROJECT_NAME}/include/$header"
done

# Create corresponding source files in the src directory
for header in "${HEADERS[@]}"; do
  source_file="${header%.*}.cpp"
  touch "${PROJECT_NAME}/src/$source_file"
  echo "Created file: ${PROJECT_NAME}/src/$source_file"
done

# Create the main application file
touch "${PROJECT_NAME}/src/main.cpp"
echo "Created file: ${PROJECT_NAME}/src/main.cpp"

# Create a basic Makefile
touch "${PROJECT_NAME}/Makefile"
echo "Created file: ${PROJECT_NAME}/Makefile"

echo "Project structure for '$PROJECT_NAME' created successfully!"
