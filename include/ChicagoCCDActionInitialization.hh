// ChicagoCCDActionInitialization.hh
// definition of the ChicagoCCDActionInitialization class

#ifndef ChicagoCCDActionInitialization_h
#define ChicagoCCDActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

class ChicagoCCDDetectorConstruction;
/// Action initialization class.

class ChicagoCCDActionInitialization : public G4VUserActionInitialization
{
  public:
    ChicagoCCDActionInitialization(ChicagoCCDDetectorConstruction* detectConstruct);
    virtual ~ChicagoCCDActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;
  private:
    ChicagoCCDDetectorConstruction* detectorConstruction;
};


#endif

    
