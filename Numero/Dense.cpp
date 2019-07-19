#include <assert.h>
#include "Dense.h"

using namespace Numero;
using namespace Numero::DataTypes;

#pragma region MEMORY_MANIPULATION
// matrix data memory allocation method
template <class T>
void Dense<T>::Allocate(unsigned int rows, unsigned int cols)
{
	matrixData = new T[rows*cols];
	ResetToConstant(static_cast<T>(0));
}

template<class T>
void Dense<T>::Allocate(unsigned int rows, unsigned int cols, const T * data)
{
    matrixData = new T[rows*cols];

    for (int i = 0; i < rows*cols; i++)
    {
        matrixData[i] = data[i];
    }
}
;

// matrix data memory deallocation method
template <class T>
void Dense<T>::Deallocate()
{
	delete matrixData;
}
#pragma endregion



// number of elements in matrix
template <class T>
unsigned int Dense<T>::Numel() const
{
	return nRows*nCols;
}

// reset all values in matrix to a given value
template <class T>
void Dense<T>::ResetToConstant(T constantVal)
{
	unsigned int numElements = Numel();
	for (unsigned int index(0); index < numElements; index++)
	{
		matrixData[index] = constantVal;
	}
}



#pragma region BASE_INTERFACE_IMPLEMENTATION
// get value at i,j index
template <class T>
T Dense<T>::GetValue(unsigned int row, unsigned int col) const
{
	return matrixData[Matrix2Index(row, col)];
}

// set value in i,j index
template <class T>
void Dense<T>::SetValue(unsigned int row, unsigned int col, T value)
{
	matrixData[Matrix2Index(row, col)] = value;
}

// get value in i,j index
template <class T>
T Dense<T>::operator()(unsigned int row, unsigned int col) const
{
	return GetValue(row, col);
}

// set value in i,j index
template <class T>
void Dense<T>::operator()(unsigned int row, unsigned int col, T value)
{
	SetValue(row, col, value);
}
#pragma endregion


#pragma region OPERATOR_OVERLOADS

template <class T>
Dense<T> Dense<T>::operator*(T scalar) const
{
	return CopyMulScalar(scalar);
}

template <class T>
Dense<T> Dense<T>::operator*(const Dense<T>& other) const
{
	// empirically, naive multiplication is faster in very small matrices
	if (nRows > 2)
	{
		return MulTransposed(other);
	}
	else
	{
		return MulNaive(other);
	}
}

template <class T>
Dense<T> Dense<T>::operator+(const Dense<T>& other) const
{
	return CopyAddMatrix(other);
}

