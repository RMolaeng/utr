/*
utr - Geant4 simulation of the UTR at HIGS
Copyright (C) 2017 the developing team (see README.md)

This file is part of utr.

utr is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

utr is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with utr.  If not, see <http://www.gnu.org/licenses/>.
*/

//**************************************************************//
//	LaBr Crystal (Saint Gobain BrilLanCe 380) 3x3" (TUD)
//**************************************************************//

#include <vector>

#ifndef LABR_3X3_HH
#define LABR_3X3_HH 1

#include "G4LogicalVolume.hh"

using std::vector;

class LaBr_3x3 {
  public:
	LaBr_3x3(G4LogicalVolume *World_Logical, G4String name);
	~LaBr_3x3(){};

	void Construct(G4ThreeVector global_coordinates, G4double theta, G4double phi,
		       G4double dist_from_center, bool use_filter_case, bool use_filter_case_ring, bool use_housing);
	// Possibility to add disks of solid material in front of the detector to shield low-energy
	// radiation. The first filter will be placed onto the front part of the detector that faces
	// the target, all others will be placed on top of the previous one in the direction of the
	// target.
	void Add_Filter(G4String filter_material, G4double filter_thickness, G4double filter_radius);
	// Possibility to wrap the detector end cap in a sheet of solid material
	// to shield low-energy radiation. Similar to the filters, additional wraps
	// will be wrapped around previous ones.
	void Add_Wrap(G4String wrap_material, G4double wrap_thickness);

  private:
	G4LogicalVolume *world_Logical;	
	G4String detector_name;

	vector<G4String> filter_materials;
	vector<G4double> filter_thicknesses;
	vector<G4double> filter_radii;

	vector<G4String> wrap_materials;
	vector<G4double> wrap_thicknesses;
};

#endif
