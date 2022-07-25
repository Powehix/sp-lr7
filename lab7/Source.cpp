#include<windows.h>
#include<iostream>
#include<time.h>

using namespace std;

/* 9 �������:  ����� worker ������ ����� �������� ���������� �������� ��������� �������. 
��� ������������� ������� ������������ �������. */

int n;
int *arr;
float sum = 0;
HANDLE hSemaphore;

long double factorial(int num);

DWORD WINAPI worker(LPVOID lpData)
{
	setlocale(LC_ALL, "Russian");

	WaitForSingleObject(hSemaphore, INFINITE);

	//����� �������� �������� ��������� �������
	for (int i = 0; i < n; i++)
	{
		sum += arr[i];
		Sleep(12);
	}
	cout << "\n������� �������� ��������� �������: " << sum / n << endl << endl;

	//����� ���������� �������� �������� �������
	for (int i = 0; i < n; i++)
	{
		if (arr[i] % 2 != 0)
		{
			cout << "��������� ����� " << arr[i] << " = " << factorial(arr[i]) << endl;
		}
	}

	ReleaseSemaphore(hSemaphore, 1, NULL);
	return sum;
}


int main()
{
	setlocale(LC_ALL, "Russian");

	//�������� ������� ����� �����
	cout << "������� ������ �������: ";
	cin >> n;
	arr = new int[n];

	//���������� ������� ���������� �������
	//srand(time(NULL));
	for (int i = 0; i < n; i++)
	{
		//��� ��� �� ������� ������� ���������� ����� ��������� �������� ��������� �������, �������� ������������ ��������� ����� �� ������ ���� ������� �������
		arr[i] = rand() % 10;
	}

	//������ ������ worker
	hSemaphore = CreateSemaphore(NULL, 1, 1, "Semaphore");
	DWORD threadID;
	HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)worker, NULL, 0, &threadID);

	//���������� "���������"
	int temp;
	WaitForSingleObject(hSemaphore, INFINITE);
	for (int i = 0; i < n - 1; i++) 
	{
		for (int j = 0; j < n - i - 1; j++)
		{
			if (arr[j] > arr[j + 1]) 
			{
				temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}

	//����� ���������������� �������
	cout << "\n��������������� ������:\n\n";
	for (int i = 0; i < n; i++) 
	{
		cout << "arr[" << i << "] = " << arr[i] << endl;
	}
	ReleaseSemaphore(hSemaphore, 1, NULL);

	//���������� ������ worker � �������� ��������
	//WaitForSingleObject(hSemaphore, INFINITE);
	WaitForSingleObject(hThread, INFINITE);
	//ReleaseSemaphore(hSemaphore, 1, NULL);
	CloseHandle(hSemaphore);
	CloseHandle(hThread);

	//������� ���������� ��������� � �������, �������� ������� ������ �������� �������� ��������� �������
	int count = 0;
	for (int i = 0; i < n; i++)
	{
		if (arr[i] > sum / n)
		{
			count++;
		}
	}
	cout << "\n���������� ��������� �������, �������� ������� ���� ��������: " << count << endl << endl;

	delete[]arr;
	system("PAUSE");
	return 0;
}

long double factorial(int num)
{
	if (num < 0)
	{
		return 0;
	}
	if (num == 0)
	{
		return 1;
	}
	else
	{
		return num * factorial(num - 1);
	}
}
