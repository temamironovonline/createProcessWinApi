#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>
#include <string.h>

float* getCoefficients();


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	char* argumentsForCalculation = getCoefficients();
	HANDLE hProcess = NULL;
	HANDLE hThread = NULL;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));

	BOOL newProcess;
	newProcess = CreateProcessA(
		"C:\\Users\\�����������\\Source\\Repos\\createProcessWinApi\\Debug\\manipulatingData.exe",// ��� ������������ ������
		argumentsForCalculation, // ��������� ������
		NULL, // SD (���������� ������������)
		NULL,   // SD
		FALSE,                       // ���������� ��������� ������������
		0,                      // ������ ��������
		NULL,                       // ����� ���� ������������
		NULL,                 // ��� �������� ��������
		&si,                // ���������� �������������
		&pi // ���������� � ��������
	);
	WaitForSingleObject(pi.hProcess, INFINITE);

	HANDLE fileStart;
	fileStart = CreateFile(L"Result.txt", // ����������� ����
		GENERIC_READ, // ��������� ��� ������
		FILE_SHARE_READ, // ��� ����������� ������
		NULL, // ������ �� ���������
		OPEN_EXISTING, // ������ ������������ ����
		FILE_ATTRIBUTE_NORMAL, // ������� ����
		NULL); // ��������� ������� ���
	if (fileStart == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, L"��� �������� ����� �������� ������!", L"���� ���������", MB_OK);
		return 0;
	}
	DWORD countFileSymbols;
	LPCSTR resultFromFile = calloc(100, sizeof(CHAR));
	if (!ReadFile(fileStart, resultFromFile, 100, &countFileSymbols, NULL))
	{
		MessageBox(NULL, L"��� ������ ������ �� ����� �������� ������!", L"���� ���������", MB_OK);
		return 0;
	}
	else MessageBoxA(NULL, resultFromFile, "���� ���������", MB_OK);
}

float* getCoefficients() // ������ �� ����� � ������ � ������
{
	HANDLE fileStart;
	fileStart = CreateFile(L"arguments.txt", // ����������� ����
		GENERIC_READ, // ��������� ��� ������
		FILE_SHARE_READ, // ��� ����������� ������
		NULL, // ������ �� ���������
		OPEN_EXISTING, // ������ ������������ ����
		FILE_ATTRIBUTE_NORMAL, // ������� ����
		NULL); // ��������� ������� ���

	if (fileStart == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, L"��� �������� ����� �������� ������!", L"���� ���������", MB_OK);
		return 0;
	}

	DWORD countFileSymbols;
	LPCSTR argumentsFromFile = calloc(100, sizeof(CHAR));
	if (!ReadFile(fileStart, argumentsFromFile, 100, &countFileSymbols, NULL))
	{
		MessageBox(NULL, L"��� ������ ������ �� ����� �������� ������!", L"���� ���������", MB_OK);
		return 0;
	}

	CloseHandle(fileStart); //��������� ���������� ����������� � �����
	
	return argumentsFromFile;
	char* argumentsForSplit = calloc(countFileSymbols, sizeof(char)); //�������� ������ ������ �� ��������������� ���������� �������� � ����� (countFileSymbols)
	for (int i = 0; i < (int)(countFileSymbols + 1); i++) //���������� �������� � ����� ���������
	{
		argumentsForSplit[i] = argumentsFromFile[i];
	}
	free(argumentsFromFile); //����������� ������ ���������

	return argumentsForSplit;

}