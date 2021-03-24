#pragma once
#include "field.h"

class Field;
class AnimalPool; 

class SimMap
{
public:
	friend class AnimalPool; 
	
	SimMap(unsigned int SizeX, unsigned int SizeY);

	unsigned int Dummy = 0; 
	
	~SimMap();

	unsigned int GetX() { return X; }
	unsigned int GetY() { return Y; }
	
	Field* GetField(unsigned int InX, unsigned int InY);

	Field* GetField(unsigned int inID);

	void SetPool(AnimalPool* InPool);

	void PrintToScreen();

	void TurnReset(); 
	void ProcessHusbandry();
	void ProcessPredation();
	void ProcessMovement();
	void SpawnAnimal(unsigned int inX, unsigned int inY, bool bIsFemale, bool bIsCarnivore, unsigned int n = 1);

private:
	unsigned int X;
	unsigned int Y;
	Field* MapArr;
	AnimalPool* Pool; 
};
