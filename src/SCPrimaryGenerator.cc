#include "SCPrimaryGenerator.hh"
#include "Randomize.hh"
#include "G4RandomTools.hh"

#include <fstream>
#include <sstream>
#include <cctype>

SCPrimaryGenerator::SCPrimaryGenerator()
{
    ApplyMessengers();

    fParticleGun = new G4ParticleGun(1);
    G4ThreeVector pos(xSource *mm, ySource *mm, zSource* mm);
    fParticleGun->SetParticlePosition(pos);
}

SCPrimaryGenerator::~SCPrimaryGenerator()
{
    delete fParticleGun;
    delete fMessengerSource;
}

void SCPrimaryGenerator::ApplyMessengers() {

    fMessengerSource = new G4GenericMessenger(
        this,
        "/source/",
        "source"
    );

    fMessengerSource->DeclareProperty(
        "isIon",
        isIon,
        "isIon"
    );

    fMessengerSource->DeclareProperty(
        "pdgSource",
        pdgSource,
        "pdgSource"
    );

    fMessengerSource->DeclareProperty(
        "isSecondSource",
        isSecondSource,
        "isSecondSource"
    );

    fMessengerSource->DeclareProperty(
        "energySource2",
        energySource2,
        "energySource2"
    );
    
    fMessengerSource->DeclareProperty(
        "pdgSource2",
        pdgSource2,
        "pdgSource2"
    );

    fMessengerSource->DeclareProperty(
        "fraction2",
        fraction2,
        "fraction2"
    );

    fMessengerSource->DeclareProperty(
        "Z",
        Z,
        "Z"
    );
 
    fMessengerSource->DeclareProperty(
        "A",
        A,
        "A"
    );

    fMessengerSource->DeclareProperty(
        "energySource",
        energySource,
        "energySource"
    );

    fMessengerSource->DeclareProperty(
        "spectrumFile",
        spectrumFile,
        "Path to spectrum file with columns: E(MeV) w(~pdf)"
    );


    fMessengerSource->DeclareProperty(
        "xSource",
        xSource,
        "xSource"
    );

    fMessengerSource->DeclareProperty(
        "ySource",
        ySource,
        "ySource"
    );

    fMessengerSource->DeclareProperty(
        "zSource",
        zSource,
        "zSource"
    );
    
    isIon = 1;
    pdgSource = 22;
    Z = 55;
    A = 137;
    energySource = 1.;
    xSource = 0;
    ySource = 0;
    zSource = 0;

    isSecondSource = 0;
    pdgSource2     = 22;
    energySource2  = 4.438;
    fraction2      = 0.6;
}

void SCPrimaryGenerator::ensureSpectrumLoaded()
{
    if (fSpectrum && spectrumFile == loadedSpectrumFile) return;

    std::ifstream in(spectrumFile);
    if (!in) {
        throw std::runtime_error("Cannot open spectrum file: " + std::string(spectrumFile));
    }

    std::vector<double> E, W;
    std::string line;
    while (std::getline(in, line)) {
        // trim leading spaces
        size_t k = 0;
        while (k < line.size() && std::isspace(static_cast<unsigned char>(line[k]))) ++k;
        if (k == line.size()) continue;      // blank
        if (line[k] == '#') continue;        // comment

        std::istringstream iss(line);
        double e, w;
        if (!(iss >> e >> w)) continue;      // or throw if you prefer strict
        E.push_back(e);
        W.push_back(w);
    }

    fSpectrum = std::make_unique<SpectrumSampler>(std::move(E), std::move(W));
    loadedSpectrumFile = spectrumFile;
}

void SCPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    G4ThreeVector pos(xSource *mm, ySource *mm, zSource *mm);
    fParticleGun->SetParticlePosition(pos);

    if (isIon==1) {
        G4ParticleDefinition *ion  = G4IonTable::GetIonTable()->GetIon(Z, A, 0.);
        fParticleGun->SetParticleDefinition(ion);
        fParticleGun->SetParticleEnergy(0.);

        // Create vertex
        fParticleGun->GeneratePrimaryVertex(anEvent);
    }
    else if (isIon==0) {
        G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
        G4ParticleDefinition* particle = particleTable->FindParticle(pdgSource);
        fParticleGun->SetParticleDefinition(particle);

        if (energySource >= 0) {
            fParticleGun->SetParticleEnergy(energySource * MeV);
        }
        else if (energySource < 0) {
            ensureSpectrumLoaded();
            const double u = G4UniformRand();
            const double sampledEnergySource = fSpectrum->sample(u);
            fParticleGun->SetParticleEnergy(sampledEnergySource * MeV);
        }

        // Random isotropic direction
        G4ThreeVector dir = G4RandomDirection();
        fParticleGun->SetParticleMomentumDirection(dir);

        // Create vertex
        fParticleGun->GeneratePrimaryVertex(anEvent);

        if ((isSecondSource == 1) and (G4UniformRand() < fraction2)) {
            // Another Random isotropic direction
            G4ThreeVector dir2 = G4RandomDirection();
            fParticleGun->SetParticleMomentumDirection(dir2);

            G4ParticleDefinition* particle2 = particleTable->FindParticle(pdgSource2);
            fParticleGun->SetParticleDefinition(particle2);
            fParticleGun->SetParticleEnergy(energySource2 * MeV);

            // Create vertex
            fParticleGun->GeneratePrimaryVertex(anEvent);
        }
    }
}