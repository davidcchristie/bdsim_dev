/* BDSIM code.    Version 1.0
   Author: Grahame A. Blair, Royal Holloway, Univ. of London.
   Last modified 9.3.2003
   Copyright (c) 2003 by G.A.Blair.  ALL RIGHTS RESERVED. 
*/
#ifndef BDSSynchRadPhysics_h
#define BDSSynchRadPhysics_h 1

#include "globals.hh"
#include "G4ios.hh"
#include "G4VPhysicsConstructor.hh"
#include "BDSSynchRadBuilder.hh"

class BDSSynchRadPhysics : public G4VPhysicsConstructor
{
public: 
  BDSSynchRadPhysics(const G4String& name ="BDSSynchRad");
  virtual ~BDSSynchRadPhysics();
  
public: 
  virtual void ConstructParticle();
  virtual void ConstructProcess();
  
private:
  
  BDSSynchRadBuilder  theBDSSynchRadBuilder;
};


#endif





