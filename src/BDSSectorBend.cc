#include "BDSBeamPipeFactory.hh"
#include "BDSDebug.hh"
#include "BDSGlobalConstants.hh" 
#include "BDSMagnet.hh"
#include "BDSMagnetOuterInfo.hh"
#include "BDSMagnetType.hh"
#include "BDSSectorBend.hh"
#include "BDSUtilities.hh"        // for calculateorientation

#include "globals.hh" // geant4 types / globals
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"

#include <cmath>

BDSSectorBend::BDSSectorBend(G4String            name,
			     G4double            arcLength,
			     G4double            angleIn,
			     G4double            /*bField*/,
			     G4double            /*bGrad*/,
			     BDSBeamPipeInfo*    beamPipeInfo,
			     BDSMagnetOuterInfo* magnetOuterInfo):
  BDSMagnet(BDSMagnetType::sectorbend, name, arcLength,
	    beamPipeInfo, magnetOuterInfo, nullptr, 0)
{
  /// BDSMagnet doesn't provide the ability to pass down angle to BDSAcceleratorComponent
  /// - this results in a wrongly chord length
  angle                  = angleIn;
  magnetOuterInfo->angle = angle;
  if (BDS::IsFinite(angle))
    {
      chordLength = 2.0 * arcLength * sin(0.5*angleIn) / angleIn;
      // prepare normal vectors for input and output planes
      // calculate components of normal vectors (in the end mag(normal) = 1)
      G4int orientation   = BDS::CalculateOrientation(angleIn);
      G4double in_z = cos(0.5*fabs(angleIn)); 
      G4double in_x = sin(0.5*fabs(angleIn));
      inputface     = G4ThreeVector(-orientation*in_x, 0.0, -1.0*in_z);
      //-1 as pointing down in z for normal
      outputface    = G4ThreeVector(-orientation*in_x, 0.0, in_z);
    }
  else
    {chordLength = arcLength;}
#ifdef BDSDEBUG
  G4cout << __METHOD_NAME__ << "angle:        " << angle     << G4endl;
  G4cout << __METHOD_NAME__ << "arc length:   " << arcLength << G4endl;
  G4cout << __METHOD_NAME__ << "chord length: " << chordLength << G4endl;
#endif
}

void BDSSectorBend::Build()
{
#ifdef BDSDEBUG
  G4cout << __METHOD_NAME__ << G4endl;
#endif
  BDSMagnet::Build();
  /*
  if(BDSGlobalConstants::Instance()->GetIncludeIronMagFields())
    {
      G4double polePos[4];
      G4double Bfield[3];
      
      //coordinate in GetFieldValue
      polePos[0]=0.;
      polePos[1]=BDSGlobalConstants::Instance()->GetMagnetPoleRadius();
      polePos[2]=0.;
      polePos[3]=-999.;//flag to use polePos rather than local track
      
      itsMagField->GetFieldValue(polePos,Bfield);
      G4double BFldIron=
	sqrt(Bfield[0]*Bfield[0]+Bfield[1]*Bfield[1])*
	BDSGlobalConstants::Instance()->GetMagnetPoleSize()/
	(BDSGlobalConstants::Instance()->GetComponentBoxSize()/2-
	 BDSGlobalConstants::Instance()->GetMagnetPoleRadius());
      
      // Magnetic flux from a pole is divided in two directions
      BFldIron/=2.;
      
      BuildOuterFieldManager(2, BFldIron,CLHEP::halfpi);
      }*/
}

void BDSSectorBend::BuildBeampipe()
{
#ifdef BDSDEBUG
  G4cout << __METHOD_NAME__ << "sector bend version " << G4endl;
#endif
  BDSBeamPipeFactory* factory = BDSBeamPipeFactory::Instance();
  if (BDS::IsFinite(angle))
    {
      beampipe = factory->CreateBeamPipeAngledInOut(beamPipeInfo->beamPipeType,
						    name,
						    chordLength - lengthSafety,
						    -angle*0.5,
						    -angle*0.5,
						    beamPipeInfo->aper1,
						    beamPipeInfo->aper2,
						    beamPipeInfo->aper3,
						    beamPipeInfo->aper4,
						    beamPipeInfo->vacuumMaterial,
						    beamPipeInfo->beamPipeThickness,
						    beamPipeInfo->beamPipeMaterial);
    }
  else
    {
      beampipe = factory->CreateBeamPipe(name,
					 chordLength - lengthSafety,
					 beamPipeInfo);
    }

  RegisterDaughter(beampipe);

  SetAcceleratorVacuumLogicalVolume(beampipe->GetVacuumLogicalVolume());
}
