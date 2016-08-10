#pragma once

#include "ComplexNumber.h"
#include "Vector.h"
#include "Utils.h"
#include "Exceptions.h"

#include <vector>
#include <algorithm>

NS_BEGIN_NAMESPACE

template<typename T>
class SparseMatrix;

/**
 * @brief An Iterator to traverse through the filled entries of a sparse matrix.
 * @tparam T Internal data type.
 * @details This iterator can not be created outside of SparseMatrix.
 * @sa SparseMatrix
 * @sa SparseMatrixRowIterator
 * @sa SparseMatrixColumnIterator
 */
template<typename T>
class SparseMatrixIterator
{
protected:
	friend SparseMatrix<T>;
	SparseMatrixIterator(const SparseMatrix<T>& m, Index i1, Index i2);

public:
	/** 
	 * @brief Returns the current row.
	 * @details If row is equal to the row count of the sparse matrix,
	 * this iterator represents an end iterator and shouldn't be accessed.
	 * @return Row index this iterator is pointing at.
	 * @sa column()
	 */
	Index row() const
	{
		return mIndex1;
	}

	/**
	* @brief Returns the current column.
	* @details If column is equal to the column count of the sparse matrix,
	* this iterator represents an end iterator and shouldn't be accessed.
	* @return Column index this iterator is pointing at.
	* @sa row()
	*/
	Index column() const
	{
		return mIndex2;
	}

	/**
	* @brief Checks if iterator is valid
	*/
	bool isValid() const
	{
		return mIndex1 < mMatrix->rows() && mIndex2 < mMatrix->columns();
	}

	/**
	 * @brief Checks if both iterators are equal.
	 * @param other The other iterator. Has to be from the same sparse matrix.
	 * @return Returns true if the iterators match, false otherwise.
	 * @sa operator!=()
	 */
	bool operator ==(const SparseMatrixIterator& other) const
	{
		return  (!isValid() && !other.isValid()) ||
			(mMatrix == other.mMatrix && mIndex1 == other.mIndex1 && mIndex2 == other.mIndex2);
	}

	/**
	 * @brief Checks if both iterators are not equal.
	 * @param other The other iterator.
	 * @return Returns false if the iterators match, true otherwise.
	 * @sa operator==()
	 */
	bool operator !=(const SparseMatrixIterator& other) const
	{
		return !(*this == other);
	}

	/**
	 * @brief Accessing the filled entry this iterator is pointing at.
	 * @return Returns the copy of the entry at the position `at(row(),column())`
	 * @sa row()
	 * @sa column()
	 */
	T operator *() const;

	/**
	* @brief Accessing the filled entry this iterator is pointing at.
	* @return Returns a reference of the entry at the position `at(row(),column())`
	* @sa row()
	* @sa column()
	*/
	T& operator *();

	/**
	 * @brief Moves the iterator forward.
	 * @details The moving direction is from left to right, top to down.\n
	 * First all the columns from the current row are traversed and then the next row.
	 * @return Reference to `this` iterator.
	 */
	virtual SparseMatrixIterator& operator++ ();

	/**
	* @brief Moves the iterator forward.
	* @details The moving direction is from left to right, top to down.\n
	* First all the columns from the current row are traversed and then the next row.
	* @return Copy of `this` iterator before moving forward.
	*/
	SparseMatrixIterator operator++ (int);

protected:
	const SparseMatrix<T>* mMatrix;
	Index mIndex1;
	Index mIndex2;
	Index mColumnPtrIndex;
};

/**
* @brief An Iterator to traverse through one row of a sparse matrix.
* @tparam T Internal data type.
* @details This iterator can not be created outside of SparseMatrix.
* @sa SparseMatrix
* @sa SparseMatrixIterator
* @sa SparseMatrixColumnIterator
*/
template<typename T>
class SparseMatrixRowIterator : public SparseMatrixIterator<T>
{
private:
	friend SparseMatrix<T>;
	SparseMatrixRowIterator(const SparseMatrix<T>& m, Index i1, Index i2);

public:
	/**
	* @brief Moves the iterator forward.
	* @details The moving direction is from left to right.\n
	* Only the current row will be traversed.
	* @return Reference to `this` iterator.
	*/
	SparseMatrixRowIterator& operator++ ();

