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
#ifndef BDSFIELDEINTERPOLATED3D_H
#define BDSFIELDEINTERPOLATED3D_H

#include "BDSFieldEInterpolated.hh"

#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"

class BDSInterpolator3D;

/**
 * @brief A 3D field from an interpolated array with any interpolation.
 *
 * Owns interpolator.
 *
 * This provides a simple interface for electric fields to use a 3D
 * interpolator irrespective of which type of interpolator it is.
 *
 * @author Laurie Nevay
 */

class BDSFieldEInterpolated3D: public BDSFieldEInterpolated
{
public:
  BDSFieldEInterpolated3D(BDSInterpolator3D* interpolatorIn,
			  G4Transform3D      offset     = G4Transform3D::Identity,
			  G4double           eScalingIn = 1.0);

  virtual ~BDSFieldEInterpolated3D();

  /// Return the interpolated field value at a given point.
  virtual G4ThreeVector GetField(const G4ThreeVector& position,
				 const G4double       t = 0) const;

  inline const BDSInterpolator3D* Interpolator() const {return interpolator;}

private:
  /// Private default constructor to force use of provided one.
  BDSFieldEInterpolated3D() = delete;

  BDSInterpolator3D* interpolator; ///< Interpolator the field is based on.
};

#endif
