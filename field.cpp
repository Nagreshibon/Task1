#include "field.h"
#include "pool.h"

#include <iostream>

Field::Field() : ID(IDGen++)
{
	X = 0;
	Y = 0;
	CurrentMap = nullptr; 
};

void Field::AddAnimal(Animal* inAnimal)
{
	if(!inAnimal->Pool)
	{
		std::cout << "orphaned animal";
		return; 
	}
	
	//std::cout << "adding animal ID " << inAnimal->GetID() << " to: " << X << ":" << Y << " | ";
	AnimalVector.push_back(inAnimal);
	inAnimal->CurrentField = this; 
	
	if (inAnimal->bIsFemale())
	{
		if (inAnimal->bIsCarnivore())
		{
			FCarnivoreCount++;
			inAnimal->GetPool()->modActiveCarnivores(1);
			inAnimal->GetPool()->modActiveFemales(1);
		}
		else
		{
			FHerbivoreCount++;
			inAnimal->GetPool()->modActiveHerbivores(1);
			inAnimal->GetPool()->modActiveFemales(1);
		}
	}
	else
	{
		if (inAnimal->bIsCarnivore())
		{
			MCarnivoreCount++;
			inAnimal->GetPool()->modActiveCarnivores(1);
			inAnimal->GetPool()->modActiveMales(1);
		}
		else
		{
			MHerbivoreCount++;
			inAnimal->GetPool()->modActiveHerbivores(1);
			inAnimal->GetPool()->modActiveMales(1);
		}
	}

	inAnimal->GetPool()->modInactive(-1);
}

void Field::AddAnimalBlind(Animal* inAnimal)
{
	AnimalVector.push_back(inAnimal);
	inAnimal->CurrentField = this;

	if(inAnimal->bIsFemale())
	{
		if (inAnimal->bIsCarnivore()) FCarnivoreCount++;
		else FHerbivoreCount++; 
	}
	else
	{
		if (inAnimal->bIsCarnivore()) MCarnivoreCount++;
		else MHerbivoreCount++; 
	}
}

SimMap* Field::GetCurrentMap() { return CurrentMap; };
unsigned int Field::GetCurrentXSize() { return CurrentMap->GetX(); }
unsigned int Field::GetCurrentYSize() { return CurrentMap->GetY(); }
void Field::SetCurrentMap(SimMap* InMap) { CurrentMap = InMap; }

bool Field::RemoveAnimal(Animal* inAnimal)
{
	auto g = std::find(AnimalVector.begin(), AnimalVector.end(), inAnimal);

	if(g !=  AnimalVector.end())
	{
		AnimalVector.erase(g); 
		//std::cout << "e ";

		inAnimal->CurrentField = nullptr;
		inAnimal->GetPool()->modInactive(1);
				
		if (inAnimal->bIsFemale())
		{
			if (inAnimal->bIsCarnivore())
			{
				inAnimal->GetPool()->modActiveCarnivores(-1);
				FCarnivoreCount--;
			}
			else
			{
				inAnimal->GetPool()->modActiveHerbivores(-1);
				FHerbivoreCount--;
			}
		}
		else if (inAnimal->bIsCarnivore())
		{
			inAnimal->GetPool()->modActiveCarnivores(-1);
			MCarnivoreCount--;
		}
		else
		{
			inAnimal->GetPool()->modActiveHerbivores(-1);
			MHerbivoreCount--;
		}
		
		return true;
	}

	//for (auto g : AnimalVector)
	//{
	//	if (g == inAnimal)
	//	{
	//		std::cout << "input animal (ID:" << inAnimal->GetID() << ") found, erasing\n";
	//		//AnimalVector.erase(std::find(inAnimal));

	//		inAnimal->CurrentField = nullptr;
	//		inAnimal->GetPool()->modInactive(1);
	//		
	//		if (inAnimal->bIsFemale())
	//		{
	//			if (inAnimal->bIsCarnivore()) FCarnivoreCount--;
	//			else FHerbivoreCount--;
	//		}
	//		else
	//			if (inAnimal->bIsCarnivore()) MCarnivoreCount--;
	//			else MHerbivoreCount--;

	//		
	//		return true;
	//	}
	//}
	std::cout << "input animal not found in the vector \n";
	
	return false;
}

void Field::PrintToScreen()
{
	std::cout << "|" << MHerbivoreCount << ":" << FHerbivoreCount << ":" << MCarnivoreCount << ":" << FCarnivoreCount << "|";
}

void Field::PrintVector()
{
	for (auto g: AnimalVector)
	{
		std::cout << "aID: " << g->GetID() << " bhM: " << g->bHasMoved << "\n";
	}
}

