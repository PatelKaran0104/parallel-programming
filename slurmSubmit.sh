#!/usr/bin/env bash
####### Mail Notify / Job Name / Comment #######
#SBATCH --job-name="karan patel - pi"

####### Partition #######
#SBATCH --partition=all

####### Ressources #######
#SBATCH --time=0-00:05:00

####### Node Info #######
#SBATCH --exclusive
#SBATCH --nodes=1

####### Output #######
#SBATCH --output=/home/fd0003348/out/blockpi.out.%j
#SBATCH --error=/home/fd0003348/out/blockpi.err.%j

export OMP_NUM_THREADS=8
#cd /path/to/bin
for i in {1..10}; do
    time ./blockpi
done