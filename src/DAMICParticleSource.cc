#include <cmath>

#include "DAMICVolumeCoordinateStore.hh"
#include "DAMICParticleSource.hh"
#include "SkipperAnalysis.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4PrimaryParticle.hh"
#include "G4Event.hh"
#include "Randomize.hh"
#include "G4TransportationManager.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4IonTable.hh"
#include "G4Ions.hh"
#include "G4TrackingManager.hh"
#include "G4Track.hh"
#include "G4Material.hh"


DAMICParticleSource::DAMICParticleSource(){

      ParticleEnergy= 0.0* MeV;
      ParticleCharge = 0.0;
      ParticleMomentumDirection = G4ParticleMomentum(1.,0.,0.);
      ParticleTime = 0.0;
      ParticleDefinition = NULL;
      ParticlePosition = G4ThreeVector(0.,0.,0.0);

      //Position
      bSurface = false; //True on surface
      bVolume = false; //True if Volume
      bMaterial = false;
      bSource = true;
      bShape = false;
      VolumeSource = "Volume"; // choose between Volume and Material
      MotherVolume = "MotherVolume";
      Material = "Material";
      MaterialSource = "MaterialSource";
      Shape = "Para";
      VolumesUse = {};
      VolumesUseMass = {};
      VolumesConcentration = {};
      Proba = {};
      PositionNum = {};
      EnergyNum = {};
      DirectionNum = {};
      changes = false;

      //Energy
      DistriNRJ = "Uniform";
      bMonoNRJ = true;
      bDistriNRJ = false;

      // Direction

      DistriD = "Isotropic";
      bDistriD = false;
      bOneD = true;

      // Tritium

      CalculatedMaxTri = false;
      MaxTritium = 0;

      //Navigator for Geometry
      gNavigator = G4TransportationManager::GetTransportationManager()
    ->GetNavigatorForTracking();

    //Messenger
    Messenger = new DAMICParticleSourceMessenger(this);
		CoordinateStore = new DAMICVolumeCoordinateStore(G4PhysicalVolumeStore::GetInstance());
}

DAMICParticleSource::~DAMICParticleSource(){
  delete Messenger;
	delete CoordinateStore;
}

void DAMICParticleSource::SetParticleEnergy(G4double Energy){
  ParticleEnergy = Energy;
}

void DAMICParticleSource::SetParticleCharge(G4double Charge){
  ParticleCharge = Charge;
}

void DAMICParticleSource::SetParticleMomentumDirection(G4ParticleMomentum Momentum){
  ParticleMomentumDirection = Momentum;
}

void DAMICParticleSource::SetParticleTime(G4double Time){
  ParticleTime = Time;
}

void DAMICParticleSource::SetParticleDefinition(G4ParticleDefinition* Def){
  ParticleDefinition = Def;
  G4double charge = ParticleDefinition->GetPDGCharge();
  SetParticleCharge(charge);
}

void DAMICParticleSource::SetParticlePosition(G4ThreeVector Pos){
  ParticlePosition = Pos;
}



//SETS POSITIONS STRING VECTORS AND RESET --- -- --- --- --



void DAMICParticleSource::SetMaterial(G4String Mat){
  Material = Mat;
}

void DAMICParticleSource::SetVolumeSource(G4String Vol){
  VolumeSource = Vol;
}

void DAMICParticleSource::SetMaterialSource(G4String MatSource){
  Material = MatSource;
}

void DAMICParticleSource::SetMotherVolume(G4String MotherVol){
  MotherVolume = MotherVol;
}

void DAMICParticleSource::SetSourceShape(G4String ShapeSrc){
  Shape = ShapeSrc;
}

void DAMICParticleSource:: SetPositionNumSize(G4int size){
  PositionNum = std::vector<G4double> (size,0);
}

void DAMICParticleSource:: SetPositionNumValue(G4double val, G4int id){
  PositionNum[id] = val;
}

void DAMICParticleSource:: ResetPositionNum(){
  PositionNum = {};
}




//SETS POSITION FUNCTION BOOL ----------------------------

void DAMICParticleSource::SetSurface(G4bool Sur){
  bSurface = Sur;
}

void DAMICParticleSource::SetVolume(G4bool Vol){
  bVolume = Vol;
}

void DAMICParticleSource::SetMaterial(G4bool Mat){
  bMaterial = Mat;
}

void DAMICParticleSource::SetSource(G4bool Sour){
  bSource = Sour;
}

void DAMICParticleSource::SetShape(G4bool Shap){
  bShape = Shap;
}

//Energy Sets and Reset

