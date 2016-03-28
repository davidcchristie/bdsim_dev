#ifndef BDSFIELDMAG_H
#define BDSFIELDMAG_H

#include "globals.hh" // geant4 types / globals
#include "G4MagneticField.hh"
#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"

/**
 * @brief Interface for static magnetic fields that may or may not be local.
 * 
 * Defines abstract interface for all derived magnetic fields. More common
 * to use G4ThreeVectors than arrays so provides common functionality
 * 
 * @author Laurie Nevay
 */

class BDSFieldMag: public G4MagneticField
{
public:
  /// Constructor takes transform in, which is inverted and applied to positions
  /// before querying the 'pure' derived field object. This allows local offset,
  /// translations, reflections and scalings to be applied if requried - ie for
  /// misalignment from the local coordinates.
  BDSFieldMag();
  BDSFieldMag(G4Transform3D transformIn);

  virtual ~BDSFieldMag(){;}

  /// Get the magnetic field vector in local coordinates.
  virtual G4ThreeVector GetField(const G4ThreeVector &position) const = 0;
  
  /// Implement interface to this class's GetField to fulfill G4MagneticField
  /// inheritance and allow a BDSFieldMag instance to be passed around in the field
  /// factory even if it's not wrapped in a BDSFieldGlobal instance and is in fact
  /// in local coordinates.
  virtual void GetFieldValue(const G4double point[4],
			     G4double* field) const;
  
  /// Get the field value after applying transform for local offset.
  G4ThreeVector GetFieldValueTransformed(const G4ThreeVector& position) const;

private:
  /// Transform to apply for the field relative to the local coordinates of the geometry.
  G4Transform3D transform;
};

#endif
