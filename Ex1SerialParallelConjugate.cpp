#include <iostream>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cassert>
#include <vector>
#include <mpi.h>
using namespace std;

#define MASTER 0

const long double EPS = 1e-10;
const int dim_mx = 20;
const int val_mx = 1000000;

template <class T>
class Matrix {
public:
	size_t row_size, col_size;
	std::vector< std::vector < T > > mat;

	// public:
	Matrix();
	Matrix(const size_t r_s, const size_t c_s);
	Matrix(const Matrix<T>& M);
	Matrix(Matrix<T>&& M);

	Matrix<T>& operator = (Matrix<T>&& M);
	Matrix<T>& operator = (Matrix<T>& M);

	T getVal(const size_t row, const size_t col);
	T setVal(const size_t row, const size_t col, const T value);
	size_t getRowSize();
	size_t getColSize();

	template <class ElType>
	friend Matrix<ElType> operator - (const Matrix<ElType>& M1);
	template <class ElType>
	friend Matrix<ElType> operator + (const Matrix<ElType>& M1, const Matrix<ElType>& M2);
	template <class ElType>
	friend Matrix<ElType> operator - (const Matrix<ElType>& M1, const Matrix<ElType>& M2);
	template <class ElType>
	friend Matrix<ElType> operator * (const Matrix<ElType>& M1, const Matrix<ElType>& M2);
	template <class ElType>
	friend void operator += (Matrix<ElType>& M1, const Matrix<ElType>& M2);
	template <class ElType>
	friend void operator -= (Matrix<ElType>& M1, const Matrix<ElType>& M2);
	template <class ElType>
	friend void operator *= (Matrix<ElType>& M1, const Matrix<ElType>& M2);

	~Matrix();
};

/* constructor declarations */
/* empty matrix initialised */
template <class T>
Matrix<T>::Matrix() {
	row_size = col_size = 0;
}

/*
	set the dimensions of the matrix
	all values are set to 0
*/
template <class T>
Matrix<T>::Matrix(const size_t r_s, const size_t c_s) {
	row_size = r_s;
	col_size = c_s;
	mat.resize(row_size);
	for (size_t i = 0; i < row_size; i++) {
		mat[i].resize(col_size);
	}
	for (size_t i = 0; i < row_size; ++i) {
		for (size_t j = 0; j < col_size; ++j) {
			mat[i][j] = 0;
		}
	}
}

/*
	copy constructor
	deepcopy performed
	Additional time of O(row*col) used
*/
template <class T>
Matrix<T>::Matrix(const Matrix<T>& M) {
	row_size = M.row_size;
	col_size = M.col_size;
	mat.resize(row_size);
	for (size_t i = 0; i < row_size; i++) {
		mat[i].resize(col_size);
	}
	for (size_t i = 0; i < row_size; i++) {
		for (size_t j = 0; j < col_size; j++) {
			mat[i][j] = M.mat[i][j];
		}
	}
}

/*
	move constructor: moves the rhs value to the matrix
	O(1) time complexity
*/
template <class T>
Matrix<T>::Matrix(Matrix<T>&& M) {
	row_size = std::move(M.row_size);
	col_size = std::move(M.col_size);
	mat = std::move(M.mat);
}

/* destructor declaration */
template <class T>
Matrix<T>::~Matrix() {
	mat.clear();
}

/* Class functions */
template <class T>
T Matrix<T>::getVal(const size_t row, const size_t col) {
	return mat[row][col];
}
template <class T>
T Matrix<T>::setVal(const size_t row, const size_t col, const T value) {
	return mat[row][col] = value;
}
template <class T>
size_t Matrix<T>::getRowSize() {
	return row_size;
}
template <class T>
size_t Matrix<T>::getColSize() {
	return col_size;
}


/*
	Move Assignment operator
	O(1) time complexity
*/
template<class T>
Matrix<T>& Matrix<T>::operator = (Matrix<T>&& M) {
	row_size = std::move(M.row_size);
	col_size = std::move(M.col_size);
	mat = std::move(M.mat);
	return *this;
}

/*
	Copy Assignment operator
	performs deepcopy : O(row*col) time complexity
*/
template<class T>
Matrix<T>& Matrix<T>::operator = (Matrix<T>& M) {
	row_size = M.row_size;
	col_size = M.col_size;
	for (size_t i = 0; i < row_size; i++) {
		for (size_t j = 0; j < row_size; j++) {
			mat[i][j] = M.mat[i][j];
		}
	}
	return *this;
}

