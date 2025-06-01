#!/bin/bash

# Script to compare test execution times between current and previous commit.
#
# Do not use when the branch is detached.
# Error and common messages will be routed to LOG_FILE.
# This script depends on changes in Rak source code do print compilation and execution
#   time to stderr after running the script.
# Close applications that can interfere with the system load, like the browser.
# Output will be printed on screen. Copy and paste the result to a spreadsheet and analyze.
# From given results, calculate the average and standard deviation to check if
#   the result is relevant. Disregard the test (do it again) if standard deviation
#   is too big.

###################### Configuration ######################
buildCommand() {
    ./clean.sh && ./build.sh Release ENABLE_BENCHMARK=ON
}
testCommand() {
    ./build/rak benchmark.rak
}
LOG_FILE="benchStatus.log"

######################### Script ##########################
# Get current commit hash
CURRENT_COMMIT=$(git rev-parse --abbrev-ref HEAD)
CURRENT_COMMIT_SHORT=$(git rev-parse --short HEAD)
echo "Current commit: $CURRENT_COMMIT_SHORT ($CURRENT_COMMIT)" > $LOG_FILE
# Get previous commit hash
PREVIOUS_COMMIT=$(git rev-parse HEAD~1)
PREVIOUS_COMMIT_SHORT=$(git rev-parse --short HEAD~1)
echo "Previous commit: $PREVIOUS_COMMIT_SHORT ($PREVIOUS_COMMIT)" >> $LOG_FILE

# Function to build and test 25 times
run_tests() {
    local commit=$1
    echo -e "\nRunning tests for $commit..." >> $LOG_FILE
    echo "Building..." >> $LOG_FILE
    buildCommand >> $LOG_FILE
    run_exit_code=$?
    if [ $run_exit_code -ne 0 ]; then
        echo "Build failed for $commit"
        exit 1
    fi
    echo "Running tests... $commit" >> $LOG_FILE
    echo "Compile$commit,Execute$commit"
    for ((i=1; i<=25; i++)); do
        testCommand 1>> $LOG_FILE
        run_exit_code=$?
        if [ $run_exit_code -ne 0 ]; then
            echo "Tests failed for $commit"
            break
        fi
    done
    return $run_exit_code
}

echo -e "Please wait for compilation and tests..."

# Benchmark!
EXIT_CODE=0
run_tests "CurrentCommit" || EXIT_CODE=$?
echo -e "\nChecking out previous commit $PREVIOUS_COMMIT_SHORT..." >> $LOG_FILE
git checkout "$PREVIOUS_COMMIT" &>> $LOG_FILE
run_tests "PreviousCommit" || ((EXIT_CODE+=$?))
echo -e "\nReturning to original commit $CURRENT_COMMIT_SHORT..." >> $LOG_FILE
git checkout "$CURRENT_COMMIT" &>> $LOG_FILE

if (( EXIT_CODE == 0 )); then
    echo -e "\nDone!"
else
    echo -e "\nSome error occurred, check '$LOG_FILE' for details."
fi

exit $EXIT_CODE
