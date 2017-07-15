#ifndef _DENSE_H_
#define _DENSE_H_

#include "../Numero.Definitions/DataTypeDefines.h"
#include "Matrix.h"
#include <sstream>

namespace Numero
{
	using namespace std;
	using namespace Definitions;

	namespace DataTypes
	{
		// Dense class
		// represents dense matrix
		// implements matrix base class
		template <class T>
		class Dense : Matrix<T>
		{
		private:
			T* matrixData;
		protected:
			void Allocate(uint rows, uint cols);
			void Deallocate();
		public:
			Dense(uint rows, uint cols) : Matrix(rows, cols) { Allocate(rows, cols); };

			void ResetToConstant(T constantVal);
			uint Numel() const;

			// base class implementations - get/set for elements in matrix
			virtual T GetValue(uint row, uint col) const;
			virtual void SetValue(uint row, uint col, T value);
			virtual T operator()(uint row, uint col) const;
			virtual void operator()(uint row, uint col, T value);

			// concatenation methods
			Dense<T> ConcatRows(const Dense<T>& _matrix_b);
			Dense<T> ConcatCols(const Dense<T>& _matrix_b);
			Dense<T> SubMatrix(uint startRow, uint endRow, uint startCol, uint endCol);

			// mathematical methods
			T Trace() const;
			T Determinant() const;
			Dense<T> Transpose() const;
			Dense<T> Diagonal() const;
			void RowInterchange(uint rowA, uint rowB);
			void ColInterchange(uint colA, uint colB);
			Dense<T> Minor(uint deletedRowIndex, uint deletedColIndex) const;
			void MulRowByScalar(uint row, T scalar);
			void MulColByScalar(uint col, T scalar);
			Dense<T> MulElementwise(Dense<T>& other) const;

			// helper functions
			uint Matrix2Index(uint row, uint col) const;

			// IO functions
			string ToString() const;
		};
	}
}

#endif // !_DENSE_H_
