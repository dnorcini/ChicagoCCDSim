#ifndef PSPhysicsList_H
#define PSPhysicsList_H

#include "G4VModularPhysicsList.hh"
#include "globals.hh"


//Not implemented
class PSPhysicsList : public G4VModularPhysicsList {
public:
	PSPhysicsList();
	~PSPhysicsList();

	virtual void ConstructParticle();
	virtual void ConstructProcess();

	virtual void SetCuts();

private:
  double defaultCut;
};

#endif
