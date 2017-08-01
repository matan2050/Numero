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

			// --- constructors / destructor
			Dense(uint rows, uint cols) : Matrix(rows, cols) { Allocate(rows, cols); };
			~Dense() { Deallocate(); };

			void ResetToConstant(T constantVal);
			uint Numel() const;

			// --- base class implementations
			virtual T GetValue(uint row, uint col) const;
			virtual void SetValue(uint row, uint col, T value);
			virtual T operator()(uint row, uint col) const;
			virtual void operator()(uint row, uint col, T value);

			// --- operator overloads
			Dense<T> operator*(const Dense<T>& other) const;
			Dense<T> operator+(const Dense<T>& other) const;
			Dense<T> operator*(T scalar) const;
			Dense<T> operator+(T scalar) const;

			// --- concatenation methods
			Dense<T> ConcatRows(const Dense<T>& _matrix_b);
			Dense<T> ConcatCols(const Dense<T>& _matrix_b);
			Dense<T> SubMatrix(uint startRow, uint endRow, uint startCol, uint endCol);

			// --- mathematical methods
			T Trace() const;
			T Determinant() const;
			Dense<T> Transpose() const;
			Dense<T> Diagonal() const;
			Dense<T> Minor(uint deletedRowIndex, uint deletedColIndex) const;
			Dense<T> InverseByMinors() const;

			// ------ linear actions on matrix
			void RowInterchange(uint rowA, uint rowB);
			void ColInterchange(uint colA, uint colB);
			void MulRowByScalar(uint row, T scalar);
			void MulColByScalar(uint col, T scalar);

			// ------ matrix multiplication methods
			Dense<T> MulElementwise(Dense<T>& other) const;
			Dense<T> MulNaive(Dense<T>& other) const;
			Dense<T> MulTransposed(Dense<T>& other) const;

			// ------ matrix addition methods
			void AddScalar(T scalar);
			void MulScalar(T scalar);
			void AddMatrix(const Dense<T>& other);

			// ------ matrix arithmetic methods on a new matrix
			Dense<T> CopyAddScalar(T scalar) const;
			Dense<T> CopyMulScalar(T scalar) const;
			Dense<T> CopyAddMatrix(const Dense<T>& other) const;

			// helper functions
			uint Matrix2Index(uint row, uint col) const;

			// IO functions
			string ToString() const;
		};
	}
}
#endif // !_DENSE_H_