#!/bin/bash
#SBATCH --job-name=test
#SBATCH --output=res.txt
#SBATCH --time=10:00
#SBATCH --nodes=2
echo $SLURM_CPUS_ON_NODE
echo $SLURM_JOB_NUM_NODES   
num_core=$SLURM_CPUS_ON_NODE
num_node=$SLURM_JOB_NUM_NODES
let proc_num=$num_core*$num_node
echo $proc_num
module load gcc/5.5.0 # carga el modulo de openmpi version 2.1.6
srun -n $proc_num ./a.out
module unload openmpi/5.5.0