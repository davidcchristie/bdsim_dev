#ifndef BDSCOMPONENTFACTORY_H
#define BDSCOMPONENTFACTORY_H

#include <map>
#include <list>

#include "globals.hh"
#include "parser/element.h"
#include "BDSAcceleratorComponent.hh"
#include "BDSBeamPipe.hh"
#include "BDSBeamPipeInfo.hh"
#include "BDSMagnetOuterInfo.hh"

class BDSCavityInfo;

class BDSTiltOffset;

/**
 * @brief Factory to produce all types of BDSAcceleratorComponents.
 * 
 * Creates from a parser Element the appropriate
 * object (that inherits BDSAcceleratorComponent) and returns it. Will return
 * nullptr if invalid type or nothing to be constructed for that particular type.
 * Basic calculations on field strength and angle as well as basic parameter validity
 * (zero length?) are done here.
 * 
 */

class BDSComponentFactory
{
public:
  BDSComponentFactory();
  ~BDSComponentFactory();

  /// Create component from parser Element
  BDSAcceleratorComponent* CreateComponent(GMAD::Element& element);

  // for each of them - special cases need only for ring logic
  BDSAcceleratorComponent* CreateTerminator();
  BDSAcceleratorComponent* CreateTeleporter(const G4ThreeVector teleporterDetla);
  /// Create the tilt and offset information object by inspecting the parser element
  BDSTiltOffset*           CreateTiltOffset(GMAD::Element& element);
 
private:
  G4bool   verbose;

  G4double lengthSafety;
  
  G4double _charge, _momentum, _brho;
  
  std::list<GMAD::Element>::iterator _elementIter, _previousElementIter, _nextElementIter;
  /// beamline
  std::list<BDSAcceleratorComponent*> itsBeamline;
  /// element for storing instead of passing around
  GMAD::Element _element;
  
  BDSAcceleratorComponent* CreateSampler();
  BDSAcceleratorComponent* CreateCSampler();
  BDSAcceleratorComponent* CreateDump();
  BDSAcceleratorComponent* CreateDrift();
  BDSAcceleratorComponent* CreateRF();
  BDSAcceleratorComponent* CreateSBend();
  BDSAcceleratorComponent* CreateRBend();
  BDSAcceleratorComponent* CreateKicker(G4bool isVertical);
  BDSAcceleratorComponent* CreateQuad();  
  BDSAcceleratorComponent* CreateSextupole();
  BDSAcceleratorComponent* CreateOctupole();
  BDSAcceleratorComponent* CreateDecapole();
  BDSAcceleratorComponent* CreateMultipole();
  BDSAcceleratorComponent* CreateElement();
  BDSAcceleratorComponent* CreateSolenoid();
  BDSAcceleratorComponent* CreateRectangularCollimator();
  BDSAcceleratorComponent* CreateEllipticalCollimator();
  BDSAcceleratorComponent* CreateMuSpoiler();
  BDSAcceleratorComponent* CreateDegrader();
  BDSAcceleratorComponent* CreateLaser();
  BDSAcceleratorComponent* CreateScreen();
  BDSAcceleratorComponent* CreateAwakeScreen();
  BDSAcceleratorComponent* CreateTransform3D();

  /// Testing function
  G4bool HasSufficientMinimumLength(GMAD::Element& element);
  
  ///@{ Utility function to prepare model info
  BDSMagnetOuterInfo* PrepareMagnetOuterInfo(GMAD::Element& element);
  G4double            PrepareOuterDiameter(GMAD::Element& element);
  BDSBeamPipeInfo*    PrepareBeamPipeInfo(GMAD::Element& element);
  BDSCavityInfo*      PrepareCavityModelInfo(const GMAD::Element& element);
  ///@}

  /// Utility function to check if the combination of outer diameter, angle and length
  /// will result in overlapping entrance and exit faces and therefore whether to abort.
  void CheckBendLengthAngleWidthCombo(G4double chordLength,
				      G4double angle,
				      G4double outerDiameter,
				      G4String name = "not given");

  /// Prepare all RF cavity models in the component factory. Kept here and copies delivered.
  /// This class deletes them upon destruction.
  void PrepareCavityModels();

  /// Map of cavity model info instances by name
  std::map<G4String, BDSCavityInfo*> cavityInfos;
};
#endif
