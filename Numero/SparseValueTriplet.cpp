#include "SparseValueTriplet.h"

using namespace Numero;
using namespace Numero::DataTypes;

template <class T>
SparseValueTriplet<T>::SparseValueTriplet(unsigned int row, unsigned int col, T val)
{
    rowIndex = row;
    columnIndex = col;
    value = val;
}

template <class T>
SparseValueTriplet<T>::SparseValueTriplet(SparseValueTriplet& other)
{
    rowIndex = other.rowIndex;
    columnIndex = other.columnIndex;
    value = other.value;
}