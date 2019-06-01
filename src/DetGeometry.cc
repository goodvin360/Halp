#define GEN
#define PolyBOX


#include "DetGeometry.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4Color.hh"
#include "G4RotationMatrix.hh"
#include "G4Sphere.hh"
#include "G4SubtractionSolid.hh"

DetGeometry::DetGeometry() {

}

DetGeometry::~DetGeometry() {}

G4VPhysicalVolume* DetGeometry::Construct(){

    G4NistManager* nist = G4NistManager::Instance();

    G4double size = 5*m;

    G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");

    G4Box* solidWorld =
      new G4Box("World",
         0.5*size, 0.5*size, 0.5*size);

    G4LogicalVolume* logicWorld =
      new G4LogicalVolume(solidWorld,
                          world_mat,
                          "World");

logicWorld->SetVisAttributes(G4VisAttributes::Invisible);

 G4VPhysicalVolume* physWorld =
      new G4PVPlacement(0,
                        G4ThreeVector(),
                        logicWorld,
                        "World",
                        0,
                        false,
                        0);



// Define meterials (Polyethylene, Boron, oil, insulator etc.)

    G4String name,symbol;
    G4double a, abundance, z, ncomponents,
    izo_density, oil_density,
    Boron_density,
    mix_density,
    temperature, pressure, fractionmass;
    G4int nIsotopes,natoms;
    temperature = 300.*kelvin;
    pressure = 1.*atmosphere;
    Boron_density = 2.34*g/cm3;
    mix_density = 1*g/cm3;
    izo_density = 1.5348*g/cm3;
    oil_density = 0.95*g/cm3;

    //define elements for oil and insulator materials
    a = 1.016*g/mole;
    G4Element*elH = new G4Element(name="Hydrogen", symbol="H", z= 1., a);

    a = 16*g/mole;
    G4Element*elO = new G4Element(name="Oxygen", symbol="O", z=8., a);

    a = 12*g/mole;
    G4Element*elC = new G4Element(name="Carbon", symbol="C", z=6., a);


    //Define isotopes of B
    G4Isotope*B10 = new G4Isotope(name="B10", 5, 10, 10.012*g/mole);
    G4Isotope*B11 = new G4Isotope(name="B11", 5, 11, 11.009*g/mole);


    //Define element  Boron from isotopes
    G4Element* elBoron = new G4Element(name="Boron", symbol="B", nIsotopes=2);
    elBoron->AddIsotope(B10,abundance= 18.5*perCent);
    elBoron->AddIsotope(B11,abundance= 81.5*perCent);

    //Define Boron material

    G4Material*Boron_mat = new G4Material(name="Boron_mat", Boron_density, ncomponents = 1, kStateSolid, temperature, pressure);
    Boron_mat->AddElement(elBoron, natoms = 1);

    //Define Polyethylene
    G4Material*Poly_mat = nist->FindOrBuildMaterial("G4_POLYETHYLENE");


    //Define Boron polyethylene

    G4Material*Mix_mat = new G4Material(name="Mixture", mix_density, ncomponents = 2, kStateSolid, temperature, pressure);

    Mix_mat->AddMaterial(Boron_mat,fractionmass = 3*perCent);

    Mix_mat->AddMaterial(Poly_mat,fractionmass = 97*perCent);


    //define insulator material

    G4Material*insulator = new G4Material(name="Insulator", izo_density, ncomponents = 3);
    insulator->AddElement(elO, natoms = 1164);
    insulator->AddElement(elH, natoms = 2479);
    insulator->AddElement(elC, natoms = 1580);

    //define oil material

    G4Material*oil = new G4Material(name="Oil", oil_density, ncomponents = 2);
    oil->AddElement(elH, natoms = 232);
    oil->AddElement(elC, natoms = 176);

    //define cuprum
    G4Material*cuprum = nist->FindOrBuildMaterial("G4_Cu");

    //define aluminium
    G4Material*aluminium = nist->FindOrBuildMaterial("G4_Al");

    G4double angle = 360*degree;


    //neutron generator housing

#ifdef GEN

    G4ThreeVector cont_pos = G4ThreeVector(0, 0, 0*cm);

    G4Tubs*conteiner = new G4Tubs("gen_front_wall", 0*cm, 8.25*cm, 0.175*cm, 0*angle, 1*angle);

    G4LogicalVolume*logicCont = new G4LogicalVolume(conteiner, aluminium, "gen_front_wall");

    G4VisAttributes*logicContcolour = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0));

    logicCont->SetVisAttributes(logicContcolour);


    new G4PVPlacement(0,
                      cont_pos,
                      logicCont,
                      "gen_front_wall",
                      logicWorld,
                      false,
                      0);


    G4ThreeVector cont_pos1 = G4ThreeVector(0, 0, 10.175*cm);

    G4Tubs*conteiner1 = new G4Tubs("gen_side_wall", 7.9*cm, 8.25*cm, 10*cm, 0*angle, 1*angle);

    G4LogicalVolume*logicCont1 = new G4LogicalVolume(conteiner1, aluminium, "gen_side_wall");

    G4VisAttributes*logicContcolour1 = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0));

    logicCont1->SetVisAttributes(logicContcolour1);


    new G4PVPlacement(0,
                      cont_pos1,
                      logicCont1,
                      "gen_side_wall",
                      logicWorld,
                      false,
                      0);





    G4ThreeVector cont_pos2 = G4ThreeVector(0, 0, 0.425*cm);

    G4Tubs*conteiner2 = new G4Tubs("Insulator", 0*cm, 4.25*cm, 0.25*cm, 0*angle, 1*angle);

    G4LogicalVolume*logicCont2 = new G4LogicalVolume(conteiner2, insulator, "Insulator");

    G4VisAttributes*logicContcolour2 = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));

    logicCont2->SetVisAttributes(logicContcolour2);

    new G4PVPlacement(0,
                      cont_pos2,
                      logicCont2,
                      "InsulatorL",
                      logicWorld,
                      false,
                      0);



    G4ThreeVector cont_pos3 = G4ThreeVector(0, 0, 0.75*cm);

    G4Tubs*conteiner3 = new G4Tubs("gen_in_front_wall", 0*cm, 4.25*cm, 0.075*cm, 0*angle, 1*angle);

    G4LogicalVolume*logicCont3 = new G4LogicalVolume(conteiner3, aluminium, "gen_in_front_wall");

    G4VisAttributes*logicContcolour3 = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0));

    logicCont3->SetVisAttributes(logicContcolour3);


    new G4PVPlacement(0,
                      cont_pos3,
                      logicCont3,
                      "gen_in_front_wall",
                      logicWorld,
                      false,
                      0);




    G4ThreeVector cont_pos4 = G4ThreeVector(0, 0, 0.975*cm);

    G4Tubs*conteiner4 = new G4Tubs("Cuprum_template", 0*cm, 1.1325*cm, 0.15*cm, 0*angle, 1*angle);

    G4LogicalVolume*logicCont4 = new G4LogicalVolume(conteiner4, cuprum, "Cuprum_template");

    G4VisAttributes*logicContcolour4 = new G4VisAttributes(G4Colour(1.0, 0.0, 1.0));

    logicCont4->SetVisAttributes(logicContcolour4);


    new G4PVPlacement(0,
                      cont_pos4,
                      logicCont4,
                      "Cuprum_template",
                      logicWorld,
                      false,
                      0);


    G4ThreeVector cont_pos5 = G4ThreeVector(0, 0, 10.175*cm);

    G4Tubs*conteiner5 = new G4Tubs("Oil", 6.25*cm, 7.9*cm, 10*cm, 0*angle, 1*angle);

    G4LogicalVolume*logicCont5 = new G4LogicalVolume(conteiner5, oil, "Oil");

    G4VisAttributes*logicContcolour5 = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0));

    logicCont5->SetVisAttributes(logicContcolour5);


    new G4PVPlacement(0,
                      cont_pos5,
                      logicCont5,
                      "Oil",
                      logicWorld,
                      false,
                      0);


    G4ThreeVector cont_pos6 = G4ThreeVector(0, 0, 10.325*cm);

    G4Tubs*conteiner6 = new G4Tubs("gen_in_side_wall", 3.5*cm, 4.25*cm, 9.5*cm, 0*angle, 1*angle);

    G4LogicalVolume*logicCont6 = new G4LogicalVolume(conteiner6, aluminium, "gen_in_side_wall");

    G4VisAttributes*logicContcolour6 = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0));

    logicCont6->SetVisAttributes(logicContcolour6);


    new G4PVPlacement(0,
                      cont_pos6,
                      logicCont6,
                      "gen_in_side_wall",
                      logicWorld,
                      false,
                      0);


    G4ThreeVector cont_pos7 = G4ThreeVector(0, 0, 5.325*cm);

    G4Tubs*conteiner7 = new G4Tubs("Cuprum_cup", 3.25*cm, 3.5*cm, 4.5*cm, 0*angle, 1*angle);

    G4LogicalVolume*logicCont7 = new G4LogicalVolume(conteiner7, cuprum, "Cuprum_cup");

    G4VisAttributes*logicContcolour7 = new G4VisAttributes(G4Colour(1.0, 0.0, 1.0));

    logicCont7->SetVisAttributes(logicContcolour7);


    new G4PVPlacement(0,
                      cont_pos7,
                      logicCont7,
                      "Cuprum_cup",
                      logicWorld,
                      false,
                      0);



    G4ThreeVector cont_pos8 = G4ThreeVector(0, 0, 1.075*cm);

    G4Tubs*conteiner8 = new G4Tubs("Oil2", 1.135*cm, 3.25*cm, 0.25*cm, 0*angle, 1*angle);

    G4LogicalVolume*logicCont8 = new G4LogicalVolume(conteiner8, oil, "Oil2");

    G4VisAttributes*logicContcolour8 = new G4VisAttributes(G4Colour(0.0, 1.0, 1.0));

    logicCont8->SetVisAttributes(logicContcolour8);


    new G4PVPlacement(0,
                      cont_pos8,
                      logicCont8,
                      "Oil2",
                      logicWorld,
                      false,
                      0);

    G4ThreeVector cont_pos9 = G4ThreeVector(0, 0, 20.35*cm);

    G4Tubs*conteiner9 = new G4Tubs("gen_back_wall", 0*cm, 8.25*cm, 0.175*cm, 0*angle, 1*angle);

    G4LogicalVolume*logicCont9 = new G4LogicalVolume(conteiner9, aluminium, "gen_back_wall");

    G4VisAttributes*logicContcolour9 = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0));

    logicCont9->SetVisAttributes(logicContcolour9);


    new G4PVPlacement(0,
                      cont_pos9,
                      logicCont9,
                      "gen_back_wall",
                      logicWorld,
                      false,
                      0);

#endif

//polyethylene box

#ifdef PolyBOX


    G4ThreeVector sub1_pos = G4ThreeVector(0*cm, 0*cm, 10*cm);

    G4Box*box0 = new G4Box("Box0", 11*cm, 11*cm, 40*cm);

    G4Box*box1 = new G4Box("Box1", 51*cm, 51*cm, 80*cm);

    G4SubtractionSolid*Sub1 = new G4SubtractionSolid ("Sub1", box1, box0);
    G4LogicalVolume*logicSub1 = new G4LogicalVolume(Sub1, Poly_mat, "Sub1");
    G4VisAttributes*logicSub1colour = new G4VisAttributes(G4Colour(0.0, 1.0, 1.0));
    logicSub1->SetVisAttributes(logicSub1colour);


    new G4PVPlacement(0,
                      sub1_pos,
                      logicSub1,
                      "Sub1",
                      logicWorld,
                      false,
                      0);

#endif



    return physWorld;
}
