#include <iostream>

#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "SCPhysicsList.hh"
#include "SCDetectorConstruction.hh"
#include "SCActionInitialization.hh"

int main(int argc, char** argv) {

    G4RunManager *runManager = new G4RunManager;
    G4UIExecutive *ui = nullptr;

    G4String macroFile  = "";
    G4String outputFile = "output.root";
    G4int    nPrimaries = 1; 
    G4int    seed       = 1;

    // Simple manual arg parsing
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-m" || arg == "--macro") {
            if (i + 1 < argc) {
                macroFile = argv[++i];
            } else {
                G4cerr << "Error: " << arg << " requires a macro file.\n";
                return 1;
            }
        }
        else if (arg == "--outfile") {
            if (i + 1 < argc) {
                outputFile = argv[++i];
            } else {
                G4cerr << "Error: --outfile requires a path.\n";
                return 1;
            }
        }
        else if (arg == "--primaries") {
            if (i + 1 < argc) {
                nPrimaries = std::stoi(argv[++i]);  // e.g. 10000
            } else {
                G4cerr << "Error: --primaries requires an integer.\n";
                return 1;
            }
        }
        else if (arg == "--seed") {
            if (i + 1 < argc) {
                seed = std::stoi(argv[++i]);
            } else {
                G4cerr << "Error: --seed requires an integer.\n";
                return 1;
            }
        }
        else if (arg == "-i") {
                ui = new G4UIExecutive(argc, argv);
            }
        else if (arg == "-h" || arg == "--help") {
            G4cout
                << "Usage: " << argv[0] << " [options]\n"
                << "  -m, --macro <file>      macro to execute\n"
                << "      --outfile <path>    outfile path (default: ./)\n"
                << "      --primaries <N>     number of primaries / events\n";
            return 0;
        }
        else {
            // Optional: treat bare first arg as macro for backward compatibility
            if (macroFile.empty()) {
                macroFile = argv[i];
            } else {
                G4cerr << "Unknown argument: " << arg << "\n";
                return 1;
            }
        }}

    G4Random::setTheSeed(seed);
    G4cout << "=== RNG seed: " << seed << G4endl;
    G4Random::showEngineStatus();

    // Physics list
    runManager->SetUserInitialization(new SCPhysicsList());

    // Detector construction
    runManager->SetUserInitialization(new SCDetectorConstruction());

    // Action initialization
    runManager->SetUserInitialization(new SCActionInitialization(outputFile));

    //if (argc == 1)
    //{
    //    ui = new G4UIExecutive(argc, argv);
    //}

    G4VisManager *visManager = new G4VisExecutive();
    visManager->Initialize();

    G4UImanager *UImanager = G4UImanager::GetUIpointer();

    if (ui) {
        if (!macroFile.empty()) {
            UImanager->ApplyCommand("/control/execute " + macroFile);
        }

        UImanager->ApplyCommand("/control/execute ../macros/vis.mac");
        ui->SessionStart();
    }
    else
    {
        G4String command = "/control/execute ../macros/";
        //G4String fileName = argv[1];
        UImanager->ApplyCommand(command + macroFile);

        runManager->BeamOn(nPrimaries);
    }

    delete runManager;

    return 0;
}