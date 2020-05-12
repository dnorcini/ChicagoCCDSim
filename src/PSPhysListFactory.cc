#include "PSPhysListFactory.hh"
#include "PSPhysicsList.hh"

PSPhysListFactory::PSPhysListFactory() {
	//	referenceListName="FTFP_BERT";
	//	referenceListName="LBE"
	referenceListName="QBBC";
}

PSPhysListFactory::~PSPhysListFactory() {}

PSPhysicsList *PSPhysListFactory::BuildPhysicsList() {
	G4VModularPhysicsList* list=GetReferencePhysList(referenceListName);

	PSPhysicsList* outlist=new PSPhysicsList();

	for (G4int i=0; ++i; ) {
		G4VPhysicsConstructor* elem=const_cast<G4VPhysicsConstructor*> (list->GetPhysics(i));
		if (elem==NULL) break;
		G4cout << "Register physics: " << elem->GetPhysicsName() << G4endl;
		outlist->RegisterPhysics(elem);
	}
	return outlist;
}
