#include "pool.h"

#include <chrono>

AnimalPool::AnimalPool(unsigned int Size)
{
	PoolStart = new Animal[Size];
	pSize = Size;
	nInactive = Size;
	next = PoolStart;
	tail = PoolStart + Size;

	for(unsigned int i = 0; i<pSize; i++)
	{
		PoolStart[i].Pool = this; 
	}
}

//Animal* AnimalPool::Allocate(int inID, Animal* inAnimal)
//{
//	PoolStart[inID] = *inAnimal;
//	delete inAnimal;
//	return PoolStart + inID;
//}

Animal* AnimalPool::Allocate(Animal* inAnimal)
{
	if (next)
	{
		Animal* current = next;
		*current = *inAnimal;
		delete inAnimal;
		if (next != tail && (next+1)->GetField() == nullptr)
		{
			next = next + 1; 
		}
		else if (nInactive != 0)
		{
			for (unsigned int i = 0; i<pSize; i++)
			{
				if (PoolStart[i].GetField() == nullptr)
				{
					next = PoolStart + i;
				}
			}
		}

		//nInactive--;
		return current;
	}
	else
	{
		std::cout << "allocation exceeding pool size";
		return nullptr;
	}
}

Animal* AnimalPool::Allocate(bool bIsFemale, bool bIsCarnivore)
{
	if (nInactive != 0)
	{
		Animal* current = next;
		current->Female = bIsFemale;
		current->Carnivore = bIsCarnivore;
		current->FedTimer = 3;
		current->bHasMoved = false; 
		
		if (next != tail && (next + 1)->GetField() == nullptr)
		{
			next = next + 1;
		}
		else
		{
			for (unsigned int i = 0; i < pSize; i++)
			{
				if (PoolStart[i].GetField() == nullptr)
				{
					next = PoolStart + i;
				}
			}
		}

		//std::cout << "alloc: " << current->GetID() << ":" << current->bHasMoved << "|";

		//nInactive--; 
		return current;
	}
	else
	{
		std::cout << "allocation exceeding pool size";
		return nullptr;
	}
}

void AnimalPool::Fill()
{
	auto gen = std::bind(std::uniform_int_distribution<>(0, 1), std::default_random_engine());
	for (unsigned int i = 0; i < pSize; i++)
	{
		PoolStart[i].Carnivore = gen();
		PoolStart[i].Female = gen();
	}
}

void AnimalPool::Distribute(SimMap* InMap, unsigned int Num)
{
	if (Num > nInactive)
	{
		std::cout << "allocation exceeding pool size"; 
		return;
	}
	
	auto gen = std::bind(std::uniform_int_distribution<>(0, InMap->GetX()*InMap->GetY()-1), std::default_random_engine());
	auto bgen = std::bind(std::uniform_int_distribution<>(0, 1), std::default_random_engine());

	std::random_device rd;
	std::mt19937::result_type seed = rd() ^ (
		(std::mt19937::result_type)
		std::chrono::duration_cast<std::chrono::seconds>(
			std::chrono::system_clock::now().time_since_epoch()
			).count() +
		(std::mt19937::result_type)
		std::chrono::duration_cast<std::chrono::microseconds>(
			std::chrono::high_resolution_clock::now().time_since_epoch()
			).count());

	std::mt19937 gen2(seed);
	std::uniform_int_distribution<unsigned> distrib(0, 1);
	
	for (unsigned int i = 0; i < Num; i++)
	{
		unsigned int k = gen();
		PoolStart[i].CurrentField = nullptr; 
		
		//PoolStart[i].Place(InMap->GetField(k));
		
		if (distrib(gen2) == 1)
		{
			std::cout << "adding female "; 
			PoolStart[i].Female = true;
			nActiveFemales++; 
		}
		else
		{
			std::cout << "adding male ";
			PoolStart[i].Female = false;
			nActiveMales++;
		}

		if (distrib(gen2) == 1)
		{
			std::cout << "adding carnivore ";
			PoolStart[i].Carnivore = true;
			nActiveCarnivores++;
		}
		else
		{
			std::cout << "adding herbivore ";
			PoolStart[i].Carnivore = false;
			nActiveHerbivores++;
		}

		InMap->GetField(k)->AddAnimalBlind(&PoolStart[i]);
		
		
		//nInactive--; 
		//
		////if(PoolStart[i].bIsFemale())
		//if(bgen() == 1)
		//{
		//	PoolStart[i].Female = true; 
		//	nActiveFemales++;
		//}
		//else
		//{
		//	PoolStart[i].Female = false; 
		//	nActiveMales++;
		//}

		//if(bgen() == 1)
		//{
		//	PoolStart[i].Carnivore = true; 
		//	nActiveCarnivores++;
		//}
		//else
		//{
		//	PoolStart[i].Carnivore = false;
		//	nActiveHerbivores++;
		//}
		//std::cout << "placing animal: " << PoolStart[i].GetID() << " at: " << k << "\n"; 
	}
	//nInactive = 0;
	nInactive = nInactive -Num; 
	next = next + Num; 
	std::cout << "Carnivores: " << nActiveCarnivores << " Herbivores: " << nActiveHerbivores << " Females:" << nActiveFemales << " Males:" << nActiveMales << "\n";
	
}

void AnimalPool::Link(SimMap* InMap)
{
	InMap->SetPool(this);
}

void AnimalPool::PrintToScreen()
{
	for (unsigned int i = 0; i<pSize; i++)
	{
		if (PoolStart[i].GetField() == nullptr) std::cout << ".";
		else std::cout << "+"; 
	}
	std::cout << "\n"; 
	std::cout << "Carnivores: " << nActiveCarnivores << " Herbivores: " << nActiveHerbivores << " Females:" << nActiveFemales << " Males:" << nActiveMales << "\n";
	std::cout << "nInactive:" << nInactive;
}

void AnimalPool::modInactive(unsigned int k)
{
	nInactive += k;
}

void AnimalPool::modActiveCarnivores(unsigned int k)
{
	nActiveCarnivores += k;
}

void AnimalPool::modActiveHerbivores(unsigned int k)
{
	nActiveHerbivores += k;
}

void AnimalPool::modActiveFemales(unsigned int k)
{
	nActiveFemales += k;
}

void AnimalPool::modActiveMales(unsigned int k)
{
	nActiveMales += k;
}

