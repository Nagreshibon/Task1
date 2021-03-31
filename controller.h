#pragma once

#include "pool.h"
#include <Windows.h>
#include <chrono>

class Controller
{
public:
	Controller(SimMap* inMap, AnimalPool* inPool);

	void ToggleTick();
	void Display(float DeltaTime);

	void ReadMap();
	float* GetVOffset() { return &vOffset; }
	float* GetHOffset() { return &hOffset; }
	
	
	float Tick(float DeltaTime);
	float TickTimer = 0.f;
	float TickInterval = 0.5f;
	void ProcessTurn(float DeltaTime);

private:
	bool bTickActive = true; 
	
	AnimalPool* Pool;
	SimMap* Map;

	float vOffset = 0;
	float hOffset = 0;

	unsigned int CurrentStage = 0; 
	
	unsigned int nScreenWidth = 120;
	unsigned int nScreenHeight = 40;
	
	wchar_t* screen;
	HANDLE hConsole;
	DWORD dwBytesWritten = 0;
};
