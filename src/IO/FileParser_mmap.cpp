#include "FileIO.hpp"

#include <windows.h>
#include <iostream>

size_t parseFile_mmap(const std::string& filepath)
{
	// we don't use ifstream instead using CreateFileA()

	HANDLE hFile = CreateFileA(
		filepath.c_str(),                     // file path
		GENERIC_READ,                         // Only reading
		FILE_SHARE_READ,                      // let other programs read it
		NULL,                                 // default security
		OPEN_EXISTING,                        // the file must already exist
		FILE_ATTRIBUTE_NORMAL,                // standard file
		NULL                                  // no template
	);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		std::cerr << "Error: File couldn't Open..\n";
		return 0;
	}

	LARGE_INTEGER fileSize; //cos file is 1GB
	
	if (!GetFileSizeEx(hFile, &fileSize))
	{
		std::cerr << "Failed to get file Size...\n";
		CloseHandle(hFile);
		return 0;
	}



	HANDLE hMapping = CreateFileMappingA(
		hFile,                                          // File handle
		NULL,                                           // default security
		PAGE_READONLY,                                  // only reading
		0, 0,                                           // map the whol file
		NULL	                                        // no name needed
	);

	if (hMapping == NULL)
	{
		std::cerr << "Failed to Create file Mapping..\n";
		CloseHandle(hFile);
		return 0;
	}



	const char* data = (const char*)MapViewOfFile(
		hMapping,                                         // mapping object
		FILE_MAP_READ,                                    // read access
		0, 0,                                             // starting
		0                                                 // entire file
	);

	if (data == NULL)
	{
		std::cerr << "Failed to map view of file..\n";
		CloseHandle(hMapping);
		CloseHandle(hFile);
		return 0;
	}


	size_t lineCount = 0;
	for (LONGLONG i = 0; i < fileSize.QuadPart; i++)
	{
		if (data[i] == '\n')
		{
			lineCount++;
		}
	}

	UnmapViewOfFile(data);
	CloseHandle(hMapping);
	CloseHandle(hFile);

	return lineCount;

}