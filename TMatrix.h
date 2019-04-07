#pragma once
#include <iostream>
#include <algorithm>
#include <type_traits>
#include <complex>
#include "TVector.h"
using namespace std;

template<typename Type>
class TMatrix;

typedef TMatrix<int> MatrixInt;
typedef TMatrix<double> MatrixDouble;
typedef TMatrix<complex<double>> MatrixComplex;

template<typename Type>
ostream &operator<< (ostream &output, const TMatrix<Type> &object)
{
	for (int i = 0; i != object.row; i++)
		output << object.vptr[i];
	output << endl;
	return output;
}


template<typename Type>
istream &operator >> (istream &input, TMatrix<Type> &obj)
{
	for (int i = 0; i < obj.row; i++)
		input >> obj.vptr[i];
	return input;
}

template<typename Type, typename Type_other>
TMatrix<std::common_type_t<Type, Type_other>> operator+(TMatrix<Type> a, TMatrix<Type_other> b)
{
	//cout << typeid(a).name() << endl;
	//cout << typeid(b).name() << endl;
	TMatrix<std::common_type_t<Type, Type_other>> res(a.row, a.col);
	if ((a.row != b.row)|| (a.col != b.col))
	{
		exit(1);
	}
	else
	{
		for (int i = 0; i != a.row; i++)
		{
			for (int j = 0; j != a.col; j++)
				res[i][j] = a[i][j] + b[i][j];
		}
		return res;
	}
}

template<typename Type, typename Type_other>
TMatrix<std::common_type_t<Type, Type_other>> operator-(TMatrix<Type> a, TMatrix<Type_other> b)
{
	TMatrix<std::common_type_t<Type, Type_other> > res(a.row, a.col);
	if ((a.row != b.row) || (a.col != b.col))
	{
		exit(1);
	}
	else
	{
		for (int i = 0; i != a.row; i++)
		{
			for (int j = 0; j != a.col; j++)
				res[i][j] = a[i][j] - b[i][j];
		}
		return res;
	}
}



template<typename Type, typename Type_other>
TMatrix<std::common_type_t<Type, Type_other>> operator*(TMatrix<Type> a, TMatrix<Type_other> b)
{
	TMatrix<std::common_type_t<Type, Type_other>> res(a.get_col(), b.get_row());
	if (a.get_col() != b.get_row())
	{
		exit(1);
	}
	else
	{
		for (int i = 0; i != a.row; i++)
		{
			for (int j = 0; j != b.col; j++)
			{
				res[i][j] = 0.0;
				for (int k = 0; k != a.col; k++)
					res[i][j] += a[i][k] * b[k][j];
			}
		}
		return res;
	}
}






template<typename Type, typename Type_other>
TVector<std::common_type_t<Type, Type_other>> operator*(TMatrix<Type> a, TVector<Type_other> b)
{
	TVector<std::common_type_t<Type, Type_other>> res(a.row);
	if (a.col != b.get_size())
	{
		cerr << "wrong size";
		exit(1);
	}
	else
	{
		for (int i = 0; i != a.row; i++)
		{
			res[i] = 0;
			for (int j = 0; j != a.col; j++)
			{
				res[i] += a[i][j] * b[j];
			}
		}
	}
	return res;
}

template <typename Type>
TMatrix<Type> & operator / (double x, TMatrix<Type> & other)
{
	TMatrix<Type> *inv = new TMatrix<Type>(other.row, other.col);
	*inv = inv->inverse(other);
	return *inv;
}

template <typename Type, typename U>
TVector<std::common_type_t<Type, U>> & operator / (TVector<U> & b, TMatrix<Type> & other)
{
	TMatrix<Type> inv(other.row, other.col);
	inv = inv.inverse(other);
	TVector<std::common_type_t<Type, U>> *res= new TVector<std::common_type_t<Type, U>>(other.row);
	*res = inv*b;
	return *res;
}



template<typename Type>
class TMatrix
{
	int row, col;
	TVector<Type> * vptr;
public:
	TMatrix() {}
	TMatrix(int row, int col)
	{
		this->row = row;
		this->col = col;
		vptr = new TVector<Type>[row];
		for (int i = 0; i != row; i++)
		{
			vptr[i] = TVector<Type>(col);
		}
		for (int i = 0; i != row; i++)
		{
			for (int j = 0; j != col; j++)
				vptr[i][j] = 0.0;
		}
	}
	explicit TMatrix(int row) // единичная
	{
		this->col = this->row = row;

		vptr = new TVector<Type>[row];
		for (int i = 0; i != row; i++)
		{
			vptr[i] = TVector<Type>(row);
		}
		for (int i = 0; i != row; i++)
		{
			for (int j = 0; j != row; j++)
				if (i == j)
					vptr[i][j] = 1.0;
				else vptr[i][j] = 0.0;
		}
	}

	~TMatrix()
	{
		delete[] vptr;
		vptr = nullptr;
	}

	TVector<Type> & operator [] (int num)
	{
		try {
			if ((num >= 0) && (num < row))
				return vptr[num];
			else
				throw "size error";
		}
		catch (char *str)
		{
			cout << str << endl;
		}
	}