	/**
	* @brief Moves the iterator forward.
	* @details The moving direction is from left to right.\n
	* Only the current row will be traversed.
	* @return Copy of `this` iterator before moving forward.
	*/
	SparseMatrixRowIterator operator++ (int);
};

/**
* @brief An Iterator to traverse through one column of a sparse matrix.
* @attention This iterator type does not give you any performance benefits. Use Row or Standard if possible.
* @tparam T Internal data type.
* @details This iterator can not be created outside of SparseMatrix.
* @sa SparseMatrix
* @sa SparseMatrixIterator
* @sa SparseMatrixRowIterator
*/
template<typename T>
class SparseMatrixColumnIterator : public SparseMatrixIterator<T>
{
private:
	friend SparseMatrix<T>;
	SparseMatrixColumnIterator(const SparseMatrix<T>& m, Index i1, Index i2);

public:
	/**
	* @brief Moves the iterator forward.
	* @details The moving direction is from top to down.\n
	* Only the current column will be traversed.
	* @return Reference to `this` iterator.
	*/
	SparseMatrixColumnIterator& operator++ ();

	/**
	* @brief Moves the iterator forward.
	* @details The moving direction is from top to down.\n
	* Only the current column will be traversed.
	* @return Copy of `this` iterator before moving forward.
	*/
	SparseMatrixColumnIterator operator++ (int);
};

/**
 * @brief A high dimensional sparse matrix class.
 * @details A sparse matrix is most of the time the right decision,
 * but for little sized matrices a dense Matrix implementation is recommended.\n
 * Internally it uses the CRS (Compressed Row Storage) method.\n
 * Using the row iterator or the standard iterator is recommend over direct access or the column iterator.
 *
 * @par Topological Order
 * The order of the entries is from left to right, and then from top to down.\n
 * Columns are traversed first in each row.\n
 * Access is the same as in most math scripts.
 * \f$A_{ij}\f$ is the entry at the row `i` and column `j`, which would be the same as `A.at(i,j)` in NumericSim.
 *
 * @note All complexity values are calculated with all operations of T assumed to be of \f$ O(1) \f$ complexity.
 *
 * @tparam T Internal data type.
 * @sa Matrix
 */
template<typename T>
class SparseMatrix
{
	friend SparseMatrixIterator<T>;
	friend SparseMatrixRowIterator<T>;
	friend SparseMatrixColumnIterator<T>;
private:
	std::vector<T> mValues;
	std::vector<Index> mColumnPtr;
	std::vector<Index> mRowPtr;

	Dimension mColumnCount;

	T mEmpty;//Used for references; Could also be static, but that would break the header only approach

private:
	const T& internal_at(Index i1, Index i2, Index& columnPtrIndex, bool& found, bool needNear = false) const;
	void remove_at(Index i1, Index i2);
	void set_at(Index i1, Index i2, const T& v);
	size_t row_entry_count(Index i, Index& rowPtr) const;

public:
	/**
	 * @brief The standard iterator.
	 * @sa SparseMatrixIterator
	 */
	typedef SparseMatrixIterator<T> iterator;

	/**
	* @brief A const variant of the standard iterator.
	* @sa SparseMatrixIterator
	*/
	typedef const SparseMatrixIterator<T> const_iterator;

	/**
	* @brief The row iterator.
	* @sa SparseMatrixRowIterator
	*/
	typedef SparseMatrixRowIterator<T> row_iterator;

	/**
	* @brief A const variant of the row iterator.
	* @sa SparseMatrixRowIterator
	*/
	typedef const SparseMatrixRowIterator<T> const_row_iterator;

	/**
	* @brief The column iterator.
	* @sa SparseMatrixIterator
	*/
	typedef SparseMatrixColumnIterator<T> column_iterator;

	/**
	* @brief A const variant of the column iterator.
	* @sa SparseMatrixIterator
	*/
	typedef const SparseMatrixColumnIterator<T> const_column_iterator;

	/**
	* @brief Constructs an empty sparse matrix of size(d1,d2)
	* @param d1 Row dimension
	* @param d2 Column dimension
	 */
	SparseMatrix(Dimension d1, Dimension d2);