template <class T>
Dense<T> Dense<T>::operator+(T scalar) const
{
	return CopyAddScalar(scalar);
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

	unsigned int newRowNum = nRows + matrixB.nRows;

	Dense<T> concat(newRowNum, nCols);

	for (unsigned int i(0); i < newRowNum; i++)
	{
		for (unsigned int j(0); j < nCols; j++)
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

	unsigned int newColNum = nCols + matrixB.nCols;

	Dense<T> concat(nRows, newColNum);

	for (unsigned int i(0); i < nRows; i++)
	{
		for (unsigned int j(0); j < newColNum; j++)
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
Dense<T> Dense<T>::SubMatrix(unsigned int startRow, unsigned int endRow, unsigned int startCol, unsigned int endCol)
{
	unsigned int newRowCount = endRow - startRow + 1;	// +1 becuase zero based
	unsigned int newColCount = endCol - startCol + 1;

	Dense<T> sub(newRowCount, newColCount);

	for (unsigned int i(0); i < newRowCount; i++)
	{
		for (unsigned int j(0); j < newColCount; j++)
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
	unsigned int nElements = Numel();
	unsigned int diagonalJump = nCols + 1;

	for (unsigned int diagonalIndex(0); diagonalIndex < nElements; diagonalIndex += diagonalJump)
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
	// asserting square matrix
	assert(nRows == nCols);
	
	T det = (T)0;

	// stopping condition - matrix of size 1x1
	if (nRows == 1)
	{
		return GetValue(0, 0);
	}

	// or matrix of size 2x2
	if (nRows == 2)
	{
		det += GetValue(0, 0)*GetValue(1, 1) - GetValue(0, 1)*GetValue(1, 0);
		return det;
	}

	int sign;

	for (unsigned int i(0); i < nCols; i++)
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
	int transCols = nRows;
	int transRows = nCols;

	Dense<T> transposed(transRows, transCols);

	for (int rowIndex(0); rowIndex < transRows; rowIndex++)
	{
		for (int colIndex(0); colIndex < transCols; colIndex++)
		{
			transposed.SetValue(rowIndex, colIndex, GetValue(colIndex, rowIndex));
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

	for (unsigned int i(0); i < nRows; i++)
	{
		diag.matrixData[i] = matrixData[i + i*nCols];
	}

	return diag;
}

// validated
// function to interchange two rows
template <class T>
void Dense<T>::RowInterchange(unsigned int rowA, unsigned int rowB)
{
	T* tempBuffer = new T[nCols];

	// copy first row to a buffer
	for (unsigned int rowElement(0); rowElement < nCols; rowElement++)
	{
		tempBuffer[rowElement] = matrixData[rowA*nCols + rowElement];
	}

	// interchange the two rows using the buffer
	for (unsigned int rowElement(0); rowElement < nCols; rowElement++)
	{
		matrixData[rowA*nCols + rowElement] = matrixData[rowB*nCols + rowElement];
		matrixData[rowB*nCols + rowElement] = tempBuffer[rowElement];
	}

	delete[] tempBuffer;
}

// validated
// function to interchange two columns
template <class T>
void Dense<T>::ColInterchange(unsigned int colA, unsigned int colB)
{
	T* tempBuffer = new T[nRows];

	// copy first row to a buffer
	for (unsigned int colElement(0); colElement < nCols; colElement++)
	{
		tempBuffer[colElement] = matrixData[colA + colElement * nCols];
	}

	// interchange the two rows using the buffer
	for (unsigned int colElement(0); colElement < nRows; colElement++)
	{
		matrixData[colA + colElement * nCols] = matrixData[colB + colElement * nCols];
		matrixData[colB + colElement * nCols] = tempBuffer[colElement];
	}

	delete[] tempBuffer;
}

// validated
// multiplies a row by a scalar value
template <class T>
void Dense<T>::MulRowByScalar(unsigned int row, T scalar)
{
	for (unsigned int i(0); i < nCols; i++)
	{
		matrixData[row*nCols + i] *= scalar;
	}
}

// validated
// function returns a new matrix which is the first minor of the original matrix
// excluding the inputted row and column
template <class T>
Dense<T> Dense<T>::Minor(unsigned int excludedRowIndex, unsigned int excludedColIndex) const
{
	Dense<T> sub(nRows - 1, nCols - 1);

	unsigned int subRowIndex, subColIndex;
	for (unsigned int rowIndex(0), subRowIndex(0); rowIndex < nRows; rowIndex++)
	{
		if (rowIndex == excludedRowIndex)
			continue;

		for (unsigned int colIndex(0), subColIndex(0); colIndex < nCols; colIndex++)
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

// validated for 3x3
// add in the future Moore-Penrose pseudoinverse algorithm
template <class T>
Dense<T> Dense<T>::InverseByMinors() const
{
	assert(nRows == nCols);
	
	T determinant = Determinant();

	assert(determinant != 0);		//TODO: remove assertion and return exception

	Dense<T> inverse(nRows, nCols);
	int sign = 1;

	for (unsigned int i(0); i < nRows; i++)
	{

		if (i % 2 == 1)
		{
			sign = 1;
		}
		else
		{
			sign = -1;
		}

		for (unsigned int j(0); j < nCols; j++)
		{
			sign *= -1;

			Dense<T> currentMinor = Minor(i, j);
			inverse.SetValue(i, j, (1/determinant)*sign*currentMinor.Determinant());
		}
	}

	return inverse.Transpose();
}

// validated
// multiplies a column by a scalar value
template <class T>
void Dense<T>::MulColByScalar(unsigned int col, T scalar)
{
	for (unsigned int i(0); i < nRows; i++)
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
	unsigned int nElements = Numel();
	Dense<T> multiplied(nRows, nCols);

	for (unsigned int i(0); i < nElements; i++)
	{
		multiplied.matrixData[i] = matrixData[i] * other.matrixData[i];
	}
	
	return multiplied;
}

// validated
// naive code for matrix multiplication. consider refactoring and optimizations
// currently, all access to matrix data is done through GetValue and SetValue
template <class T>
Dense<T> Dense<T>::MulNaive(Dense<T>& other) const
{
	assert(nCols == other.nRows);

	unsigned int productRows = nRows;
	unsigned int productCols = other.nCols;

	Dense<T> product(productRows, productCols);

	for (unsigned int i(0); i < productRows; i++)
	{
		for (unsigned int j(0); j < productCols; j++)
		{
			T element = (T)0;

			for (unsigned int k(0); k < nCols; k++)
			{
				element += GetValue(i, k) * other.GetValue(k, j);
			}

			product.SetValue(i, j, element);
		}
	}

	return product;
}

// validated
// code for multiplying two matrices with transposing the rhs matrix
// causes optimization for large matrices.
template <class T>
Dense<T> Dense<T>::MulTransposed(Dense<T>& other) const
{
	Dense<T> otherTransposed = other.Transpose();

	unsigned int productRows = nRows;
	unsigned int productCols = other.nCols;

	Dense<T> product(productRows, productCols);

	for (unsigned int i(0); i < productRows; i++)
	{
		for (unsigned int j(0); j < productCols; j++)
		{
			T element = (T)0;

			for (unsigned int k(0); k < nCols; k++)
			{
				element += GetValue(i, k) * otherTransposed.GetValue(j, k);
			}

			product.SetValue(i, j, element);
		}
	}

	return product;
}

template <class T>
void Dense<T>::AddScalar(T scalar)
{
	int nElements = Numel();

	for (int i(0); i < nElements; i++)
	{
		matrixData[i] += scalar;
	}
}

template <class T>
Dense<T> Dense<T>::CopyAddScalar(T scalar) const
{
	Dense<T> sum(nRows, nCols);
	int nElements = sum.Numel();

	for (int i(0); i < nElements; i++)
	{
		sum.matrixData[i] = matrixData[i] + scalar;
	}
	return sum;
}

template <class T>
void Dense<T>::MulScalar(T scalar)
{
	int nElements = sum.Numel();

	for (int i(0); i < nElements; i++)
	{
		matrixData[i] *= scalar;
	}
}

template <class T>
void Dense<T>::AddMatrix(const Dense<T>& other)
{
	assert((nCols == other.nCols) && (nRows == other.nRows));

	int nElements = Numel();

	for (int i(0); i < nElements; i++)
	{
		matrixData[i] += other.matrixData[i];
	}
}

template <class T>
Dense<T> Dense<T>::CopyMulScalar(T scalar) const
{
	Dense<T> sum(nRows, nCols);
	int nElements = sum.Numel();

	for (int i(0); i < nElements; i++)
	{
		sum.matrixData[i] = matrixData[i] * scalar;
	}
	return sum;
}

template <class T>
Dense<T> Dense<T>::CopyAddMatrix(const Dense<T>& other) const
{
	assert((nCols == other.nCols) && (nRows == other.nRows));

	Dense<T> sum(nRows, nCols);
	int nElements = Numel();

	for (int i(0); i < nElements; i++)
	{
		sum.matrixData[i] = matrixData[i] + other.matrixData[i];
	}

	return sum;
}
#pragma endregion


#pragma region HELPER_FUNCTIONS
// convert two-dimensional index to one-dimensional index
template <class T>
unsigned int Dense<T>::Matrix2Index(unsigned int row, unsigned int col) const
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
	for (unsigned int row(0); row < nRows; row++)
	{
		for (unsigned int col(0); col < nCols; col++)
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