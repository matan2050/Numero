#include "Dense.h"

using namespace Numero;
using namespace Numero::DataTypes;

#pragma region MEMORY_MANIPULATION
// matrix data memory allocation method
template <class T>
void Dense<T>::Allocate(uint rows, uint cols)
{
	matrixData = new T[rows*cols];
};

// matrix data memory deallocation method
template <class T>
void Dense<T>::Deallocate()
{
	delete matrixData;
}
#pragma endregion


#pragma region BASE_INTERFACE_IMPLEMENTATION
template <class T>
T Dense<T>::GetValue(uint row, uint col) const
{
	return matrixData[Matrix2Index(row, col)];
}

template <class T>
void Dense<T>::SetValue(uint row, uint col, T value)
{
	matrixData[Matrix2Index(row, col)] = value;
}

template <class T>
T Dense<T>::operator()(uint row, uint col) const
{
	return GetValue(row, col);
}

template <class T>
void Dense<T>::operator()(uint row, uint col, T value)
{
	SetValue(row, col, value);
}
#pragma endregion


#pragma region HELPER_FUNCTIONS
template <class T> uint Dense<T>::Matrix2Index(uint row, uint col) const
{
	return col + row*(nCols);
}
#pragma endregion