#include<windows.h>
#include<iostream>
#include<time.h>

using namespace std;

/* 9 вариант:  Поток worker должен найти значение факториала нечетных элементов массива. 
Для синхронизации потоков использовать семафор. */

int n;
int *arr;
float sum = 0;
HANDLE hSemaphore;

long double factorial(int num);

DWORD WINAPI worker(LPVOID lpData)
{
	setlocale(LC_ALL, "Russian");

	WaitForSingleObject(hSemaphore, INFINITE);

	//поиск среднего значения элементов массива
	for (int i = 0; i < n; i++)
	{
		sum += arr[i];
		Sleep(12);
	}
	cout << "\nСреднее значение элементов массива: " << sum / n << endl << endl;

	//поиск факториала нечетных значений массива
	for (int i = 0; i < n; i++)
	{
		if (arr[i] % 2 != 0)
		{
			cout << "Факториал числа " << arr[i] << " = " << factorial(arr[i]) << endl;
		}
	}

	ReleaseSemaphore(hSemaphore, 1, NULL);
	return sum;
}


int main()
{
	setlocale(LC_ALL, "Russian");

	//создание массива целых чисел
	cout << "Введите размер массива: ";
	cin >> n;
	arr = new int[n];

	//заполнение массива случайными числами
	//srand(time(NULL));
	for (int i = 0; i < n; i++)
	{
		//так как по условию задания необходимо найти факториал нечетных элементов массива, диапазон генерируемых случайных чисел не должен быть слишком большим
		arr[i] = rand() % 10;
	}

	//запуск потока worker
	hSemaphore = CreateSemaphore(NULL, 1, 1, "Semaphore");
	DWORD threadID;
	HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)worker, NULL, 0, &threadID);

	//сортировка "пузырьком"
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

	//вывод отсортированного массива
	cout << "\nОтсортированный массив:\n\n";
	for (int i = 0; i < n; i++) 
	{
		cout << "arr[" << i << "] = " << arr[i] << endl;
	}
	ReleaseSemaphore(hSemaphore, 1, NULL);

	//завершение потока worker и закрытие семафора
	//WaitForSingleObject(hSemaphore, INFINITE);
	WaitForSingleObject(hThread, INFINITE);
	//ReleaseSemaphore(hSemaphore, 1, NULL);
	CloseHandle(hSemaphore);
	CloseHandle(hThread);

	//подсчет количества элементов в массиве, значение которых больше среднего значения элементов массива
	int count = 0;
	for (int i = 0; i < n; i++)
	{
		if (arr[i] > sum / n)
		{
			count++;
		}
	}
	cout << "\nКоличество элементов массива, значение которых выше среднего: " << count << endl << endl;

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
