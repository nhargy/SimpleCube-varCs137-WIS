#ifndef SCACTIONINITIALIZATION_HH
#define SCACTIONINITIALIZATION_HH

#include "G4VUserActionInitialization.hh"

#include "SCPrimaryGenerator.hh"
#include "SCRunAction.hh"

class SCActionInitialization : public G4VUserActionInitialization
{
public:
    SCActionInitialization(const G4String& outputFilePath = "output.root");
    ~SCActionInitialization();
    
    virtual void Build() const;

private:
    G4String fOutputFilePath;
};

#endif