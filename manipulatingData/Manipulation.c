#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>

void decisionCompleteEquation(float*, char*, int);
void decisionIncompleteEquation(float*, char*, int);
void writeResultToFile(char*, int);

int main(int argc, char** argv)
{
    float* argumentsForCalculation = calloc(3, sizeof(float));
	/*argumentsForCalculation[0] = atof(argv[1]);
	argumentsForCalculation[1] = atof(argv[2]);
	argumentsForCalculation[2] = atof(argv[3]);*/
	int i;
	for (i = 0; i < argc; i++) {
		printf("%s\n", argv[i]);
	}
	printf("WASSSSUP");
	/*char* result = "";
	int resultCountSymbols = 0;

	if (argumentsForCalculation[0] == 0)
	{
		result = calloc(strlen("��������� �� �������� ����������, �.�. a == 0"), sizeof(char));
		sprintf(result, "��������� �� �������� ����������, �.�. a == 0");
		resultCountSymbols = strlen(result);
		writeResultToFile(result, resultCountSymbols);
	}
	else if (argumentsForCalculation[1] != 0 && argumentsForCalculation[2] != 0) decisionCompleteEquation(argumentsForCalculation, result, resultCountSymbols);
	else decisionIncompleteEquation(argumentsForCalculation, result, resultCountSymbols);*/
}

void decisionCompleteEquation(float* forCoefficients, char* forAnswer, int forAnswerCountSymbols) //���������� ��� ������ ���������� ���������
{
	float discriminant;
	float x1, x2;

	discriminant = pow(forCoefficients[1], 2) - 4 * forCoefficients[0] * forCoefficients[2];

	if (discriminant > 0)
	{
		x1 = (-forCoefficients[1] + sqrt(discriminant)) / (2 * forCoefficients[0]);
		x2 = (-forCoefficients[1] - sqrt(discriminant)) / (2 * forCoefficients[0]);

		forAnswer = calloc(strlen("������������ = %f, x1 = %f, x2 = %f", discriminant, x1, x2), sizeof(char));
		sprintf(forAnswer, "������������ = %f, x1 = %f, x2 = %f", discriminant, x1, x2);
		forAnswerCountSymbols = strlen(forAnswer);
	}
	else if (discriminant == 0)
	{
		x1 = (-forCoefficients[1] + sqrt(discriminant)) / (2 * forCoefficients[0]);

		forAnswer = calloc(strlen("������������ = %f, x1 = %f", discriminant, x1), sizeof(char));
		sprintf(forAnswer, "������������ = %f, x1 = %f", discriminant, x1);
		forAnswerCountSymbols = strlen(forAnswer);
	}
	else
	{
		forAnswer = calloc(strlen("������ ���!"), sizeof(char));
		sprintf(forAnswer, "������ ���!");
		forAnswerCountSymbols = strlen(forAnswer);
	}
	writeResultToFile(forAnswer, forAnswerCountSymbols);
}

void decisionIncompleteEquation(float* forCoefficients, char* forAnswer, int forAnswerCountSymbols) //���������� ��� �������� ���������� ���������
{
	float x1, x2;

	if (forCoefficients[1] == 0 && forCoefficients[2] != 0)
	{
		if (-(forCoefficients[2] / forCoefficients[0]) >= 0)
		{
			x1 = sqrt(-(forCoefficients[2] / forCoefficients[0]));
			x2 = -x1;

			forAnswer = calloc(strlen("�������� ���������� ���������, b = 0, x1 = %f, x2 = %f", x1, x2), sizeof(char));
			sprintf(forAnswer, "�������� ���������� ���������, b = 0, x1 = %f, x2 = %f", x1, x2);
			forAnswerCountSymbols = strlen(forAnswer);
		}
		else
		{
			forAnswer = calloc(strlen("��� ������, �.�. -(c/a) < 0"), sizeof(char));
			sprintf(forAnswer, "��� ������, �.�. -(c/a) < 0");
			forAnswerCountSymbols = strlen(forAnswer);
		}
	}
	else if (forCoefficients[2] == 0 && forCoefficients[1] != 0)
	{
		x1 = 0;
		x2 = -(forCoefficients[1] / forCoefficients[0]);

		forAnswer = calloc(strlen("�������� ���������� ���������, c = 0, x1 = %f, x2 = %f", x1, x2), sizeof(char));
		sprintf(forAnswer, "�������� ���������� ���������, c = 0, x1 = %f, x2 = %f", x1, x2);
		forAnswerCountSymbols = strlen(forAnswer);
	}
	else
	{
		x1 = 0;

		forAnswer = calloc(strlen("�������� ���������� ���������, c = 0 b = 0, x = %f", x1), sizeof(char));
		sprintf(forAnswer, "�������� ���������� ���������, c = 0 b = 0, x = %f", x1);
		forAnswerCountSymbols = strlen(forAnswer);
	}
	writeResultToFile(forAnswer, forAnswerCountSymbols);
}

void writeResultToFile(char* forAnswer, int forCountSymbols) //������ ������ � ����
{
	HANDLE fileResult = CreateFile(L"Result.txt", // ����������� ����
		GENERIC_WRITE, // ����������� ��� ������
		FILE_SHARE_WRITE, // ��������� �� ������������
		NULL, // ������ �� ���������
		CREATE_ALWAYS, // ������������ ������������
		FILE_ATTRIBUTE_NORMAL, // ����������� ����/����� I/O
		NULL); // ��������� ������� ���
	DWORD countFileSymbols;
	WriteFile(fileResult, forAnswer, forCountSymbols, &countFileSymbols, NULL);
	CloseHandle(fileResult);
}