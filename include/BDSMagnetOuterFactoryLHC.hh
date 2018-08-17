/* 
Beam Delivery Simulation (BDSIM) Copyright (C) Royal Holloway, 
University of London 2001 - 2018.

This file is part of BDSIM.

BDSIM is free software: you can redistribute it and/or modify 
it under the terms of the GNU General Public License as published 
by the Free Software Foundation version 3 of the License.

BDSIM is distributed in the hope that it will be useful, but 
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with BDSIM.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef BDSMAGNETOUTERFACTORYLHC_H
#define BDSMAGNETOUTERFACTORYLHC_H

#include "BDSMagnetOuterFactoryBase.hh"

#include "globals.hh"           // geant4 globals / types

class BDSBeamPipe;
class G4Material;

/**
 * @brief The base factory for the LHC magnet geometry factory.
 * 
 * This class is not a singleton unlike the other factories
 * as it will be constructed twice by each derived class
 * 
 * @author Laurie Nevay
 */

class BDSMagnetOuterFactoryLHC: public BDSMagnetOuterFactoryBase
{
public:
  explicit BDSMagnetOuterFactoryLHC(G4bool isLeftOffset);
  virtual ~BDSMagnetOuterFactoryLHC(){;};
  
  /// sector bend outer volume
  virtual BDSMagnetOuter* CreateSectorBend(G4String           name,             // name
					   G4double           length,           // full length [mm]
					   const BDSBeamPipe* beamPipe,         // beampipe
					   G4double           containerLength,  // full length to make AccComp container
					   const BDSMagnetOuterInfo* recipe);   // recipe for geometry

  /// rectangular bend outer volume
  virtual BDSMagnetOuter* CreateRectangularBend(G4String           name,             // name
						G4double           length,           // full length [mm]
						const BDSBeamPipe* beamPipe,         // beampipe
						G4double           containerLength,  // full length to make AccComp container
						const BDSMagnetOuterInfo* recipe);   // recipe for geometry
  
  /// quadrupole outer volume
  virtual BDSMagnetOuter* CreateQuadrupole(G4String     name,                  // name
					   G4double     length,                // length [mm]
					   BDSBeamPipe* beamPipe,              // beampipe
					   G4double     horizontalWidth,       // full width
					   G4double     containerLength,       // full length to make AccComp container
					   G4Material*  outerMaterial = nullptr,// material for outer volume
					   G4bool       buildEndPiece = false
					   );

  
  /// sextupole outer volume
  virtual BDSMagnetOuter* CreateSextupole(G4String     name,                  // name
					  G4double     length,                // length [mm]
					  BDSBeamPipe* beamPipe,              // beampipe
					  G4double     horizontalWidth,       // full width
					  G4double     containerLength,       // full length to make AccComp container
					  G4Material*  outerMaterial = nullptr,// material for outer volume
					  G4bool       buildEndPiece = false
					  );
  
  /// octupole outer volume
  virtual BDSMagnetOuter* CreateOctupole(G4String     name,                  // name
					 G4double     length,                // length [mm]
					 BDSBeamPipe* beamPipe,              // beampipe
					 G4double     horizontalWidth,       // full width
					 G4double     containerLength,       // full length to make AccComp container
					 G4Material*  outerMaterial = nullptr,// material for outer volume
					 G4bool       buildEndPiece = false
					 );
  
  /// decapole outer volume
  virtual BDSMagnetOuter* CreateDecapole(G4String     name,                  // name
					 G4double     length,                // length [mm]
					 BDSBeamPipe* beamPipe,              // beampipe
					 G4double     horizontalWidth,       // full width
					 G4double     containerLength,       // full length to make AccComp container
					 G4Material*  outerMaterial = nullptr,// material for outer volume
					 G4bool       buildEndPiece = false
					 );

  /// solenoid  outer volume
  virtual BDSMagnetOuter* CreateSolenoid(G4String     name,                  // name
					 G4double     length,                // length [mm]
					 BDSBeamPipe* beamPipe,              // beampipe
					 G4double     horizontalWidth,       // full width
					 G4double     containerLength,       // full length to make AccComp container
					 G4Material*  outerMaterial = nullptr,// material for outer volume
					 G4bool       buildEndPiece = false
					 );

  /// general multipole outer volume - could be any 2N order multipole
  virtual BDSMagnetOuter* CreateMultipole(G4String     name,                  // name
					  G4double     length,                // length [mm]
					  BDSBeamPipe* beamPipe,              // beampipe
					  G4double     horizontalWidth,       // full width
					  G4double     containerLength,       // full length to make AccComp container
					  G4Material*  outerMaterial = nullptr,// material for outer volume
					  G4bool       buildEndPiece = false
					  );
  
  /// RF cavity outer volume
  virtual BDSMagnetOuter* CreateRfCavity(G4String     name,                  // name
					 G4double     length,                // length [mm]
					 BDSBeamPipe* beamPipe,              // beampipe
					 G4double     horizontalWidth,       // full width
					 G4double     containerLength,       // full length to make AccComp container
					 G4Material*  outerMaterial = nullptr,// material for outer volume
					 G4bool       buildEndPiece = false
					 );
  
  /// muon spoiler outer volume
  virtual BDSMagnetOuter* CreateMuonSpoiler(G4String     name,                  // name
					    G4double     length,                // length [mm]
					    BDSBeamPipe* beamPipe,              // beampipe
					    G4double     horizontalWidth,       // full width
					    G4double     containerLength,       // full length to make AccComp container
					    G4Material*  outerMaterial = nullptr,// material for outer volume
					    G4bool       buildEndPiece = false
					    );
  
  /// horizontal and vertical kicker outer volume
  virtual BDSMagnetOuter* CreateKicker(G4String                  name,            // name
				       G4double                  length,          // length [mm]
				       const BDSBeamPipe*        beamPipe,        // beampipe
				       G4double                  containerLength, // full length to make AccComp container
				       const BDSMagnetOuterInfo* recipe,          // geometry recipe
				       G4bool                    vertical);       // is it a vertical kicker?
  
protected:
  G4bool isLeftOffset;
  G4bool IsLeftOffset(){return isLeftOffset;}
  
private:
  /// test inputs for no null pointers or overlapping volumes due to poorly defined sizes
  void TestInputParameters(const BDSBeamPipe* beamPipe,
			   G4double&    boxSizeIn,
			   G4Material*& outerMaterialIn);
  
};

#endif
