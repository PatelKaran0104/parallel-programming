#!/usr/bin/env bash
####### Mail Notify / Job Name / Comment #######
#SBATCH --job-name="karan patel - matmul"

####### Partition #######
#SBATCH --partition=all

####### Ressources #######
#SBATCH --time=0-00:05:00

####### Node Info #######
#SBATCH --exclusive
#SBATCH --nodes=1

####### Output #######
#SBATCH --output=/home/fd0003348/out/matmul.out.%j
#SBATCH --error=/home/fd0003348/out/matmul.err.%j

export OMP_NUM_THREADS=8
#cd /path/to/bin
./matmul