Animal* Field::GetAnimal(unsigned int idx)
{
	return AnimalVector[idx];
}

void Field::TurnReset()
{
	for (auto g : AnimalVector)
	{
		g->bHasMoved = false; 
	}
}

void Field::ProcessHusbandry(AnimalPool* inPool)
{
	unsigned int num = 0;
	if(MHerbivoreCount>0 && FHerbivoreCount>0)
	{
		auto gen = std::bind(std::uniform_int_distribution<>(0, 1), std::default_random_engine());
		unsigned int t_FHerbivoreCount = FHerbivoreCount;
		for (unsigned int n = 0; n < t_FHerbivoreCount; n++)
		{			
			if (Animal* MyAnimal = inPool->Allocate(gen(), false))
			{
				AddAnimal(MyAnimal);
				num++; 
			}
			else
			{
				std::cout << "unable to allocate more animals in the pool (src: husbandry)"; 
				return;
			}
		}
	}
	if (MCarnivoreCount > 0 && FCarnivoreCount > 0)
	{
		auto gen = std::bind(std::uniform_int_distribution<>(0, 1), std::default_random_engine());
		unsigned int t_FCarnivoreCount = FCarnivoreCount;
		for (unsigned int n = 0; n < t_FCarnivoreCount; n++)
		{
			if (Animal* MyAnimal = inPool->Allocate(gen(), true))
			{
				AddAnimal(MyAnimal);
				num++;
			}
			else
			{
				std::cout << "unable to allocate more animals in the pool (src: husbandry)";
				return;
			}
		}
	}
	
	std::cout << "Husbandry tally:" << num << " from field: " << X << ":" << Y << "\n";
}

void Field::ProcessPredation(AnimalPool* inPool)
{
	if(MCarnivoreCount + FCarnivoreCount > 0)
	{
		std::vector<Animal*> PredationVector; 
		
		if(MHerbivoreCount + FHerbivoreCount > 0)
		{
			unsigned int k = 0;
			unsigned int l = 0;
			unsigned int eatlim = std::min(FCarnivoreCount + MCarnivoreCount, FHerbivoreCount+MHerbivoreCount);
			std::cout << "eating " << eatlim << " herbivores\n";
			
			for (auto& g : AnimalVector)
			{
				if (k>= eatlim && l>=eatlim) return;
				
				if (g->bIsCarnivore())
				{
					g->FedTimer--;
					
					if (k <= eatlim) //if less than >eatlim carnivores have eaten
					{
						g->FedTimer = 3;
						k++;
					}
					else //if k=eatlim carnivores have already eaten
					{
						//g->FedTimer--;
						if (g->FedTimer <= 0) PredationVector.push_back(g);
					}
				}
				else if (l<=eatlim) //if less than >eatlim herbivores have been eaten
				{
					PredationVector.push_back(g);
					l++;
				}
			}
		}
		else
		{
			unsigned int num_s = 0;
			unsigned int num_d = 0;
			for (auto &g : AnimalVector)
			{
				if(g->bIsCarnivore())
				{
					g->FedTimer--;
					num_s++; 
					if (g->FedTimer <= 0)
					{
						PredationVector.push_back(g);
						num_d++; 
					}
				}
			}
			std::cout << "no herbivores to eat in field: " << X << ":" << Y << " , hungry predators: " << num_s << " dead:" << num_d << "\n";
		}

		for (auto p : PredationVector)
		{
			RemoveAnimal(p);
		}
		
	}
	else
	{
		std::cout << "no predators in field: " << X << ":" << Y << "\n";
	}
}

void Field::ProcessMovement()
{
	//unsigned int k = 0;
	//std::cout << "field: " << X << ":" << Y << ", processing " << AnimalVector.size() << " animals\n";
	//PrintVector();

	std::vector<Animal*> MoveVector;
	
	for (auto a = AnimalVector.begin(); a!= AnimalVector.end(); ++a)
	{
		//std::cout << "aID: " << (*a)->GetID() << " bHM: " << (*a)->bHasMoved << "| ";
		if (!(*a)->bHasMoved)
		{
			MoveVector.push_back(*a); 
		}
	}

	for (auto m : MoveVector)
	{
		m->RandomMove(); 
	}
	
	//std::cout << "animals moved from field:" << X << ":" << Y << " - " << k << "\n";
}

bool Field::bIsEmpty()
{
	if (MHerbivoreCount + FHerbivoreCount + FCarnivoreCount + MCarnivoreCount == 0) return true;
	return false;
}

unsigned int Field::GetVectorSize()
{
	return AnimalVector.size();
}
