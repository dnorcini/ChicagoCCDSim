#ifndef PSPhysListFactory_H
#define PSPhysListFactory_H

#include <string>
#include "G4PhysListFactory.hh"
#include "globals.hh"

class PSPhysicsList;

class PSPhysListFactory : public G4PhysListFactory {
public:
	PSPhysListFactory();
	~PSPhysListFactory();

	PSPhysicsList *BuildPhysicsList();
	
private:
	std::string referenceListName;
};
	
#endif
