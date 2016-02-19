#include "BDSFieldMagSkew.hh"
#include "BDSUtilities.hh"

#include "globals.hh"
#include "G4RotationMatrix.hh"

BDSFieldMagSkew::BDSFieldMagSkew(BDSFieldMag* fieldIn,
				 G4double     angle):
  field(fieldIn)
{
  rotation = new G4RotationMatrix();
  antiRotation = new G4RotationMatrix();
  rotation->rotateZ(angle);
  rotation->rotateZ(-angle);
}

BDSFieldMagSkew::~BDSFieldMagSkew()
{
  delete field;
  delete rotation;
  delete antiRotation;
}

G4ThreeVector BDSFieldMagSkew::GetFieldValue(const G4ThreeVector& position) const
{
  G4ThreeVector rotatedPosition(position);
  rotatedPosition           = rotatedPosition.transform(*rotation);
  G4ThreeVector normalField = field->GetFieldValue(rotatedPosition);
  return (*antiRotation)*normalField;
}