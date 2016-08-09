#ifndef BDSINTEGRATORSET_H
#define BDSINTEGRATORSET_H

#include "BDSFieldType.hh"
#include "BDSIntegratorSetType.hh"
#include "BDSIntegratorType.hh"

#include "globals.hh" // geant4 types / globals

struct BDSIntegratorSet;

namespace BDS
{
  /// @{ Return the appropraiate set of integrators to use for each magnet type.
  const BDSIntegratorSet* IntegratorSet(G4String             set);
  const BDSIntegratorSet* IntegratorSet(BDSIntegratorSetType set);
  /// @}

  /// Get appropriate integrator from a set based on the field type.
  BDSIntegratorType Integrator(const BDSIntegratorSet* set,
			       const BDSFieldType      field);
}

/**
 * @brief Which integrator to use for each type of magnet / field object.
 * 
 * There is one to match every type of field in BDSFieldType.
 * 
 * @author Laurie Nevay
 */

struct BDSIntegratorSet
{
  BDSIntegratorSet(BDSIntegratorType solenoidIn,
		   BDSIntegratorType dipoleIn,
		   BDSIntegratorType quadrupoleIn,
		   BDSIntegratorType sextupoleIn,
		   BDSIntegratorType octupoleIn,
		   BDSIntegratorType decapoleIn,
		   BDSIntegratorType multipoleIn,
		   BDSIntegratorType muonspoilerIn,
		   BDSIntegratorType rfcavityIn,
		   BDSIntegratorType rfIn,
		   BDSIntegratorType generalIn,
		   BDSIntegratorType skewQuadrupoleIn,
		   BDSIntegratorType skewSextupoleIn,
		   BDSIntegratorType skewOctupoleIn,
		   BDSIntegratorType skewDecapoleIn):
    solenoid(solenoidIn),
    dipole(dipoleIn),
    quadrupole(quadrupoleIn),
    sextupole(sextupoleIn),
    octupole(octupoleIn),
    decapole(decapoleIn),
    multipole(multipoleIn),
    muonspoiler(muonspoilerIn),
    rfcavity(rfcavityIn),
    rf(rfIn),
    general(generalIn),
    skewQuadrupole(skewQuadrupoleIn),
    skewSextupole(skewSextupoleIn),
    skewOctupole(skewOctupoleIn),
    skewDecapole(skewDecapoleIn)
  {;}

  BDSIntegratorType solenoid;
  BDSIntegratorType dipole;
  BDSIntegratorType quadrupole;
  BDSIntegratorType sextupole;
  BDSIntegratorType octupole;
  BDSIntegratorType decapole;
  BDSIntegratorType multipole;
  BDSIntegratorType muonspoiler;
  BDSIntegratorType rfcavity;
  BDSIntegratorType rf;
  BDSIntegratorType general;
  BDSIntegratorType skewQuadrupole;
  BDSIntegratorType skewSextupole;
  BDSIntegratorType skewOctupole;
  BDSIntegratorType skewDecapole;
};

#endif
