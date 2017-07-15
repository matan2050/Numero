#include <assert.h>
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



#pragma region CONCATENATION_FUNCTIONS
// validated
// vertical concatenation of two matrices into a new matrix
// TODO: consider switching GetValue and SetValue to actual usage of matrixData
template <class T>
Dense<T> Dense<T>::ConcatRows(const Dense<T>& matrixB)
{
	assert(nCols == matrixB.nCols);

	uint newRowNum = nRows + matrixB.nRows;

	Dense<T> concat(newRowNum, nCols);

	for (uint i(0); i < newRowNum; i++)
	{
		for (uint j(0); j < nCols; j++)
		{
			if (i >= nRows)
				concat.SetValue(i, j, matrixB.GetValue(i - nRows, j));
			else
				concat.SetValue(i, j, GetValue(i, j));
		}
	}
	return concat;
}

// validated
// horizontal concatenation of two matrices into a new matrix
// TODO: consider switching SetValue to actual usage of matrixData
template <class T>
Dense<T> Dense<T>::ConcatCols(const Dense<T>& matrixB)
{
	assert(nRows == matrixB.nRows);

	uint newColNum = nCols + matrixB.nCols;

	Dense<T> concat(nRows, newColNum);

	for (uint i(0); i < nRows; i++)
	{
		for (uint j(0); j < newColNum; j++)
		{
			if (j >= nCols)
				concat.SetValue(i, j, matrixB.GetValue(i, j - nCols));
			else
				concat.SetValue(i, j, GetValue(i, j));
		}
	}
	return concat;
}

template <class T>
Dense<T> Dense<T>::SubMatrix(uint startRow, uint endRow, uint startCol, uint endCol)
{
	uint newRowCount = endRow - startRow + 1;	// +1 becuase zero based
	uint newColCount = endCol - startCol + 1;

	Dense<T> sub(newRowCount, newColCount);

	for (uint i(0); i < newRowCount; i++)
	{
		for (uint j(0); j < newColCount; j++)
		{
			sub.SetValue(i, j, GetValue(startRow + i, startCol + j));
		}
	}

	return sub;
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
// function to calculate determinant of Dense matrix
template <class T>
T Dense<T>::Determinant() const
{
	assert(nRows == nCols);
	
	T det = (T)0;

	if (nRows == 1)
	{
		return GetValue(0, 0);
	}

	if (nRows == 2)
	{
		det += GetValue(0, 0)*GetValue(1, 1) - GetValue(0, 1)*GetValue(1, 0);
		return det;
	}

	int sign;

	for (int i(0); i < nCols; i++)
	{
		if (i % 2 != 0)
		{
			sign = -1;
		}
		else
		{
			sign = 1;
		}

		Dense<T> minor = Minor(0, i);
		T scalarFactor = GetValue(0, i);
		det += sign*scalarFactor*minor.Determinant();
	}

	return det;
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

// validated
// outputs a vector containing elements in the diagonal positions
template <class T>
Dense<T> Dense<T>::Diagonal() const
{
	Dense<T> diag(1, nRows);

	for (uint i(0); i < nRows; i++)
	{
		diag.matrixData[i] = matrixData[i + i*nCols];
	}

	return diag;
}

// validated
// function to interchange two rows
template <class T>
void Dense<T>::RowInterchange(uint rowA, uint rowB)
{
	T* tempBuffer = new T[nCols];

	// copy first row to a buffer
	for (uint rowElement(0); rowElement < nCols; rowElement++)
	{
		tempBuffer[rowElement] = matrixData[rowA*nCols + rowElement];
	}

	// interchange the two rows using the buffer
	for (uint rowElement(0); rowElement < nCols; rowElement++)
	{
		matrixData[rowA*nCols + rowElement] = matrixData[rowB*nCols + rowElement];
		matrixData[rowB*nCols + rowElement] = tempBuffer[rowElement];
	}

	delete[] tempBuffer;
}

// validated
// function to interchange two columns
template <class T>
void Dense<T>::ColInterchange(uint colA, uint colB)
{
	T* tempBuffer = new T[nRows];

	// copy first row to a buffer
	for (uint colElement(0); colElement < nCols; colElement++)
	{
		tempBuffer[colElement] = matrixData[colA + colElement * nCols];
	}

	// interchange the two rows using the buffer
	for (uint colElement(0); colElement < nRows; colElement++)
	{
		matrixData[colA + colElement * nCols] = matrixData[colB + colElement * nCols];
		matrixData[colB + colElement * nCols] = tempBuffer[colElement];
	}

	delete[] tempBuffer;
}

// validated
// multiplies a row by a scalar value
template <class T>
void Dense<T>::MulRowByScalar(uint row, T scalar)
{
	for (uint i(0); i < nCols; i++)
	{
		matrixData[row*nCols + i] *= scalar;
	}
}

// validated
// function returns a new matrix which is the first minor of the original matrix
// excluding the inputted row and column
template <class T>
Dense<T> Dense<T>::Minor(uint excludedRowIndex, uint excludedColIndex) const
{
	Dense<T> sub(nRows - 1, nCols - 1);

	uint subRowIndex, subColIndex;
	for (uint rowIndex(0), subRowIndex(0); rowIndex < nRows; rowIndex++)
	{
		if (rowIndex == excludedRowIndex)
			continue;

		for (uint colIndex(0), subColIndex(0); colIndex < nCols; colIndex++)
		{
			if (colIndex == excludedColIndex)
				continue;

			sub.SetValue(subRowIndex,
				subColIndex,
				GetValue(rowIndex, colIndex));

			subColIndex++;
		}

		subRowIndex++;
	}
	return sub;
}

// validated
// multiplies a column by a scalar value
template <class T>
void Dense<T>::MulColByScalar(uint col, T scalar)
{
	for (uint i(0); i < nRows; i++)
	{
		matrixData[col + i*nCols] *= scalar;
	}
}

// validated
// element wise matrix to matrix multiplication
template <class T>
Dense<T> Dense<T>::MulElementwise(Dense<T>& other) const
{
	assert((nRows == other.nRows) && (nCols == other.nCols));
	uint nElements = Numel();
	Dense<T> multiplied(nRows, nCols);

	for (uint i(0); i < nElements; i++)
	{
		multiplied.matrixData[i] = matrixData[i] * other.matrixData[i];
	}
	
	return multiplied;
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