#ifndef BDSBEAMPIPE_H
#define BDSBEAMPIPE_H

#include "BDSGeometryComponent.hh"

#include "globals.hh"               // geant4 globals / types
#include "G4LogicalVolume.hh"
#include "G4VSolid.hh"
#include <utility>                  // for std::pair

class BDSExtent;

/**
 * @brief A holder class for all information required for a
 * piece of beampipe.  
 * 
 * This does not implement the construction of the beampipe but merely
 * holds all relevant objects and information. 
 * 
 * @author Laurie Nevay
 */

class BDSBeamPipe: public BDSGeometryComponent
{
public:
  /// constructor has BDSGeometryComponent members first,
  /// then everything extra for this derived class
  BDSBeamPipe(G4VSolid*                 containerSolidIn,
	      G4LogicalVolume*          containerLVIn,
	      BDSExtent                 extentIn,
	      G4VSolid*                 containerSubtractionSolidIn,
	      G4LogicalVolume*          vacuumLVIn,
	      G4bool                    containerIsCircularIn = false,
	      G4double                  containerRadiusIn     = 0.0,
	      G4ThreeVector             inputFaceNormalIn  = G4ThreeVector(0,0,-1),
	      G4ThreeVector             outputFaceNormalIn = G4ThreeVector(0,0, 1));
  
  virtual ~BDSBeamPipe(); /// default destructor sufficient as G4 manages solids and LVs

  /// Access a solid for beampipe subtraction - note this is typically longer
  /// than the actual beampipe for unambiguous subtraction
  inline G4VSolid* GetContainerSubtractionSolid() const {return containerSubtractionSolid;}
  /// Access the vacuum volume to set fields and limits
  inline G4LogicalVolume* GetVacuumLogicalVolume() const {return vacuumLogicalVolume;};
  /// Flag to tell whether the parent volume needn't use a subtraction
  /// solid and can simply use a G4Tubs for example
  inline G4bool ContainerIsCircular() const {return containerIsCircular;}
  /// If it is circular, we need the radius
  inline G4double GetContainerRadius() const {return containerRadius;}

  /// @{ Accessor
  inline G4ThreeVector InputFaceNormal()  const {return inputFaceNormal;}
  inline G4ThreeVector OutputFaceNormal() const {return outputFaceNormal;}
  /// @}
  
protected:
  G4VSolid*        containerSubtractionSolid;
  G4LogicalVolume* vacuumLogicalVolume;
  G4bool           containerIsCircular;
  G4double         containerRadius;
  G4ThreeVector    inputFaceNormal;
  G4ThreeVector    outputFaceNormal;
};

#endif