/* friend functions*/
/* returns -1*Matrix */
template <class T>
Matrix<T> operator- (const Matrix<T>& M) {
	Matrix<T> res(M.row_size, M.col_size);
	for (size_t i = 0; i < M.row_size; i++) {
		for (size_t j = 0; j < M.col_size; j++) {
			res.mat[i][j] = -M.mat[i][j];
		}
	}
	return res;
}

/*
	operator overloaded for matrix addition
	returns Matrix1 + Matrix2
*/
template <class T>
Matrix<T> operator+ (const Matrix<T>& M1, const Matrix<T>& M2) {
	/*to check wheter the matrices have same dimensions*/
	if (M1.row_size != M2.row_size || M1.col_size != M2.col_size) {
		return M1;
	}
	Matrix<T> res(M1.row_size, M1.col_size);
	for (size_t i = 0; i < M1.row_size; i++) {
		for (size_t j = 0; j < M1.col_size; j++) {
			res.mat[i][j] = M1.mat[i][j] + M2.mat[i][j];
		}
	}
	return res;
}

/*
	operator overloaded for matrix subtraction
	returns Matrix1 - Matrix2
*/
template <class T>
Matrix<T> operator- (const Matrix<T>& M1, const Matrix<T>& M2) {
	/*to check wheter the matrices have same dimensions*/
	if (M1.row_size != M2.row_size || M1.col_size != M2.col_size) {
		return M1;
	}
	Matrix<T> res(M1.row_size, M1.col_size);
	for (size_t i = 0; i < M1.row_size; i++) {
		for (size_t j = 0; j < M1.col_size; j++) {
			res.mat[i][j] = M1.mat[i][j] - M2.mat[i][j];
		}
	}
	return res;
}

/*
	operator overloaded for matrix multiplication
	returns Matrix1 * Matrix2
*/
template <class T>
Matrix<T> operator* (const Matrix<T>& M1, const Matrix<T>& M2) {
	/* to check if matrix multiplication is dimensionally possible */
	if (M1.col_size != M2.row_size) {
		return M1;
	}
	/* O(M1.row * M1.col * M2.col) time complexity */
	Matrix<T> res(M1.row_size, M2.col_size);
	for (size_t i = 0; i < M1.row_size; i++) {
		for (size_t j = 0; j < M2.col_size; j++) {
			res.mat[i][j] = 0;
			for (size_t k = 0; k < M1.col_size; k++)
				res.mat[i][j] += M1.mat[i][k] * M2.mat[k][j];
		}
	}
	return res;
}

/*
	operator overloaded for matrix self addition
	performs Matrix1 = Matrix1 + Matrix2
*/
template <class T>
void operator+= (Matrix<T>& M1, const Matrix<T>& M2) {
	/* to check wheter the matrices have same dimensions */
	if (M1.row_size != M2.row_size || M1.col_size != M2.col_size) {
		return;
	}
	for (size_t i = 0; i < M1.row_size; i++) {
		for (size_t j = 0; j < M1.col_size; j++) {
			M1.mat[i][j] = M1.mat[i][j] + M2.mat[i][j];
		}
	}
	return;
}

/*
	operator overloaded for matrix self subtraction
	performs Matrix1 = Matrix1 - Matrix2
*/
template <class T>
void operator-= (Matrix<T>& M1, const Matrix<T>& M2) {
	/* to check wheter the matrices have same dimensions */
	if (M1.row_size != M2.row_size || M1.col_size != M2.col_size) {
		return;
	}
	for (size_t i = 0; i < M1.row_size; i++) {
		for (size_t j = 0; j < M1.col_size; j++) {
			M1.mat[i][j] = M1.mat[i][j] - M2.mat[i][j];
		}
	}
	return;
}

/*
	operator overloaded for matrix self multiplication
	performs Matrix1 = Matrix1 * Matrix2
*/
template <class T>
inline void operator*=(Matrix<T>& M1, const Matrix<T>& M2) {
	/* to check wheter the matrices have same dimensions */
	if (M1.col_size != M2.row_size) {
		return;
	}
	/* alocating O(row*col) additional memory to find the resultant */
	Matrix<T> res(M1.row_size, M2.col_size);
	for (size_t i = 0; i < M1.row_size; i++) {
		for (size_t j = 0; j < M2.col_size; j++) {
			res.mat[i][j] = 0;
			for (size_t k = 0; k < M1.col_size; k++)
				res.mat[i][j] += M1.mat[i][k] * M2.mat[k][j];
		}
	}
	size_t r = M1.row_size, c = M2.col_size;
	for (size_t i = 0; i < M1.row_size; i++) {
		M1.mat[i].clear();
	}
	M1.mat.clear();
	M1.mat.resize(r);
	M1.row_size = r;
	M1.col_size = c;
	for (size_t i = 0; i < r; i++) {
		M1.mat[i].resize(c);
	}
	for (size_t i = 0; i < M1.row_size; i++) {
		for (size_t j = 0; j < M1.col_size; j++) {
			M1.mat[i][j] = res.mat[i][j];
		}
	}
	return;
}
/* returns Trace: sum of the body diagnol elements of a square matrix */
template <class T>
T trace(Matrix<T>& M) {
	auto t = 0;
	for (size_t i = 0; i < std::min(M.getRowSize(), M.getColSize()); i++) {
		t += M.getVal(i, i);
	}
	return t;
}

