#include "stdafx.h"
#include "Utilities.h"
#include <Windows.h>
#include <iostream>

std::string path::GetAbsPath(const char* directory)
{
	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::string strBuffer = buffer;
	auto pos = strBuffer.find_last_of("\\/");
	return strBuffer.substr(0, pos) + "\\" + directory;
}

