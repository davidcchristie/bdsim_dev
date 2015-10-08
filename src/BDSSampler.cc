#include "BDSGlobalConstants.hh" 
#include "BDSExecOptions.hh"
#include "BDSMaterials.hh"
#include "BDSSampler.hh"
#include "BDSDebug.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4UserLimits.hh"
#include "BDSSamplerSD.hh"
#include "BDSSDManager.hh"

std::vector <G4String> BDSSampler::outputNames;

int BDSSampler::nSamplers = 0;

// created here, so only one is created with fixed known name
BDSSamplerSD* BDSSampler::SensitiveDetector = new BDSSamplerSD("BDSSampler","plane");

int BDSSampler::GetNSamplers() { return nSamplers; }

void BDSSampler::AddExternalSampler(G4String name) { nSamplers++; outputNames.push_back(name); }

BDSSampler::BDSSampler(G4String name,
		       G4double length):
  BDSAcceleratorComponent(name, length, 0, "sampler")
{
  nThisSampler= nSamplers + 1;
  nSamplers++;
#ifdef BDSDEBUG
  G4cout << __METHOD_NAME__ << " Nsamplers " << nSamplers << G4endl;
#endif
  BDSSampler::outputNames.push_back(GetName());
}

void BDSSampler::BuildContainerLogicalVolume()
{
#ifdef BDSDEBUG
  G4cout << __METHOD_NAME__ << G4endl;
#endif
  G4String name = GetName();
  G4Material* emptyMaterial = BDSMaterials::Instance()->GetMaterial(BDSGlobalConstants::Instance()->GetEmptyMaterial());
  G4double samplerDiameter = BDSGlobalConstants::Instance()->GetSamplerDiameter() * 0.5; 
  containerSolid = new G4Box(name + "_solid",
			     samplerDiameter,
			     samplerDiameter,
			     chordLength*0.5);
  containerLogicalVolume = new G4LogicalVolume(containerSolid,
					       emptyMaterial,
					       name);
  
#ifndef NOUSERLIMITS
  G4UserLimits* outerUserLimits = new G4UserLimits();
  outerUserLimits->SetMaxAllowedStep(1*CLHEP::m);
  containerLogicalVolume->SetUserLimits(outerUserLimits);
  RegisterUserLimits(outerUserLimits);
#endif
  if (BDSExecOptions::Instance()->GetVisDebug())
    {containerLogicalVolume->SetVisAttributes(BDSGlobalConstants::Instance()->GetVisibleDebugVisAttr());}
  else
    {containerLogicalVolume->SetVisAttributes(BDSGlobalConstants::Instance()->GetInvisibleVisAttr());}
  
  containerLogicalVolume->SetSensitiveDetector(BDSSDManager::Instance()->GetSamplerPlaneSD());
}

BDSSampler::~BDSSampler()
{
  --nSamplers;
  if(nSamplers<0) G4cerr << __METHOD_NAME__ << "WARNING: more samplers deleted than created!" << G4endl;
}
