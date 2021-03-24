#pragma once

#include "field.h"

class AnimalPool; 

class Animal
	{
	public:

		friend class Field;
		friend class AnimalPool;
	
		Animal(bool InCarnivore, bool InFemale);
		Animal();

		Animal& operator=(const Animal& src);

		void RandomMove();
		void Place(Field* inField);
		void Kill(); 
	
		const unsigned int GetID() { return ID; }
		const bool bIsCarnivore() { return Carnivore; }
		const bool bIsFemale() { return Female; }
		const unsigned int GetFedTimer() { return FedTimer; }
		Field* GetField() { return CurrentField; }
		AnimalPool* GetPool() { return Pool; }

	private:
		static unsigned int IDGen;
	
		const unsigned int ID;
		bool Carnivore;
		bool Female;
		unsigned int FedTimer = 3;
		Field* CurrentField;
		AnimalPool* Pool;
		bool bHasMoved = false; 

	};