template <class T>
Matrix<T> trans(Matrix<T>& X) {
	Matrix <T> ret(X.getColSize(), X.getRowSize());
	for (size_t i = 0; i < X.getRowSize(); ++i) {
		for (size_t j = 0; j < X.getColSize(); ++j) {
			ret.setVal(j, i, X.getVal(i, j));
		}
	}
	return ret;
}

template <class T>
T dot(Matrix<T> a, Matrix<T> b) {
	assert(a.getRowSize() == b.getRowSize() && a.getColSize() == b.getColSize());
	T ret = 0;
	for (size_t i = 0; i < a.getRowSize(); ++i) {
		for (size_t j = 0; j < b.getColSize(); ++j) {
			ret += a.getVal(i, j) * b.getVal(i, j);
		}
	}
	return ret;
}

template <class T>
Matrix <T> operator * (const T& val, Matrix <T>& A) {
	Matrix <T> ret(A.getRowSize(), A.getColSize());
	for (size_t i = 0; i < A.getRowSize(); ++i) {
		for (size_t j = 0; j < A.getColSize(); ++j) {
			ret.setVal(i, j, val * A.getVal(i, j));
		}
	}
	return ret;
}

template < typename T >
class Matrix_coo {
public:
	vector < int > row, col;
	vector < T > val;
	int size, n, m;

	Matrix_coo() {}

	Matrix_coo(int N, int M) {
		n = N, m = M;
	}

	void Insert_element(int a, int b, T c) {
		row.push_back(a);
		col.push_back(b);
		val.push_back(c);
		size = row.size();
		return;
	}

	void print() {
		cout << "...matrix contents(COO)..." << endl;
		for (int i = 0; i < size; ++i) {
			cout << "[" << row[i] << "," << col[i] << "]: " << val[i] << endl;
		}
		return;
	}
};

template < class T>
Matrix <T> operator * (Matrix_coo <T> A, Matrix <T> b) {
	Matrix < T > ret(A.n, b.getColSize());
	for (size_t i = 0; i < A.size; ++i) {
		for (size_t j = 0; j < b.getColSize(); ++j) {
			ret.mat[A.row[i]][j] += A.val[i] * b.mat[A.col[i]][j];
		}
	}
	return ret;
}

template < class T>
Matrix_coo <T> operator * (const T& val, Matrix_coo <T> A) {
	Matrix_coo < T > ret(A.n, A.m);
	for (size_t i = 0; i < A.size; ++i) {
		ret.Insert_element(A.row[i], A.col[i], val * A.val[i]);
	}
	return ret;
}

int map_to_int(int i, int j, int nx, int ny) {
	if (i >= 0 && i < nx && j >= 0 && j < ny) {
		return i * ny + j;
	}
	return -1;
}

int dx[] = { -1, 0, 1, 0 };
int dy[] = { 0, 1, 0, -1 };

pair <Matrix <long double>, Matrix <long double>> generate_dense_matrix(int nx_max, int ny_max) {
	int nx = nx_max;
	int ny = ny_max;
	Matrix < long double > b(nx * ny, 1);
	for (int i = 0; i < b.getRowSize(); ++i) {
		b.mat[i][0] = (rand() % val_mx) * 1.0;
	}
	Matrix < long double > A(nx * ny, nx * ny);
	for (int i = 0; i < A.getRowSize(); ++i) {
		for (int j = 0; j < A.getColSize(); ++j) {
			A.mat[i][j] = 0;
		}
	}
	for (int i = 0; i < nx; ++i) {
		for (int j = 0; j < ny; ++j) {
			int curr = map_to_int(i, j, nx, ny);
			for (int k = 0; k < 4; ++k) {
				int r = map_to_int(i + dx[k], j + dy[k], nx, ny);
				if (r != -1) A.mat[curr][r] = 1;
			}
			A.mat[curr][curr] = -4;
		}
	}
	return { A, b };
}
pair <Matrix <long double>, Matrix <int> > generate_sparse_matrix(int nx_max, int ny_max) {
	int nx = nx_max;
	int ny = ny_max;
	Matrix < long double > b(nx * ny, 1);
	for (int i = 0; i < b.getRowSize(); ++i) {
		b.mat[i][0] = (rand() % val_mx) * 1.0;
	}
	Matrix < int > u(nx, ny);
	for (int i = 0; i < nx; ++i) {
		for (int j = 0; j < ny; ++j) {
			u.mat[i][j] = map_to_int(i, j, nx, ny);
		}
	}
	return { b, u };
}

