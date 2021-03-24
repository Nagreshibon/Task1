#include "map.h"
#include "pool.h"
#include <iostream>

SimMap::SimMap(unsigned int SizeX, unsigned int SizeY)
{
	MapArr = new Field[SizeX * SizeY];
	//std::cout << "creating " << SizeX * SizeY << " fields\n"; 
	X = SizeX;
	Y = SizeY;
	
	for (unsigned int y = 0; y < SizeY; y++)
	{
		for (unsigned int x = 0; x < SizeX; x++)
		{
			//std::cout << j << "," << i << "," << j+i*SizeY << "|";
			MapArr[x + (y * SizeX)].X = x;
			MapArr[x + (y * SizeX)].Y = y;
			MapArr[x + (y * SizeX)].SetCurrentMap(this);
			
		}
		//std::cout << "\n";
	}
};

SimMap::~SimMap()
{
	//delete[] MapArr;
	MapArr = nullptr;
};

Field* SimMap::GetField(unsigned int InX, unsigned int InY)
{
	return (MapArr + (InX + InY * X));
};

Field* SimMap::GetField(unsigned int inID)
{
	return (MapArr + inID);
}

void SimMap::SetPool(AnimalPool* InPool)
{
	Pool = InPool; 
}

void SimMap::PrintToScreen()
{
	std::cout << "\n";
	for(unsigned int y = 0; y<Y; y++)
	{
		for(unsigned int x = 0; x<X; x++)
		{
			GetField(x, y)->PrintToScreen();
		}
		std::cout << "\n";
	}
}


void SimMap::TurnReset()
{
	Dummy++; 
	for (unsigned int y = 0; y < Y; y++)
	{
		for (unsigned int x = 0; x < X; x++)
		{
			if (!GetField(x, y)->bIsEmpty())
			{
				GetField(x, y)->TurnReset();
			}
		}
	}
}

void SimMap::ProcessHusbandry()
{
	for(unsigned int y = 0; y<Y; y++)
	{
		for (unsigned int x = 0; x < X; x++)
		{
			if (!GetField(x, y)->bIsEmpty())
			{
				GetField(x, y)->ProcessHusbandry(Pool);
			}
		}
	}	
}
void SimMap::ProcessPredation()
{
	for (unsigned int y = 0; y < Y; y++)
	{
		for (unsigned int x = 0; x < X; x++)
		{
			if (!GetField(x, y)->bIsEmpty())
			{
				GetField(x, y)->ProcessPredation(Pool);
			}
		}
	}
}
void SimMap::ProcessMovement()
{
	
	for (unsigned int y = 0; y < Y; y++)
	{
		for (unsigned int x = 0; x < X; x++)
		{
			if (!GetField(x, y)->bIsEmpty())
			{
				//GetField(x, y)->PrintVector(); 
				GetField(x, y)->ProcessMovement();
			}
		}
	}
}
void SimMap::SpawnAnimal(unsigned int inX, unsigned int inY, bool bIsFemale, bool bIsCarnivore, unsigned int n)
{
	if(!Pool)
	{
		std::cout << "no pool linked to map";
		return;
	}
	
	for (unsigned int k = 0; k < n; k++)
	{
		if (Animal* AnimalToSpawn = Pool->Allocate(bIsFemale, bIsCarnivore))
		{
			//std::cout << "spawning AID: " << AnimalToSpawn->GetID() << "|"; 
			GetField(inX, inY)->AddAnimal(AnimalToSpawn);
		}
		else
		{
			std::cout << "unable to allocate more animals in the pool (src: direct spawn)";
			return;
		}
	}

	//GetField(inX, inY)->PrintVector(); 
}
;