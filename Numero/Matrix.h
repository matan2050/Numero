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
			unsigned int nRows;
			unsigned int nCols;
		public:
			Matrix(unsigned int rows, unsigned int cols) : nRows(rows), nCols(cols) {};

			virtual T GetValue(unsigned int row, unsigned int col) const = 0;
			virtual void SetValue(unsigned int row, unsigned int col, T value) = 0;

			virtual T operator()(unsigned int _row, unsigned int _col) const = 0;
			virtual void operator()(unsigned int _row, unsigned int _col, T _value) = 0;
		};
	}
}

#endif // _MATRIX_H_
