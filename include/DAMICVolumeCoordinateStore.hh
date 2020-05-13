#ifndef DAMICVolumeCoordinateStore_h
#define DAMICVolumeCoordinateStore_h
//TODO: cleanup includes, move to .cc file
#include "G4PhysicalVolumeStore.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4VSolid.hh"
#include "G4LogicalVolume.hh"
#include <vector>



class DAMICVolumeCoordinateStore {
private:
	G4int NumberOfVolumes;
public:
	DAMICVolumeCoordinateStore(G4PhysicalVolumeStore*);
	~DAMICVolumeCoordinateStore();

	G4bool AddVolume(G4String Volume, G4double Concentration=1);
	G4bool RemoveVolume(G4String Volume);

	G4ThreeVector GetPointOnSurfaceByName(G4String VolumeName);
	G4ThreeVector GetPointOnSurfaceByID(G4int VolumeID);

	G4int GetNoVolumes() {return NumberOfVolumes;}
	
	void Clear();
	//	G4ThreeVector GetGlobalCoordinates(G4VPhysicalVolume *Volume);
	G4ThreeVector GetPointOnSurface();
	void BuildHistogram();
	
private:
	//Vector to store the volumes that we're using in
	std::vector<G4VPhysicalVolume*> PhysicalVolumes;
	std::vector<G4LogicalVolume*> LogicalVolumes;
	//Holds pointers to the solids
	std::vector<G4VSolid*> Solids;
	//Vector to store the origin of the volumes, in the *global* coordinate system
	std::vector<G4ThreeVector> Translations;
	std::vector<G4RotationMatrix*> Rotations;
	//Vectors to hold the geometric surface area/volume of our volumes
	std::vector<G4double> Masses;
	std::vector<G4double> CubicVolumes;
	std::vector<G4double> SurfaceAreas;
	std::vector<G4double> Concentrations;
	//Holds the mothers
	std::vector<std::vector<G4VPhysicalVolume*> > Mothers;
	G4PhysicalVolumeStore *theStore;


	//Holds information about probabiility for particle generator
	std::vector<G4double> surfaceProbabilities;
	G4double totalSurfaceProbability;
	G4bool histogramBuilt;

	G4int GetRandomSurfaceGeneratorID();
	
};

#endif
