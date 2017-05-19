#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <windows.h>

#include <cstdlib>

#include "p2kapi.h"

//some global functions
TRC_P2K_STATUS_FUNCTION P2K_StatusFunction;
trcP2K_Init P2K_Init;
trcP2K_GetStatus P2K_GetStatus;
trcP2K_File_VolInfo P2K_File_VolInfo;
trcP2K_File_Count P2K_File_Count;
trcP2K_Restart P2K_Restart;
trcP2K_File_List P2K_File_List;
trcP2K_File_Create P2K_File_Create;
trcP2K_File_Close P2K_File_Close;
trcP2K_File_Read P2K_File_Read;

HMODULE hLib;

void phone_status(int status)
{
	if(status == 0)
	{
		std::cout << "Status report: phone disconnected" << std::endl;
	}
	else if(status == 1)
	{
		std::cout << "Status report: phone connected" << std::endl;
	}
	else
	{
		std::cout << "Status report: unknown status " << status << std::endl;
	}
}


void InitLibrary()
{
	hLib = LoadLibrary("p2kapi.dll");
	if(hLib == NULL)
	{
		throw std::runtime_error("Failed to load DLL");
	}
	P2K_Init = reinterpret_cast<trcP2K_Init>(GetProcAddress(hLib, "P2K_Init"));
	if(P2K_Init == NULL)
	{
		throw std::runtime_error("Failed to get init function");
	}
	P2K_GetStatus = reinterpret_cast<trcP2K_GetStatus>(GetProcAddress(hLib, "P2K_GetStatus"));
	P2K_File_VolInfo = reinterpret_cast<trcP2K_File_VolInfo>(GetProcAddress(hLib, "File_VolInfo"));
	P2K_File_Count = reinterpret_cast<trcP2K_File_Count>(GetProcAddress(hLib, "File_Count"));
	P2K_Restart = reinterpret_cast<trcP2K_Restart>(GetProcAddress(hLib, "P2K_Restart"));
	P2K_File_List = reinterpret_cast<trcP2K_File_List>(GetProcAddress(hLib, "File_List"));
	P2K_File_Create = reinterpret_cast<trcP2K_File_Create>(GetProcAddress(hLib, "File_Create"));
	P2K_File_Close = reinterpret_cast<trcP2K_File_Close>(GetProcAddress(hLib, "File_Close"));
	P2K_File_Read = reinterpret_cast<trcP2K_File_Read>(GetProcAddress(hLib, "File_Read"));
	P2K_StatusFunction = &phone_status;
	std::cout << "Library initialized" << std::endl;
}

void DeInitLibrary()
{
	FreeLibrary(hLib);
	std::cout << "Library deinitialized" << std::endl;
}

void OutputFileList(const std::vector<char>& data, int files)
{
	std::cout << "=== FILE LIST ===" << std::endl;
	int size;
	short attr1,attr2;
	char name[0x1F8];
	const char* ptr = &data[0];
	for(int i = 0 ; i < files ; ++i)
	{
		ptr = &data[0] + (0x200*i);
		memcpy(&size, ptr, sizeof(int)); ptr+= sizeof(int);
		memcpy(&attr1, ptr, sizeof(short)); ptr+= sizeof(short);
		memcpy(&attr2, ptr, sizeof(short)); ptr+= sizeof(short);
		memcpy(&name[0], ptr, 0x1F8); ptr+= 0x1F8;

		std::cout << i << ") " << name << "\nSize: " << size << "bytes\n" << "Attributes: " << attr1 << " " << attr2;
		std::cout << std::endl << std::endl;
	}
	std::cout << "=== END OF FILE LIST ===" << std::endl;
	std::cout << "Read " << files << " files" << std::endl;
}

int main()
{
	try
	{
		InitLibrary();
		std::cout << "Hello world!" << std::endl;
		int result = P2K_Init(P2K_StatusFunction);
		std::cout << "We're still alive! " << result << std::endl;
		while(!P2K_GetStatus());
		//std::cout << "Found phone." << std::endl;
		Sleep(500);
		char volumeData[130];
		memset(volumeData, 0, 130);
		int freeSpace = P2K_File_VolInfo(volumeData);
		int fileCount = P2K_File_Count();
		std::cout << "Volume name: " << volumeData << std::endl
		<< "Free space: " << freeSpace << std::endl
		<< "File count: " << fileCount << std::endl;
		Sleep(500);

		std::vector<char> buffer(fileCount * 0x200);
		P2K_File_List(&buffer[0], fileCount);
		OutputFileList(buffer, fileCount);

		//access ALARM_CLOCK
		std::cout << "Accessing /a/ALARMCLOCK" << std::endl;
		result = P2K_File_Create("/a/ALARMCLOCK", 0);
		if(result == TRC_P2K_SUCCESS)
		{
			std::vector<char> alarmclock(300);	//size hardcoded;]
			P2K_File_Read(&alarmclock[0], 300);

			std::ofstream fOut("ALARMCLOCK", std::ios::binary);
			fOut.write(&alarmclock[0], alarmclock.size());
			fOut.close();


			P2K_File_Close();

		}
		else
		{
			std::cout << "Failed to access /a/ALARMCLOCK" << std::endl;
		}


		Sleep(500);
		P2K_Restart();
		Sleep(5000);

		DeInitLibrary();
	}
	catch(const std::runtime_error& e)
	{
		std::cout << "Exception " << e.what() << std::endl;
	}
	std::exit(0);
	return 0;
}