	TMatrix(const TMatrix<Type> & other)
	{
		this->row = other.row;
		this->col = other.col;

		this->vptr = new TVector<Type>[row];
		for (int i = 0; i != row; i++)
			this->vptr[i] = TVector<Type>(col);
		for (int i = 0; i != row; i++)
			this->vptr[i] = other.vptr[i];
	}

	Type determinant()
	{
		if (this->col != this->row)
		{
			exit(1);
		}
		TMatrix<Type> B = *this;
		// приведение к верхне треугольному виду
		for (int step = 0; step != row; step++)
		{
			for (int r = step + 1; r != row; r++)
			{
				if (B[step][step] != 0.0)
				{
					Type coef = -B[r][step] / B[step][step];
					for (int c = step; c != row; c++)
						B[r][c] += B[step][c] * coef;
					//cout << B<<endl;
				}
				else
				{
					cerr << "матрица вырождена" << endl;
					system("pause");
					exit(1);
				}

			}
		}
		Type det = 1.0;
		for (int i = 0; i != row; i++)
		{
			det *= B[i][i];
		}
		return det;

	}
	int get_row()
	{
		return row;
	}
	int get_col()
	{
		return col;
	}

	template <typename Type, typename Type_other>
	friend TMatrix <std::common_type_t<Type, Type_other>>
		operator +(TMatrix<Type> a, TMatrix<Type_other> b);

	template <typename Type, typename Type_other>
	friend TMatrix <std::common_type_t<Type, Type_other>>
		operator -(TMatrix<Type> a, TMatrix<Type_other> b);

	template <typename Type, typename Type_other>
	friend TMatrix <std::common_type_t<Type, Type_other>>
		operator *(TMatrix<Type> a, TMatrix<Type_other> b);

	
	template<typename Type>
	TMatrix<Type> & inverse(TMatrix<Type> & other)
	{
		TMatrix<Type> * inver = new TMatrix<Type>(other.get_row(), other.get_col());
		TMatrix<Type> temp = other;
		int ki, kj;
		for (int indexi = 0; indexi != temp.row; indexi++)
		{
			for (int indexj = 0; indexj != temp.col; indexj++)
			{
				ki = indexi; kj = indexj;
				TMatrix<Type> adj(temp.row - 1, temp.col - 1);
				int si = 0, sj = 0;
				for (int i = 0; i != temp.row; i++)
				{
					if (i != ki)
					{
						sj = 0;
						for (int j = 0; j != temp.col; j++)
						{
							if (j != kj)
							{
								adj[si][sj] = temp[i][j];
								sj++;
							}
						}
						si++;
					}
				}
				//cout << "A" << indexi+1 << indexj+1 << "  =  " <<endl<< adj<<endl;
				(*inver)[indexi][indexj] = adj.determinant()*pow(-1.0, indexi + indexj);
			}
		}

		for (int indexi = 0; indexi != temp.row-1; indexi++)
		{
			for (int indexj = indexi+1; indexj != temp.col; indexj++)
			{
				swap((*inver)[indexj][indexi], (*inver)[indexi][indexj]);
			}
		}
		Type det = temp.determinant();
		for (int indexi = 0; indexi != temp.row; indexi++)
		{
			for (int indexj = 0; indexj != temp.col; indexj++)
			{
				(*inver)[indexi][indexj] = (*inver)[indexi][indexj] * 1.0 / det;
			}
		}
		return (*inver);
	}
	
	

	template <typename Type, typename Type_other>
	friend TVector<std::common_type_t<Type, Type_other>>
		operator *(TMatrix<Type> a, TVector<Type_other> b);
	
	template<typename U>
	operator TMatrix<U>()
	{
		TMatrix<U> temp(row, col);
		for (int i = 0; i != row; i++)
		{
			for (int j = 0; j != col; j++)
			{
				temp[i][j] = static_cast<U>(this->vptr[i][j]);
			}
		}
		return temp;
	}

	template <typename Type, typename U>
	friend TVector<std::common_type_t<Type, U>> & operator / (TVector<U> & b, TMatrix<Type> & other);
	

	template<typename Type>
	friend TMatrix<Type> & operator / (double x, TMatrix<Type> & other);

	

	template<typename Type>
	TMatrix<Type> operator*(Type b)
	{
		for (int i = 0; i != row; i++)
		{
			for(int j = 0 ; j!=col;j++)
				vptr[i][j] *= b;
		}
		return (*this);
	}

	


	TMatrix<Type>& operator= (TMatrix<Type> & other)
	{
		if (this != &other)
		{
			
			delete[] vptr;
			col = other.col;
			row = other.row;
			vptr = new TVector<Type>[row];
			for (int i = 0; i != row; i++)
				vptr[i] = TVector<Type>(col);
			for (int i = 0; i != row; i++)
				for (int j = 0; j != col; j++)
					vptr[i][j] = other.vptr[i][j];
		}
		return (*this);
	}
	friend ostream &::operator<< <Type> (ostream &output, const TMatrix<Type> &object);
	friend istream &operator >> <Type> (istream &input, TMatrix<Type> &obj);
	void show()
	{
		for (int i = 0; i != row; i++)
		{
			for (int j = 0; j != col; j++)
			{
				cout << vptr[i][j] << "  ";
			}
			cout << endl;
		}
	}
};


