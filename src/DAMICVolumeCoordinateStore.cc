#include "DAMICVolumeCoordinateStore.hh"

#include "Randomize.hh"

DAMICVolumeCoordinateStore::DAMICVolumeCoordinateStore(G4PhysicalVolumeStore* theStore) : theStore(theStore) {
	NumberOfVolumes=0;
}

DAMICVolumeCoordinateStore::~DAMICVolumeCoordinateStore() {
	for (G4int i=0; i < Rotations.size(); i++) {
		delete Rotations[i];
	}
}

G4bool DAMICVolumeCoordinateStore::AddVolume(G4String Volume, G4double Concentration) {
	histogramBuilt=false;
	G4bool done=false;
	G4bool checked=false;
	G4int volsfound=0;
	G4VPhysicalVolume *WorldPV = theStore->GetVolume("World");
	G4VPhysicalVolume *currentPV = WorldPV;
	G4LogicalVolume *currentLV;
	std::vector<G4VPhysicalVolume*> currentMothers;

	G4RotationMatrix *rotation=NULL;
	G4ThreeVector translation;
	//Holds the tree of which daughter we've gone down
	std::vector<G4int> currentDaughter={0};
	G4int depth=0;
	//Descend through all the daughters looking for all instances of the Volume
	//And add it to our list	
	while (!done) {
		currentLV = currentPV->GetLogicalVolume();
		//First check if our current volume is one of the ones we want
		if ((currentPV->GetName()==Volume) && !checked) {
			PhysicalVolumes.push_back(currentPV);
			Masses.push_back(currentLV->GetMass());
			LogicalVolumes.push_back(currentLV);
			Solids.push_back(currentLV->GetSolid());
			CubicVolumes.push_back(currentLV->GetSolid()->GetCubicVolume());
			SurfaceAreas.push_back(currentLV->GetSolid()->GetSurfaceArea());
			Concentrations.push_back(Concentration);
			Mothers.push_back(currentMothers);
			volsfound+=1;
			//TODO: add global coordinates of those volumes
			//Crude, but we add the current PV to the end of the Mothers, then remove it later (simpifies the algorithm)
			rotation=new G4RotationMatrix(0,0,0);
			translation=G4ThreeVector();
			currentMothers.push_back(currentPV);
			for (G4int i=0; i < currentMothers.size(); i++) {
				//				G4cout << "Object translation: " << currentMothers[i]->GetObjectTranslation() << " Translation: " << currentMothers[i]->GetTranslation() << " Frame Translation" << currentMothers[i]->GetFrameTranslation() << G4endl;
				
				translation+= (*rotation)*currentMothers[i]->GetTranslation();
				if (currentMothers[i]->GetRotation() != NULL) {
					//G4cout << "Object rotation: " << *currentMothers[i]->GetObjectRotation() << " Rotation: " << *currentMothers[i]->GetRotation() << " Frame Rotation:" << *currentMothers[i]->GetFrameRotation() <<  G4endl;
					*rotation *= *(currentMothers[i]->GetObjectRotation());
				}

			}
			Rotations.push_back(rotation);
			//			G4cout << "Total Translation: " << translation << G4endl;
			Translations.push_back(translation);
			currentMothers.pop_back();
		}
		//Next get the daughter, if it has one
		if (currentLV->GetNoDaughters() > currentDaughter[depth] ) {
			checked=false;
			//Add our mother to the chain
			currentMothers.push_back(currentPV);
			//get the daughter
			currentPV=currentLV->GetDaughter(currentDaughter[depth]);
			//increment our counters
			currentDaughter[depth]+=1;
			currentDaughter.push_back(0);
			depth+=1;
		} else if (currentPV->GetName()=="World") {
			done=true;
		} else { 
			//Go back up the chain
			checked=true; // We've already checked if want to add this to our Volumes
			currentPV=currentMothers.back();
			currentMothers.pop_back();
			currentDaughter.pop_back();
			depth-=1;
		}
	}
	if (volsfound == 0) {
		G4cout << "Warning: found no volumes" << G4endl;
		return false;
	}
	NumberOfVolumes+=volsfound;
	//	G4cout << "Found " << volsfound << " volumes" << G4endl;
	return true;
}

