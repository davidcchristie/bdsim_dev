#ifndef BDSINTEGRATORDIPOLE_H
#define BDSINTEGRATORDIPOLE_H

#include "BDSIntegratorBase.hh"

#include "globals.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4Mag_EqRhs.hh"
#include "G4ThreeVector.hh"

class BDSMagnetStrength;

/**
 * @brief Stepper that calculates trajectory through uniform magnetic field.
 *
 * This calculates the coordinates of a particle moving through a dipole field.
 * This ignores the field value supplied from Geant4 and uses its own field and
 * quadrupole gradient from the BDSMagnetStrength instance supplied w.r.t. the 
 * rigidity also supplied.
 * 
 * Originally part of BDSIM by many authors.
 */

class BDSIntegratorDipole: public BDSIntegratorBase
{
public:
  BDSIntegratorDipole(const BDSMagnetStrength* strength,
		      const G4double           brho,
		      G4Mag_EqRhs* const       eqRHSIn);
    
  ~BDSIntegratorDipole(){;}

  /// Stepper for this integrator.  Calculates the new coordinates of a particle
  /// through a uniform magnetic field.
  virtual void Stepper(const G4double y[],
		       const G4double dydx[],
		       const G4double h,
		       G4double yout[],
		       G4double yerr[]);
  
protected:
  /// Calcaulte the new particle coordinates. A first order Step along a solenoid inside the field.
  void AdvanceHelix(const G4double yIn[],
		    const G4double dydx[],
		    G4ThreeVector  Bfld,
		    G4double h,
		    G4double yOut[],
		    G4double yErr[]);
  
private:
  /// Data stored in order to find the chord.
  G4ThreeVector yInitial, yMidPoint, yFinal;

  /// Angle that the dipole induces in the reference trajectory
  const G4double angle;

  /// Arc length of the magnetic field.
  const G4double length;

  /// Quadrupolar field gradient - B'
  G4double bPrime;

  /// Uniform magnetic field in global Y direction
  G4double bField;

  /// Total beam energy
  G4double nominalEnergy; 
};

#endif 