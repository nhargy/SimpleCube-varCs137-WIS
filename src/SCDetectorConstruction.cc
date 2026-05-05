#include "SCDetectorConstruction.hh"

#include "globals.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include <G4UnionSolid.hh>
#include <G4IntersectionSolid.hh>
#include <G4SubtractionSolid.hh>

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Isotope.hh"
#include "G4Element.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4GenericMessenger.hh"

std::map<G4String, G4double> SCDetectorConstruction::m_hGeometryParameters;

SCDetectorConstruction::SCDetectorConstruction()
{
    ApplyMessengers();
}

SCDetectorConstruction::~SCDetectorConstruction()
{
}

void SCDetectorConstruction::ApplyMessengers()
{
    fMessengerCube = new G4GenericMessenger(this, "/cube/", "cube");

    fMessengerCube->DeclareProperty( "xCube", xCube, "xCube");
    fMessengerCube->DeclareProperty("yCube", yCube, "yCube");
    fMessengerCube->DeclareProperty("zCube", zCube, "zCube");
    fMessengerCube->DeclareProperty("Cube_Width", Cube_Width, "Cube_Width");
    fMessengerCube->DeclareProperty("Cube_Length", Cube_Length, "Cube_Length");
    fMessengerCube->DeclareProperty("Cube_Height", Cube_Height, "Cube_Height");
    fMessengerCube->DeclareProperty("FiducialCube_Width", FiducialCube_Width, "CubFiducialCube_Widthe_Width");
    fMessengerCube->DeclareProperty("FiducialCube_Length", FiducialCube_Length, "FiducialCube_Length");
    fMessengerCube->DeclareProperty("FiducialCube_Height", FiducialCube_Height, "FiducialCube_Height");
    fMessengerCube->DeclareProperty("Li7_percent", Li7_percent, "Li7_percent");
    fMessengerCube->DeclareProperty("Li6_percent", Li6_percent, "Li6_percent");

    // Default Values
    xCube = 40.;
    yCube = 0.;
    zCube = 6.;

    Cube_Width  = 10.;
    Cube_Length = 10.;
    Cube_Height = 10.;

    FiducialCube_Width  = 8.;
    FiducialCube_Length = 8.;
    FiducialCube_Height = 8.;

    // Natural abundance ~ Li7 (92.5%), Li6 (7.5%)
    Li7_percent = 92.5;
    Li6_percent = 7.5;
}

G4double SCDetectorConstruction::GetGeometryParameter(const char *szParameter)
{
  if (m_hGeometryParameters.find(szParameter) != m_hGeometryParameters.end()){
    return m_hGeometryParameters[szParameter];
  }
  else {
    G4cout<< "Parameter: " << szParameter << " is not defined!!!!!" << G4endl;
    return 0;
  }
}

void SCDetectorConstruction::DefineGeometryParameters()
{
     //=========================== Tolerances =================================
    m_hGeometryParameters["kTol"] = 0.1 *mm;

    //============================ Laboratory =================================
    m_hGeometryParameters["Lab_Width"]  = 1000. *mm;
    m_hGeometryParameters["Lab_Length"] = 1000. *mm;
    m_hGeometryParameters["Lab_Height"] = 1000. *mm;

    //============================== Holder ===================================
    m_hGeometryParameters["Plate_oD"]        = 100. *mm;
    m_hGeometryParameters["Plate_iD"]        =  12. *mm;
    m_hGeometryParameters["Plate_Thickness"] =   4. *mm;

    m_hGeometryParameters["SourceTube_oD"]     = 11. *mm;
    m_hGeometryParameters["SourceTube_iD"]     = 7. *mm;
    m_hGeometryParameters["SourceTube_Height"] = 15. *mm;

    //============================== Floor ====================================
    m_hGeometryParameters["Floor_Width"]     = 999. *mm;
    m_hGeometryParameters["Floor_Length"]    = 999. *mm;
    m_hGeometryParameters["Floor_Thickness"] =  50. *mm;

    //============================ Pb-Shielding ===============================
    m_hGeometryParameters["PbBox_Width"]     = 290. *mm;
    m_hGeometryParameters["PbBox_Length"]    = 260. *mm;
    m_hGeometryParameters["PbBox_Height"]    = 200. *mm;
    m_hGeometryParameters["PbBox_Thickness"] =  50. *mm;

    m_hGeometryParameters["PbTube_oD"]     = 120. *mm;
    m_hGeometryParameters["PbTube_iD"]     = 115. *mm;
    m_hGeometryParameters["PbTube_Height"] = 100. *mm;
}

