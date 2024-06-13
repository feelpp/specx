#!/bin/bash



#################################
 
function main(){
    # To ensure we are in the right directory we test
    # that CMakeCache.txt file and Examples directory exist.
    if [ ! -f CMakeCache.txt ] || [ ! -d Examples ]; then
        echo "Please run this script from the build directory."
        return 1
    fi

    # Create a directory to store the results
    # with the format results_[date]_[time]
    results_dir="results_$(date +%Y%m%d_%H%M%S)"
    mkdir $results_dir

    echo "Running benchmarks, storing results in $results_dir"

    # Run the benchmarks
    NB_LOOPS=10

    # AXPY
    ./Benchmark/axpy/axpy --lp=$NB_LOOPS --minnbb=10 --maxnbb=50 --minbs=128 --maxbs=512 --cuth=256 --od="$results_dir"

    # Cholesky/gemm
    cholesky_gemm/cholesky
    cholesky_gemm/gemm

    # Particles
    particles/particles-simu
}