G4bool DAMICVolumeCoordinateStore::RemoveVolume(G4String Volume) {
	histogramBuilt=false;
	G4bool removeall = (Volume=="DELETEALL");
	//Go backwards so if we delete anything we don't have to reindex
	G4int volsdeleted=0;
	for (G4int i=PhysicalVolumes.size()-1; i >= 0; i-- ) {
		if ((PhysicalVolumes[i]->GetName()==Volume) || removeall) {
			PhysicalVolumes.erase(PhysicalVolumes.begin()+i);
			LogicalVolumes.erase(LogicalVolumes.begin()+i);
			Solids.erase(Solids.begin()+i);
			Masses.erase(Masses.begin()+i);
			CubicVolumes.erase(CubicVolumes.begin()+i);
			SurfaceAreas.erase(SurfaceAreas.begin()+i);
			Concentrations.erase(Concentrations.begin()+i);
			Mothers.erase(Mothers.begin()+i);
			delete Rotations[i];
			Rotations.erase(Rotations.begin()+i);
			Translations.erase(Translations.begin()+i);
			volsdeleted+=1;
		}
	}
	if (volsdeleted == 0) {
		return false;
	}
	NumberOfVolumes-=volsdeleted;
	return true;
}

void DAMICVolumeCoordinateStore::Clear() {
	RemoveVolume("DELETEALL");
};


G4ThreeVector DAMICVolumeCoordinateStore::GetPointOnSurfaceByName(G4String VolumeName) {
	for (G4int i=0; i < PhysicalVolumes.size(); i++) {
		if (PhysicalVolumes[i]->GetName()==VolumeName) {
			return GetPointOnSurfaceByID(i);
		}
	}
	G4cout << "Warning: Volume not found in coordinate store" << G4endl;
	return G4ThreeVector(0,0,0);
}


G4ThreeVector DAMICVolumeCoordinateStore::GetPointOnSurfaceByID(G4int VolumeID) {
	if (VolumeID >= Solids.size()) {
		G4cout << "Warning: volume not found in coordinate store" << G4endl;
		return G4ThreeVector(0,0,0);
	}
	G4ThreeVector globalPoint=((*Rotations[VolumeID])*Solids[VolumeID]->GetPointOnSurface())+Translations[VolumeID];
	//	G4ThreeVector globalPoint = localPoint * (*Rotations[VolumeID]) + Translations[VolumeID];
	//	G4ThreeVector globalPoint=((Solids[VolumeID]->GetPointOnSurface())+Translations[VolumeID]);
	return globalPoint;
}
		
void DAMICVolumeCoordinateStore::BuildHistogram() {
	totalSurfaceProbability=0;
	surfaceProbabilities.clear();
	for (G4int i=0; i <=PhysicalVolumes.size(); i++) {
		surfaceProbabilities.push_back(totalSurfaceProbability);
		if (i < PhysicalVolumes.size()) {
			totalSurfaceProbability+=SurfaceAreas[i]*Concentrations[i];
		}
	}
	histogramBuilt=true;
}

G4int DAMICVolumeCoordinateStore::GetRandomSurfaceGeneratorID() {
	//Note: this algorithm can be made faster/cleaner
	if (!histogramBuilt) BuildHistogram();
	G4double prob=totalSurfaceProbability*G4UniformRand();
	G4int ID=-1;
	G4int i=0;
	for (; i < surfaceProbabilities.size()-1; i++ ) {
		if ((surfaceProbabilities[i]<prob) && surfaceProbabilities[i+1]>prob) {
			ID=i;
			break;
		}
	}
	if (ID==-1) ID=i+1;
	return ID;
}

G4ThreeVector DAMICVolumeCoordinateStore::GetPointOnSurface() {
	G4int ID = GetRandomSurfaceGeneratorID();
	G4ThreeVector Pos = GetPointOnSurfaceByID(ID);
	//	G4cout << "Spawning at: " << Pos << G4endl;
	//	return GetPointOnSurfaceByID(ID);
	return Pos;
};