void SCDetectorConstruction::DefineMaterials()
{
    G4NistManager *nist = G4NistManager::Instance();

    // --- Air ----------------------------------------------------------------
    Air = nist->FindOrBuildMaterial("G4_AIR");

    // --- Lithium Fluoride ---------------------------------------------------

    // Lithium Isotopes
    Li7 = new G4Isotope(
        "Li7",
        3,
        7,
        7.016 * g/mole
    );

    Li6 = new G4Isotope(
        "Li6",
        3,
        6,
        6.015 * g/mole
    );

    enLi = new G4Element(
        "enriched Li", 
        "enLi", 
        2
    );
    enLi->AddIsotope(Li7, Li7_percent * perCent);
    enLi->AddIsotope(Li6, Li6_percent * perCent);

    // Fluorine
    F19 = nist->FindOrBuildElement("F");

    // https://physics.nist.gov/cgi-bin/Star/compos.pl?matno=185
    G4double LiF_density = 2.635 * g/cm3;
    enLiF = new G4Material("enriched LiF", LiF_density, 2);

    enLiF->AddElement(enLi, 1);  // 1 Li atom
    enLiF->AddElement(F19,  1);  // 1 F atom

    // --- Concrete -----------------------------------------------------------
    Concrete = nist->FindOrBuildMaterial("G4_CONCRETE");

    // --- Polyactic Acid (PLA) -----------------------------------------------
    G4Element* C = nist->FindOrBuildElement("C");
    G4Element* H = nist->FindOrBuildElement("H");
    G4Element* O = nist->FindOrBuildElement("O");

    G4double PLA_density = 1.25 * g/cm3;
    G4int ncomponents = 3;

    PLA = new G4Material("PLA", PLA_density, ncomponents);
    PLA->AddElement(C, 3);
    PLA->AddElement(H, 4);
    PLA->AddElement(O, 2);
}


// **************************************************************************//
// *** Start Construction ***************************************************//
// **************************************************************************//

G4VPhysicalVolume* SCDetectorConstruction::Construct()
{
    DefineMaterials();
    DefineGeometryParameters();
    ConstructLab();
    ConstructCube();
    ConstructHolder();
    ConstructRoom();

    return phys_Lab;
}

void SCDetectorConstruction::ConstructLab()
{
    G4double Lab_Width  = GetGeometryParameter("Lab_Width");
    G4double Lab_Length = GetGeometryParameter("Lab_Length");
    G4double Lab_Height = GetGeometryParameter("Lab_Height");

    G4Box* solid_Lab = new G4Box(
        "solid_Lab", 
        Lab_Width/2, 
        Lab_Length/2, 
        Lab_Height/2
    );

    logic_Lab = new G4LogicalVolume(
        solid_Lab, 
        Air, 
        "logic_Lab"
    );

    auto vector_Lab = G4ThreeVector(0., 0., 0.);
    phys_Lab = new G4PVPlacement(
        0, 
        vector_Lab, 
        logic_Lab, 
        "phys_Lab", 
        nullptr, 
        false, 
        0, 
        checkOverlaps
    );
}

void SCDetectorConstruction::ConstructCube()
{
    G4Box* solid_Cube = new G4Box(
        "solid_Cube", 
        Cube_Width/2, 
        Cube_Length/2, 
        Cube_Height/2
    );

    logic_Cube = new G4LogicalVolume(
        solid_Cube, 
        enLiF, 
        "logic_Cube"
    );

    auto vector_Cube = G4ThreeVector(xCube *mm, yCube *mm, zCube *mm);
    phys_Cube = new G4PVPlacement(
        0, 
        vector_Cube, 
        logic_Cube, 
        "phys_Cube", 
        logic_Lab, 
        false, 
        0, 
        checkOverlaps
    );

    /* VisAttributes */
    auto colour_Cube = G4Colour(0, 1., 1., 1.);
    G4VisAttributes *vis_Cube = new G4VisAttributes(colour_Cube);
    vis_Cube   ->SetVisibility(true);
    vis_Cube   ->SetForceWireframe(true);
    vis_Cube   ->SetForceAuxEdgeVisible(true);
    logic_Cube ->SetVisAttributes(vis_Cube);

    G4Box* solid_FiducialCube = new G4Box(
        "solid_FiducialCube", 
        FiducialCube_Width/2, 
        FiducialCube_Length/2, 
        FiducialCube_Height/2
    );

    logic_FiducialCube = new G4LogicalVolume(
        solid_FiducialCube, 
        enLiF, 
        "logic_FiducialCube"
    );

    auto vector_FiducialCube = G4ThreeVector(0., 0., 0.);
    phys_FiducialCube = new G4PVPlacement(
        0, 
        vector_FiducialCube, 
        logic_FiducialCube, 
        "phys_Cube", 
        logic_Cube, 
        false, 
        0, 
        checkOverlaps
    );

    /* VisAttributes */
    auto colour_FiducialCube = G4Colour(Li7_percent/100, 0., Li6_percent/100);
    G4VisAttributes *vis_FiducialCube = new G4VisAttributes(colour_FiducialCube);
    vis_FiducialCube   ->SetVisibility(true);
    vis_FiducialCube   ->SetForceSolid(true);
    vis_FiducialCube   ->SetForceAuxEdgeVisible(true);
    logic_FiducialCube ->SetVisAttributes(vis_FiducialCube);
}

