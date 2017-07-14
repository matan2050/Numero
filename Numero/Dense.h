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

			// mathematical methods
			T Trace() const;
			Dense<T> Transpose() const;

			// helper functions
			uint Matrix2Index(uint row, uint col) const;

			// IO functions
			string ToString() const;
		};
	}
}

#endif // !_DENSE_H_
