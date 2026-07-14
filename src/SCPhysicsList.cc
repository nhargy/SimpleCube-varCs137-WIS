#include "SCPhysicsList.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4ThermalNeutrons.hh"

SCPhysicsList::SCPhysicsList()
    : FTFP_BERT_HP()
{
    ReplacePhysics(new G4EmStandardPhysics_option4());
    RegisterPhysics(new G4ThermalNeutrons());
}

SCPhysicsList::~SCPhysicsList() = default;