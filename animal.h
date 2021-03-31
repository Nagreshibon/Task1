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

		void MarkForMovement(); 
		void MarkForCollection();
		//void MarkForCollection(unsigned int inIndex);
	
		const unsigned int GetID() { return ID; }
		const bool bIsCarnivore() { return Carnivore; }
		const bool bIsFemale() { return Female; }
		const unsigned int GetFedTimer() { return FedTimer; }
		Field* GetField() { return CurrentField; }
		AnimalPool* GetPool() { return Pool; }

	private:
		Field* CurrentField; //8 bytes
		AnimalPool* Pool; //8 bytes
		static unsigned int IDGen; //4 bytes
		unsigned int ID; //4 bytes
		unsigned int FedTimer = 3; //4 bytes
		bool bHasMoved = false; //1 byte
		bool Carnivore; //1 byte
		bool Female; //1 byte

	};