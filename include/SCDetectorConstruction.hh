#ifndef SCDETECTORCONSTRUCTION_HH   
#define SCDETECTORCONSTRUCTION_HH 1

// Includes
#include "G4VUserDetectorConstruction.hh"
#include "SCSensitiveDetector.hh"
#include <map>

// Forward declarations
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Element;
class G4Isotope;
class G4Material;
class G4GenericMessenger;

class SCDetectorConstruction : public G4VUserDetectorConstruction
{
public:
    SCDetectorConstruction();
    virtual ~SCDetectorConstruction();

    virtual G4VPhysicalVolume* Construct();

    G4double GetGeometryParameter(const char *szParameter);

private:
    void DefineMaterials();
    void ConstructLab();
    void ConstructCube();
    void ConstructHolder();
    void ConstructRoom();

    virtual void ConstructSDandField();

    void ApplyMessengers();
    void DefineGeometryParameters();

    static std::map<G4String, G4double> m_hGeometryParameters;

    G4bool checkOverlaps = true;

    /* Lab */
    G4LogicalVolume*   logic_Lab = nullptr;
    G4VPhysicalVolume* phys_Lab  = nullptr;

    /* Cube */
    G4LogicalVolume*   logic_Cube = nullptr;
    G4VPhysicalVolume* phys_Cube  = nullptr;

    /* Fiducial Cube */
    G4LogicalVolume*   logic_FiducialCube = nullptr;
    G4VPhysicalVolume* phys_FiducialCube  = nullptr;

    /* Plate */
    G4LogicalVolume*   logic_Plate = nullptr;
    G4VPhysicalVolume* phys_Plate  = nullptr;

    /* Source Tube */
    G4LogicalVolume*   logic_SourceTube = nullptr;
    G4VPhysicalVolume* phys_SourceTube  = nullptr;

    /* Floor  */
    G4LogicalVolume*   logic_Floor = nullptr;
    G4VPhysicalVolume* phys_Floor  = nullptr;

    /* == Special Coordinates  ==*/
    G4double zpos_Plate = 0.;

    // Material definitions
    G4Material* Air      = nullptr;
    G4Material* LiF      = nullptr;
    G4Material* enLiF    = nullptr;
    G4Material* PLA      = nullptr;
    G4Material* Concrete = nullptr;

    // Isotop definitions
    G4Isotope* Li7  = nullptr;
    G4Isotope* Li6  = nullptr;
    G4Element* enLi = nullptr;
    G4Element* F19  = nullptr;

    // Messengers
    G4GenericMessenger* fMessengerCube = nullptr;

    G4double xCube = 0.;
    G4double yCube = 0.;
    G4double zCube = 0.;

    G4double Cube_Width  = 0.;
    G4double Cube_Length = 0.;
    G4double Cube_Height = 0.;

    G4double FiducialCube_Width  = 0.;
    G4double FiducialCube_Length = 0.;
    G4double FiducialCube_Height = 0.;

    G4double Li7_percent = 0.;
    G4double Li6_percent = 0.;
};

#endif
