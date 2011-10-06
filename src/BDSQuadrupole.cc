//  
//   BDSIM, (C) 2001-2006 
//   
//   version 0.3
//  
//
//
//   Quadrupole class
//
//   History
//
//     21 Nov 2006 by Agapov,  v.0.3
//     22 Mar 2005 by Agapov, Carter,  v.0.2
//     x  x   2002 by Blair
//
//

#include "BDSGlobalConstants.hh" // must be first in include list

#include "BDSQuadrupole.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Trd.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4UserLimits.hh"
#include "G4TransportationManager.hh"

#include <map>


//============================================================

typedef std::map<G4String,int> LogVolCountMap;
extern LogVolCountMap* LogVolCount;

typedef std::map<G4String,G4LogicalVolume*> LogVolMap;
extern LogVolMap* LogVol;

extern BDSMaterials* theMaterials;

extern G4RotationMatrix* RotY90;
//============================================================

BDSQuadrupole::BDSQuadrupole(G4String aName, G4double aLength, 
			     G4double bpRad, G4double FeRad,
			     G4double bGrad, G4double tilt, G4double outR,
                             std::list<G4double> blmLocZ, std::list<G4double> blmLocTheta,
			     G4String aTunnelMaterial, G4String aMaterial, G4String spec):
  BDSMultipole(aName, aLength, bpRad, FeRad, SetVisAttributes(), blmLocZ, blmLocTheta, aTunnelMaterial, aMaterial),
  itsBGrad(bGrad)
{
#ifdef DEBUG 
  G4cout<<"BDSQUADRUPOLE : SPEC : "<<spec<<G4endl;
#endif
  // get specific quadrupole type
  G4String qtype = getParameterValueString(spec, "type");
#ifdef DEBUG 
  G4cout<<"qtype : "<<qtype<<G4endl;
#endif

  SetOuterRadius(outR);
  itsTilt=tilt;
  itsType="quad";

  if (!(*LogVolCount)[itsName])
    {
      //
      // build external volume
      // 
#ifdef DEBUG
      G4cout<<"Building marker volume "<<G4endl;
#endif
      BuildDefaultMarkerLogicalVolume();

      //
      //build tunnel
      //
      if(BDSGlobals->GetBuildTunnel()){
        BuildTunnel();
      }
     
      //
      // build beampipe (geometry + magnetic field)
      //
#ifdef DEBUG
      G4cout<<"Building beam pipe field and stepper "<<G4endl;
#endif
      BuildBPFieldAndStepper();
#ifdef DEBUG 
      G4cout<<"Building beam pipe field manager "<<G4endl;
#endif
      BuildBPFieldMgr(itsStepper,itsMagField);
#ifdef DEBUG 
      G4cout<<"Building beam pipe "<<G4endl;
#endif
      BuildBeampipe();

      //
      // build magnet (geometry + magnetic field)
      // according to quad type
      //
      if(qtype=="standard") 
	BuildOuterLogicalVolume(); // standard - quad with poles and pockets
      else if(qtype=="cylinder")  
        BuildDefaultOuterLogicalVolume(itsLength); // cylinder outer volume
      //BuildEllipticalOuterLogicalVolume(itsLength); // cylinder outer volume
      else //default - cylinder
        BuildDefaultOuterLogicalVolume(itsLength); // cylinder outer volume
      //BuildEllipticalOuterLogicalVolume(itsLength); // cylinder outer volume

      if(BDSGlobals->GetIncludeIronMagFields())
	{
	  G4double polePos[4];
	  G4double Bfield[3];

	  //coordinate in GetFieldValue
	  polePos[0]=-BDSGlobals->GetMagnetPoleRadius()*sin(pi/4);
	  polePos[1]=BDSGlobals->GetMagnetPoleRadius()*cos(pi/4);
	  polePos[2]=0.;
	  polePos[3]=-999.;//flag to use polePos rather than local track

	  itsMagField->GetFieldValue(polePos,Bfield);
	  G4double BFldIron=
	    sqrt(Bfield[0]*Bfield[0]+Bfield[1]*Bfield[1])*
	    BDSGlobals->GetMagnetPoleSize()/
	    (BDSGlobals->GetComponentBoxSize()/2-
	     BDSGlobals->GetMagnetPoleRadius());

	  // Magnetic flux from a pole is divided in two directions
	  BFldIron/=2.;

	  BuildOuterFieldManager(4, BFldIron,pi/4);
	}
      //Build the beam loss monitors
      BuildBLMs();
      //
      // define sensitive volumes for hit generation
      //
      if(BDSGlobals->GetSensitiveBeamPipe()){
        G4cout << "BDSQuadrupole.cc:> setting sensitive beam pipe" << G4endl;
        SetMultipleSensitiveVolumes(itsBeampipeLogicalVolume);
      }
      if(BDSGlobals->GetSensitiveComponents()){
        G4cout << "BDSQuadrupole.cc:> setting sensitive outer volume" << G4endl;
        SetMultipleSensitiveVolumes(itsOuterLogicalVolume);
      }
      //
      // set visualization attributes
      //
      itsVisAttributes=SetVisAttributes();
      itsVisAttributes->SetForceSolid(true);
      itsOuterLogicalVolume->SetVisAttributes(itsVisAttributes);

      //
      // append marker logical volume to volume map
      //
      (*LogVolCount)[itsName]=1;
      (*LogVol)[itsName]=itsMarkerLogicalVolume;
    }
  else
    {
      (*LogVolCount)[itsName]++;
      if(BDSGlobals->GetSynchRadOn()&& BDSGlobals->GetSynchRescale())
	{
	  // with synchrotron radiation, the rescaled magnetic field
	  // means elements with the same name must have different
	  // logical volumes, because they have different fields
	  itsName+=BDSGlobals->StringFromInt((*LogVolCount)[itsName]);

	  //
	  // build external volume
	  // 
	  BuildDefaultMarkerLogicalVolume();

	  //
	  // build beampipe (geometry + magnetic field)
	  //
	  BuildBPFieldAndStepper();
	  BuildBPFieldMgr(itsStepper,itsMagField);
	  BuildBeampipe();

	  //
	  // build magnet (geometry + magnetic field)
	  // according to quad type
	  //
	  if(qtype=="standard") 
	    BuildOuterLogicalVolume(); // standard - quad with poles and pockets
	  else if(qtype=="cylinder")  
            BuildDefaultOuterLogicalVolume(itsLength); // cylinder outer volume
            //	    BuildEllipticalOuterLogicalVolume(itsLength); // cylinder outer volume
	  else //default
            BuildDefaultOuterLogicalVolume(itsLength); // cylinder outer volume
          //BuildEllipticalOuterLogicalVolume(itsLength); // cylinder outer volume
	  if(BDSGlobals->GetIncludeIronMagFields())
	    {
	      G4double polePos[4];
	      G4double Bfield[3];
	      
	      //coordinate in GetFieldValue
	      polePos[0]=-BDSGlobals->GetMagnetPoleRadius()*sin(pi/4);
	      polePos[1]=BDSGlobals->GetMagnetPoleRadius()*cos(pi/4);
	      polePos[2]=0.;
	      polePos[3]=-999.;//flag to use polePos rather than local track

	      itsMagField->GetFieldValue(polePos,Bfield);
	      G4double BFldIron=
		sqrt(Bfield[0]*Bfield[0]+Bfield[1]*Bfield[1])*
		BDSGlobals->GetMagnetPoleSize()/
		(BDSGlobals->GetComponentBoxSize()/2-
		 BDSGlobals->GetMagnetPoleRadius());

	      // Magnetic flux from a pole is divided in two directions
	      BFldIron/=2.;
	      
	      BuildOuterFieldManager(4, BFldIron,pi/4);
	    }
	  //When is SynchRescale(factor) called?

	  //
	  // define sensitive volumes for hit generation
	  //
          if(BDSGlobals->GetSensitiveBeamPipe()){
            G4cout << "BDSQuadrupole.cc:> setting sensitive beampipe 2" << G4endl;            
            SetMultipleSensitiveVolumes(itsBeampipeLogicalVolume);
          }
          if(BDSGlobals->GetSensitiveComponents()){
            G4cout << "BDSQuadrupole.cc:> setting sensitive outer volume 2" << G4endl;           
            SetMultipleSensitiveVolumes(itsOuterLogicalVolume);
          }
		  
	  //
	  // set visualization attributes
	  //
	  itsVisAttributes=SetVisAttributes();
	  itsVisAttributes->SetForceSolid(true);
	  itsOuterLogicalVolume->SetVisAttributes(itsVisAttributes);
	  
	  //
	  // append marker logical volume to volume map
	  //
	  (*LogVol)[itsName]=itsMarkerLogicalVolume;
	}
      else
	{
	  //
	  // use already defined marker volume
	  //
	  itsMarkerLogicalVolume=(*LogVol)[itsName];
	}      
    }
}

