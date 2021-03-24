#pragma once

#include "map.h"
#include "animal.h"
#include <vector>

class SimMap; 

class Field
{
public:

	friend class Animal;

	Field();

	void AddAnimal(Animal* inAnimal);
	void AddAnimalBlind(Animal* inAnimal);

	bool RemoveAnimal(Animal* inAnimal);

	SimMap* GetCurrentMap();
	unsigned int GetCurrentXSize();
	unsigned int GetCurrentYSize();
	void SetCurrentMap(SimMap* InMap);

	void PrintToScreen();
	void PrintVector();

	Animal* GetAnimal(unsigned int idx);

	void TurnReset();
	void ProcessHusbandry(AnimalPool* inPool);
	void ProcessPredation(AnimalPool* inPool);
	void ProcessMovement();
	bool bIsEmpty();

	unsigned int GetVectorSize(); 

	unsigned int X;
	unsigned int Y; 
	
private:
	static unsigned int IDGen;

	const unsigned int ID;

	unsigned int MHerbivoreCount = 0;
	unsigned int FHerbivoreCount = 0;
	unsigned int MCarnivoreCount = 0;
	unsigned int FCarnivoreCount = 0;
	
	std::vector<Animal*> AnimalVector;
	SimMap* CurrentMap; 
};
