#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>
#include <string.h>

float* getCoefficients();
int main()
{
	float* argumentsForCalculation = getCoefficients();
	//printf("%f %f %f", argumentsForCalculation[0], argumentsForCalculation[1], argumentsForCalculation[2]);

	HANDLE hProcess = NULL;
	HANDLE hThread = NULL;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	BOOL newProcess;
	LPWSTR arguments = calloc(20, sizeof(LPWSTR));
	LPWSTR arg = L"12 ";
	LPWSTR arg1 = L"13 ";
	LPWSTR arg2 = L"14 ";

	sprintf(arguments, L"%f %f %f", argumentsForCalculation[0], argumentsForCalculation[1], argumentsForCalculation[2]);
	newProcess = CreateProcess(
		L"C:\\Users\\aserg\\source\\repos\\createProcessWinApi\\x64\\Debug\\manipulatingData.exe",// имя исполняемого модуля
		arguments, // командная строка
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
}
//int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
//{
//	float* argumentsForCalculation = getCoefficients();
//	printf("%s", argumentsForCalculation);
//	char* result = "";
//	int resultCountSymbols = 0;
//
//	if (argumentsForCalculation[0] == 0)
//	{
//		result = calloc(strlen("Уравнение не является квадратным, т.к. a == 0"), sizeof(char));
//		sprintf(result, "Уравнение не является квадратным, т.к. a == 0");
//		resultCountSymbols = strlen(result);
//		writeResultToFile(result, resultCountSymbols);
//	}
//	else if (argumentsForCalculation[1] != 0 && argumentsForCalculation[2] != 0) decisionCompleteEquation(argumentsForCalculation, result, resultCountSymbols);
//	else decisionIncompleteEquation(argumentsForCalculation, result, resultCountSymbols);
//}

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

	LPCSTR* argumentsForSplit = calloc(countFileSymbols, sizeof(LPCSTR)); //Выделяем память исходя из действительного количества символов в файле (countFileSymbols)
	for (int i = 0; i < (int)(countFileSymbols + 1); i++) //Записываем значения в новый указатель
	{
		argumentsForSplit[i] = argumentsFromFile[i];
	}
	free(argumentsFromFile); //Освобождаем старый указатель

	char* argumentsForConverting = strtok(argumentsForSplit, " ");
	float* arguments = calloc(3, sizeof(float));

	for (int i = 0; argumentsForConverting != NULL; i++)
	{
		arguments[i] = atof(argumentsForConverting);
		argumentsForConverting = strtok(NULL, " ");
	}

	return arguments;
}