void BDSQuadrupole::SynchRescale(G4double factor)
{
  itsStepper->SetBGrad(factor*itsBGrad);
  itsMagField->SetBGrad(factor*itsBGrad);
#ifdef DEBUG 
  G4cout << "Quad " << itsName << " has been scaled" << G4endl;
#endif
}

G4VisAttributes* BDSQuadrupole::SetVisAttributes()
{
  itsVisAttributes=new G4VisAttributes(G4Colour(1,0,0));
  return itsVisAttributes;
}

void BDSQuadrupole::BuildBPFieldAndStepper()
{
  // set up the magnetic field and stepper
  itsMagField=new BDSQuadMagField(itsBGrad);
  itsEqRhs=new G4Mag_UsualEqRhs(itsMagField);
  
  itsStepper=new BDSQuadStepper(itsEqRhs);
  itsStepper->SetBGrad(itsBGrad);
}

void BDSQuadrupole::BuildOuterLogicalVolume()
{
  G4double outerRadius = itsOuterR;
  if(itsOuterR==0) outerRadius = BDSGlobals->GetComponentBoxSize()/2;

  outerRadius = outerRadius/sqrt(2.0);

  itsOuterLogicalVolume=
    new G4LogicalVolume(new G4Tubs(itsName+"_outer_solid",
				   itsInnerIronRadius,
				   outerRadius * sqrt(2.0),
				   itsLength/2,
				   0,twopi*radian),
			theMaterials->GetMaterial(BDSGlobals->GetVacuumMaterial()),
			itsName+"_outer");
  
  // create one quadrant of the quadrupole
  G4LogicalVolume* lQuadrant = 
    new G4LogicalVolume(new G4Tubs(itsName+"_outer_solid",
				   itsInnerIronRadius,
				   outerRadius * sqrt(2.0),
				   itsLength/2,
				   0,pi/ 2 *radian),
			theMaterials->GetMaterial(BDSGlobals->GetVacuumMaterial()),
			itsName+"_outer");
  
  // pole 
  G4double poleR = itsBpRadius;
  G4double phiStart = -pi / 4;
  G4double dPhi = pi / 2;

  G4LogicalVolume* lPole = 
    new G4LogicalVolume(new G4Tubs(itsName+"_pole",
				   0,
				   poleR,
				   itsLength/2,
				   phiStart,
				   dPhi),
			theMaterials->GetMaterial("Iron"),
			itsName+"pole_outer");

  G4RotationMatrix* rotPole = new G4RotationMatrix;
  //rotPole = NULL;
  rotPole->rotateZ(3.*pi / 4.);

  G4double xPole = (poleR + itsBpRadius) / sqrt(2.0);
  G4double yPole = (poleR + itsBpRadius) / sqrt(2.0);


  G4VPhysicalVolume* itsPhysiQPole1;
  itsPhysiQPole1 = new G4PVPlacement(
		      rotPole,			    // rotation
		      G4ThreeVector(xPole,yPole,0), // its position
		      lPole,                        // its logical volume
		      itsName+"_solid",	            // its name
		      lQuadrant,                    // its mother  volume
		      false,		            // no boolean operation
		      0);		            // copy number
  

  // color-coding for the pole
  G4VisAttributes* VisAtt = 
    new G4VisAttributes(G4Colour(1., 0., 0.));
  VisAtt->SetForceSolid(true);
  lPole->SetVisAttributes(VisAtt);


  // yoke pieces
  G4double rYoke = outerRadius - poleR - itsBpRadius + poleR * cos(dPhi / 2);

  if(rYoke > 0 ) // place yoke
    {

      // random ...
      G4double rYoke1 =  outerRadius; // outer length
      G4double rYoke2 =  itsBpRadius;  // inner length 

      G4LogicalVolume* lYoke1 = 
	new G4LogicalVolume(new G4Trd(itsName+"_yoke1",
				      rYoke1 / 2,
				      rYoke2 / 2,
				      itsLength/2,
				      itsLength/2,
				      rYoke/2),
			    theMaterials->GetMaterial("Iron"),
			    itsName+"yoke_outer1");

      G4RotationMatrix* rotYoke = new G4RotationMatrix;
      //rotYoke = NULL;
      rotYoke->rotateX( - pi / 2.);
      rotYoke->rotateY(  pi / 4.);

      G4double xYoke = (poleR - poleR * cos(dPhi / 2) + itsBpRadius + rYoke/2) / sqrt(2.0);
      G4double yYoke = (poleR - poleR * cos(dPhi / 2) + itsBpRadius + rYoke/2) / sqrt(2.0);


      G4VPhysicalVolume* itsPhysiQYoke1;
      itsPhysiQYoke1 = new G4PVPlacement(
			  rotYoke,                      // rotation
			  G4ThreeVector(xYoke,yYoke,0), // its position
			  lYoke1,                       // its logical volume
			  itsName+"_yoke_solid",        // its name
			  lQuadrant,                    // its mother volume
			  false,                        // no boolean operation
			  0);                           // copy number
      SetMultiplePhysicalVolumes(itsPhysiQYoke1);

      // color-coding 
      G4VisAttributes* VisAtt1 = 
	new G4VisAttributes(G4Colour(1., 0., 0.4));
      VisAtt1->SetForceSolid(true);
      lYoke1->SetVisAttributes(VisAtt1);
    }
  else
    {
      G4cerr<<"Not enough place for yoke..."<<G4endl;
    }


  // put all quadrants in the outer volume

 
  G4VPhysicalVolume* itsPhysiQuadrant1;
  itsPhysiQuadrant1 = new G4PVPlacement(
					(G4RotationMatrix*)0,                  // rotation
					(G4ThreeVector)0,                     // its position
					lQuadrant,             // its logical volume
					itsName+"_solid",      // its name
					itsOuterLogicalVolume, // its mother volume
					false,                 // no boolean operation
					0);                    // copy number


  G4RotationMatrix* rotQ2= new  G4RotationMatrix;
  rotQ2->rotateZ( pi / 2.);

  G4VPhysicalVolume* itsPhysiQuadrant2;
  itsPhysiQuadrant2 = new G4PVPlacement(
		      rotQ2,                 // rotation
		      (G4ThreeVector)0,                     // its position
		      lQuadrant,             // its logical volume
		      itsName+"_solid",	     // its name
		      itsOuterLogicalVolume, // its mother volume
		      false,                 // no boolean operation
		      0);                    // copy number

  G4RotationMatrix* rotQ3= new  G4RotationMatrix;
  rotQ3->rotateZ( pi );
  
  G4VPhysicalVolume* itsPhysiQuadrant3;
  itsPhysiQuadrant3 = new G4PVPlacement(
		      rotQ3,                 // rotation
		      (G4ThreeVector)0,                     // its position
		      lQuadrant,             // its logical volume
		      itsName+"_solid",	     // its name
		      itsOuterLogicalVolume, // its mother volume
		      false,                 // no boolean operation
		      0);                    // copy number


  G4RotationMatrix* rotQ4= new  G4RotationMatrix;
  rotQ4->rotateZ( 3. * pi / 2.);
  
  G4VPhysicalVolume* itsPhysiQuadrant4;
  itsPhysiQuadrant4 = new G4PVPlacement(
		      rotQ4,                  // rotation
		      (G4ThreeVector)0,                      // its position
		      lQuadrant,              // its logical volume
		      itsName+"_solid",	      // its name
		      itsOuterLogicalVolume,  // its mother volume
		      false,                  // no boolean operation
		      0);                     // copy number


  //rotQ->rotateZ( pi / 4.);


  // insert the outer volume into the marker volume
  itsPhysiComp = 
    new G4PVPlacement(
		      (G4RotationMatrix*)0,                      // no rotation
		      (G4ThreeVector)0,                      // its position
		      itsOuterLogicalVolume,  // its logical volume
		      itsName+"_outer_phys",  // its name
		      itsMarkerLogicalVolume, // its mother  volume
		      false,                  // no boolean operation
		      0);                     // copy number
  
  SetMultiplePhysicalVolumes(itsPhysiQPole1);
  SetMultiplePhysicalVolumes(itsPhysiQuadrant1);
  SetMultiplePhysicalVolumes(itsPhysiQuadrant2);
  SetMultiplePhysicalVolumes(itsPhysiQuadrant3);
  SetMultiplePhysicalVolumes(itsPhysiQuadrant4);
  SetMultiplePhysicalVolumes(itsPhysiComp);

  itsOuterUserLimits =
    new G4UserLimits("quadrupole cut",itsLength,DBL_MAX,DBL_MAX,
		     BDSGlobals->GetThresholdCutCharged());
  //  itsOuterUserLimits->SetMaxAllowedStep(aLength);
  itsOuterLogicalVolume->SetUserLimits(itsOuterUserLimits);
}

