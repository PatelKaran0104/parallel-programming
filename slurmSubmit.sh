#!/usr/bin/env bash
####### Mail Notify / Job Name / Comment #######
#SBATCH --job-name="karan patel - sum_taskloop"

####### Partition #######
#SBATCH --partition=all

####### Ressources #######
#SBATCH --time=0-00:05:00

####### Node Info #######
#SBATCH --exclusive
#SBATCH --nodes=1

####### Output #######
#SBATCH --output=/home/fd0003348/out/sum_taskloop.out.%j
#SBATCH --error=/home/fd0003348/out/sum_taskloop.err.%j

export OMP_NUM_THREADS=8
#cd /path/to/bin
sum=0

for i in {1..20}; do
    t=$(./sum_taskloop)
    echo "Run $i: $t"
    sum=$(echo "$sum + $t" | bc -l)
done

avg=$(echo "$sum / 20" | bc -l)
echo "Average: $avg"