void DAMICParticleSource::SetDistriNRJ(G4bool bDistri, G4String Distri ){
  bDistriNRJ = bDistri;
  DistriNRJ = Distri;
}

void DAMICParticleSource::SetMonoNRJ(G4bool Mono){
  bMonoNRJ = Mono;
}

void DAMICParticleSource::SetEnergyNumSize(G4int size){
  EnergyNum = std::vector<G4double> (size,0);
}

void DAMICParticleSource::SetEnergyNumValue(G4double val, G4int id){
  EnergyNum[id] = val;
}

void DAMICParticleSource::ResetEnergyNum(){
  EnergyNum = {};
}

// Direction Sets and Reset

void DAMICParticleSource::SetDistriD(G4bool bDistri, G4String Distri){
  bDistriD = bDistri;
  DistriD = Distri;
}

void DAMICParticleSource::SetOneD(G4bool one){
  bOneD = one;
}

void DAMICParticleSource::SetDirectionNumSize(G4int size){
  DirectionNum = std::vector<G4double>  (size,0);
  G4cout << DirectionNum.size() << G4endl;
}

void DAMICParticleSource::SetDirectionNumValue(G4double val, G4int id){
  DirectionNum[id] = val;
}

void DAMICParticleSource::ResetDirectionNum(){
  DirectionNum = {};
}



//IS FUNCTIONS Position --------------------------


G4bool DAMICParticleSource::IsSurface(){
  return bSurface;
}

G4bool DAMICParticleSource::IsVolume(){
  return bVolume;
}

G4bool DAMICParticleSource::IsMaterial(){
  return bMaterial;
}

G4bool DAMICParticleSource::IsSource(){
  return bSource;
}

G4bool DAMICParticleSource::IsShape(){
  return bShape;
}

// Is Functions NRJ --------------------------

G4bool DAMICParticleSource::IsMonoNRJ(){
  return bMonoNRJ;
}

G4bool DAMICParticleSource::IsDistriNRJ(){
  return bDistriNRJ;
}

// Is Function Distri
G4bool DAMICParticleSource::IsOneD(){
  return bOneD;
}

G4bool DAMICParticleSource:: IsDistriD(){
  return bDistriD;
}


// DO FUNCTIONS Position --------------------

void DAMICParticleSource::DoVolume(){
  SetMaterial(false);
  SetVolume(true);
  SetSource(false);
  SetShape(false);
}

void DAMICParticleSource::DoMaterial(){
  SetMaterial(true);
  SetVolume(false);
  SetSource(false);
  SetShape(false);
}

void DAMICParticleSource::DoSource(){
  SetMaterial(false);
  SetVolume(false);
  SetSource(true);
  SetShape(false);
}

void DAMICParticleSource::DoShape(G4String sha){
  SetMaterial(false);
  SetVolume(false);
  SetSource(false);
  SetShape(true);
  SetSourceShape(sha);
}

void DAMICParticleSource::DoSurface() {
  SetMaterial(false);
  SetVolume(false);
  SetShape(false);
  SetSurface(true);
  SetSource(false);
}

// ENERGY DO  ----------------------------

void DAMICParticleSource::DoMonoNRJ(){
  SetMonoNRJ(true);
  SetDistriNRJ(false, "");

}

void DAMICParticleSource::DoDistriNRJ(G4String Distri){
  SetMonoNRJ(false);
  SetDistriNRJ(true, Distri);
}

//Direction DO --------------------------------

void DAMICParticleSource::DoOneD(){
  SetOneD(true);
  SetDistriD(false, "");
}

void DAMICParticleSource::DoDistriD(G4String distri){
  SetOneD(false);
  SetDistriD(true, distri);
}

// ADD VOLUME FOR TON OF VOLUMES ------------------



void DAMICParticleSource::AddVolume(G4String Volume, G4double Concentration){

	CoordinateStore->AddVolume(Volume, Concentration);
  G4PhysicalVolumeStore* PVStore = G4PhysicalVolumeStore::GetInstance();
  G4int i = 0;
  G4bool found = false;
  G4double Mass;
  G4double Dens;
  G4VPhysicalVolume* tempPV;
  while (!found && i<(G4int)PVStore->size())
    {
      tempPV = (*PVStore)[i];
      found  = tempPV->GetName() == Volume;

      if (!found){i++;}
    }


  if(found == true) {
    Mass = tempPV->GetLogicalVolume()->GetMass();
    G4cout << tempPV->GetName() << " and " << Mass << G4endl;
  }
  else {
    G4cout << " **** Error: Volume does not exist **** " << G4endl;
    G4cout << " Ignoring confine condition" << G4endl;

    Mass = 0;
  }
  G4int sizeV= VolumesUse.size();
  G4bool same = false;
  for (G4int j = 0;j<sizeV;j++){
    if (VolumesUse[j]==Volume)
    {
      same = true;
    }
  }

  if (!same){
    VolumesUse.push_back(Volume);
    VolumesUseMass.push_back(Mass);
    VolumesConcentration.push_back(Concentration);
    changes = true;
  }


  VolumesConcentration.push_back(Concentration);
}