void SCDetectorConstruction::ConstructHolder()
{
    // --- Plate --------------------------------------------------------------
    G4double Plate_oD         = GetGeometryParameter("Plate_oD");
    G4double Plate_iD         = GetGeometryParameter("Plate_iD");
    G4double Plate_Thickness  = GetGeometryParameter("Plate_Thickness");

    G4Tubs* solid_Plate = new G4Tubs(
        "solid_Plate",
        Plate_iD/2,
        Plate_oD/2,
        Plate_Thickness/2,
        0.  *deg,
        360.*deg
    );

    logic_Plate = new G4LogicalVolume(
        solid_Plate,
        PLA,
        "logic_Plate"
    );

    zpos_Plate = zCube - Cube_Height/2 - Plate_Thickness/2;
    auto vector_Plate = G4ThreeVector(0., 0., zpos_Plate);

    phys_Plate = new G4PVPlacement(
        0, 
        vector_Plate, 
        logic_Plate, 
        "phys_Plate", 
        logic_Lab, 
        false, 
        0, 
        checkOverlaps
    );

    /* -- VisAttributes ---------------------------------------------------- */
    auto colour_PLA = G4Colour(1., 0., 1.); // magenta
    G4VisAttributes *vis_PLA = new G4VisAttributes(colour_PLA);
    vis_PLA   ->SetVisibility(true);
    vis_PLA   ->SetForceSolid(true);
    vis_PLA   ->SetForceAuxEdgeVisible(true);

    logic_Plate      ->SetVisAttributes(vis_PLA);
}

void SCDetectorConstruction::ConstructRoom()
{
    // --- Floor --------------------------------------------------------------
    G4double Floor_Width     = GetGeometryParameter("Floor_Width");
    G4double Floor_Length    = GetGeometryParameter("Floor_Length");
    G4double Floor_Thickness = GetGeometryParameter("Floor_Thickness");

    G4Box* solid_Floor = new G4Box(
        "solid_Floor", 
        Floor_Width/2, 
        Floor_Length/2, 
        Floor_Thickness/2
    );

    logic_Floor = new G4LogicalVolume(
        solid_Floor, 
        Concrete, 
        "logic_Floor"
    );

    G4double Plate_Thickness  = GetGeometryParameter("Plate_Thickness");
    G4double zpos_Floor = zpos_Plate - Plate_Thickness/2 - Floor_Thickness/2;
    auto vector_Floor = G4ThreeVector(0. , 0., zpos_Floor);
    phys_Floor = new G4PVPlacement(
        0, 
        vector_Floor, 
        logic_Floor, 
        "phys_Floor", 
        logic_Lab, 
        false, 
        0, 
        checkOverlaps
    );

    /* VisAttributes */
    auto colour_Floor = G4Colour(0.3, 0.3, 0.3, 0.7);
    G4VisAttributes *vis_Floor = new G4VisAttributes(colour_Floor);
    vis_Floor   ->SetVisibility(true);
    vis_Floor   ->SetForceSolid(true);
    vis_Floor   ->SetForceAuxEdgeVisible(true);
    logic_Floor ->SetVisAttributes(vis_Floor);

}

void SCDetectorConstruction::ConstructSDandField()
{
    SCSensitiveDetector *sensDet = new SCSensitiveDetector("SensitveDetector");
    logic_FiducialCube->SetSensitiveDetector(sensDet);
}
