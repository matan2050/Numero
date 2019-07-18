#ifndef _SPARSE_VALUE_TRIPLET_H_
#define _SPARSE_VALUE_TRIPLET_H_

#include "../Numero.Definitions/DataTypeDefines.h"

namespace Numero
{
    using namespace Definitions;

    namespace DataTypes
    {
        template <class T>
        class SparseValueTriplet
        {
        private:
            unsigned int rowIndex;
            unsigned int columnIndex;
            T value;
        public:
            SparseValueTriplet(unsigned int row, unsigned int col, T value);
            SparseValueTriplet(SparseValueTriplet& other);
        };
    }
}

#endif
