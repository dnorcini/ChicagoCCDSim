#include "PSPhysicsList.hh"
#include "G4ProcessManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"


PSPhysicsList::PSPhysicsList() : G4VModularPhysicsList() {
	defaultCut = 1.*micrometer;
}

PSPhysicsList::~PSPhysicsList() {}


// Bosons
#include "G4ChargedGeantino.hh"
#include "G4Geantino.hh"
#include "G4Gamma.hh"
#include "G4OpticalPhoton.hh"

// leptons
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4NeutrinoMu.hh"
#include "G4AntiNeutrinoMu.hh"

#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4NeutrinoE.hh"
#include "G4AntiNeutrinoE.hh"

// Mesons
#include "G4PionPlus.hh"
#include "G4PionMinus.hh"
#include "G4PionZero.hh"
#include "G4Eta.hh"
#include "G4EtaPrime.hh"

#include "G4KaonPlus.hh"
#include "G4KaonMinus.hh"
#include "G4KaonZero.hh"
#include "G4AntiKaonZero.hh"
#include "G4KaonZeroLong.hh"
#include "G4KaonZeroShort.hh"

// Baryons
#include "G4Proton.hh"
#include "G4AntiProton.hh"
#include "G4Neutron.hh"
#include "G4AntiNeutron.hh"

// Nuclei
#include "G4Deuteron.hh"
#include "G4Triton.hh"
#include "G4Alpha.hh"
#include "G4GenericIon.hh"

#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4IonTable.hh"
#include "G4Ions.hh"


#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

void PSPhysicsList::ConstructParticle() {
// pseudo-particles
  G4Geantino::GeantinoDefinition();
  G4ChargedGeantino::ChargedGeantinoDefinition();
  
// gamma
  G4Gamma::GammaDefinition();
  
// optical photon
  G4OpticalPhoton::OpticalPhotonDefinition();

// leptons
  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
  G4MuonPlus::MuonPlusDefinition();
  G4MuonMinus::MuonMinusDefinition();

  G4NeutrinoE::NeutrinoEDefinition();
  G4AntiNeutrinoE::AntiNeutrinoEDefinition();
  G4NeutrinoMu::NeutrinoMuDefinition();
  G4AntiNeutrinoMu::AntiNeutrinoMuDefinition();  

// // mesons
//   G4PionPlus::PionPlusDefinition();
//   G4PionMinus::PionMinusDefinition();
//   G4PionZero::PionZeroDefinition();
//   G4Eta::EtaDefinition();
//   G4EtaPrime::EtaPrimeDefinition();
//   G4KaonPlus::KaonPlusDefinition();
//   G4KaonMinus::KaonMinusDefinition();
//   G4KaonZero::KaonZeroDefinition();
//   G4AntiKaonZero::AntiKaonZeroDefinition();
//   G4KaonZeroLong::KaonZeroLongDefinition();
//   G4KaonZeroShort::KaonZeroShortDefinition();

// // barions
//   G4Proton::ProtonDefinition();
//   G4AntiProton::AntiProtonDefinition();
//   G4Neutron::NeutronDefinition();
//   G4AntiNeutron::AntiNeutronDefinition();

	// // ions
  // G4Deuteron::DeuteronDefinition();
  // G4Triton::TritonDefinition();
  // G4Alpha::AlphaDefinition();
  // G4GenericIon::GenericIonDefinition();

	//  mesons
	G4MesonConstructor mConstructor;
	mConstructor.ConstructParticle();

	//  baryons
	G4BaryonConstructor bConstructor;
	bConstructor.ConstructParticle();

	//  ions
	G4IonConstructor iConstructor;
	iConstructor.ConstructParticle();

	G4ShortLivedConstructor sConstructor;
	sConstructor.ConstructParticle();

}

#include "G4EmProcessOptions.hh"

void PSPhysicsList::ConstructProcess() {
  // Transportation
  //
  AddTransportation();

	//	G4VPhysicsConstructor *fEmPhysicsList=new G4EmStandardPhysics_option4();
	//	G4VPhysicsConstructor *fEmPhysicsList=new G4EmLiveremorePhysics();
	G4VPhysicsConstructor *fEmPhysicsList=new G4EmPenelopePhysics();

  // Electromagnetic physics list
  //
  fEmPhysicsList->ConstructProcess();
  
  // Em options
  //
  // Main options and setting parameters are shown here.
  // Several of them have default values.
  //
  G4EmProcessOptions emOptions;
  
  //physics tables
  //
  //emOptions.SetMinEnergy(100*eV);        //default    
  //emOptions.SetMaxEnergy(100*TeV);        //default  
  //emOptions.SetDEDXBinning(12*20);        //default=12*7  
  //emOptions.SetLambdaBinning(12*20);        //default=12*7

  emOptions.SetBuildCSDARange(true);     
  emOptions.SetMaxEnergyForCSDARange(10*GeV);
  //emOptions.SetDEDXBinningForCSDARange(12*20);
  
  //emOptions.SetSplineFlag(true);        //default
     
  emOptions.SetVerbose(0);  

	// Add Decay Process
	G4DecayPhysics* decay = new G4DecayPhysics();
	RegisterPhysics(decay);

	G4RadioactiveDecayPhysics* radioactiveDecay = new G4RadioactiveDecayPhysics();
	RegisterPhysics(radioactiveDecay);

	std::cout << "Registered radiactive decay " << std::endl;


}


#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4ProductionCuts.hh"

void PSPhysicsList::SetCuts() {
	// SetCutValue(defaultCut, "gamma");
	// SetCutValue(defaultCut, "e-");
	// SetCutValue(defaultCut, "e+");
	// SetCutValue(defaultCut, "mu-");
	// SetCutValue(defaultCut, "mu+");
	
	// G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(100*eV, 1*GeV);

	// G4ProductionCuts* cut8 = new G4ProductionCuts;
	// cut8->SetProductionCut(defaultCut);
	// G4Region* Region8 = G4RegionStore::GetInstance()->GetRegion("Region8");
	// Region8->SetProductionCuts(cut8);

}

