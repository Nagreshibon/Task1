// Task1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "map.h"
#include "pool.h"
#include <iostream>
#include <Windows.h>

#include "controller.h"

unsigned int Animal::IDGen;
unsigned int Field::IDGen;

auto tp1 = std::chrono::system_clock::now();
auto tp2 = std::chrono::system_clock::now();
float fElapsedTime = 0.f;

int main()
{

	SimMap Map(150, 150);
	AnimalPool Pool(1000000);
	Controller Controller(&Map, &Pool); 
	//P.Fill();
	Pool.Link(&Map);
	//Pool.Distribute(&Map, 400);
	Pool.Distribute(&Map, 40000, false);
	Pool.Distribute(&Map, 10000, true); //spawn more carnivores
	//Map.SpawnAnimal(1, 1, true, true, 4);
	//Map.SpawnAnimal(1, 1, false, true, 4);
	//Map.SpawnAnimal(1, 1, true, false, 4);

	//Map.PrintToScreen();

	//Map.ProcessPredation(); 
	//Map.GetField(1, 1)->ProcessPredation(&Pool);
	//Pool.ClearGarbage(); 

	bool MainLoop = true;
	//MainLoop = false; 
	
	while(MainLoop)
	{
		tp2 = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		fElapsedTime = elapsedTime.count();
		
		if (GetAsyncKeyState((unsigned short)'Q') & 0x8000) Controller.ToggleTick();

		if (GetAsyncKeyState((unsigned short)'W') & 0x8000) *(Controller.GetVOffset()) += -0.1;
		if (GetAsyncKeyState((unsigned short)'S') & 0x8000) *(Controller.GetVOffset()) += 0.1;
		if (GetAsyncKeyState((unsigned short)'A') & 0x8000) *(Controller.GetHOffset()) += -0.1;
		if (GetAsyncKeyState((unsigned short)'D') & 0x8000) *(Controller.GetHOffset()) += 0.1;

		if (GetAsyncKeyState((unsigned short)'E') & 0x8000) return 0;

		Controller.Tick(fElapsedTime);
		//Controller.Display(fElapsedTime);
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
