#ifndef BDSEXECOPTIONS_H
#define BDSEXECOPTIONS_H

#include <getopt.h>
#include <string>

#include "globals.hh" // geant4 types / globals

#include "BDSOutputFormat.hh"

#include "parser/options.h"

/**
 * @brief Executable option processing for BDSIM
 *
 * @author Stewart Boogert
 */


class BDSExecOptions
{
public: 
  BDSExecOptions(int argc, char **argv);

  ~BDSExecOptions(){;}

  /// Print out the available executable commands.
  void Usage() const;

  /// Print out the commands and their set values.
  void Print() const;

  /// Accessor for options generated by command line parsing.
  const GMAD::Options& Options() const {return options;}

  /// Acessor for convenience for the one thing that's needed before the parser options.
  inline G4String InputFileName() const {return options.inputFileName;}
  
  /*
  
  inline G4String        GetVisMacroFilename() const     {return visMacroFilename;}
  inline G4bool          GetVisDebug() const             {return visDebug;}
  inline G4String        GetOutputFilename() const       {return outputFilename;}
  inline G4bool          GetOutputFilenameSet() const    {return outputFilenameSet;}
  inline BDSOutputFormat GetOutputFormat() const         {return outputFormat;}
  inline G4bool          GetGFlash() const               {return gflash;}
  inline G4double        GetGFlashEMax() const           {return gflashemax;}
  inline G4double        GetGFlashEMin() const           {return gflashemin;}  
  inline G4bool          GetVerbose() const              {return verbose;}
  inline G4bool          GetVerboseEvent() const         {return verboseEvent;}
  inline G4bool          GetVerboseStep() const          {return verboseStep;}
  inline G4int           GetVerboseEventNumber() const   {return verboseEventNumber;}
  inline G4bool          GetBatch() const                {return batch;}
  inline G4int           GetVerboseRunLevel() const      {return verboseRunLevel;}
  inline G4int           GetVerboseEventLevel() const    {return verboseEventLevel;}
  inline G4int           GetVerboseTrackingLevel() const {return verboseTrackingLevel;}
  inline G4int           GetVerboseSteppingLevel() const {return verboseSteppingLevel;}
  inline G4bool          GetCircular() const             {return circular;}
  inline G4int           GetSeed() const                 {return seed;}
  inline G4bool          IsSeedSet() const               {return setSeed;}
  inline G4String        GetBDSIMPATH() const            {return itsBDSIMPATH;}
  inline G4bool          SetSeedState() const            {return setSeedState;}
  inline G4String        GetSeedStateFilename() const    {return seedStateFilename;}
  inline G4int           GetNGenerate() const            {return nGenerate;}
  inline G4bool          ExportGeometry() const          {return exportGeometry;}
  inline G4bool          GeneratePrimariesOnly() const   {return generatePrimariesOnly;}
  inline G4bool          GetSurvey() const               {return survey;}
  inline G4String        GetSurveyFilename() const       {return surveyFilename;}
  inline G4String        GetExportType() const           {return exportType;}
  inline G4String        GetExportFileName() const       {return exportFileName;}
  */
  
private:
  /// Private default constructor to force use of provided one.
  BDSExecOptions();
  
  /** Parse the command line options
   * @param[in] argc command line input number of variables
   * @param[in] argv array of char* of the input parameters
   */
  void Parse(int arcg, char **argv);
  
  /// Helper method to set the BDSIMPath correctly
  G4String GetPath(G4String filename);

  /// The options instance that is populated by parsing the command line options.
  GMAD::Options options;
};

#endif
