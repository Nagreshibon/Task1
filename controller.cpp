#include "controller.h"
#include "pool.h"
#include <iostream>
#include <thread>
#include <future>

Controller::Controller(SimMap* inMap, AnimalPool* inPool)
{
	Map = inMap;
	Pool = inPool;

	inMap->SetPool(inPool);
	
	screen = new wchar_t[nScreenWidth * nScreenHeight];
	hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);

	for(unsigned int n = 0; n<nScreenWidth*nScreenHeight; n++)
	{
		screen[n] = ' ';
	}
	screen[nScreenWidth * nScreenHeight - 1] = '\0';
}

void Controller::ToggleTick()
{
	if (bTickActive)
		bTickActive = false;
	else
		bTickActive = true; 
}

void Controller::Display(float DeltaTime)
{
	ReadMap();
	//screen[100] = '@'; 

	if(Pool->GetActiveCarnivores() == 0)
	{
		swprintf_s(screen, 32, L"All carnivores have died out");
		bTickActive = false;
		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
		return; 
	}

	if (Pool->GetActiveHerbivores() == 0)
	{
		swprintf_s(screen, 32, L"All herbivores have died out");
		bTickActive = false;
		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
		return; 
	}

	if (Pool->GetInactive() == 0)
	{
		swprintf_s(screen, 32, L"Pool filled - can't allocate new animals");
		bTickActive = false;
		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
		return; 
	}
	
	swprintf_s(screen, 64, L"n=%d, P=%d, H=%d, FPS=%3.2f, T=%3.2f, D=%d, S=%d", (Pool->GetSize() - Pool->GetInactive()), Pool->GetActiveCarnivores(), Pool->GetActiveHerbivores(), 1.0f / DeltaTime, TickTimer, Map->Dummy, CurrentStage);
	
	WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
}

void Controller::ReadMap()
{
	for(unsigned int y = 0; y<nScreenHeight; y++)
	{
		for (unsigned int x = 0; x < nScreenWidth; x++)
		{
			if (x + hOffset < Map->GetX() && x+hOffset > 0 && y + vOffset < Map->GetY() && y + vOffset > 0)
			{
				if(Map->GetField(x+(int)hOffset, y+(int)vOffset)->GetVectorSize() > 0)
				{
					screen[x + y * nScreenWidth] = '+';
				}
				else
				{
					screen[x + y * nScreenWidth] = '.';
				}
			}
			else
			{
				screen[x + y * nScreenWidth] = ' ';
			}
		}
	}
}

float Controller::Tick(float DeltaTime)
{
	if (!bTickActive)
	{
		Display(DeltaTime);
		return 0.f; 
	}
	TickTimer += DeltaTime;
	if (TickTimer > TickInterval)
	{
		TickTimer = 0.f;
		ProcessTurn(DeltaTime); 	
	}
	
	return 0.0f;
}

void Controller::ProcessTurn(float DeltaTime)
{
	CurrentStage = 0;
	Display(DeltaTime);
	Map->TurnReset();
	
	CurrentStage = 1;
	Display(DeltaTime);
	Map->ProcessPredation();
	
	CurrentStage = 2;
	Display(DeltaTime);
	Pool->ClearGarbage();
	
	CurrentStage = 3;
	Display(DeltaTime);
	Map->ProcessHusbandry();
	
	CurrentStage = 4;
	Display(DeltaTime);
	Map->ProcessMovement();
	
	CurrentStage = 5;
	Display(DeltaTime);
	Pool->ExecuteMovement();
	
	CurrentStage = 6;
	Display(DeltaTime);

}