	/**
	 * @brief Constructs a sparse matrix from the two dimensional initializer list.
	 * @details The initializer list size dynamically sets the dimension of the matrix.
	 *
	 * @par Example
	 * `SparseMatrix<float> A = { {1,0,1},{0,1,0},{2,0,2} }` will generate the sparse matrix 
	 * `[(0,0;1),(0,2;1),(1,1;1),(2,0;2),(2,2;2)]`\n
	 * Here every entry in parentheses represent an entry with (row,column;value).
	 *
	 * @note Every missing entry will be initialized with 0.
	 *
	 * @param list A two dimensional initializer list.
	 * @throw MatrixInitializerListFailedException
	 */
	SparseMatrix(std::initializer_list<std::initializer_list<T> > list);

	virtual ~SparseMatrix();

	/**
	* @brief Resize dimension of the matrix.
	* @par Complexity
	* Unknown
	* @param d1 New row dimension.
	* @param d2 New column dimension.
	* @todo Implement this!
	*/
	void resize(Dimension d1, Dimension d2);

	/**
	 * @brief Returns an iterator pointing at the first topological entry.
	 * @par Complexity
	 * Worst case: \f$ O(D1) \f$
	 * @return Const sparse matrix iterator.
	 */
	const_iterator begin() const;

	/**
	* @brief Returns an iterator representing the end.
	* @note This iterator can not be dereferenced.
	* @par Complexity
	* Always: \f$ O(1) \f$
	* @return Const sparse matrix iterator.
	*/
	const_iterator end() const;

	/**
	 * @brief Returns an iterator pointing at the first topological entry.
	 * @par Complexity
	 * Worst case: \f$ O(D1) \f$
	 * @return Sparse matrix iterator.
	 */
	iterator begin();

	/**
	* @brief Returns an iterator representing the end.
	* @note This iterator can not be dereferenced.
	* @par Complexity
	* Always: \f$ O(1) \f$
	* @return Sparse matrix iterator.
	*/
	iterator end();

	/**
	* @copydoc begin() const
	*/
	const_iterator cbegin() const;

	/**
	* @copydoc end() const
	*/
	const_iterator cend() const;

	/**
	* @brief Returns a row iterator pointing at the first topological entry at the given row.
	* @par Complexity
	* Always: \f$ O(1) \f$
	* @param i Index of the row.
	* @return Const sparse matrix row iterator.
	*/
	const_row_iterator row_begin(Index i) const;

	/**
	* @brief Returns a row iterator representing the end of the row.
	* @note This iterator can not be dereferenced.
	* @par Complexity
	* Always: \f$ O(1) \f$
	* @param i Index of the row.
	* @return Const sparse matrix row iterator.
	*/
	const_row_iterator row_end(Index i) const;

	/**
	* @brief Returns a row iterator pointing at the first topological entry at the given row.
	* @par Complexity
	* Always: \f$ O(1) \f$
	* @param i Index of the row.
	* @return Sparse matrix row iterator.
	*/
	row_iterator row_begin(Index i);

	/**
	* @brief Returns a row iterator representing the end.
	* @note This iterator can not be dereferenced.
	* @par Complexity
	* Always: \f$ O(1) \f$
	* @param i Index of the row.
	* @return Sparse matrix row iterator.
	*/
	row_iterator row_end(Index i);

	/**
	* @copydoc row_begin(Index) const
	*/
	const_row_iterator row_cbegin(Index i) const;

	/**
	* @copydoc row_end(Index) const
	*/
	const_row_iterator row_cend(Index i) const;

	/**
	* @brief Returns a column iterator pointing at the first topological entry at the given column.
	* @par Complexity
	* Worst case: \f$ O(D1+D2) \f$
	* @param j Index of the column.
	* @return Const sparse matrix column iterator.
	*/
	const_column_iterator column_begin(Index j) const;

	/**
	* @brief Returns a column iterator representing the end of the column.
	* @note This iterator can not be dereferenced.
	* @par Complexity
	* Always: \f$ O(1) \f$
	* @param j Index of the column.
	* @return Const sparse matrix column iterator.
	*/
	const_column_iterator column_end(Index j) const;

