#ifndef _SPARSE_H_
#define _SPARSE_H

#include "../Numero.Definitions/DataTypeDefines.h"

namespace Numero
{
    using namespace Definitions;

    namespace DataTypes
    {
        template <class T>
        class Sparse : Matrix<T>
        {
        private:
            
        public:
            Sparse(unsigned int rows, unsigned int cols) : Matrix(rows, cols) {};
        };
    }
}

#endif
