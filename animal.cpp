#include "animal.h"
#include <algorithm>
#include <functional>
#include <chrono>
#include <iostream>
#include <random>

Animal::Animal(bool InCarnivore, bool InFemale) : ID(IDGen++)
{
	Carnivore = InCarnivore;
	Female = InFemale;
	CurrentField = nullptr;
	//Pool = nullptr;
};

Animal::Animal() : ID(IDGen++)
{
	Carnivore = false;
	Female = false;
	CurrentField = nullptr;
	//Pool = nullptr;
};

Animal& Animal::operator=(const Animal& src)
{
	if (this == &src) return *this; 
	
		Carnivore = src.Carnivore;
		Female = src.Female;
		FedTimer = src.FedTimer;
		CurrentField = src.CurrentField;
		//Pool = src.Pool;

	return *this;
}

void Animal::RandomMove()
{
	//std::cout << "m1 ";
	if (CurrentField)
	{
		//std::cout << "m2 ";
		std::vector<int> MoveVec; 

		//N - 8
		if (CurrentField->Y > 0) MoveVec.push_back(8);
		//S - 2
		if (CurrentField->Y < CurrentField->GetCurrentYSize() - 1) MoveVec.push_back(2);
		//E - 6
		if (CurrentField->X < CurrentField->GetCurrentXSize()-1) MoveVec.push_back(6);
		//W - 4
		if (CurrentField->X > 0) MoveVec.push_back(4);

		//NE - 9
		if (CurrentField->Y > 0 && CurrentField->X < CurrentField->GetCurrentXSize() - 1) MoveVec.push_back(9);
		//NW - 7
		if (CurrentField->Y > 0 && CurrentField->X > 0) MoveVec.push_back(7);
		//SE - 3
		if (CurrentField->Y < CurrentField->GetCurrentYSize() - 1 && CurrentField->X < CurrentField->GetCurrentXSize() - 1) MoveVec.push_back(3);
		//SW - 1
		if (CurrentField->Y < CurrentField->GetCurrentYSize() - 1 && CurrentField->X > 0) MoveVec.push_back(1);

		std::random_device rd;
		/*std::mt19937::result_type seed = rd() ^ (
			(std::mt19937::result_type)
			std::chrono::duration_cast<std::chrono::seconds>(
				std::chrono::system_clock::now().time_since_epoch()
				).count() +
			(std::mt19937::result_type)
			std::chrono::duration_cast<std::chrono::microseconds>(
				std::chrono::high_resolution_clock::now().time_since_epoch()
				).count());

		std::mt19937 gen(seed);
		std::uniform_int_distribution<unsigned> distrib(0, MoveVec.size()-1);*/

		unsigned int seed = rd() ^ std::chrono::duration_cast<std::chrono::seconds>(
			std::chrono::system_clock::now().time_since_epoch()).count();
		
		auto gen = std::bind(std::uniform_int_distribution<int>(0, MoveVec.size()-1), std::default_random_engine(seed));
		//std::cout << "MoveVec size: " << MoveVec.size() << " rand:" << distrib(gen) << "\n";

		//int d1 = 0; int d2 = 0; int d3 = 0; int d4 = 0; int d6 = 0; int d7 = 0; int d8 = 0; int d9 = 0;
		
		
		switch(MoveVec[gen()])
		{
		case 1:
			Place(CurrentField + CurrentField->GetCurrentXSize() - 1);
			break;
		case 2:
			Place(CurrentField + CurrentField->GetCurrentXSize());
			break;
		case 3:
			Place(CurrentField + CurrentField->GetCurrentXSize() + 1);
			break;
		case 4:
			Place(CurrentField - 1);
			break;
		case 6:
			Place(CurrentField + 1);
			break;
		case 7:
			Place(CurrentField - CurrentField->GetCurrentXSize() - 1);
			break;
		case 8:
			Place(CurrentField - CurrentField->GetCurrentXSize());
			break;
		case 9:
			Place(CurrentField - CurrentField->GetCurrentXSize() + 1);
			break;
		default:
			std::cout << "no possible move\n";
			break;
		}

		bHasMoved = true;
						
	}
	else
	{
		//std::cout << "animal has no field: " << ID << " field ID:" << CurrentField->ID << " bHasMoved: " << bHasMoved << " |"; 
	}
}

void Animal::Place(Field* inField)
{
	if (CurrentField)
	{
		CurrentField->RemoveAnimal(this);
	}
	inField->AddAnimal(this);
	
}

void Animal::Kill()
{
	CurrentField->RemoveAnimal(this);
	CurrentField = nullptr;
	FedTimer = 2; 
}
