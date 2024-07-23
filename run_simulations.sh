#!/bin/bash

# Initial and final energy values in MeV
START_ENERGY=9.4
END_ENERGY=10.0
STEP=0.1

# Loop over the specified energy range
for energy in $(seq $START_ENERGY $STEP $END_ENERGY); do
  # Convert the energy to keV and format it as needed for the filename
  energy_keV=$(awk "BEGIN {printf \"%.0f\", $energy * 1000}")

  # Create a macro file for each energy
  macro_file="run_${energy_keV}keV.mac"
  cat <<EOF > $macro_file
/run/initialize
/gps/particle gamma
/gps/pos/type Point
/gps/pos/centre 0. 0. 0. mm
/gps/ang/type iso
/gps/ene/type Mono
/gps/ene/mono ${energy} MeV

# Set the output filename to contain the simulated energy in a uniform number format and disable appendage of additional file IDs
/utr/setUseFilenameID False
/utr/appendZerosToVar E 2 ${energy}
/utr/setFilename Efficiency_${energy_keV}_keV

# Run the simulation
/run/beamOn 100000000
EOF
  # Run the simulation with the generated macro file
  build/utr -m  $macro_file -t 8
done







