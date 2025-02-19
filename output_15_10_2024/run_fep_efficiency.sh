#!/bin/bash

# Loop over detector numbers from 0 to 27
for det in {0..27}
do
  # Construct the command with the current detector number
  command="/home/refilwemolaeng/Geant4/utr/build/OutputProcessing/fep_efficiency.sh Efficiency_ 200 5000 _keV_hist_det${det}.txt 10000000"
  
  # Run the command
  echo "Running command: $command"
  $command
  
done
