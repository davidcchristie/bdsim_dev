/* BDSIM code.    Version 1.0
   Author: Grahame A. Blair, Royal Holloway, Univ. of London.
   Last modified 24.7.2002
   Copyright (c) 2002 by G.A.Blair.  ALL RIGHTS RESERVED. 
*/

#ifndef BDSSamplerSD_h
#define BDSSamplerSD_h 1

#include "G4VSensitiveDetector.hh"
#include "BDSSamplerHit.hh"
#include "G4Navigator.hh"
#include "G4TransportationManager.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class BDSSamplerSD : public G4VSensitiveDetector
{

  public:
      BDSSamplerSD(G4String name);
      ~BDSSamplerSD();

      void Initialize(G4HCofThisEvent*HCE);
      G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);
      void EndOfEvent(G4HCofThisEvent*HCE);
      void clear();
      void DrawAll();
      void PrintAll();

  private:
      BDSSamplerHitsCollection *SamplerCollection;
      G4Navigator* StepperNavigator;
      G4bool StoreHit;

};




#endif

