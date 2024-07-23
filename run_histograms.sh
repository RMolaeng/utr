#!/bin/bash

# Define the base command
base_command="build/OutputProcessing/getHistogram -d ./output -a -e 12.0 -n 27 -q .root"

# Loop through the energies from 1000 keV to 2000 keV in steps of 100 keV
for energy in $(seq 5100 100 10000)
do
    # Construct the parameter for the efficiency
    efficiency_param="-p Efficiency_${energy}_keV"
    
    # Run the command with the current energy
    ${base_command} ${efficiency_param}
done
  # Run the command with the current energy
    ${base_command} ${efficiency_param}
done
