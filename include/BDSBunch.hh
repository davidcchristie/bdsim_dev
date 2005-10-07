// Provide interface to generating
// the primary particles


#ifndef BDSBunch_h
#define BDSBunch_h 

#include "parser/gmad.h"
#include "Randomize.hh"
#include "globals.hh"
#include <fstream.h>

#if CLHEP_VERSION > 8
using namespace CLHEP;
#endif

// for CLHEP < 1.9
//class RandGauss;
//class RandFlat;


class BDSBunch {

public: 
  BDSBunch(); // default constructor
  ~BDSBunch();

public:

  void SetOptions(struct Options&);

  G4double GetSigmaT();  // get initial bunch distribution parameters in Gaussian case 
  G4double GetSigmaX(); 
  G4double GetSigmaY(); 
  G4double GetSigmaXp();
  G4double GetSigmaYp();

  // get next coordinate 
  G4double GetNextX(); 
  G4double GetNextY(); 
  G4double GetNextZ(); 
  G4double GetNextXp();
  G4double GetNextYp();
  G4double GetNextT(); 
  // the same in a bulk
  void GetNextParticle(G4double& x0,G4double& y0,G4double& z0,
		       G4double& xp,G4double& yp,G4double& zp,
		       G4double& t, G4double& E);
  

  G4double GetEmitX();
  G4double GetEmitY();

  G4double GetAlphaX();
  G4double GetAlphaY();

  G4double GetBetaX();
  G4double GetBetaY();


  void SetSigmaT(double);  // set initial bunch distribution parameters in Gaussian case 
  void SetSigmaX(double); 
  void SetSigmaY(double); 
  void SetSigmaXp(double);
  void SetSigmaYp(double);

  void SetX0(double); 
  void SetY0(double); 
  void SetXp0(double);
  void SetYp0(double);

  void SetEmitX(double);
  void SetEmitY(double);

  void SetAlphaX(double);
  void SetAlphaY(double);

  void SetBetaX(double);
  void SetBetaY(double);


private:
  // parameters for Gaussian distribution
  G4double sigmaX; // sigmas for Gaussian bunches (m)
  G4double sigmaY;
  G4double sigmaT;
  G4double sigmaXp;
  G4double sigmaYp;
  
  // distribution centre
  G4double X0;
  G4double Y0;
  G4double rMin;
  G4double rMax;
  G4double Xp0;
  G4double Yp0;
  
  G4double betaX;
  G4double betaY;
  G4double alphaX;
  G4double alphaY;
  G4double emitX;
  G4double emitY;

  int distribType;
  G4double energySpread;

  // input filename
  G4String inputfile;
  std::ifstream InputBunchFile;

 // Gaussian Random number generator:
  RandGauss* GaussGen;
  RandFlat* FlatGen;
  
};

#endif
