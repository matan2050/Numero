#ifndef _MATRIX_H_
#define _MATRIX_H_

#include "../Numero.Definitions/DataTypeDefines.h"

namespace Numero
{
	using namespace Definitions;

	namespace DataTypes
	{
		// Matrix class
		// Base class for dense and sparse matrices
		// implements only GetValue, SetValue and corresponding functors
		template <class T>
		class Matrix
		{
		private:
		protected:
			uint nRows;
			uint nCols;
		public:
			Matrix(uint rows, uint cols) : nRows(rows), nCols(cols) {};

			virtual T GetValue(uint row, uint col) const = 0;
			virtual void SetValue(uint row, uint col, T value) = 0;

			virtual T operator()(uint _row, uint _col) const = 0;
			virtual void operator()(uint _row, uint _col, T _value) = 0;
		};
	}
}

#endif // _MATRIX_H_
