#pragma once

#include "map.h"
#include <random>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include "animal.h"

//using namespace std; 
class AnimalPool
{
public:

	AnimalPool(unsigned int Size);

	Animal* Allocate(Animal* inAnimal);
	Animal* Allocate(bool bIsFemale, bool bIsCarnivore);

	void Fill();
	void Distribute(SimMap* InMap, unsigned int Num);
	void Distribute(SimMap* InMap, unsigned int Num, bool IsCarnivore);
	void Link(SimMap* InMap);

	void PrintToScreen();

	struct AnimalAddress
	{
		unsigned int Field;
		unsigned int Index;

		AnimalAddress(unsigned int inField, unsigned int inIndex)
		{
			Field = inField;
			Index = inIndex; 
		}
	};
	
	//std::vector<AnimalAddress> GarbageVector;
	std::vector<Animal*> MoveVector; 
	std::vector<Animal*> GarbageVector_Pointers;
	std::map<Animal*, unsigned int> GarbageMap; 

	void AddToMovement(Animal* inAnimal);
	void ExecuteMovement(); 
	void AddToGarbage(Animal* inAnimal); 
	void AddToGarbage(Animal* inAnimal, unsigned int inIndex);
	void ClearGarbage();
	
	unsigned int GetInactive() { return nInactive; }
	unsigned int GetSize() { return pSize; }
	unsigned int GetActiveHerbivores() { return nActiveHerbivores; }
	unsigned int GetActiveCarnivores() { return nActiveCarnivores; }

	void modInactive(unsigned int k);
	void modActiveCarnivores(unsigned int k);
	void modActiveHerbivores(unsigned int k);
	void modActiveFemales(unsigned int k);
	void modActiveMales(unsigned int k);

	SimMap* Map;

private:
	Animal* PoolStart;
	unsigned int pSize;

	Animal* tail; 
	Animal* next; 
	unsigned int nActiveCarnivores = 0;
	unsigned int nActiveHerbivores = 0;
	unsigned int nActiveFemales = 0;
	unsigned int nActiveMales = 0;
	unsigned int nInactive;
};