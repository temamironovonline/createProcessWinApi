# Использование процессов

## **Что делает программа?**

Программа решает квадратные уравнения следующим образом:
+ В главном процессе считывается файл с аргументами
  + В функции `getCoefficients()` открываем файл для чтения
    ```
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
    ```
  + После закрываем дескриптор подключения к файлу и считываем данные в массив
    ```
    CloseHandle(fileStart); //Закрываем дескриптор подключения к файлу
	
	return argumentsFromFile;
	char* argumentsForSplit = calloc(countFileSymbols, sizeof(char)); //Выделяем память исходя из действительного количества символов в файле (countFileSymbols)
	for (int i = 0; i < (int)(countFileSymbols + 1); i++) //Записываем значения в новый указатель
	{
		argumentsForSplit[i] = argumentsFromFile[i];
	}
	free(argumentsFromFile); //Освобождаем старый указатель

	return argumentsForSplit;
    ```
  + Создаем дочерний процесс и отправляем полученные аргументы
    ```
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
    ```
+ В дочернем процессе проходят вычисления корней квадратного уравнения
  + Присваиваются переданные аргументы в массив и проверяется на соответствие их нулю. Если `a = 0`, то уравнение не является квадратным и файл отправляется соответствующее сообщение. Если `b = 0` или `c = 0`, то выполняется функция для поиска корней неполного квадратного уравнения `decisionIncompleteEquation()`, если аргументы не равны нулю, тогда выполняется функция для поиска корней полного квадратного уравнения  `decisionСompleteEquation()`
    ```
    int main(int argc, char** argv)
    {
        float* argumentsForCalculation = calloc(3, sizeof(float));


        argumentsForCalculation[0] = atof(argv[0, 0]);
        argumentsForCalculation[1] = atof(argv[0, 1]);
        argumentsForCalculation[2] = atof(argv[0, 2]);

        char* result = "";
        int resultCountSymbols = 0;

        if (argumentsForCalculation[0] == 0)
        {
            result = calloc(strlen("Уравнение не является квадратным, т.к. a == 0"), sizeof(char));
            sprintf(result, "Уравнение не является квадратным, т.к. a == 0");
            resultCountSymbols = strlen(result);
            writeResultToFile(result, resultCountSymbols);
        }
        else if (argumentsForCalculation[1] != 0 && argumentsForCalculation[2] != 0) decisionCompleteEquation(argumentsForCalculation, result, resultCountSymbols);
        else decisionIncompleteEquation(argumentsForCalculation, result, resultCountSymbols);
    }
    ```

  + `decisionCompleteEquation()`, аргументами которого являются `char* forCoefficients` - сами коэффициенты, `char* forAnswer` - указатель для ответа, который будет записан в файл, `int forAnswerCountSymbols` - размер указателя для ответа (значение будет присваиваться в зависимости от длины строки ответа)

    ```
    void decisionCompleteEquation(float* forCoefficients, char* forAnswer, int forAnswerCountSymbols) //Вычисления при полном квадратном уравнении
        {
            float discriminant;
            float x1, x2;

            discriminant = pow(forCoefficients[1], 2) - 4 * forCoefficients[0] * forCoefficients[2];

            if (discriminant > 0)
            {
                x1 = (-forCoefficients[1] + sqrt(discriminant)) / (2 * forCoefficients[0]);
                x2 = (-forCoefficients[1] - sqrt(discriminant)) / (2 * forCoefficients[0]);

                forAnswer = calloc(strlen("Дискриминант = %f, x1 = %f, x2 = %f", discriminant, x1, x2), sizeof(char));
                sprintf(forAnswer, "Дискриминант = %f, x1 = %f, x2 = %f", discriminant, x1, x2);
                forAnswerCountSymbols = strlen(forAnswer);
            }
            else if (discriminant == 0)
            {
                x1 = (-forCoefficients[1] + sqrt(discriminant)) / (2 * forCoefficients[0]);

                forAnswer = calloc(strlen("Дискриминант = %f, x1 = %f", discriminant, x1), sizeof(char));
                sprintf(forAnswer, "Дискриминант = %f, x1 = %f", discriminant, x1);
                forAnswerCountSymbols = strlen(forAnswer);
            }
            else
            {
                forAnswer = calloc(strlen("Корней нет!"), sizeof(char));
                sprintf(forAnswer, "Корней нет!");
                forAnswerCountSymbols = strlen(forAnswer);
            }
            writeResultToFile(forAnswer, forAnswerCountSymbols);
    }
    ```

  + `decisionCompleteEquation()`, аргументами которого являются `char* forCoefficients` - сами коэффициенты, `char* forAnswer` - указатель для ответа, который будет записан в файл, `int forAnswerCountSymbols` - размер указателя для ответа (значение будет присваиваться в зависимости от длины строки ответа)
    ```
    void decisionIncompleteEquation(float* forCoefficients, char* forAnswer, int forAnswerCountSymbols) //Вычисления при неполном квадратном уравнении
    {
	float x1, x2;

	if (forCoefficients[1] == 0 && forCoefficients[2] != 0)
	{
		if (-(forCoefficients[2] / forCoefficients[0]) >= 0)
		{
			x1 = sqrt(-(forCoefficients[2] / forCoefficients[0]));
			x2 = -x1;

			forAnswer = calloc(strlen("Неполное квадратное уравнение, b = 0, x1 = %f, x2 = %f", x1, x2), sizeof(char));
			sprintf(forAnswer, "Неполное квадратное уравнение, b = 0, x1 = %f, x2 = %f", x1, x2);
			forAnswerCountSymbols = strlen(forAnswer);
		}
		else
		{
			forAnswer = calloc(strlen("Нет корней, т.к. -(c/a) < 0"), sizeof(char));
			sprintf(forAnswer, "Нет корней, т.к. -(c/a) < 0");
			forAnswerCountSymbols = strlen(forAnswer);
		}
	}
	else if (forCoefficients[2] == 0 && forCoefficients[1] != 0)
	{
		x1 = 0;
		x2 = -(forCoefficients[1] / forCoefficients[0]);

		forAnswer = calloc(strlen("Неполное квадратное уравнение, c = 0, x1 = %f, x2 = %f", x1, x2), sizeof(char));
		sprintf(forAnswer, "Неполное квадратное уравнение, c = 0, x1 = %f, x2 = %f", x1, x2);
		forAnswerCountSymbols = strlen(forAnswer);
	}
	else
	{
		x1 = 0;

		forAnswer = calloc(strlen("Неполное квадратное уравнение, c = 0 b = 0, x = %f", x1), sizeof(char));
		sprintf(forAnswer, "Неполное квадратное уравнение, c = 0 b = 0, x = %f", x1);
		forAnswerCountSymbols = strlen(forAnswer);
	}
	writeResultToFile(forAnswer, forAnswerCountSymbols);
    }
    ```
  + В конце выполнения каждой функции вызывается функция `writeResultToFile()`, аргументами которого являются `char* forAnswer` - сам ответ, `int forCountSymbols` - количество символов в ответе
    ```
    void writeResultToFile(char* forAnswer, int forCountSymbols) //Запись ответа в файл
    {
        HANDLE fileResult = CreateFile(L"Result.txt", // создаваемый файл
            GENERIC_WRITE, // открывается для записи
            FILE_SHARE_WRITE, // совместно не используется
            NULL, // защита по умолчанию
            CREATE_ALWAYS, // переписывает существующий
            FILE_ATTRIBUTE_NORMAL, // асинхронный ввод/вывод I/O
            NULL); // атрибутов шаблона нет
        DWORD countFileSymbols;
        WriteFile(fileResult, forAnswer, forCountSymbols, &countFileSymbols, NULL);
        CloseHandle(fileResult);
    }
    ```
+ После выполнения вычислений, программа возвращается в главный процесс, в котором считывает файл с ответом и выводит ответ пользователю в виде окна
    ```
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
    ```

___

Введем следующие аргументы `a = 1`, `b = 4`, `c = 2` и посмотрим результат:

![](https://github.com/temamironovonline/createProcessWinApi/blob/master/resultScreen.jpg)

Связаться можно:
+ ВК: https://vk.com/id_sergeichev

По другим вопросам:
+ https://ya.ru/



