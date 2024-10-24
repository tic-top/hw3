#!/bin/bash
# (See https://arc-ts.umich.edu/greatlakes/user-guide/ for command details)
# Set up batch job settings

#SBATCH --job-name=submit
#SBATCH --mail-type=BEGIN,END
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=18
#SBATCH --exclusive
#SBATCH --mem-per-cpu=1g
#SBATCH --time=00:05:00
#SBATCH --account=cse587f24_class
#SBATCH --partition=standard

# Run your program
# (">" redirects the print output of your pfogram,
# in this case to "output.txt")

g++ -std=c++11 -O3 -fopenmp -o s hw3p.cpp
OMP_NUM_THREADS=18 ./s > output18.txt