	/**
	* @brief Returns a column iterator pointing at the first topological entry at the given column.
	* @par Complexity
	* Worst case: \f$ O(D1+D2) \f$
	* @param j Index of the column.
	* @return Sparse matrix column iterator.
	*/
	column_iterator column_begin(Index j);

	/**
	* @brief Returns a column iterator representing the end.
	* @note This iterator can not be dereferenced.
	* @par Complexity
	* Always: \f$ O(1) \f$
	* @param j Index of the column.
	* @return Sparse matrix column iterator.
	*/
	column_iterator column_end(Index j);

	/**
	* @copydoc column_begin(Index) const
	*/
	const_column_iterator column_cbegin(Index j) const;

	/**
	* @copydoc column_end(Index) const
	*/
	const_column_iterator column_cend(Index j) const;

	/**
	 * @brief Returns a copy of the value at the respective location.
	 * @par Complexity
	 * Worst case: \f$ O(D2) \f$
	 * @param i Index of the row.
	 * @param j Index of the column.
	 * @return Copy of the value at \f$ A_{ij} \f$
	 * @sa linear_at
	 */
	T at(Index i, Index j) const;

	/**
	* @brief Sets the value at the respective location.
	* @par Complexity
	* Worst case: \f$ O(D1*D2) \f$\n
	* If entry already exists and `val != 0` then \f$ O(D2) \f$
	* @param i Index of the row.
	* @param j Index of the column.
	* @param val New value replacing the old one.
	* @note If `val == 0` the entry will be removed.
	* @sa linear_set
	*/
	void set(Index i, Index j, const T& val);

	/**
	 * @brief Returns a copy of the value at the respective location based on linear coordinates.
	 * @details The transformation is based on \f$ lin = i*D_2 + j \f$.
	 * @par Complexity
	 * Worst case: \f$ O(D2) \f$
	 * @param i Linear index.
	 * @return Copy of the value.
	 * @sa at
	 */
	T linear_at(Index i) const;

	/**
	* @brief Sets the value at the respective location.
	* @details The transformation is based on \f$ lin = i*D_2 + j \f$.
	* @par Complexity
	* Worst case: \f$ O(D1*D2) \f$\n
	* If entry already exists and `val != 0` then \f$ O(D2) \f$
	* @param i Linear index.
	* @param val New value replacing the old one.
	* @note If `val == 0` the entry will be removed.
	* @sa set
	*/
	void linear_set(Index i, const T& val);

	/**
	 * @brief Returns true if the entry at the respective location has a value different then 0.
	 * @details Same as `at(i,j) != 0`
	 * @par Complexity
	 * Worst case: \f$ O(D2) \f$
	 * @param i Index of the row.
	 * @param j Index of the column.
	 * @return true if `at(i,j) != 0`, false otherwise.
	 * @sa at
	 * @sa has(Index,Index,T&) const
	 */
	bool has(Index i, Index j) const;

	/**
	* @brief Returns true if the entry at the respective location has a value different then 0.
	* @details Same as `at(i,j) != 0`
	* @par Complexity
	* Worst case: \f$ O(D2) \f$
	* @param i Index of the row.
	* @param j Index of the column.
	* @param[out] val The value at(i,j)
	* @return true if `at(i,j) != 0`, false otherwise.
	* @sa at
	* @sa has(Index,Index) const
	*/
	bool has(Index i, Index j, T& val) const;

	/**
	* @brief Sets the value of the entry the iterator is pointing at.
	* @par Complexity
	* Worst case (if `val != 0`): \f$ O(1) \f$\n
	* Worst case (if `val == 0`): \f$ O(D1*D2) \f$
	* @param it Standard iterator.
	* @param val The new value.
	* @return A new iterator pointing at a different location (when `val == 0`) or the same as `it`.
	* @note If `val == 0` the entry will be removed and this operation is the same as erase(const SparseMatrixIterator<T, D1, D2>&).
	* @sa erase(const iterator&);
	*/
	iterator set(const iterator& it, const T& val);

	/**
	* @brief Removes the entry the iterator is pointing at.
	* @par Complexity
	* Worst case: \f$ O(D1*D2) \f$
	* @param it Standard iterator.
	* @return A new iterator pointing at the next entry or to end().
	* @sa set(const iterator&, const T&);
	*/
	iterator erase(const iterator& it);

