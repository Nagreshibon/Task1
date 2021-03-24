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

	SimMap E(50, 50);
	AnimalPool P(1000000);
	Controller C(&E, &P); 
	//P.Fill();
	P.Link(&E);
	P.Distribute(&E, 4000);
	P.Distribute(&E, 1000, true); //spawn more carnivores
	
	E.PrintToScreen();

	bool MainLoop = true; 
	
	while(MainLoop)
	{
		tp2 = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		fElapsedTime = elapsedTime.count();
		
		if (GetAsyncKeyState((unsigned short)'Q') & 0x8000) C.ToggleTick();

		if (GetAsyncKeyState((unsigned short)'W') & 0x8000) *(C.GetVOffset()) += -0.1;
		if (GetAsyncKeyState((unsigned short)'S') & 0x8000) *(C.GetVOffset()) += 0.1;
		if (GetAsyncKeyState((unsigned short)'A') & 0x8000) *(C.GetHOffset()) += -0.1;
		if (GetAsyncKeyState((unsigned short)'D') & 0x8000) *(C.GetHOffset()) += 0.1;

		if (GetAsyncKeyState((unsigned short)'E') & 0x8000) return 0;

		C.Tick(fElapsedTime);		
		C.Display(fElapsedTime);
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