void DAMICParticleSource::CalculProba()
{
  //G4cout << " je suis calcProba" << G4endl;
  if (changes){
    //G4cout << "changes" << G4endl;
    Proba={};
    G4double total = 0;
    G4int sizevols = VolumesUse.size();
    //G4cout << sizevols << G4endl;
    for (G4int i = 0; i <sizevols; i++){
      G4double temp = VolumesConcentration[i]*VolumesUseMass[i];
      total += temp;
      Proba.push_back(temp);
    }

    for (G4int i =0; i <sizevols; i++){
      Proba[i] = Proba[i]/total;
      //G4cout << Proba[i] << G4endl;
    }
    changes = false;
    for (G4int i =1; i <sizevols;i++){
      Proba[i] += Proba[i-1];
      //G4cout << Proba[i] << G4endl;
    }
  }
}

void DAMICParticleSource::ChooseVolume()
{
  //G4cout << " je suis chovol" << G4endl;
  G4double tryVol = 1*G4UniformRand();
  G4bool found = false;
  G4int i = 0;
  while(!found){
    if (tryVol <= Proba[i]){
      //G4cout << VolumesUse[i] << G4endl;

      found = true;
      VolumeSource = VolumesUse[i];
    }
    i++;
  }
}


//MATERIAL COORDINATES----------------
void DAMICParticleSource::MaterialCoordinates(){
  CalculPosition(MotherVolume, Material, "NULL");

}


//VOLUME COORDINATES FUNCTION CHECK

void DAMICParticleSource::VolumeCoordinates(){
  G4VPhysicalVolume *tempPV      = NULL;
  G4PhysicalVolumeStore *PVStore = 0;
  G4String theRequiredVolumeName = VolumeSource;
  PVStore = G4PhysicalVolumeStore::GetInstance();

  G4int i = 0;
  G4bool found = false;
  while (!found && i<(G4int)PVStore->size())
    {
      tempPV = (*PVStore)[i];
      found  = tempPV->GetName() == theRequiredVolumeName;

      if (!found){i++;}
    }


  if(found == true) {
      SetMotherVolume(tempPV->GetMotherLogical()->GetName());
      SetMaterial(tempPV->GetLogicalVolume()->GetMaterial()->GetName());
      CalculPosition(MotherVolume, Material, VolumeSource);
  }
  else {
    G4cout << " **** Error: Volume does not exist **** " << G4endl;
    G4cout << " Ignoring confine condition" << G4endl;
    VolumeSource = "NULL";

  }
}


//SURFACE POSITION FUNCTION ---------
void DAMICParticleSource::SurfaceCoordinates() {
  G4ThreeVector Pos = CoordinateStore->GetPointOnSurface();
  SetParticlePosition(Pos);
}



// SOURCE POSITION FUNCTION -------------


void DAMICParticleSource::SourceCoordinates(){
  G4ThreeVector Pos = G4ThreeVector(PositionNum[0], PositionNum[1], PositionNum[2]);
  SetParticlePosition(Pos);
}



//HIT AND MISS FOR VOLUME MATERIAL -----------