	/**
	* @brief Adds entries element wise.
	* @par Complexity
	* Worst case: \f$ O((D1*D2)^2) \f$
	* @param m The other matrix.
	* @return A reference to this matrix.
	* @todo Is there a better approach, without using set(i,j,v)?
	*/
	SparseMatrix& operator +=(const SparseMatrix& m);

	/**
	* @brief Subtracts entries element wise.
	* @par Complexity
	* Worst case: \f$ O((D1*D2)^2) \f$
	* @param m The other matrix.
	* @return A reference to this matrix.
	* @todo Is there a better approach, without using set(i,j,v)?
	*/
	SparseMatrix& operator -=(const SparseMatrix& m);

	/**
	* @brief Multiplies entries element wise.
	* @note Don't mistake this with real matrix matrix multiplication.\n
	* Use mul() instead for matrix matrix multiplication.
	* @par Complexity
	* Worst case: \f$ O((D1*D2)^2) \f$
	* @param m The other matrix.
	* @return A reference to this matrix.
	* @todo Is there a better approach?
	*/
	SparseMatrix& operator *=(const SparseMatrix& m);

	/**
	* @brief Multiplies entries with a scalar.
	* @par Complexity
	* Worst case: \f$ O(D1*D2) \f$
	* @param f Scalar.
	* @return A reference to this matrix.
	* @todo Is there a better approach?
	*/
	SparseMatrix& operator *=(const T& f);

	/**
	* @brief The column count
	* @par Complexity
	* Always: \f$ O(1) \f$
	* @return D2
	*/
	constexpr Dimension columns() const;

	/**
	* @brief The row count
	* @par Complexity
	* Always: \f$ O(1) \f$
	* @return D1
	*/
	constexpr Dimension rows() const;

	/**
	* @brief The linear dimension of the matrix.
	* @par Complexity
	* Always: \f$ O(1) \f$
	* @return D1*D2
	*/
	constexpr Dimension size() const;

	/**
	* @brief The amount of entries currently filled (v != 0)
	* @par Complexity
	* Always: \f$ O(1) \f$
	* @return A value between 0 and D1*D2.
	*/
	Dimension filledCount() const;

	/**
	 * @brief The sum of all entries.
	 * @par Complexity
	 * Worst case: \f$ O(D1*D2) \f$
	 * @return Sum of entries \f$ \sum A_{ij} \f$
	 */
	T sum() const;

	/**
	* @brief The max element in the matrix. Counts empty entries as 0.
	* @par Complexity
	* Worst case: \f$ O(D1*D2) \f$
	* @return The maximum entry \f$ \max A_{ij} \f$
	*/
	T max() const;

	/**
	* @brief The min element in the matrix. Counts empty entries as 0.
	* @par Complexity
	* Worst case: \f$ O(D1*D2) \f$
	* @return The maximum entry \f$ \min A_{ij} \f$
	*/
	T min() const;

	/**
	* @brief The average of all entries. Counts empty entries as 0.
	* @par Complexity
	* Worst case: \f$ O(D1*D2) \f$
	* @return The average of all entries \f$ \frac{\sum A_{ij}}{D1*D2} \f$
	*/
	T avg() const;

	/**
	* @brief Swaps the entries of the matrices.
	* @par Complexity
	* Always: \f$ O(1) \f$
	* @param m The other matrix.
	*/
	void swap(SparseMatrix& m);

	/**
	* @brief Returns true if matrix has NaN entries
	* @par Complexity
	* Worst case: \f$ O(D1*D2) \f$
	* @return True if matrix has a NaN entry, false otherwise.
	*/
	bool hasNaN() const;

	/**
	* @brief Returns true if matrix has infinite entries
	* @par Complexity
	* Worst case: \f$ O(D1*D2) \f$
	* @return True if matrix has a infinite entry, false otherwise.
	*/
	bool hasInf() const;

	/**
	* @brief Returns true if matrix has a entry with 0.
	* @par Complexity
	* Always: \f$ O(1) \f$
	* @return True if matrix has an 0 entry, false otherwise.
	* @sa isEmpty()
	*/
	bool hasZero() const;

