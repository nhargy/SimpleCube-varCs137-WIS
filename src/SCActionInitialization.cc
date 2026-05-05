#include "SCActionInitialization.hh"

SCActionInitialization::SCActionInitialization(const G4String& outputFilePath) : fOutputFilePath(outputFilePath)
{}

SCActionInitialization::~SCActionInitialization()
{}

void SCActionInitialization::Build() const
{
    SCPrimaryGenerator *generator = new SCPrimaryGenerator();
    SetUserAction(generator);

    SCRunAction *runAction = new SCRunAction();
    runAction->SetOutputDirectory(fOutputFilePath);
    SetUserAction(runAction);
}