#include "Gallery.h"
#include <iostream>
#include <windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>
#pragma comment(lib, "User32.lib")

using namespace assignment2;

void DisplayErrorBox(LPTSTR lpszFunction);

Gallery::Gallery(): dir("ply_files"), _i(0)
{
	auto filenames = LoadFilesWin32(dir);
	for (auto f : filenames)
	{
		PlyEntity entity;
		entity.first = f;
		entity.second = nullptr;
		allPlys.push_back(entity);
	}
	std::cout << "total ply number=" << allPlys.size() << std::endl;
	load();
}

void Gallery::load()
{
	std::string f = dir + "/" + allPlys[_i].first;
	allPlys[_i].second = Ply::Load(f);
}

Ply& Gallery::current()
{
	if (allPlys[_i].second == nullptr)
	{
		load();
	}
	std::cout << "current file=" << allPlys[_i].first << std::endl;
	return *allPlys[_i].second;
}

Ply& Gallery::next()
{
	++_i %= allPlys.size();
	return current();
}

Ply& Gallery::prev()
{
	_i = (_i > 0) ? _i - 1 : allPlys.size()-1;
	return current();
}

Gallery::~Gallery()
{
}

std::vector<std::string> 
Gallery::LoadFilesWin32(const std::string & dir)
{
	// copied from MS website
	// https://msdn.microsoft.com/en-us/library/windows/desktop/aa365200(v=vs.85).aspx
	WIN32_FIND_DATA ffd;
	LARGE_INTEGER filesize;
	TCHAR szDir[MAX_PATH];
	size_t length_of_arg;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;
	StringCchCopy(szDir, MAX_PATH, dir.c_str());
	StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

	std::vector<std::string> filenames;

	// Find the first file in the directory.

	hFind = FindFirstFile(szDir, &ffd);

	if (INVALID_HANDLE_VALUE == hFind)
	{
		DisplayErrorBox(TEXT("LoadFilesWin32"));
		std::cout << "dwError=" << dwError << std::endl;;
	}

	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			_tprintf(TEXT("  %s   <DIR>\n"), ffd.cFileName);
		}
		else
		{
			filesize.LowPart = ffd.nFileSizeLow;
			filesize.HighPart = ffd.nFileSizeHigh;
			_tprintf(TEXT("  %s   %ld bytes\n"), 
				ffd.cFileName, filesize.QuadPart);
			filenames.push_back(std::string(ffd.cFileName));
		}
	} while (FindNextFile(hFind, &ffd) != 0);

	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES)
	{
		DisplayErrorBox(TEXT("LoadFilesWin32"));
	}

	FindClose(hFind);
	return filenames;
}

void DisplayErrorBox(LPTSTR lpszFunction)
{
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	// Display the error message and clean up

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
}