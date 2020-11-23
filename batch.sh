#!/bin/bash
#SBATCH --job-name=test
#SBATCH --output=res.txt

module load gcc/5.5.0 # carga el modulo de openmpi version 2.1.6
srun  ./a.out
module unload openmpi/5.5.0