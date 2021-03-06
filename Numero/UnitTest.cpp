#include "Dense.cpp"
#include <iostream>
#include <ctime>

using namespace std;
using namespace Numero::DataTypes;

int main()
{
	// define and initialize matrix
	Dense<int> simple3x3(3,3);

	// test initialization, allocation and setting as 0 matrix
	cout << "matrix after initialization:" << endl;
	cout << simple3x3.ToString();

	simple3x3(0, 0, 1);
	simple3x3(0, 1, 2);
	simple3x3(0, 2, 3);
	simple3x3(1, 0, 4);
	simple3x3(1, 1, 5);
	simple3x3(1, 2, 6);
	simple3x3(2, 0, 7);
	simple3x3(2, 1, 8);
	simple3x3(2, 2, 9);

	// test matrix setting values
	cout << "matrix after adding consecutive values:" << endl;
	std::cout << simple3x3.ToString();

	// test inverse
	Dense<float> toInverse(3, 3);
	toInverse(0, 0, 3);
	toInverse(0, 1, 0);
	toInverse(0, 2, 2);
	toInverse(1, 0, 2);
	toInverse(1, 1, 0);
	toInverse(1, 2, -2);
	toInverse(2, 0, 0);
	toInverse(2, 1, 1);
	toInverse(2, 2, 1);

	cout << "matrix to inverse:" << endl << toInverse.ToString();
	Dense<float> inv(3, 3);
	clock_t begin = clock();
	for (int i(0); i < 1000; i++)
	{
		inv = toInverse.InverseByMinors();
	}
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	cout << inv.ToString();
	cout << "time to inverse: " << elapsed_secs << endl;

	// define another matrix
	Dense<int> another3x4(3, 4);
	another3x4.ResetToConstant(1);
	cout << "another matrix of size [3,4] and all ones:" << endl;
	cout << another3x4.ToString();

	/*another3x4(0, 1, 2);
	another3x4(0, 2, 3);
	another3x4(0, 3, 4);
	another3x4(1, 0, 5);
	another3x4(1, 1, 6);
	another3x4(1, 2, 7);
	another3x4(1, 3, 8);
	another3x4(2, 0, 9);
	another3x4(2, 1, 0);
	another3x4(2, 2, 1);
	another3x4(2, 3, 2);*/

	// test matrix multpilication

	int aSize = 2;
	Dense<int> a(aSize, aSize);
	Dense<int> b(aSize, aSize);
	a.ResetToConstant(17);
	b.ResetToConstant(50);

	int cyclesComparison = 10000;

	begin = clock();
	for (int i(0); i < cyclesComparison; i++)
	{
		Dense<int> mult = a.MulNaive(b);
	}
	end = clock();
	elapsed_secs = double(end - begin);
	cout << "using naive multiplication: " << elapsed_secs << endl;

	begin = clock();
	for (int i(0); i < cyclesComparison; i++)
	{
		Dense<int> mult = a.MulTransposed(b);
	}
	end = clock();
	elapsed_secs = double(end - begin);
	cout << "using transposed multiplication: " << elapsed_secs << endl;

	cout << "multiplication of the two above matrices:" << endl;
	//cout << mult.ToString();

	// test operator overload for multiplying by scalar
	Dense<int> mulBy10 = simple3x3 * 10;
	cout << "Matrix: " << endl << simple3x3.ToString();
	cout << "Multiplied by 10: " << endl << mulBy10.ToString();

	// test adding scalar to matrix
	Dense<float> mat(3, 4);
	mat(0, 0, 1);
	mat(0, 1, 10);
	mat(0, 2, 100);
	mat(0, 3, 1000);
	mat(1, 0, 10000);
	mat(1, 1, 100000);
	mat(1, 2, 1000000);
	mat(1, 3, 10000000);
	mat(2, 0, 100000000);
	mat(2, 1, 1000000000);
	mat(2, 2, 10000000000);
	mat(2, 3, 100000000000);

	cout << "matrix:" << endl << mat.ToString();;
	mat.AddScalar(1);
	cout << "matrix plus 1:" << endl << mat.ToString();

	Dense<float> matPlus1 = mat.CopyAddScalar(1);
	cout << "matrix plus another 1, but in new matrix:" << endl << matPlus1.ToString();

	// test matrix addition
	Dense<int> d(2, 3);
	Dense<int> e(2, 3);

	d(0, 0, 1);
	d(0, 1, 2);
	d(0, 2, 3);
	d(1, 0, 3);
	d(1, 1, 2);
	d(1, 2, 1);
	
	e(0, 0, 3);
	e(0, 1, 2);
	e(0, 2, 1);
	e(1, 0, 1);
	e(1, 1, 2);
	e(1, 2, 3);

	cout << "matrix d:" << endl << d.ToString();
	cout << "matrix e:" << endl << e.ToString();

	Dense<int> c = d + e;

	cout << "sum of a+b:" << endl << c.ToString();

	d.AddMatrix(e);
	cout << "sum of a+b in a:" << endl << d.ToString();

	// test trace function
	int trace = simple3x3.Trace();
	cout << "trace: " << trace << endl;

	// test determinant function
	int det = simple3x3.Determinant();
	cout << "determinant (should be 0): " << det << endl;
	Dense<int> other3x3 = simple3x3;
	cout << "same matrix:" << endl;
	cout << other3x3.ToString() << endl;
	other3x3(0, 2, 7);
	cout << "changed element [1,3] to 7:" << endl;
	cout << other3x3.ToString();
	cout << "determinant (should be -12): " << other3x3.Determinant() << endl;

	Dense<int> simple5x5(5, 5);
	simple5x5(0, 0, 1);
	simple5x5(0, 1, 1);
	simple5x5(0, 2, 1);
	simple5x5(0, 3, 1);
	simple5x5(0, 4, 1);
	simple5x5(1, 0, 1);
	simple5x5(1, 1, 1);
	simple5x5(1, 2, 1);
	simple5x5(1, 3, 1);
	simple5x5(1, 4, 1);
	simple5x5(2, 0, 1);
	simple5x5(2, 1, 1);
	simple5x5(2, 2, 1);
	simple5x5(2, 3, 1);
	simple5x5(2, 4, 1);
	simple5x5(3, 0, 1);
	simple5x5(3, 1, 1);
	simple5x5(3, 2, 1);
	simple5x5(3, 3, 1);
	simple5x5(3, 4, 1);
	simple5x5(4, 0, 1);
	simple5x5(4, 1, 1);
	simple5x5(4, 2, 1);
	simple5x5(4, 3, 1);
	simple5x5(4, 4, 1);

	cout << "new 5x5 matrix of ones:" << endl;
	cout << simple5x5.ToString();
	cout << "determinant (should be 0): " << simple5x5.Determinant() << endl;

	simple5x5(1, 1, 2);
	simple5x5(2, 2, 3);
	simple5x5(3, 3, 4);
	simple5x5(4, 4, 5);

	cout << "new 5x5 matrix of consecutive values in the diagonal:" << endl;
	cout << simple5x5.ToString();
	cout << "determinant (should be 24): " << simple5x5.Determinant() << endl;

	// test diagonal function
	cout << "matrix:" << endl;
	cout << simple5x5.ToString();
	Dense<int> diag = simple5x5.Diagonal();
	cout << "vector of diagonal elements:" << endl;
	cout << diag.ToString();

	Dense<int> nonSquare4x5 = simple5x5.SubMatrix(0, 3, 0, 4);
	cout << "matrix:" << endl;
	cout << nonSquare4x5.ToString();
	Dense<int> nonSquareDiag = nonSquare4x5.Diagonal();
	cout << "vector of diagonal elements:" << endl;
	cout << nonSquareDiag.ToString();


	// test transpose function
	Dense<int> transposed = simple3x3.Transpose();
	cout << "matrix transpose:" << endl;
	cout << transposed.ToString();

	// test multiplying element-wise
	Dense<int> elementwiseMul = simple3x3.MulElementwise(transposed);
	cout << "matrix A:" << endl;
	cout << simple3x3.ToString();
	cout << "matrix B:" << endl;
	cout << transposed.ToString();
	cout << "element-wise multiplication:" << endl;
	cout << elementwiseMul.ToString();



	// test vertical concat function
	Dense<int> vertConcat = simple3x3.ConcatRows(transposed);
	cout << "vertical concatenation of normal and transposed matrix:" << endl;
	cout << vertConcat.ToString();

	// test horizontal concat function
	Dense<int> horConcat = simple3x3.ConcatCols(transposed);
	cout << "horizontal concatenation of normal and transposed matrix:" << endl;
	cout << horConcat.ToString();

	// test submatrix function
	Dense<int> sub = simple3x3.SubMatrix(0, 1, 0, 1);
	cout << "sub matrix of rows and columns 1 and 2:" << endl;
	cout << sub.ToString();

	// test minor function
	Dense<int> minor = simple3x3.Minor(0, 0);
	cout << "minor of matrix excluding row 1 and column 1:" << endl;
	cout << minor.ToString();

	// test row interchanging
	simple3x3.RowInterchange(0, 2);
	cout << "row 1 and row 3 interchanged:" << endl;
	cout << simple3x3.ToString();

	// return to previous state
	simple3x3.RowInterchange(0, 2);
	cout << "returned to previous state:" << endl;
	cout << simple3x3.ToString();

	// test column interchage
	simple3x3.ColInterchange(0, 2);
	cout << "column 1 and column 3 interchanged:" << endl;
	cout << simple3x3.ToString();
	//simple3x3.SetValue(0, 1, 2);

	// return to previous state
	simple3x3.ColInterchange(0, 2);
	cout << "returned to previous state:" << endl;
	cout << simple3x3.ToString();

	// test multiplying a row by scalar
	simple3x3.MulRowByScalar(0, 2);
	cout << "row 1 multiplied by 2:" << endl;
	cout << simple3x3.ToString();
	
	// test multiplying a column by a scalar
	simple3x3.MulColByScalar(0, 2);
	cout << "column 1 multiplied by 2:" << endl;
	cout << simple3x3.ToString();


	return 0;
}