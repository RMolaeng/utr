#!/bin/bash

# Loop over energies from 200 MeV to 500 MeV (increment by 10 MeV)
for energy in $(seq 100 100 10000)
do
  # Replace "3500_keV" in the filename with the current energy value
  command="build/OutputProcessing/histogramToTxt ./output/Efficiency_${energy}_keV_hist.root"
  
  # Run the command
  echo "Running command: $command"
  $command
  
done