void DAMICParticleSource::CalculPosition(G4String MotherUse, G4String MaterialUse, G4String VolumeUse){

  G4double posx = 0;
  G4double posy = 0;
  G4double posz = 0;
  G4bool found = false;
  G4VPhysicalVolume* theVolume;
  G4String theVolName;
  G4String theMaterialName;
  G4bool MaterialFit;
  G4bool VolumeFit;
  while(!found){

    if (MotherUse == "WorldLV" ){
      posx = 2.15*m*G4UniformRand()-1.075*m;
      posy = 2.2*m*G4UniformRand()-1.1*m;
      posz = 2.2*m*G4UniformRand()-1.1*m;
    }

    else if (MotherUse == "VesselLV" || MotherUse =="Assembly1"|| MotherUse =="Assembly2"){

      posx = 2.0*m*G4UniformRand()-1*m;
      posy = 2.0*m*G4UniformRand()-1*m;
      posz = 2.0*m*G4UniformRand()-1*m;

    }
    else if (MotherUse == "EmptyBox"){
      posx = 304.8*mm*G4UniformRand()-304.8/2*mm;
      posy = 304.8*mm*G4UniformRand()-304.8/2*mm;
      posz = 482.6*mm*G4UniformRand()-482.6/2*mm-186.4*mm;
    }
    else if (MotherUse == "PrinTubLV"){
      G4double RadiusRan = 191.287/2*mm * G4UniformRand();
      G4double AngleRan = M_PI*2*G4UniformRand();
      G4double HeightRan = 413.512*mm *(G4UniformRand()-0.5);
      posx = RadiusRan* cos(AngleRan);
      posy = RadiusRan* sin(AngleRan);
      posz = -120*mm+ HeightRan;
    }
    else {
      G4double tempX = 120*mm*(G4UniformRand()-0.5);
      G4double tempY = 120*mm*(G4UniformRand()-0.5);
      G4double tempZ = 160*mm*(G4UniformRand()-0.5);
      posx = tempX;
      posy = tempY;
      posz = tempZ -120*mm - 121.5*mm;
    }
    G4ThreeVector *ptr;
    theVolume=gNavigator->LocateGlobalPointAndSetup(G4ThreeVector(posx,posy, posz),ptr,true);
    theVolName = theVolume->GetName();
    theMaterialName = theVolume->GetLogicalVolume()->GetMaterial()->GetName();
    MaterialFit = theMaterialName == MaterialUse;
    VolumeFit = theVolName == VolumeUse;
    if (MaterialFit && IsMaterial()){
      found = true;
    }
    if (VolumeFit && IsVolume()){
      found = true;
    }
  }
  SetParticlePosition(G4ThreeVector(posx, posy, posz));
}




// SHAPE COORDINATES ------------------



void DAMICParticleSource::ShapeCoordinates(){
  G4double X;
  G4double Y;
  G4double Z;
  if (Shape == "Para"){
    G4ThreeVector Center = G4ThreeVector(PositionNum[0], PositionNum[1], PositionNum[2]);
    G4ThreeVector Sides = G4ThreeVector(PositionNum[3], PositionNum[4], PositionNum[5]);
    X = Center.getX()+ Sides.getX()*(G4UniformRand()-0.5);
    Y = Center.getY()+ Sides.getY()*(G4UniformRand()-0.5);
    Z = Center.getZ()+ Sides.getZ()*(G4UniformRand()-0.5);
  }
  else if (Shape == "Sphere"){
    G4ThreeVector Center = G4ThreeVector(PositionNum[0], PositionNum[1], PositionNum[2]);
    G4double Radius = PositionNum[3];
    G4double cosTeta = 2*(G4UniformRand()-0.5);
    G4double sinTeta = sqrt(1-cosTeta*cosTeta);
    G4double AnglePhi = M_PI*2*G4UniformRand();
    G4double Rad = Radius*G4UniformRand();
    X = Rad*sin(AnglePhi)*cosTeta+ Center.getX();
    Y = Rad*sin(AnglePhi)*sinTeta+ Center.getY();
    Z = Rad*cos(AnglePhi)+ Center.getZ();

  }
  else if (Shape == "Cyl"){
    G4ThreeVector Center = G4ThreeVector(PositionNum[0], PositionNum[1], PositionNum[2]);
    G4double Radius = PositionNum[3];
    G4double Height = PositionNum[4];
    G4double Rad = Radius*G4UniformRand();
    G4double AngleTeta = M_PI*2*G4UniformRand();
    X = Center.getX() + Rad*cos(AngleTeta);
    Y = Center.getY()+ Rad * sin(AngleTeta);
    Z =Center.getZ() + Height *(G4UniformRand()-0.5);
  }
  else{
    G4cout << " *****This Shape is not defined***** " << G4endl;
    X = 0;
    Y = 0;
    Z = 0;
  }

  SetParticlePosition(G4ThreeVector(X,Y,Z));
}



// ENERGY VALUES

