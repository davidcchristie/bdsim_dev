#ifndef BDSFIELDINFO_H
#define BDSFIELDINFO_H

#include "BDSFieldType.hh"
#include "BDSIntegratorType.hh"

#include "globals.hh" // geant4 types / globals
#include "G4Transform3D.hh"

class BDSCavityInfo;
class BDSMagnetStrength;

/**
 * @brief All info required to build complete field of any type.
 * 
 * With an instance of this class, the field factory should be able
 * to create the desired fully functional field with all required 
 * associated Geant4 objects.
 * 
 * Owns the magnetic strength instance.
 *
 * @author Laurie Nevay
 */

class BDSFieldInfo
{
public:
  BDSFieldInfo(BDSFieldType       fieldTypeIn,
	       G4double           brhoIn,
	       BDSIntegratorType  integratorTypeIn,
	       G4Transform3D      transformIn              = G4Transform3D(),
	       BDSMagnetStrength* magnetStrengthIn         = nullptr,
	       BDSCavityInfo*     cavityInfoIn             = nullptr,
	       G4bool             provideGlobalTransformIn = true,
	       G4String           magneticFieldFilePathIn  = "",
	       G4String           electricFieldFilePathIn  = "");
  ~BDSFieldInfo();

  inline BDSFieldType       FieldType()      const {return fieldType;}
  inline G4double           BRho()           const {return brho;}
  inline BDSIntegratorType  IntegratorType() const {return integratorType;}
  inline G4Transform3D      Transform()      const {return transform;}
  inline BDSMagnetStrength* MagnetStrength() const {return magnetStrength;}
  inline BDSCavityInfo*     CavityInfo()     const {return cavityInfo;}
  inline G4bool             ProvideGlobal()  const {return provideGlobalTransform;}
  inline G4String           MagneticFile()   const {return magneticFieldFilePath;}
  inline G4String           ElectricFile()   const {return electricFieldFilePath;}
  
private:
  /// Private default constructor to force use of provided one.
  BDSFieldInfo();
  
  BDSFieldType       fieldType;
  G4double           brho;
  BDSIntegratorType  integratorType;
  G4Transform3D      transform;  ///< Transform w.r.t. solid field will be attached to
  BDSMagnetStrength* magnetStrength;
  BDSCavityInfo*     cavityInfo;
  G4bool             provideGlobalTransform;
  G4String           magneticFieldFilePath;
  G4String           electricFieldFilePath;
};

#endif
