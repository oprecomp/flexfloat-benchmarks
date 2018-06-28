# FlexFloat Benchmarks
FlexFloat can be used to perform precision tuning of floating-point variables.
This repository contains some examples of program adopting the FlexFloat C++ interface.

## Build a benchmark
* Enter the folder of a specific benchmark, for instance: `cd kmeans`
* Execute the make command providing the required paths for FlexFloat: 
`make clean all FLEXFLOAT_INC_DIR=<flexfloat/include path> FLEXFLOAT_LIB_DIR=<flexfloat/build path>`

## Perform precision tuning with an external tool
Steps to perform precision tuning (using fpPrecisionTuning):
* [Prerequisites] Install Python 2.7 and its MPI support. On Ubuntu 14.04 systems (or later versions) this can be accomplished with:
`sudo apt-get install python`
`sudo apt-get install python-mpi4py`
* Download the fpPrecisionTuning toolchain from its official git repo:
`git clone https://github.com/minhhn2910/fpPrecisionTuning.git`
* Enter the folder of a specific benchmark, for instance:
`cd flexfloat-benchmarks/kmeans`
* Execute the precision tuning tool:
`mpirun -np 8 <fpPrecisionTuning path>/PrecisionAnalysis/greedy_search_mpi.py 1 kmeans_flex`

Additional notes:
* The accuracy required for the results can be tuned changing the `error_rate` parameter in the "greedy\_search\_mpi.py" script (located in fpPrecisionTuning folder).
* The available floating-point types can be modified updating the  `set_coefficient_bits` function in the "compile.py" script (located in each benchmark folder).
The current configuration is taking into account the additional floating-point types defined by the PULP smallFloat extension.
* The output provides the minimum bit-width of the mantissa for each input variable (a total order for program variables is defined in "compile.py").
* Use `make clean all` to remove all temporary files from a benchmark folder.
