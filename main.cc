#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>

#define WINMINEEXE _T("winmine.exe")

DWORD GetWineProcessId() {
	DWORD mineId = 0;
	HANDLE snapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 entry;
	memset(&entry, 0, sizeof(entry));
	entry.dwSize = sizeof(entry);
	Process32First(snapShot, &entry);
	for (BOOL cond; ; cond = Process32Next(snapShot, &entry)) {
		if (_tcsstr(entry.szExeFile, WINMINEEXE) == NULL)
			continue;
		mineId = entry.th32ProcessID;
		break;
	}
	CloseHandle(snapShot);
	return mineId;
}

int main() {
	DWORD mineId = 0;
	mineId = GetWineProcessId();
	HANDLE hMine = OpenProcess(PROCESS_ALL_ACCESS, NULL, mineId);

	POINT mineMapRect;
	ReadProcessMemory(hMine, (LPCVOID)0x01005334, &mineMapRect.x, 4, NULL);
	ReadProcessMemory(hMine, (LPCVOID)0x01005338, &mineMapRect.y, 4, NULL);

	size_t minMapSize = 32 * mineMapRect.y;
	unsigned char* mineMap = (unsigned char*)malloc(minMapSize);
	memset(mineMap, 0, minMapSize);

	_tprintf_s(_T("cheatwine start\nwine map (width-%d,height-%d)\n"), mineMapRect.x, mineMapRect.y);
	_tprintf_s(_T("please, click one field, then click to continue...\n"));
	_gettchar();
	ReadProcessMemory(hMine, (LPCVOID)0x01005361 /* map */, mineMap, minMapSize, NULL);

	_tprintf_s(_T("\n\nwine map:\n\n"));
	for (size_t i = 0; i < mineMapRect.y; ++i) {
		unsigned char* pos = mineMap + i * 32;
		for (size_t j = 0; j < mineMapRect.x; ++j) {
			if (pos[j] == 0x8f) {
				_tprintf_s(_T("1"));
			}
			else {
				_tprintf_s(_T("0"));
			}
		}
		_tprintf_s(_T("\n"));
	}

	_gettchar();
	return 0;
}