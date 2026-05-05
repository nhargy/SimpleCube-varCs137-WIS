#include "SCRunAction.hh"

#include <iomanip>   // for std::setw, std::setfill
#include <sstream>   // for std::ostringstream

SCRunAction::SCRunAction()
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

    analysisManager->CreateNtuple("Hits", "Hits");
    analysisManager->CreateNtupleIColumn("eventID");
    //analysisManager->CreateNtupleIColumn("ptrackID");
    //analysisManager->CreateNtupleIColumn("trackID");
    analysisManager->CreateNtupleIColumn("pdgID");
    analysisManager->CreateNtupleIColumn("isEntry");
    //analysisManager->CreateNtupleIColumn("isExit");
    //analysisManager->CreateNtupleSColumn("preProcName");
    analysisManager->CreateNtupleSColumn("postProcName");
    analysisManager->CreateNtupleSColumn("creatorProc");
    analysisManager->CreateNtupleDColumn("fX1");
    analysisManager->CreateNtupleDColumn("fY1");
    analysisManager->CreateNtupleDColumn("fZ1");
    analysisManager->CreateNtupleDColumn("fX2");
    analysisManager->CreateNtupleDColumn("fY2");
    analysisManager->CreateNtupleDColumn("fZ2");
    analysisManager->CreateNtupleDColumn("fK1");
    analysisManager->CreateNtupleDColumn("fK2");
    analysisManager->CreateNtupleDColumn("fEdep");
    //analysisManager->CreateNtupleDColumn("fSec");
    analysisManager->FinishNtuple(0);
}

SCRunAction::~SCRunAction()
{
}

void SCRunAction::BeginOfRunAction(const G4Run *run)
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

    G4int runID = run->GetRunID();

    G4cout << " " << G4endl;
    G4cout << "====== Starting Run: " << runID << " ======"  << G4endl;

    //std::ostringstream strRunID;
    //strRunID << std::setw(4) << std::setfill('0') << (runID+1);

    // Build filename
    G4String filename = outputDirectory;
    //if (!filename.empty() && filename.back() != '/') {
    //    filename += '/';
    //}
    //filename += "output" + strRunID.str() + ".root";
    
    analysisManager->SetFileName(filename);
    analysisManager->OpenFile();

    //analysisManager->OpenFile("output" + strRunID.str() + ".root");
}

void SCRunAction::EndOfRunAction(const G4Run *run)
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

    analysisManager->Write();

    analysisManager->CloseFile();

    G4int runID = run->GetRunID();

    G4cout << "Finishing Run " << runID << G4endl;
}