void vector_sum_MASTER(Matrix<long double>& C, Matrix <long double>& A, Matrix <long double>& B, int n, int size, int op) {
	int operation = op, subDivide = max((n / (size - 1)), 1), start = 0, end = start + subDivide;
	for (int i = 1; i < size; ++i) {
		if (i == size - 1) end = n - 1;
		MPI_Send(&operation, 1, MPI_INT, i, 12345, MPI_COMM_WORLD);
		MPI_Send(&start, 1, MPI_INT, i, 1e5, MPI_COMM_WORLD);
		MPI_Send(&end, 1, MPI_INT, i, 2e5, MPI_COMM_WORLD);
		for (int j = start; j <= end; j++) {
			MPI_Send(&A.mat[j][0], 1, MPI_LONG_DOUBLE, i, j + 1e6, MPI_COMM_WORLD);
		}
		for (int j = start; j <= end; j++) {
			MPI_Send(&B.mat[j][0], 1, MPI_LONG_DOUBLE, i, j + 2e6, MPI_COMM_WORLD);
		}
		start = end + 1;
		end = min(n - 1, start + subDivide);
	}
	start = 0, end = start + subDivide;
	for (int i = 1; i < size; ++i) {
		if (i == size - 1) end = n - 1;
		for (int j = start; j <= end; j++) {
			MPI_Recv(&C.mat[j][0], 1, MPI_LONG_DOUBLE, i, j, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
		start = end + 1;
		end = min(n - 1, start + subDivide);
	}
}
void vector_sum(int rank) {
	int n, start, end;
	MPI_Recv(&start, 1, MPI_INT, 0, 1e5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	MPI_Recv(&end, 1, MPI_INT, 0, 2e5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	Matrix < long double > A(end - start + 1, 1), B(end - start + 1, 1), C(end - start + 1, 1);
	for (int i = 0; i < end - start + 1; i++) {
		MPI_Recv(&A.mat[i][0], 1, MPI_LONG_DOUBLE, 0, i + start + 1e6, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	for (int i = 0; i < end - start + 1; i++) {
		MPI_Recv(&B.mat[i][0], 1, MPI_LONG_DOUBLE, 0, i + start + 2e6, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	for (int i = 0; i < end - start + 1; ++i) {
		C.mat[i][0] = A.mat[i][0] + B.mat[i][0];
	}
	for (int i = 0; i < end - start + 1; ++i) {
		MPI_Send(&C.mat[i][0], 1, MPI_LONG_DOUBLE, 0, i + start, MPI_COMM_WORLD);
	}
	return;
}
void vector_swap_MASTER(Matrix <long double>& A, Matrix <long double>& B, int n, int size, int op) {
	int operation = op;
	int subDivide = max((n / (size - 1)), 1);
	int start = 0, end = start + subDivide;
	for (int i = 1; i < size; ++i) {
		if (i == size - 1) end = n - 1;
		MPI_Send(&operation, 1, MPI_INT, i, 12345, MPI_COMM_WORLD);
		MPI_Send(&start, 1, MPI_INT, i, 1e5, MPI_COMM_WORLD);
		MPI_Send(&end, 1, MPI_INT, i, 2e5, MPI_COMM_WORLD);
		for (int j = start; j <= end; j++) {
			MPI_Send(&A.mat[j][0], 1, MPI_LONG_DOUBLE, i, j + 1e6, MPI_COMM_WORLD);
		}
		for (int j = start; j <= end; j++) {
			MPI_Send(&B.mat[j][0], 1, MPI_LONG_DOUBLE, i, j + 2e6, MPI_COMM_WORLD);
		}
		start = end + 1;
		end = min(n - 1, start + subDivide);
	}
	start = 0, end = start + subDivide;
	for (int i = 1; i < size; ++i) {
		if (i == size - 1) end = n - 1;
		for (int j = start; j <= end; j++) {
			MPI_Recv(&A.mat[j][0], 1, MPI_LONG_DOUBLE, i, j + 2e7, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
		for (int j = start; j <= end; j++) {
			MPI_Recv(&B.mat[j][0], 1, MPI_LONG_DOUBLE, i, j + 1e7, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
		start = end + 1;
		end = min(n - 1, start + subDivide);
	}
}
void vector_swap(int rank) {
	int n, start, end;
	MPI_Recv(&start, 1, MPI_INT, 0, 1e5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	MPI_Recv(&end, 1, MPI_INT, 0, 2e5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	Matrix < long double > A(end - start + 1, 1), B(end - start + 1, 1);
	for (int i = 0; i <= end - start; i++) {
		MPI_Recv(&A.mat[i][0], 1, MPI_LONG_DOUBLE, 0, i + start + 1e6, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	for (int i = 0; i <= end - start; i++) {
		MPI_Recv(&B.mat[i][0], 1, MPI_LONG_DOUBLE, 0, i + start + 2e6, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	for (int i = 0; i <= end - start; ++i) {
		MPI_Send(&B.mat[i][0], 1, MPI_LONG_DOUBLE, 0, i + start + 2e7, MPI_COMM_WORLD);
	}
	for (int i = 0; i <= end - start; ++i) {
		MPI_Send(&A.mat[i][0], 1, MPI_LONG_DOUBLE, 0, i + start + 1e7, MPI_COMM_WORLD);
	}
	return;
}
void vector_scalar_mult_MASTER(Matrix<long double>& C, Matrix <long double>& A, long double alpha, int n, int size, int op) {
	int operation = op, subDivide = max((n / (size - 1)), 1), start = 0, end = start + subDivide;
	for (int i = 1; i < size; ++i) {
		if (i == size - 1) end = n - 1;
		MPI_Send(&operation, 1, MPI_INT, i, 12345, MPI_COMM_WORLD);
		MPI_Send(&start, 1, MPI_INT, i, 1e5, MPI_COMM_WORLD);
		MPI_Send(&end, 1, MPI_INT, i, 2e5, MPI_COMM_WORLD);
		MPI_Send(&alpha, 1, MPI_LONG_DOUBLE, i, 4e5, MPI_COMM_WORLD);
		for (int j = start; j <= end; j++) {
			MPI_Send(&A.mat[j][0], 1, MPI_LONG_DOUBLE, i, j + 1e6, MPI_COMM_WORLD);
		}
		start = end + 1;
		end = min(n - 1, start + subDivide);
	}
	start = 0, end = start + subDivide;
	for (int i = 1; i < size; ++i) {
		if (i == size - 1) end = n - 1;
		for (int j = start; j <= end; j++) {
			MPI_Recv(&C.mat[j][0], 1, MPI_LONG_DOUBLE, i, j, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
		start = end + 1;
		end = min(n - 1, start + subDivide);
	}
}
void vector_scalar_mult(int rank) {
	int n, start, end;
	long double alpha;
	MPI_Recv(&start, 1, MPI_INT, 0, 1e5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	MPI_Recv(&end, 1, MPI_INT, 0, 2e5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	MPI_Recv(&alpha, 1, MPI_LONG_DOUBLE, 0, 4e5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	Matrix < long double > A(end - start + 1, 1), C(end - start + 1, 1);
	for (int i = 0; i <= end - start; i++) {
		MPI_Recv(&A.mat[i][0], 1, MPI_LONG_DOUBLE, 0, i + start + 1e6, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	for (int i = 0; i <= end - start; ++i) {
		C.mat[i][0] = alpha * A.mat[i][0];
	}
	for (int i = 0; i <= end - start; ++i) {
		MPI_Send(&C.mat[i][0], 1, MPI_LONG_DOUBLE, 0, i + start, MPI_COMM_WORLD);
	}
	return;
}
long double vector_dot_MASTER(Matrix <long double>& A, Matrix<long double>& B, int n, int size, int op) {
	int operation = op, subDivide = max((n / (size - 1)), 1), start = 0, end = start + subDivide;
	for (int i = 1; i < size; ++i) {
		if (i == size - 1) end = n - 1;
		MPI_Send(&operation, 1, MPI_INT, i, 12345, MPI_COMM_WORLD);
		MPI_Send(&start, 1, MPI_INT, i, 1e5, MPI_COMM_WORLD);
		MPI_Send(&end, 1, MPI_INT, i, 2e5, MPI_COMM_WORLD);
		for (int j = start; j <= end; j++) {
			MPI_Send(&A.mat[j][0], 1, MPI_LONG_DOUBLE, i, j + 1e6, MPI_COMM_WORLD);
		}
		for (int j = start; j <= end; j++) {
			MPI_Send(&B.mat[j][0], 1, MPI_LONG_DOUBLE, i, j + 2e6, MPI_COMM_WORLD);
		}
		start = end + 1;
		end = min(n - 1, start + subDivide);
	}
	long double dot = 0;
	start = 0, end = start + subDivide;
	for (int i = 1; i < size; ++i) {
		if (i == size - 1) end = n - 1;
		long double dot1;
		MPI_Recv(&dot1, 1, MPI_LONG_DOUBLE, i, i * 10 + 123121, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		dot += dot1;
		start = end + 1;
		end = min(n - 1, start + subDivide);
	}
	return dot;
}
void vector_dot(int rank) {
	int n, start, end;
	long double alpha;
	MPI_Recv(&start, 1, MPI_INT, 0, 1e5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	MPI_Recv(&end, 1, MPI_INT, 0, 2e5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	Matrix < long double > A(end - start + 1, 1), B(end - start + 1, 1);
	for (int i = 0; i <= end - start; i++) {
		MPI_Recv(&A.mat[i][0], 1, MPI_LONG_DOUBLE, 0, i + start + 1e6, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	for (int i = 0; i <= end - start; i++) {
		MPI_Recv(&B.mat[i][0], 1, MPI_LONG_DOUBLE, 0, i + start + 2e6, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	long double dot = 0;
	for (int i = 0; i <= end - start; ++i) dot += A.mat[i][0] * B.mat[i][0];
	MPI_Send(&dot, 1, MPI_LONG_DOUBLE, 0, rank * 10 + 123121, MPI_COMM_WORLD);
	return;
}
void matrix_vector_mult_MASTER(Matrix <long double>& res, Matrix<int>& u, Matrix<long double>& b, int n, int dim, int size, int subDivide, int op) {
	int num_threads = size * size, Lr = 0, Lc = 0, Rr = subDivide - 1, Rc = subDivide - 1, zero = -1;
	for (int j = 0; j < n; ++j) {
		res.mat[j][0] = 0.0;
	}
	for (int i = 1; i <= num_threads; ++i) {

		MPI_Send(&op, 1, MPI_INT, i, 12345, MPI_COMM_WORLD);
		MPI_Send(&Lr, 1, MPI_INT, i, 1e5, MPI_COMM_WORLD);
		MPI_Send(&Lc, 1, MPI_INT, i, 2e5, MPI_COMM_WORLD);
		MPI_Send(&Rr, 1, MPI_INT, i, 3e5, MPI_COMM_WORLD);
		MPI_Send(&Rc, 1, MPI_INT, i, 4e5, MPI_COMM_WORLD);
		MPI_Send(&n, 1, MPI_INT, i, 5e5, MPI_COMM_WORLD);

		for (int j = Lr - 1; j <= Rr + 1; ++j) {
			for (int k = Lc - 1; k <= Rc + 1; ++k) {
				if (j >= 0 && j < dim && k >= 0 && k < dim) {
					MPI_Send(&u.mat[j][k], 1, MPI_INT, i, 1e4 * (j + 1) + (k + 1), MPI_COMM_WORLD);
				}
				else {
					MPI_Send(&zero, 1, MPI_INT, i, 1e4 * (j + 1) + (k + 1), MPI_COMM_WORLD);
				}
			}
		}

		for (int j = 0; j < n; ++j) {
			MPI_Send(&b.mat[j][0], 1, MPI_LONG_DOUBLE, i, 1e6 + j, MPI_COMM_WORLD);
		}
		Lc += subDivide;
		Rc += subDivide;
		if (Lc >= dim) {
			Lc = 0;
			Rc = subDivide - 1;
			Lr += subDivide;
			Rr += subDivide;
		}
	}

	long double gather = 0;
	for (int i = 1; i <= num_threads; ++i) {
		for (int j = 0; j < n; ++j) {
			MPI_Recv(&gather, 1, MPI_LONG_DOUBLE, i, 1e8 + j, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			res.mat[j][0] += gather;
		}
	}

	return;
}
void matrix_vector_mult(int rank) {
	int Lr, Lc, Rr, Rc, n;

	MPI_Recv(&Lr, 1, MPI_INT, 0, 1e5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	MPI_Recv(&Lc, 1, MPI_INT, 0, 2e5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	MPI_Recv(&Rr, 1, MPI_INT, 0, 3e5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	MPI_Recv(&Rc, 1, MPI_INT, 0, 4e5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	MPI_Recv(&n, 1, MPI_INT, 0, 5e5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	Matrix < int > u(Rr - Lr + 3, Rc - Lc + 3);
	Matrix < long double > b(n, 1);

	for (int i = 0; i < Rr - Lr + 3; ++i) {
		for (int j = 0; j < Rc - Lc + 3; ++j) {
			MPI_Recv(&u.mat[i][j], 1, MPI_INT, 0, 1e4 * (Lr + i) + (Lc + j), MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
	}
	for (int i = 0; i < n; ++i) {
		MPI_Recv(&b.mat[i][0], 1, MPI_LONG_DOUBLE, 0, 1e6 + i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	Matrix < long double > res(n, 1);
	for (int i = 0; i < n; ++i) res.mat[i][0] = 0.0;
	for (int i = 1; i <= Rr - Lr + 1; ++i) {
		for (int j = 1; j <= Rc - Lc + 1; ++j) {
			int curr_pos = u.mat[i][j];
			res.mat[curr_pos][0] = 4.0 * b.mat[curr_pos][0];
			if (u.mat[i - 1][j] >= 0) res.mat[curr_pos][0] -= b.mat[u.mat[i - 1][j]][0];
			if (u.mat[i + 1][j] >= 0) res.mat[curr_pos][0] -= b.mat[u.mat[i + 1][j]][0];
			if (u.mat[i][j - 1] >= 0) res.mat[curr_pos][0] -= b.mat[u.mat[i][j - 1]][0];
			if (u.mat[i][j + 1] >= 0) res.mat[curr_pos][0] -= b.mat[u.mat[i][j + 1]][0];
		}
	}
	for (int i = 0; i < n; ++i) {
		MPI_Send(&res.mat[i][0], 1, MPI_LONG_DOUBLE, 0, 1e8 + i, MPI_COMM_WORLD);
	}
	return;
}

int main(int argc, char* argv[]) {

	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (rank == MASTER) {
		auto t = generate_sparse_matrix(40, 40);
		auto b = t.first; auto u = t.second;

		Matrix <long double> R0(b), R1(b.getRowSize(), 1);
		Matrix <long double> P0(b), P1(b.getRowSize(), 1);
		Matrix <long double> X0(b.getRowSize(), 1), X1(b.getRowSize(), 1);
		Matrix <long double> temp(b.getRowSize(), 1), another_temp(b.getRowSize(), 1);

		int True = 1, False = 0, itr = 0, row_size = 1, n = X0.getRowSize(), sq = u.getRowSize();
		clock_t begin = clock();
		cout << "..... Running Solver ....." << endl;

		for (int i = 2; i * i <= sq; ++i) {
			if (sq % i == 0 && i * i + 1 <= size) {
				row_size = i;
			}
		}

		while (sqrt(dot(R0, R0)) / sqrt(dot(b, b)) >= EPS) {

			for (int i = 1; i < size; ++i) {
				MPI_Send(&True, 1, MPI_INT, i, 1234, MPI_COMM_WORLD);
			}

			++itr;
			//cout << "#iteration: " << itr << endl;

			// alpha = (trans(R0) * R0) / (trans(P0) * A * P0);
			long double alpha = vector_dot_MASTER(R0, R0, n, size, 4);
			matrix_vector_mult_MASTER(temp, u, P0, n, sq, row_size, sq / row_size, 5);
			alpha /= vector_dot_MASTER(temp, P0, n, size, 4);

			// X1 = X0 + alpha * P0; 
			vector_scalar_mult_MASTER(temp, P0, alpha, n, size, 3);
			vector_sum_MASTER(X1, X0, temp, n, size, 1);

			// R1 = R0 - alpha * A * P0;
			alpha *= -1.0;
			matrix_vector_mult_MASTER(another_temp, u, P0, n, sq, row_size, sq / row_size, 5);
			vector_scalar_mult_MASTER(temp, another_temp, alpha, n, size, 3);
			vector_sum_MASTER(R1, R0, temp, n, size, 1);

			// beta = (trans(R1) * R1) / (trans(R0) * R0);
			long double beta = vector_dot_MASTER(R1, R1, n, size, 4) / vector_dot_MASTER(R0, R0, n, size, 4);

			// P1 = R1 + beta * P0; 
			vector_scalar_mult_MASTER(temp, P0, beta, n, size, 3);
			vector_sum_MASTER(P1, R1, temp, n, size, 1);

			// for the next iteration 
			vector_swap_MASTER(R0, R1, n, size, 2);
			vector_swap_MASTER(X0, X1, n, size, 2);
			vector_swap_MASTER(P0, P1, n, size, 2);
		}

		for (int i = 1; i < size; ++i) {
			MPI_Send(&False, 1, MPI_INT, i, 1234, MPI_COMM_WORLD);
		}

		clock_t end = clock();
		long double elapsed_secs = (long double)(end - begin) / CLOCKS_PER_SEC;
		cout << "Time Elapsed: " << elapsed_secs << " sec" << endl;
		cout << "Num. Iterations: " << itr << endl;
		cout << "Error: " << dot(R0, R0) << endl;
	}
	else {
		int num_parallel_ops = 15;
		int rank, operation, cont;
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);

		while (true) {
			MPI_Recv(&cont, 1, MPI_INT, 0, 1234, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			if (!cont) break;
			for (int i = 1; i <= num_parallel_ops; ++i) {
				MPI_Recv(&operation, 1, MPI_INT, 0, 12345, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				if (operation == 1) {
					vector_sum(rank);
				}
				else if (operation == 2) {
					vector_swap(rank);
				}
				else if (operation == 3) {
					vector_scalar_mult(rank);
				}
				else if (operation == 4) {
					vector_dot(rank);
				}
				else if (operation == 5) {
					matrix_vector_mult(rank);
				}
			}
		}
	}

	MPI_Finalize();
	return 0;
}

template <class T>
T Adot(Matrix<T>& a, Matrix_coo<T>& A, Matrix<T>& b) {
	Matrix <T> ret = trans(a) * (A * b);
	return ret.mat[0][0];
}

template <class T>
Matrix <T> conjugate_gradient(Matrix_coo <T>& A, Matrix <T>& b) {
	clock_t begin = clock();
	Matrix <T> R0(b.getRowSize(), 1), R1(b.getRowSize(), 1);
	Matrix <T> P0(b.getRowSize(), 1), P1(b.getRowSize(), 1);
	Matrix <T> X0(b.getRowSize(), 1), X1(b.getRowSize(), 1);
	int itr = 0;
	cout << "..... Running Normal Solver ....." << endl;
	P0 = R0 = b - (A * X0);
	while (sqrt(dot(R0, R0)) / sqrt(dot(b, b)) >= EPS) {
		++itr;
		T alpha = dot(R0, R0) / Adot(P0, A, P0);
		X1 = X0 + alpha * P0;
		R1 = R0 - alpha * A * P0;
		T beta = dot(R1, R1) / dot(R0, R0);
		P1 = R1 + beta * P0;
		swap(R0, R1);
		swap(X0, X1);
		swap(P0, P1);
	}
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	cout << "Time Elapsed: " << elapsed_secs << " sec" << endl;
	cout << "Iterations: " << itr << endl;
	cout << "Absolute Error: " << dot(b - A * X0, b - A * X0) << endl;
	cout << endl;
	return X0;
}

template <class T>
Matrix <T> preconditioned_conjugate_gradient(Matrix_coo<T>& B, Matrix_coo<T>& A, Matrix <T>& b) {
	clock_t begin = clock();
	Matrix <T> R0(b.getRowSize(), 1), R1(b.getRowSize(), 1);
	Matrix <T> P0(b.getRowSize(), 1), P1(b.getRowSize(), 1);
	Matrix <T> X0(b.getRowSize(), 1), X1(b.getRowSize(), 1);
	Matrix <T> Z0(b.getRowSize(), 1), Z1(b.getRowSize(), 1);
	Matrix <T> br(b.getRowSize(), 1);
	int itr = 0;
	R0 = b - A * X0;
	P0 = Z0 = B * R0;
	cout << "..... Running Preconditioned Solver ....." << endl;
	while ((sqrt(dot(R0, R0)) / sqrt(dot(b, b))) >= EPS) {
		++itr;
		T alpha = dot(R0, Z0) / Adot(P0, A, P0);
		X1 = X0 + alpha * P0;
		R1 = R0 - alpha * A * P0;
		Z1 = B * R1;
		T beta = dot(R1, Z1) / dot(R0, Z0);
		P1 = Z1 + beta * P0;
		swap(R0, R1);
		swap(X0, X1);
		swap(P0, P1);
		swap(Z0, Z1);
		br = b - A * X0;
	}
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	cout << "Time Elapsed: " << elapsed_secs << " sec" << endl;
	cout << "Iterations: " << itr << endl;
	cout << "Absolute Error: " << dot(b - A * X0, b - A * X0) << endl;
	cout << endl;
	return X0;
}