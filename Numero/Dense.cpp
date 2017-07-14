#include "Dense.h"

using namespace Numero;
using namespace Numero::DataTypes;

#pragma region MEMORY_MANIPULATION
// matrix data memory allocation method
template <class T>
void Dense<T>::Allocate(uint rows, uint cols)
{
	matrixData = new T[rows*cols];
	ResetToConstant(static_cast<T>(0));
};

// matrix data memory deallocation method
template <class T>
void Dense<T>::Deallocate()
{
	delete matrixData;
}
#pragma endregion



// number of elements in matrix
template <class T>
uint Dense<T>::Numel() const
{
	return nRows*nCols;
}

// reset all values in matrix to a given value
template <class T>
void Dense<T>::ResetToConstant(T constantVal)
{
	uint numElements = Numel();
	for (uint index(0); index < numElements; index++)
	{
		matrixData[index] = constantVal;
	}
}



#pragma region BASE_INTERFACE_IMPLEMENTATION
// get value at i,j index
template <class T>
T Dense<T>::GetValue(uint row, uint col) const
{
	return matrixData[Matrix2Index(row, col)];
}

// set value in i,j index
template <class T>
void Dense<T>::SetValue(uint row, uint col, T value)
{
	matrixData[Matrix2Index(row, col)] = value;
}

// get value in i,j index
template <class T>
T Dense<T>::operator()(uint row, uint col) const
{
	return GetValue(row, col);
}

// set value in i,j index
template <class T>
void Dense<T>::operator()(uint row, uint col, T value)
{
	SetValue(row, col, value);
}
#pragma endregion



#pragma region MATHEMATICAL_FUNCTIONS
// validated
// function to return trace - sum of diagonal elements in matrix
template <class T>
T Dense<T>::Trace() const
{
	T trace = static_cast<T>(0);
	uint nElements = Numel();
	uint diagonalJump = nCols + 1;

	for (uint diagonalIndex(0); diagonalIndex < nElements; diagonalIndex += diagonalJump)
	{
		trace += matrixData[diagonalIndex];
	}

	return trace;
}

// validated
// function to transpose matrix
template <class T>
Dense<T> Dense<T>::Transpose() const
{
	Dense<T> transposed(nCols, nRows);

	for (uint rowIndex(0); rowIndex < nRows; rowIndex++)
	{
		for (uint colIndex(0); colIndex < nCols; colIndex++)
		{
			transposed.matrixData[colIndex + rowIndex*nRows] = matrixData[rowIndex + colIndex*nCols];
		}
	}

	return transposed;
}
#pragma endregion


#pragma region HELPER_FUNCTIONS
// convert two-dimensional index to one-dimensional index
template <class T>
uint Dense<T>::Matrix2Index(uint row, uint col) const
{
	return col + row*(nCols);
}
#pragma endregion



#pragma region IO
// matlab style string outputter
template <class T>
string Dense<T>::ToString() const
{
	ostringstream ss;

	ss << "[";
	for (uint row(0); row < nRows; row++)
	{
		for (uint col(0); col < nCols; col++)
		{
			ss << GetValue(row, col);

			if (col != nCols - 1)
				ss << ",";

		}

		if (row != nRows - 1)
			ss << ";" << endl;
		else
			ss << "]" << endl;
	}

	return ss.str();
}
#pragma endregion