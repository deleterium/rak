#!/bin/bash

# Script to compare test execution times between current and previous commit

# Configuration
buildCommand() {
    ./clean.sh && ./build.sh Release ENABLE_BENCHMARK=ON
}
testCommand() {
    ./build/rak benchmark.rak
}
LOG_FILE="benchStatus.log"
RESULT_FILE="benchmark.csv"

# Get current commit hash
CURRENT_COMMIT=$(git rev-parse --abbrev-ref HEAD)
CURRENT_COMMIT_SHORT=$(git rev-parse --short HEAD)
echo "Current commit: $CURRENT_COMMIT_SHORT ($CURRENT_COMMIT)" > $LOG_FILE

# Get previous commit hash
PREVIOUS_COMMIT=$(git rev-parse HEAD~1)
PREVIOUS_COMMIT_SHORT=$(git rev-parse --short HEAD~1)
echo "Previous commit: $PREVIOUS_COMMIT_SHORT ($PREVIOUS_COMMIT)" >> $LOG_FILE

# Function to build, test and measure time
run_tests() {
    local commit=$1
    
    echo -e "\nRunning tests for $commit..." >> $LOG_FILE
    
    # Build the project
    echo "Building..." >> $LOG_FILE
    buildCommand >> $LOG_FILE
    run_exit_code=$?
    if [ $run_exit_code -ne 0 ]; then
        echo "Build failed for $commit"
        exit 1
    fi
    
    # Run tests and measure time
    echo "Running tests... $commit" >> $LOG_FILE
    echo "Compile,Execute"
    test_exit_code=0;
    for ((i=1; i<=25; i++)); do
        testCommand 1>> $LOG_FILE
        run_exit_code=$?
        if [ $run_exit_code -ne 0 ]; then
            break
        fi
    done

    # Check test status
    if [ $run_exit_code -ne 0 ]; then
        echo "Tests failed for commit $commit_short"
    fi
    
    return $test_exit_code
}

# Run tests for current commit
echo -e "\nPlease wait..."
run_tests "CurrentCommit"

# Checkout previous commit
echo -e "\nChecking out previous commit $PREVIOUS_COMMIT_SHORT..." >> $LOG_FILE
git checkout "$PREVIOUS_COMMIT" &>> $LOG_FILE

# Run tests for previous commit
run_tests "PreviousCommit"

# Return to original commit
echo -e "\nReturning to original commit $CURRENT_COMMIT_SHORT..." &>> $LOG_FILE
git checkout "$CURRENT_COMMIT" &>> $LOG_FILE

echo -e "\nDone! Check '$LOG_FILE' file for details."