	/**
	* @brief Returns true if matrix has no entries.
	* @par Complexity
	* Always: \f$ O(1) \f$
	* @return True if the matrix is empty, false otherwise.
	* @sa hasZero()
	*/
	bool isEmpty() const;// everything is zero!

	/**
	* @brief Transpose matrix.
	* @par Complexity
	* Worst case: \f$ O((D1*D2)^2) \f$
	* @note When using complex matrices most of the time you need the adjugate() operation,\n
	* as it is the most reasonable one in most of the cases. (Consider looking at your script)
	* @return Transpose of the matrix \f$ A^T \f$
	* @sa adjugate()
	*/
	SparseMatrix transpose();

	/**
	* @brief The conjugate of all entries if complex.
	* @par Complexity
	* Worst case: \f$ O((D1*D2)^2) \f$
	* @return Transpose of the matrix \f$ A^T \f$
	*/
	SparseMatrix conjugate();

	/**
	* @brief The adjugate / conjugate transpose of the matrix.
	* @note If the matrix has no complex entries, it is the same as transpose().
	* @par Complexity
	* Worst case: \f$ O((D1*D2)^2) \f$
	* @return Conjugate transpose of the matrix \f$ A^* \f$
	* @sa transpose()
	*/
	SparseMatrix adjugate();

	/**
	* @brief Returns the trace of the matrix.
	* @details The trace of the matrix is defined as:\n
	* \f[
	* trace := \sum_{i}^{\min(D1,D2)} A_{ii}
	* \f]
	* @par Complexity
	* Worst case: \f$ O(min(D1*D2,D2*D2)) \f$
	* @return The trace of the matrix
	*/
	T trace() const;

	/**
	* @brief Right side matrix matrix multiplication.
	* @details The matrix matrix multiplication is defined as:\n
	* \f[
	* A.mul(B) := A \cdot B \textrm{ with } A \in T^{D1 \times D2} \times B \in T^{D2 \times D3} \to C \in T^{D1 \times D3}
	* \f]
	* @par Complexity
	* Worst case: \f$ O(D1*D3*(D1*D3+D2^2+D2*D3)) \f$
	* @param right The other sparse matrix, which row count must match the column count of this matrix.
	* @return The result of the matrix multiplication.
	* @todo This method is critical. A sparse solution is needed.
	*/
	SparseMatrix mul(const SparseMatrix& right) const;

	/**
	* @brief Right side matrix vector multiplication.
	* @par Complexity
	* Worst case: \f$ O(D1*D2*(D1+D2)) \f$
	* @param right A vector with the same size as the column count of the matrix.
	* @return A vector with the same size as the row count.
	* @todo This method is critical. A better sparse solution is needed.
	*/
	Vector<T> mul(const Vector<T>& right) const;

	/**
	* @brief Left side matrix vector multiplication.
	* @par Complexity
	* Worst case: \f$ O(D1*D2*(D1+D2)) \f$
	* @param left A vector with the same size as the row count of the matrix.
	* @return A vector with the same size as the column count.
	* @todo This method is critical. A better sparse solution is needed.
	*/
	Vector<T> mul_left(const Vector<T>& left) const;
};

// Element wise operations
template<typename T>
SparseMatrix<T> operator +(const SparseMatrix<T>& v1, const SparseMatrix<T>& v2);
template<typename T>
SparseMatrix<T> operator -(const SparseMatrix<T>& v1, const SparseMatrix<T>& v2);
template<typename T>
SparseMatrix<T> operator -(const SparseMatrix<T>& v);
template<typename T>
SparseMatrix<T> operator *(const SparseMatrix<T>& v1, const SparseMatrix<T>& v2);
template<typename T>
SparseMatrix<T> operator *(const SparseMatrix<T>& v1, T f);
template<typename T>
SparseMatrix<T> operator *(T f, const SparseMatrix<T>& v1);

// Comparison
template<typename T>
bool operator ==(const SparseMatrix<T>& v1, const SparseMatrix<T>& v2);
template<typename T>
bool operator !=(const SparseMatrix<T>& v1, const SparseMatrix<T>& v2);

NS_END_NAMESPACE

#define _NS_SPARSEMATRIX_INL
# include "SparseMatrix.inl"
#undef _NS_SPARSEMATRIX_INL