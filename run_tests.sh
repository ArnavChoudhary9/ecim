#!/bin/bash
# Build and run tests for ecim project

set -e

echo "================================"
echo "Building ECIM Test Suite"
echo "================================"

# Get script directory
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$SCRIPT_DIR"

# Regenerate build files if needed
if [ ! -f "build/Makefile" ]; then
    echo "Generating build files..."
    premake5 gmake2
fi

# Build the test project
echo ""
echo "Building tests..."
cd build
make config=debug_x64 ecim_tests
cd ..

# Run the tests
echo ""
echo "================================"
echo "Running Tests"
echo "================================"
echo ""
./bin/Debug/ecim_tests

TEST_RESULT=$?

echo ""
if [ $TEST_RESULT -eq 0 ]; then
    echo "================================"
    echo "All tests passed!"
    echo "================================"
else
    echo "================================"
    echo "Some tests failed!"
    echo "================================"
fi

exit $TEST_RESULT
