/* 
Beam Delivery Simulation (BDSIM) Copyright (C) Royal Holloway, 
University of London 2001 - 2018.

This file is part of BDSIM.

BDSIM is free software: you can redistribute it and/or modify 
it under the terms of the GNU General Public License as published 
by the Free Software Foundation version 3 of the License.

BDSIM is distributed in the hope that it will be useful, but 
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with BDSIM.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "BDSDebug.hh"
#include "BDSIntegratorSet.hh"
#include "BDSIntegratorSetType.hh"
#include "BDSIntegratorType.hh"

#include "globals.hh" // geant4 types / globals
#include "G4Version.hh"

BDSIntegratorSet::BDSIntegratorSet(BDSIntegratorType solenoidIn,
				   BDSIntegratorType dipoleIn,
				   BDSIntegratorType dipoleQuadrupoleIn,
				   BDSIntegratorType quadrupoleIn,
				   BDSIntegratorType sextupoleIn,
				   BDSIntegratorType octupoleIn,
				   BDSIntegratorType decapoleIn,
				   BDSIntegratorType multipoleThickIn,
				   BDSIntegratorType muonSpoilerIn,
				   BDSIntegratorType rfcavityIn,
				   BDSIntegratorType rfIn,
				   BDSIntegratorType generalIn,
				   BDSIntegratorType skewQuadrupoleIn,
				   BDSIntegratorType skewSextupoleIn,
				   BDSIntegratorType skewOctupoleIn,
				   BDSIntegratorType skewDecapoleIn,
				   BDSIntegratorType dipoleFringeIn,
				   BDSIntegratorType multipoleThinIn,
				   BDSIntegratorType multipoleOuterIn,
				   BDSIntegratorType rmatrixThinIn,
					 BDSIntegratorType parallelTransporterIn):
  solenoid(solenoidIn),
  dipole(dipoleIn),
  dipoleQuadrupole(dipoleQuadrupoleIn),
  quadrupole(quadrupoleIn),
  sextupole(sextupoleIn),
  octupole(octupoleIn),
  decapole(decapoleIn),
  multipoleThick(multipoleThickIn),
  muonSpoiler(muonSpoilerIn),
  rfcavity(rfcavityIn),
  rf(rfIn),
  general(generalIn),
  skewQuadrupole(skewQuadrupoleIn),
  skewSextupole(skewSextupoleIn),
  skewOctupole(skewOctupoleIn),
  skewDecapole(skewDecapoleIn),
  dipoleFringe(dipoleFringeIn),
  multipoleThin(multipoleThinIn),
  multipoleOuter(multipoleOuterIn),
  rmatrixThin(rmatrixThinIn),
	parallelTransporter(parallelTransporterIn)
{
  isMatrix = false; //default
  // use dipolematrix integrator to check if matrix style as it is the
  // only integrator exclusive to matrix style integrator sets
  if (dipoleIn == BDSIntegratorType::dipolematrix)
    {isMatrix = true;}
}

namespace BDS
{  
  const BDSIntegratorSet* integratorsBDSIMOne =
    new BDSIntegratorSet(BDSIntegratorType::solenoid,       // solenoid
			 BDSIntegratorType::dipolerodrigues,// dipole
			 BDSIntegratorType::dipolematrix,   // dipole quadrupole
			 BDSIntegratorType::quadrupole,     // quadrupole
			 BDSIntegratorType::sextupole,      // sextupole
			 BDSIntegratorType::octupole,       // octupole
			 BDSIntegratorType::decapole,       // decapole
			 BDSIntegratorType::g4classicalrk4, // thick multipole
			 BDSIntegratorType::g4classicalrk4, // muon spoiler
			 BDSIntegratorType::g4classicalrk4, // rfcavity
			 BDSIntegratorType::g4classicalrk4, // rf
			 BDSIntegratorType::g4classicalrk4, // general
			 BDSIntegratorType::g4classicalrk4, // skew quadrupole
			 BDSIntegratorType::g4classicalrk4, // skew sextupole
			 BDSIntegratorType::g4classicalrk4, // skew octupole
			 BDSIntegratorType::g4classicalrk4, // skew decapole
			 BDSIntegratorType::dipolefringe,   // dipole fringe field
			 BDSIntegratorType::multipolethin,  // thin multipole
			 BDSIntegratorType::g4nystromrk4,   // multipole outer
			 BDSIntegratorType::rmatrixthin,
		   BDSIntegratorType::paralleltransport);   // thin rmatrix

  const BDSIntegratorSet* integratorsBDSIMTwo =
    new BDSIntegratorSet(BDSIntegratorType::dipolerodrigues2, // solenoid
			 BDSIntegratorType::dipolerodrigues2, // dipole
			 BDSIntegratorType::dipolematrix,     // dipole quadrupole
			 BDSIntegratorType::quadrupole,       // quadrupole
			 BDSIntegratorType::euler,            // sextupole
			 BDSIntegratorType::euler,            // octupole
			 BDSIntegratorType::euler,            // decapole
			 BDSIntegratorType::g4classicalrk4,   // (thick) multipole
			 BDSIntegratorType::g4classicalrk4,   // muonspoiler
			 BDSIntegratorType::g4classicalrk4,   // rfcavity
			 BDSIntegratorType::g4classicalrk4,   // rf
			 BDSIntegratorType::g4classicalrk4,   // general
			 BDSIntegratorType::g4classicalrk4,   // skew quadrupole
			 BDSIntegratorType::g4classicalrk4,   // skew sextupole
			 BDSIntegratorType::g4classicalrk4,   // skew octupole
			 BDSIntegratorType::g4classicalrk4,   // skew decapole
			 BDSIntegratorType::dipolefringe,     // dipole fringe field
			 BDSIntegratorType::multipolethin,    // thin multipole
			 BDSIntegratorType::g4nystromrk4,     // multipole outer
			 BDSIntegratorType::rmatrixthin,
		   BDSIntegratorType::paralleltransport);     // thin rmatrix
  /// Mad-x style tracking.
  const BDSIntegratorSet*  integratorsBDSIMMatrix =
    new BDSIntegratorSet(BDSIntegratorType::dipolerodrigues2, // solenoid
			 BDSIntegratorType::dipolematrix,   // dipole
			 BDSIntegratorType::dipolematrix,   // dipole quadrupole
			 BDSIntegratorType::quadrupole,     // quadrupole
			 BDSIntegratorType::euler,          // sextupole
			 BDSIntegratorType::euler,          // octupole
			 BDSIntegratorType::euler,          // decapole
			 BDSIntegratorType::g4classicalrk4, // thick multipole
			 BDSIntegratorType::g4classicalrk4, // muon spoiler
			 BDSIntegratorType::g4classicalrk4, // rfcavity
			 BDSIntegratorType::g4classicalrk4, // rf
			 BDSIntegratorType::g4classicalrk4, // general
			 BDSIntegratorType::g4classicalrk4, // skew quadrupole
			 BDSIntegratorType::g4classicalrk4, // skew sextupole
			 BDSIntegratorType::g4classicalrk4, // skew octupole
			 BDSIntegratorType::g4classicalrk4, // skew decapole
			 BDSIntegratorType::dipolefringe,   // dipole fringe field
			 BDSIntegratorType::multipolethin,  // thin multipole
			 BDSIntegratorType::g4nystromrk4,   // multipole outer
			 BDSIntegratorType::rmatrixthin,
		   BDSIntegratorType::paralleltransport);   // thin rmatrix
  /// Mad-x style tracking with fringe field momentum scaling.
  const BDSIntegratorSet*  integratorsBDSIMMatrixFringeScaling =
    new BDSIntegratorSet(BDSIntegratorType::solenoid,       // solenoid
			 BDSIntegratorType::dipolematrix,   // dipole
			 BDSIntegratorType::dipolematrix,   // dipole quadrupole
			 BDSIntegratorType::quadrupole,     // quadrupole
			 BDSIntegratorType::euler,          // sextupole
			 BDSIntegratorType::euler,          // octupole
			 BDSIntegratorType::euler,          // decapole
			 BDSIntegratorType::g4classicalrk4, // thick multipole
			 BDSIntegratorType::g4classicalrk4, // muon spoiler
			 BDSIntegratorType::g4classicalrk4, // rfcavity
			 BDSIntegratorType::g4classicalrk4, // rf
			 BDSIntegratorType::g4classicalrk4, // general
			 BDSIntegratorType::g4classicalrk4, // skew quadrupole
			 BDSIntegratorType::g4classicalrk4, // skew sextupole
			 BDSIntegratorType::g4classicalrk4, // skew octupole
			 BDSIntegratorType::g4classicalrk4, // skew decapole
			 BDSIntegratorType::dipolefringescaling, // dipole fringe field
			 BDSIntegratorType::multipolethin,  // thin multipole
			 BDSIntegratorType::g4nystromrk4,   // multipole outer
			 BDSIntegratorType::rmatrixthin,
		   BDSIntegratorType::paralleltransport);   // thin rmatrix
  /// All 4th Order Runge Kutte.
  const BDSIntegratorSet* integratorsGeant4 =
    new BDSIntegratorSet(BDSIntegratorType::g4classicalrk4, // solenoid
			 BDSIntegratorType::g4classicalrk4, // dipole
			 BDSIntegratorType::g4classicalrk4, // dipole quadrupole
			 BDSIntegratorType::g4classicalrk4, // quadrupole
			 BDSIntegratorType::g4classicalrk4, // sextupole
			 BDSIntegratorType::g4classicalrk4, // octupole
			 BDSIntegratorType::g4classicalrk4, // decapole
			 BDSIntegratorType::g4classicalrk4, // thick multipole
			 BDSIntegratorType::g4classicalrk4, // muon spoiler
			 BDSIntegratorType::g4classicalrk4, // rfcavity
			 BDSIntegratorType::g4classicalrk4, // rf
			 BDSIntegratorType::g4classicalrk4, // general
			 BDSIntegratorType::g4classicalrk4, // skew quadrupole
			 BDSIntegratorType::g4classicalrk4, // skew sextupole
			 BDSIntegratorType::g4classicalrk4, // skew octupole
			 BDSIntegratorType::g4classicalrk4, // skew decapole
			 BDSIntegratorType::dipolefringe,   // dipole fringe field
			 BDSIntegratorType::multipolethin,  // thin multipole
			 BDSIntegratorType::g4nystromrk4,   // multipole outer
			 BDSIntegratorType::rmatrixthin,
		   BDSIntegratorType::paralleltransport);   // thin rmatrix

#if G4VERSION_NUMBER > 1039
  const BDSIntegratorSet* integratorsGeant4DP =
    new BDSIntegratorSet(BDSIntegratorType::g4dormandprince745, // solenoid
			 BDSIntegratorType::g4dormandprince745, // dipole
			 BDSIntegratorType::g4dormandprince745, // dipole quadrupole
			 BDSIntegratorType::g4dormandprince745, // quadrupole
			 BDSIntegratorType::g4dormandprince745, // sextupole
			 BDSIntegratorType::g4dormandprince745, // octupole
			 BDSIntegratorType::g4dormandprince745, // decapole
			 BDSIntegratorType::g4dormandprince745, // thick multipole
			 BDSIntegratorType::g4dormandprince745, // muon spoiler
			 BDSIntegratorType::g4dormandprince745, // rfcavity
			 BDSIntegratorType::g4dormandprince745, // rf
			 BDSIntegratorType::g4dormandprince745, // general
			 BDSIntegratorType::g4dormandprince745, // skew quadrupole
			 BDSIntegratorType::g4dormandprince745, // skew sextupole
			 BDSIntegratorType::g4dormandprince745, // skew octupole
			 BDSIntegratorType::g4dormandprince745, // skew decapole
			 BDSIntegratorType::dipolefringe,       // dipole fringe field
			 BDSIntegratorType::multipolethin,      // thin multipole
  			 BDSIntegratorType::g4dormandprince745, // multipole outer
			 BDSIntegratorType::rmatrixthin);       // thin rmatrix
#endif
}

const BDSIntegratorSet* BDS::IntegratorSet(G4String sets)
{
  return BDS::IntegratorSet(BDS::DetermineIntegratorSetType(sets));
}

const BDSIntegratorSet* BDS::IntegratorSet(BDSIntegratorSetType set)
{
  switch (set.underlying())
    {
    case BDSIntegratorSetType::geant4:
      {return BDS::integratorsGeant4; break;}
#if G4VERSION_NUMBER > 1039
    case BDSIntegratorSetType::geant4dp:
      {return BDS::integratorsGeant4DP; break;}
#endif
    case BDSIntegratorSetType::bdsimone:
      {return BDS::integratorsBDSIMOne;  break;}
    case BDSIntegratorSetType::bdsimtwo:
      {return BDS::integratorsBDSIMTwo;}
    case BDSIntegratorSetType::bdsimmatrix:
      {return BDS::integratorsBDSIMMatrix;}
	case BDSIntegratorSetType::bdsimmatrixfringescaling:
	  {return BDS::integratorsBDSIMMatrixFringeScaling;}
    default:
      {return BDS::integratorsBDSIMOne;  break;}
    }
}

BDSIntegratorType BDSIntegratorSet::Integrator(const BDSFieldType field) const
{
  switch (field.underlying())
    {
    case BDSFieldType::none:
      {return general; break;}// shouldn't really happen, but for completeness.
    case BDSFieldType::bmap1d:
    case BDSFieldType::bmap2d:
    case BDSFieldType::bmap3d:
    case BDSFieldType::bmap4d:
    case BDSFieldType::ebmap1d:
    case BDSFieldType::ebmap2d:
    case BDSFieldType::ebmap3d:
    case BDSFieldType::ebmap4d:
    case BDSFieldType::emap1d:
    case BDSFieldType::emap2d:
    case BDSFieldType::emap3d:
    case BDSFieldType::emap4d:
    case BDSFieldType::mokka:
      {
	G4cout << __METHOD_NAME__ << "Warning - this is overriding the specified field maps integrator" << G4endl;
	return general;
	break;
      }
    case BDSFieldType::solenoid:
      {return solenoid;         break;}
    case BDSFieldType::dipole:
      {return dipole;           break;}
    case BDSFieldType::dipolequadrupole:
      {return dipoleQuadrupole; break;}
    case BDSFieldType::quadrupole:
      {return quadrupole;       break;}
    case BDSFieldType::sextupole:
      {return sextupole;        break;}
    case BDSFieldType::octupole:
      {return octupole;         break;}
    case BDSFieldType::decapole:
      {return decapole;         break;}
    case BDSFieldType::multipole:
      {return multipoleThick;   break;}
    case BDSFieldType::muonspoiler:
      {return muonSpoiler;      break;}
    case BDSFieldType::skewquadrupole:
      {return skewQuadrupole;   break;}
    case BDSFieldType::skewsextupole:
      {return skewSextupole;    break;}
    case BDSFieldType::skewoctupole:
      {return skewOctupole;     break;}
    case BDSFieldType::skewdecapole:
      {return skewDecapole;     break;}
    case BDSFieldType::rfcavity:
      {return rfcavity;         break;}
    case BDSFieldType::rf:
      {return rf;               break;}
    case BDSFieldType::multipoleouterdipole:
    case BDSFieldType::multipoleouterquadrupole:
    case BDSFieldType::multipoleoutersextupole:
    case BDSFieldType::multipoleouteroctupole:
    case BDSFieldType::multipoleouterdecapole:
    case BDSFieldType::skewmultipoleouterquadrupole:
    case BDSFieldType::skewmultipoleoutersextupole:
    case BDSFieldType::skewmultipoleouteroctupole:
    case BDSFieldType::skewmultipoleouterdecapole:
    case BDSFieldType::multipoleouterdipole3d:
      {return multipoleOuter;   break;}
		case BDSFieldType::rmatrix:
		  {return rmatrixThin;}
		case BDSFieldType::paralleltransporter:
		  {return parallelTransporter; break;}
    default:
      {return general;          break;}
    }
}

G4bool BDSIntegratorSet::IsMatrixIntegratorSet() const
  {return isMatrix;}
