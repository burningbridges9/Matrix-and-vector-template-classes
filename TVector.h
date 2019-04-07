#pragma once
#include <iostream>
#include <type_traits>
using namespace std;

template <typename Type>
class TVector;

typedef TVector<int> VectorInt;
typedef TVector<double> VectorDouble;
typedef TVector<complex<double>> VectorComplex;

template<typename Type>
ostream & operator<< (ostream & output, const TVector<Type>& object)
{
	for (int i = 0; i != object.size; i++)
	{
		output << object.ptr[i] << " ";
	}
	output << endl;
	return output;
}

template<typename Type>
istream & operator >> (istream & input, TVector<Type> & obj)
{
	for (int i = 0; i < obj.size; i++)
		input >> obj.ptr[i];
	return input;
}

template<typename Type, typename Type_other>
TVector<std::common_type_t<Type, Type_other>> operator + (TVector<Type> &a, TVector<Type_other> &b)
{
	TVector<std::common_type_t<Type, Type_other>> res(a.size);
	if (a.size != b.size)
	{
		exit(1);
	}
	else
	{
		for (int i = 0; i != a.size; i++)
		{
			res[i] = a[i] + b[i];
		}
		return res;
	}
}

template<typename Type, typename Type_other>
TVector<std::common_type_t<Type, Type_other>> operator - (TVector<Type> &a, TVector<Type_other> &b)
{
	TVector<std::common_type_t<Type, Type_other>> res(a.size);
	if (a.size != b.size)
	{
		exit(1);
	}
	else
	{
		for (int i = 0; i != a.size; i++)
		{
			res[i] = a[i] - b[i];
		}
		return res;
	}
}

template<typename Type, typename Type_other>
TVector<std::common_type_t<Type, Type_other>> operator * (TVector<Type> &a, TVector<Type_other> &b)
{
	TVector<std::common_type_t<Type, Type_other>> res(a.size);
	if (a.size != b.size)
	{
		exit(1);
	}
	else
	{
		for (int i = 0; i != a.size; i++)
		{
			res[i] = a[i] * b[i];
		}
		return res;
	}
}




template <typename Type>
class TVector
{
	Type * ptr;
	int size;
public:
	
	TVector()
	{
		size = 0;
		ptr = nullptr;
	}

	TVector(int size_)
	{
		size = size_;
		ptr = new Type[size];
		for (int i = 0; i != size; i++)
		{
			ptr[i] = 1.0;
		}
	}

	TVector (const TVector<Type>& other)
	{
		size = other.size;
		ptr = new Type[size];
		for (size_t i = 0; i != size; ++i)
			ptr[i] = other.ptr[i];
	}

	int get_size()
	{
		return size;
	}

	void show()
	{
		for (int i = 0; i != size; i++)
		{
			cout << ptr[i] << "    ";
		}
		cout << '\n';
	}

	Type & operator[](int num)
	{
		try {
			if ((num >= 0) && (num < size))
				return ptr[num];
			else
				throw "size error";
		}
		catch (char *str)
		{
			cout << str << endl;
		}
	}

	template<typename Type>
	bool operator==(TVector<Type>& other)
	{
		if (this->size != other.size)
			return false;
		else
			for (int i = 0; i != this->size; i++)
			{
				if (this->ptr[i] != other.ptr[i])
				{
					return false;
				}
			}
		return true;
	}

	bool operator!=(int)
	{
		if (this == NULL)
		{
			return true;
		}
		else 
			return false;
	}

	template<typename Type>
	bool operator!=(const TVector<Type>& other)
	{
		if (this->size != other.size)
			return true;
		else
			for (int i = 0; i != this->size; i++)
			{
				if (this->ptr[i] != other.ptr[i])
				{
					return true;
				}
			}
		return false;
	}

	template <typename Type, typename Type_other>
	friend TVector<std::common_type_t<Type, Type_other>>
		operator+ (TVector<Type> & a, TVector<Type_other> & b);

	template <typename Type, typename Type_other>
	friend TVector<std::common_type_t<Type, Type_other>>
		operator- (TVector<Type> & a, TVector<Type_other> & b);

	template<typename Type>
	TVector<Type> operator*(Type b)
	{
		for (int i = 0; i != size; i++)
		{
			ptr[i] *= b;
		}
		return (*this);
	}

	
	template <typename Type, typename Type_other>
	friend TVector<std::common_type_t<Type, Type_other>>
		operator* (TVector<Type> & a, TVector<Type_other> & b);

	template<typename U>  operator TVector<U>()
	{
		TVector<U> temp(size);
		
		for (int i = 0; i != size; i++)
		{
			temp[i]=static_cast<U>(ptr[i]);
			//cout << typeid(temp[i]).name();
		}
		return (temp);
	}

	TVector<Type>& operator=(TVector<Type>& other)
	{
		if (this != &other)
		{
			delete[] ptr;
			//cout << typeid(ptr).name() << endl;
			this->size = other.size;
			this->ptr = new Type[this->size];
			
			for (int i = 0; i != this->size; i++)
				this->ptr[i] = other.ptr[i];
		}
		return (*this);
	}
	
	friend ostream & ::operator<< <Type> (ostream & output, const TVector<Type>& object);

	friend istream & operator >> <Type> (istream & input, TVector<Type> & obj);
	

	~TVector()
	{
		delete[] ptr;
		ptr = nullptr;
	}
};

