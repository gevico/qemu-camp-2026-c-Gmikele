#!/bin/bash
# Test script for 20_mybash exercise
# This script runs the compiled mybash binary with test commands

cd "$(dirname "$0")/../exercises/20_mybash" || { echo "Failed to cd to exercises/20_mybash"; exit 1; }

# Create a temporary script that mybash will execute
TMP_SCRIPT=$(mktemp /tmp/mybash_test_XXXXXX.txt) || { echo "Failed to create temp file"; exit 1; }

cat > "$TMP_SCRIPT" << 'SCRIPT'
myfile bin/mybash
myfile obj/myfile/myfile.o
mysed s/unix/linux/ "unix is opensource. unix is free os."
mytrans src/mytrans/text.txt
mywc src/mytrans/text.txt
SCRIPT

# Run mybash with the test script
./bin/mybash "$TMP_SCRIPT"
EXIT_CODE=$?

# Clean up
rm -f "$TMP_SCRIPT"

exit $EXIT_CODE
