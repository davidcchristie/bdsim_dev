#include "BDSArray2DCoords.hh"
#include "BDSFieldLoaderPoisson.hh"
#include "BDSFieldValue.hh"

#include "globals.hh" // geant4 types / globals

#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <sstream>
#include <vector>

BDSFieldLoaderPoisson::BDSFieldLoaderPoisson()
{;}

BDSFieldLoaderPoisson::~BDSFieldLoaderPoisson()
{;}

BDSArray2DCoords* BDSFieldLoaderPoisson::LoadMag2D(G4String fileName)
{
  std::ifstream file;
  file.open(fileName);

  // test if valid file
  if (!file.is_open())
    {G4cerr << "Invalid file name \"" << fileName << "\"" << G4endl; exit(1);}
  else
    {G4cout << "Loading \"" << fileName << "\"" << G4endl;}

  // Pointer to where result will be stored.  Can't be constrcuted until we know
  // the dimensions.
  BDSArray2DCoords* result = nullptr;
  G4int       nX = 0;
  G4int       nY = 0;
  G4double  xMin = 0;
  G4double  yMin = 0;
  G4double  xMax = 0;
  G4double  yMax = 0;
  G4int nColumns = 0;
  std::vector<G4String> keys;
  std::vector<G4String> units;

  // temporary variables
  G4bool intoData  = false;
  G4bool pastNStep = false;
  std::string line;
  G4int indX = 0;
  G4int indY = 0;
  
  while (std::getline(file, line) && 
	 std::find_if_not(line.begin(), line.end(), isspace) != line.end())
    {// read a line only if it's not a blank one

      if (intoData)
	{
	  // General data entry
	  std::istringstream liness(line);
	  G4double value;
	  std::vector<G4double> lineData(nColumns);
	  for (G4int i = 0; i < nColumns; ++i)
	    {
	      liness >> value;
	      lineData.push_back(value);
	    }
	  // Construct field value
	  BDSFieldValue fv(lineData[0], lineData[1], lineData[2]);
	  // we could use the gradients here, but we don't
	  
	  // Copy into array.
	  (*result)(indX,indY) = fv;

	  indX++;
	  if (indX == nX)
	    {
	      indX = 0;
	      indY++;
	    }

	  continue;
	}

      // else try and parser what must be header bits.
      
      // Min
      std::regex min("^.*?\\(Xmin,Ymin\\)\\s*=\\s*", std::regex_constants::icase);
      if (std::regex_search(line, min))
	{// found line beginning with '(Xmin,YMin)'
	  std::regex minValues("\\(([0-9eE.+-]+),([0-9eE.+-]+)\\)");
	  std::smatch match;
	  std::regex_search(line, match, minValues);
	  xMin = std::stod(match[0]);
	  xMax = std::stod(match[1]);
	  continue;
	}

      // Max
      std::regex max("^.*?\\(Xmax,Ymax\\)\\s*=\\s*", std::regex_constants::icase);
      if (std::regex_search(line, min))
	{// found line beginning with '(Xmax,YMax)'
	  std::regex maxValues("\\(([0-9eE.+-]+),([0-9eE.+-]+)\\)");
	  std::smatch match;
	  std::regex_search(line, match, maxValues);
	  xMin = std::stod(match[0]);
	  xMax = std::stod(match[1]);
	  continue;
	}

      // Steps
      std::regex incr("\\s*X\\s*and\\s*Y", std::regex_constants::icase);
      if (std::regex_search(line, incr))
	{//found line for number of increments
	  std::regex nStepValues(":\\s*([0-9eE.+-]+)\\s*([0-9eE.+-]+)");
	  std::smatch match;
	  std::regex_search(line, match, nStepValues);
	  nX = std::stoi(match[0]) + 1;
	  nY = std::stoi(match[1]) + 1; // number of increments, so number of points is +1
	  pastNStep = true;
	  continue;
	}
      
      // Columns
      std::regex noParenthesis("(^((?!\\().)*$)");
      std::regex noNumbers("^(\\D)*$");
      if (pastNStep &&
	  std::regex_search(line, noNumbers) &&
	  std::regex_search(line, noParenthesis))
	{
	  std::istringstream liness(line);
	  for (G4String key; liness >> key;)
	    {keys.push_back(key);}
	  nColumns = G4int(std::distance(keys.begin(), keys.end()));
	  continue;
	}
      
      // Units
      if (pastNStep &&
	  std::regex_search(line, noNumbers) &&
	  !std::regex_search(line, noParenthesis)) // not no parenthesis - ie there are!
	{
	  // match text inside (XXX) parenthesis, ie strip them off
	  std::regex unitsRE("[\\(](\\w+\\-*\\/*\\w*)[\\)]");
	  std::smatch match;
	  std::regex_search(line, match, unitsRE);
	  for (const auto& u : match)
	    {units.push_back(G4String(u));}

	  result = new BDSArray2DCoords(nX, nY, xMin, xMax, yMin, yMax);
	  intoData = true;
	}
      //nothing heap allocated so safe to let all go out of scope
    }

  file.close();
  
  return result;
}