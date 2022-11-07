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
		"C:\\Users\\сергеичевад\\Source\\Repos\\createProcessWinApi\\Debug\\manipulatingData.exe",// имя исполняемого модуля
		argumentsForCalculation, // командная строка
		NULL, // SD (дескриптор безопасности)
		NULL,   // SD
		FALSE,                       // дескриптор параметра наследования
		0,                      // флажки создания
		NULL,                       // новый блок конфигурации
		NULL,                 // имя текущего каталога
		&si,                // информация предустановки
		&pi // информация о процессе
	);
	WaitForSingleObject(pi.hProcess, INFINITE);

	HANDLE fileStart;
	fileStart = CreateFile(L"Result.txt", // открываемый файл
		GENERIC_READ, // открываем для чтения
		FILE_SHARE_READ, // для совместного чтения
		NULL, // защита по умолчанию
		OPEN_EXISTING, // только существующий файл
		FILE_ATTRIBUTE_NORMAL, // обычный файл
		NULL); // атрибутов шаблона нет
	if (fileStart == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, L"При открытие файла возникла ошибка!", L"Окно программы", MB_OK);
		return 0;
	}
	DWORD countFileSymbols;
	LPCSTR resultFromFile = calloc(100, sizeof(CHAR));
	if (!ReadFile(fileStart, resultFromFile, 100, &countFileSymbols, NULL))
	{
		MessageBox(NULL, L"При чтении данных из файла возникла ошибка!", L"Окно программы", MB_OK);
		return 0;
	}
	else MessageBoxA(NULL, resultFromFile, "Окно программы", MB_OK);
}

float* getCoefficients() // Чтение из файла и запись в массив
{
	HANDLE fileStart;
	fileStart = CreateFile(L"arguments.txt", // открываемый файл
		GENERIC_READ, // открываем для чтения
		FILE_SHARE_READ, // для совместного чтения
		NULL, // защита по умолчанию
		OPEN_EXISTING, // только существующий файл
		FILE_ATTRIBUTE_NORMAL, // обычный файл
		NULL); // атрибутов шаблона нет

	if (fileStart == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, L"При открытие файла возникла ошибка!", L"Окно программы", MB_OK);
		return 0;
	}

	DWORD countFileSymbols;
	LPCSTR argumentsFromFile = calloc(100, sizeof(CHAR));
	if (!ReadFile(fileStart, argumentsFromFile, 100, &countFileSymbols, NULL))
	{
		MessageBox(NULL, L"При чтении данных из файла возникла ошибка!", L"Окно программы", MB_OK);
		return 0;
	}

	CloseHandle(fileStart); //Закрываем дескриптор подключения к файлу
	
	return argumentsFromFile;
	char* argumentsForSplit = calloc(countFileSymbols, sizeof(char)); //Выделяем память исходя из действительного количества символов в файле (countFileSymbols)
	for (int i = 0; i < (int)(countFileSymbols + 1); i++) //Записываем значения в новый указатель
	{
		argumentsForSplit[i] = argumentsFromFile[i];
	}
	free(argumentsFromFile); //Освобождаем старый указатель

	return argumentsForSplit;

}