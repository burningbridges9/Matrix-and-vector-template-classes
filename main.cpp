// ArchProgcppVector.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include "mVector.h"
#include "Matrix.h"
#include "TMatrix.h"
#include "TVector.h"
#include <complex>
#include <random>
using namespace std;

int main()
{ 
	
	setlocale(LC_ALL, "Russian");
	
	TMatrix<complex<double>> A(5, 5);
	for (int i = 0; i != 5; i++)
	{
		for (int j = 0; j != 5; j++)
		{
			A[i][j] = complex<double>(rand() % 10, rand() % 5);
		}
	}
	cout << "A = " << endl << A << endl;
	TVector<complex<double>> b(5);
	for (int j = 0; j != 5; j++)
	{
		b[j] = complex<double>(rand() % 10,rand()%5);
	}
	cout << "b ="<< b << endl;
	TVector<complex<double>> x = b/A;
	cout <<"Ответ : x ="<<endl<<x<<endl;
	cout<<"Проверка A * x = " <<endl<< A*x << endl;
	////cout << "inv(A) = " << endl << 1.0 / A << endl;

	
	system("pause");
    return 0;
}

