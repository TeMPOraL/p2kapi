#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "p2kapi.h"

//some global functions
TRC_P2K_STATUS_FUNCTION P2K_StatusFunction;
trcP2K_Init P2K_Init;
trcP2K_GetStatus P2K_GetStatus;
trcP2K_File_VolInfo P2K_File_VolInfo;
trcP2K_File_Count P2K_File_Count;
trcP2K_Restart P2K_Restart;

HMODULE hLib;

void phone_status(int status)
{
	if(status == 0)
	{
		printf("Status report: phone disconnected\n");
	}
	else if(status == 1)
	{
		printf("Status report: phone connected\n");
	}
	else
	{
		printf("Status report: unknown status %d\n", status);
	}
}


void InitLibrary()
{
	hLib = LoadLibrary("p2kapi.dll");
	if(hLib == NULL)
	{
		printf("Error loading library\n");
		exit(1);
	}
	P2K_Init = (trcP2K_Init)GetProcAddress(hLib, "P2K_Init");
	if(P2K_Init == NULL)
	{
		printf("Failed to get init function\n");
		exit(1);
	}
	P2K_GetStatus = (trcP2K_GetStatus)GetProcAddress(hLib, "P2K_GetStatus");
	P2K_File_VolInfo = (trcP2K_File_VolInfo)GetProcAddress(hLib, "File_VolInfo");
	P2K_File_Count = (trcP2K_File_Count)GetProcAddress(hLib, "File_Count");
	P2K_Restart = (trcP2K_Restart)GetProcAddress(hLib, "P2K_Restart");
	P2K_StatusFunction = &phone_status;
	printf("Library initialized\n");
}

void DeInitLibrary()
{
	FreeLibrary(hLib);
	printf("Library deinitialized\n");
}


int main()
{
	int result;
	char volumeData[130];
	int freeSpace;
	int fileCount;

	printf("P2K API C tester\n");
	InitLibrary();
	printf("Hello world!\n");
	result = P2K_Init(P2K_StatusFunction);
	printf("We're still alive! %d\n", result);
	while(!P2K_GetStatus());
		//std::cout << "Found phone." << std::endl;
	Sleep(500);
	memset(volumeData, 0, 130);
	freeSpace = P2K_File_VolInfo(volumeData);
	fileCount = P2K_File_Count();
	printf("Volume name: %s\n", volumeData);
	printf("Free space: %d\n", freeSpace);
	printf("File count: %d\n", fileCount);
	Sleep(500);
	P2K_Restart();
	Sleep(5000);

	DeInitLibrary();
	return 0;
}