/*
void BDSQuadrupole::BuildOuterLogicalVolume()
{
  G4double outerRadius = itsOuterR;
  if(itsOuterR==0) outerRadius = BDSGlobals->GetComponentBoxSize()/2;

  // compute sagitta:
  // why???? 
  // angle is always 0 for quadrupole (see BDSDetectorConstruction.cc)
  // and no bending is taken into account in the quadrupole stepper!

  G4double sagitta=0.;

  if(itsNSegments>1)
    {
      sagitta=itsLength/itsAngle*(1.-cos(itsAngle/2.));
    }
  
  // marker volume
  itsOuterLogicalVolume=
    new G4LogicalVolume(new G4Tubs(itsName+"_outer_solid",
				   itsInnerIronRadius+sagitta,
				   outerRadius * sqrt(2.0),
				   itsLength/2,
				   0,twopi*radian),
			theMaterials->GetMaterial(BDSGlobals->GetVacuumMaterial()),
			itsName+"_outer");
  
  // create one quadrant of the quadrupole
  G4LogicalVolume* lQuadrant = 
    new G4LogicalVolume(new G4Tubs(itsName+"_solid",
				   itsInnerIronRadius+sagitta,
				   outerRadius * sqrt(2.0),
				   itsLength/2,
				   0,pi/ 2 *radian),
			theMaterials->GetMaterial(BDSGlobals->GetVacuumMaterial()),
			itsName+"_outer");
  
  // pole 
  G4double poleR = itsBpRadius;
  G4double phiStart = -pi / 4;
  G4double dPhi = pi / 2;

  G4LogicalVolume* lPole = 
    new G4LogicalVolume(new G4Tubs(itsName+"_pole",
				   0,
				   poleR,
				   itsLength/2,
				   phiStart,
				   dPhi),
			theMaterials->GetMaterial("Iron"),
			itsName+"pole_outer");

  G4RotationMatrix* rotPole = new G4RotationMatrix;
  //rotPole = NULL;
  rotPole->rotateZ(3.*pi / 4.);

  G4double xPole = (poleR + itsBpRadius) / sqrt(2.0);
  G4double yPole = (poleR + itsBpRadius) / sqrt(2.0);


  G4VPhysicalVolume* itsPhysiQPole1;
  itsPhysiQPole1 = new G4PVPlacement(
		      rotPole,			    // rotation
		      G4ThreeVector(xPole,yPole,0), // its position
		      lPole,                        // its logical volume
		      itsName+"_solid",	            // its name
		      lQuadrant,                    // its mother  volume
		      false,		            // no boolean operation
		      0);		            // copy number
  
  // color-coding for the pole
  G4VisAttributes* VisAtt = 
    new G4VisAttributes(G4Colour(1., 0., 0.));
  VisAtt->SetForceSolid(true);
  lPole->SetVisAttributes(VisAtt);


  // yoke pieces
  G4double rYoke = itsOuterR - poleR - itsBpRadius + poleR * cos(dPhi / 2);

  if(rYoke > 0 ) // place yoke
    {

      // random ...
      G4double rYoke1 =  itsOuterR; // outer length
      G4double rYoke2 =  itsBpRadius;  // inner length 

      G4LogicalVolume* lYoke1 = 
	new G4LogicalVolume(new G4Trd(itsName+"_yoke1",
				      rYoke1 / 2,
				      rYoke2 / 2,
				      itsLength/2,
				      itsLength/2,
				      rYoke/2),
			    theMaterials->GetMaterial("Iron"),
			    itsName+"yoke_outer1");

      G4RotationMatrix* rotYoke = new G4RotationMatrix;
      //rotYoke = NULL;
      rotYoke->rotateX( - pi / 2.);
      rotYoke->rotateY(  pi / 4.);

      G4double xYoke = (poleR - poleR * cos(dPhi / 2) + itsBpRadius + rYoke/2) / sqrt(2.0);
      G4double yYoke = (poleR - poleR * cos(dPhi / 2) + itsBpRadius + rYoke/2) / sqrt(2.0);


      G4VPhysicalVolume* itsPhysiQYoke1;
      itsPhysiQYoke1 = new G4PVPlacement(
			  rotYoke,                      // rotation
			  G4ThreeVector(xYoke,yYoke,0), // its position
			  lYoke1,                       // its logical volume
			  itsName+"_yoke_solid",        // its name
			  lQuadrant,                    // its mother volume
			  false,                        // no boolean operation
			  0);                           // copy number
      SetMultiplePhysicalVolumes(itsPhysiQYoke1);

      // color-coding 
      G4VisAttributes* VisAtt1 = 
	new G4VisAttributes(G4Colour(1., 0., 0.4));
      VisAtt1->SetForceSolid(true);
      lYoke1->SetVisAttributes(VisAtt1);
    }
  else
    {
      G4cerr<<"Not enough place for yoke..."<<G4endl;
    }


  // put all quadrants in the outer volume

 
  G4VPhysicalVolume* itsPhysiQuadrant1;
  itsPhysiQuadrant1 = new G4PVPlacement(
		      NULL,                  // rotation
		      (G4ThreeVector)0,                     // its position
		      lQuadrant,             // its logical volume
		      itsName+"_solid",      // its name
		      itsOuterLogicalVolume, // its mother volume
		      false,                 // no boolean operation
		      0);                    // copy number

  G4RotationMatrix* rotQ2= new  G4RotationMatrix;
  rotQ2->rotateZ( pi / 2.);

  G4VPhysicalVolume* itsPhysiQuadrant2;
  itsPhysiQuadrant2 = new G4PVPlacement(
		      rotQ2,                 // rotation
		      (G4ThreeVector)0,                     // its position
		      lQuadrant,             // its logical volume
		      itsName+"_solid",	     // its name
		      itsOuterLogicalVolume, // its mother volume
		      false,                 // no boolean operation
		      0);                    // copy number

  G4RotationMatrix* rotQ3= new  G4RotationMatrix;
  rotQ3->rotateZ( pi );
  
  G4VPhysicalVolume* itsPhysiQuadrant3;
  itsPhysiQuadrant3 = new G4PVPlacement(
		      rotQ3,                 // rotation
		      (G4ThreeVector)0,                     // its position
		      lQuadrant,             // its logical volume
		      itsName+"_solid",	     // its name
		      itsOuterLogicalVolume, // its mother volume
		      false,                 // no boolean operation
		      0);                    // copy number


  G4RotationMatrix* rotQ4= new  G4RotationMatrix;
  rotQ4->rotateZ( 3. * pi / 2.);
  
  G4VPhysicalVolume* itsPhysiQuadrant4;
  itsPhysiQuadrant4 = new G4PVPlacement(
		      rotQ4,                  // rotation
		      (G4ThreeVector)0,                      // its position
		      lQuadrant,              // its logical volume
		      itsName+"_solid",	      // its name
		      itsOuterLogicalVolume,  // its mother volume
		      false,                  // no boolean operation
		      0);                     // copy number


  //rotQ->rotateZ( pi / 4.);


  // insert the outer volume into the marker volume

  G4RotationMatrix* Rot=NULL;
  if(itsAngle!=0) Rot=RotY90;
  
  itsPhysiComp = new G4PVPlacement(
		      Rot,                    // rotation
		      (G4ThreeVector)0,                      // its position
		      itsOuterLogicalVolume,  // its logical volume
		      itsName+"_solid",	      // its name
		      itsMarkerLogicalVolume, // its mother  volume
		      false,                  // no boolean operation
		      0);                     // copy number
  
  SetMultiplePhysicalVolumes(itsPhysiQPole1);
  SetMultiplePhysicalVolumes(itsPhysiQuadrant1);
  SetMultiplePhysicalVolumes(itsPhysiQuadrant2);
  SetMultiplePhysicalVolumes(itsPhysiQuadrant3);
  SetMultiplePhysicalVolumes(itsPhysiQuadrant4);
  SetMultiplePhysicalVolumes(itsPhysiComp);

  itsOuterUserLimits =
    new G4UserLimits("quadrupole cut",itsLength,DBL_MAX,DBL_MAX,
		     BDSGlobals->GetThresholdCutCharged());
  //  itsOuterUserLimits->SetMaxAllowedStep(aLength);
  itsOuterLogicalVolume->SetUserLimits(itsOuterUserLimits);
}
*/

BDSQuadrupole::~BDSQuadrupole()
{
  delete itsVisAttributes;
  delete itsMarkerLogicalVolume;
  delete itsOuterLogicalVolume;
  delete itsPhysiComp;
  delete itsMagField;
  delete itsEqRhs;
  delete itsStepper;
}