void DAMICParticleSource::EnergyValue(){
  if (IsMonoNRJ()){
    SetParticleEnergy(EnergyNum[0]);
  }
  if (IsDistriNRJ()){
    //G4cout << "test 2 Energy" << G4endl;
    G4double EnergyRand = 0;
    if (DistriNRJ == "Uniform"){
      EnergyRand = G4UniformRand() * (EnergyNum[1]-EnergyNum[0]) + EnergyNum[0];
    }
    if (DistriNRJ == "Tritium"){
      G4double endpoint = 18580;
      if (!CalculatedMaxTri){
        G4double max = 0;

        for (int i = 0; i < endpoint; i+=10){
          G4double tempMax = SpectrumBeta(i);
          if (tempMax > max){
            max = tempMax;
          }
        }
        CalculatedMaxTri = true;
        MaxTritium = max;
      }

      G4bool doneFind = false;
      while (!doneFind){
        G4double tempRand1 = G4UniformRand() * (endpoint-1) +1;
        G4double relation = SpectrumBeta(tempRand1)/MaxTritium;
        G4double tempRand2 = G4UniformRand();
        if ( tempRand2 < relation){
          doneFind = true;
        }
        EnergyRand = tempRand1*eV;

      }
    }
    SetParticleEnergy(EnergyRand);
  }

}

G4double DAMICParticleSource::SpectrumBeta(G4double En){
  G4double Q = 18580;
  G4double E = En;
  G4double elecMass = 511000;
  G4double FirstPart = sqrt(E*E+2*E*elecMass);
  G4double SecondPart = (Q-E)*(Q-E)*(E+elecMass);

  G4double Z = 1;
  G4double alpha = 7.2973525664 * 0.001;
  G4double nu = alpha*Z*(E+elecMass)/(FirstPart);

  G4double FermiFunction = 2*3.14*nu/(1-exp(-2*3.14*nu));

  G4double result = FirstPart*SecondPart*FermiFunction;

  return result;
}

// DIRECTION VALUE

void DAMICParticleSource::DirectionValue(){
  if (IsOneD()){
    SetParticleMomentumDirection(G4ParticleMomentum(DirectionNum[0],DirectionNum[1],DirectionNum[2]));
  }
  if (IsDistriD()){
    if (DistriD == "Isotropic"){

      G4double thetaMin = DirectionNum[0];
      G4double thetaMax = DirectionNum[1];
      G4double phiMin = DirectionNum[2];
      G4double phiMax = DirectionNum[3];

      G4double cosTheta = cos(thetaMin) - G4UniformRand()*(cos(thetaMin)-cos(thetaMax));
      G4double sinTheta = sqrt(1-cosTheta*cosTheta);

      G4double Phi = phiMin + (phiMax - phiMin) * G4UniformRand();
      G4double cosPhi = cos(Phi);
      G4double sinPhi = sin(Phi);

      G4double px = -sinTheta * cosPhi;
      G4double py = -sinTheta * sinPhi;
      G4double pz = -cosTheta;

      G4double ResMag = sqrt((px*px)+ (py*py)+(pz*pz));
      px = px/ResMag;
      py = py/ResMag;
      pz = pz/ResMag;

      SetParticleMomentumDirection(G4ParticleMomentum(px, py, pz));

    }
  }
}


//MAIN-----------------

void DAMICParticleSource::GeneratePrimaryVertex(G4Event* evt){

  if(ParticleDefinition == NULL){
    G4cout << "No particle has been defined!" << G4endl;
    return;
  }
  //  G4cout << "petit test 1" << G4endl;
  // POSITION
  // Look for the coordinates

  if (IsMaterial()){
    MaterialCoordinates();
  }
  //  G4cout << "second tes" << G4endl;
  if (IsVolume()){

    CalculProba();
    ChooseVolume();
    VolumeCoordinates();
  }
  // G4cout << "troisieme " << G4endl;

  if (IsSource()){
    SourceCoordinates();
  }

  if (IsShape()){
    ShapeCoordinates();
  }

	if (IsSurface()) {
		SurfaceCoordinates();
	}
  // G4cout << "qua tes " << G4endl;
  // ENERGY
  //G4cout << "energy" << G4endl;
  EnergyValue();

  // DIRECTION
  //G4cout << "direction" << G4endl;
  DirectionValue();
  G4PrimaryVertex* vertex = new G4PrimaryVertex(ParticlePosition, ParticleTime);
  G4double mass = ParticleDefinition->GetPDGMass();
  G4double Energy = ParticleEnergy + mass;
  G4double pmom = std::sqrt(Energy*Energy-mass*mass);
  G4double px = pmom*ParticleMomentumDirection.x();
  G4double py = pmom*ParticleMomentumDirection.y();
  G4double pz = pmom*ParticleMomentumDirection.z();
  //G4cout << "bimbimoup" << G4endl;
  G4PrimaryParticle* particle = new G4PrimaryParticle(ParticleDefinition, px, py, pz);
  particle->SetMass( mass );
  particle->SetCharge( ParticleCharge );
  vertex->SetPrimary( particle );

  evt->AddPrimaryVertex(vertex);
  //G4cout << "bimbimoup" << G4endl;
}
