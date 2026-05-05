#include "SCSensitiveDetector.hh"

SCSensitiveDetector::SCSensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{
}

SCSensitiveDetector::~SCSensitiveDetector()
{
}

void SCSensitiveDetector::Initialize(G4HCofThisEvent *)
{
}

G4bool SCSensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
    G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    G4Track *track = aStep->GetTrack();

    // Creator process (nullptr for primaries)
    const G4VProcess* creatorProc = track->GetCreatorProcess();
    G4String creatorProcName = creatorProc ? creatorProc->GetProcessName() : "primary";

    // Track info
    //G4int trackID  = track->GetTrackID();
    //G4int ptrackID = track->GetParentID();
    G4int pdgID    = track->GetDefinition()->GetPDGEncoding();

    // Pre-step Coordinates
    G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
    G4float fX1 = preStepPoint->GetPosition().x();
    G4float fY1 = preStepPoint->GetPosition().y();
    G4float fZ1 = preStepPoint->GetPosition().z();

    // Post-step Coordinates
    G4StepPoint *postStepPoint = aStep->GetPostStepPoint();
    G4float fX2 = postStepPoint->GetPosition().x();
    G4float fY2 = postStepPoint->GetPosition().y();
    G4float fZ2 = postStepPoint->GetPosition().z();

    // Kinetic Energy
    G4float fK1    = preStepPoint->GetKineticEnergy();
    G4float fK2    = postStepPoint->GetKineticEnergy();

    // Energy Deposition
    G4double fEdep = aStep->GetTotalEnergyDeposit();

    // Interaction Process
    //const G4VProcess *preProc = preStepPoint->GetProcessDefinedStep(); 
    //G4String preProcName = "NA";
    //if (preProc) preProcName = preProc->GetProcessName();

    const G4VProcess *postProc = postStepPoint->GetProcessDefinedStep(); 
    G4String postProcName = "NA";
    if (postProc) postProcName = postProc->GetProcessName();

    // Is it an entry particle?
    G4bool isEntry = (preStepPoint->GetStepStatus()==fGeomBoundary);

    // Is it an exit particle?
    //G4bool isExit = (postStepPoint->GetStepStatus()==fGeomBoundary);

    // Time
    //G4float fSec = postStepPoint->GetGlobalTime() / 1E+09;

    // Build Records Table
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillNtupleIColumn(0, 0,  eventID);
    //analysisManager->FillNtupleIColumn(0, 1,  ptrackID);
    //analysisManager->FillNtupleIColumn(0, 2,  trackID);
    analysisManager->FillNtupleIColumn(0, 1,  pdgID);
    analysisManager->FillNtupleIColumn(0, 2,  isEntry);
    //analysisManager->FillNtupleIColumn(0, 3,  isExit);
    //analysisManager->FillNtupleSColumn(0, 3,  preProcName);
    analysisManager->FillNtupleSColumn(0, 3,  postProcName);
    analysisManager->FillNtupleSColumn(0, 4,  creatorProcName);
    analysisManager->FillNtupleDColumn(0, 5,  fX1);
    analysisManager->FillNtupleDColumn(0, 6, fY1);
    analysisManager->FillNtupleDColumn(0, 7, fZ1);
    analysisManager->FillNtupleDColumn(0, 8, fX2);
    analysisManager->FillNtupleDColumn(0, 9, fY2);
    analysisManager->FillNtupleDColumn(0, 10, fZ2);
    analysisManager->FillNtupleDColumn(0, 11, fK1);
    analysisManager->FillNtupleDColumn(0, 12, fK2);
    analysisManager->FillNtupleDColumn(0, 13, fEdep);
    //analysisManager->FillNtupleDColumn(0, 16, fSec);
    analysisManager->AddNtupleRow(0);

    return true;
}

void SCSensitiveDetector::EndOfEvent(G4HCofThisEvent *)
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
}