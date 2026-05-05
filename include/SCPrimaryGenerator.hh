#ifndef SCPRIMARYGENERATOR_HH
#define SCPRIMARYGENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4IonTable.hh"
#include "G4GenericMessenger.hh"
#include "SCSpectrumSampler.hh"
#include <memory>

class SCPrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
public:
    SCPrimaryGenerator();
    ~SCPrimaryGenerator();

    virtual void GeneratePrimaries(G4Event *);

private:

    G4String spectrumFile = "absolutepath/to/data/e_dist/edist_Neutrons_AmBe_01.dat";  // macro can override
    G4String loadedSpectrumFile = "";
    std::unique_ptr<SpectrumSampler> fSpectrum;     // cached sampler

    void ensureSpectrumLoaded();

    void ApplyMessengers();

    G4ParticleGun *fParticleGun;
    
    G4GenericMessenger* fMessengerSource;
    G4int    isIon;
    G4int    pdgSource;
    G4int    Z;
    G4int    A;
    G4double energySource;
    G4double xSource;
    G4double ySource;
    G4double zSource;

    // For secondary gun
    G4int    isSecondSource;
    G4int    pdgSource2;
    G4double energySource2;
    G4double fraction2;
};

#endif