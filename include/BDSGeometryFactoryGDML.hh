#ifndef BDSGEOMETRYFACTORYGDML_H
#define BDSGEOMETRYFACTORYGDML_H

#include "BDSGeometryFactoryBase.hh"

#include "globals.hh"

#include <map>
#include <vector>

class BDSGeometryExternal;
class G4Colour;
class G4LogicalVolume;
class G4VPhysicalVolume;

class BDSGeometryFactoryGDML: public BDSGeometryFactoryBase
{
public:
  static BDSGeometryFactoryGDML* Instance();

  virtual ~BDSGeometryFactoryGDML();

  virtual BDSGeometryExternal* Build(G4String fileName,
				     std::map<G4String, G4Colour*>* colourMapping = nullptr);

private:
  /// Private default constructor as singleton.
  BDSGeometryFactoryGDML();
  
  /// Singleton instance
  static BDSGeometryFactoryGDML* instance;

  /// Recursively append pvs and lvs from a given mother volume.  Pass by reference
  /// the output vectors
  void GetAllLogicalAndPhysical(const G4VPhysicalVolume*         volume,
				std::vector<G4VPhysicalVolume*>& pvs,
				std::vector<G4LogicalVolume*>&   lvs);
};

#endif