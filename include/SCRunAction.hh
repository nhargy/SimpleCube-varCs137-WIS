#ifndef SCRUNACTION_HH
#define SCRUNACTION_HH

#include "G4UserRunAction.hh"
#include "G4Run.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

class SCRunAction : public G4UserRunAction
{
public:
    SCRunAction();
    ~SCRunAction();

    virtual void BeginOfRunAction(const G4Run *);
    virtual void EndOfRunAction(const G4Run *);

    void SetOutputDirectory(const G4String& dir) { outputDirectory = dir; }
    G4String GetOutputDirectory() const { return outputDirectory; }

private:
    G4String outputDirectory;
};

#endif