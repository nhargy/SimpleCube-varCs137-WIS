#ifndef SCSENSITIVEDETECTOR_HH
#define SCSENSITIVEDETECTOR_HH

#include "G4VSensitiveDetector.hh"

#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

class SCSensitiveDetector : public G4VSensitiveDetector
{
public:
    SCSensitiveDetector(G4String);
    ~SCSensitiveDetector();

private:
    virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *);

    virtual void Initialize(G4HCofThisEvent*) override;
    virtual void EndOfEvent(G4HCofThisEvent *) override;
};

#endif