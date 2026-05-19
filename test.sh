#!/bin/bash

# =====================================
# mysh Automated Test Script
# =====================================
# Usage:
#   chmod +x test.sh
#   ./test.sh
# =====================================

MYSH=./mysh

if [ ! -f "$MYSH" ]; then
    echo "ERROR: mysh executable not found"
    echo "Run: make"
    exit 1
fi


echo "====================================="
echo "COMPILATION TEST"
echo "====================================="
make


echo ""
echo "====================================="
echo "TEST 1: BUILT-IN COMMANDS"
echo "====================================="

printf "pwd\nexit\n" | $MYSH

printf "cd /tmp\npwd\nexit\n" | $MYSH

printf "cd\nexit\n" | $MYSH


echo ""
echo "====================================="
echo "TEST 2: EXTERNAL COMMANDS"
echo "====================================="

printf "ls\nexit\n" | $MYSH

printf "ls -la\nexit\n" | $MYSH

printf "echo hello world\nexit\n" | $MYSH

printf "nonexistent_command\nexit\n" | $MYSH


echo ""
echo "====================================="
echo "TEST 3: OUTPUT REDIRECTION"
echo "====================================="

rm -f out.txt

printf "echo hello > out.txt\ncat out.txt\nexit\n" | $MYSH


echo ""
echo "====================================="
echo "TEST 4: APPEND REDIRECTION"
echo "====================================="

rm -f out.txt

printf "echo hello > out.txt\necho world >> out.txt\ncat out.txt\nexit\n" | $MYSH


echo ""
echo "====================================="
echo "TEST 5: INPUT REDIRECTION"
echo "====================================="

echo hello > input.txt
echo world >> input.txt

printf "wc -l < input.txt\nexit\n" | $MYSH


echo ""
echo "====================================="
echo "TEST 6: COMBINED REDIRECTION"
echo "====================================="

echo banana > unsorted.txt
echo apple >> unsorted.txt
echo orange >> unsorted.txt

printf "sort < unsorted.txt > sorted.txt\ncat sorted.txt\nexit\n" | $MYSH

printf "sort > sorted2.txt < unsorted.txt\ncat sorted2.txt\nexit\n" | $MYSH


echo ""
echo "====================================="
echo "TEST 7: BACKGROUND JOBS"
echo "====================================="

printf "sleep 3 &\necho shell responsive\nexit\n" | $MYSH

printf "sleep 3 &\nsleep 5 &\necho multiple jobs running\nexit\n" | $MYSH


echo ""
echo "====================================="
echo "TEST 8: ERROR HANDLING"
echo "====================================="

printf "ls >\nexit\n" | $MYSH

printf "wc <\nexit\n" | $MYSH

printf "wc -l < nonexistent.txt\nexit\n" | $MYSH


echo ""
echo "====================================="
echo "TEST 9: EDGE CASES"
echo "====================================="

printf "\nexit\n" | $MYSH

printf "ls      -la\nexit\n" | $MYSH

printf "echo this is a long command for testing purposes\nexit\n" | $MYSH


echo ""
echo "====================================="
echo "CLEANUP"
echo "====================================="

rm -f out.txt
rm -f input.txt
rm -f unsorted.txt
rm -f sorted.txt
rm -f sorted2.txt


echo ""
echo "====================================="
echo "ALL TESTS COMPLETED"
echo "====================================="