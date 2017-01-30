#pragma once

#include "Types.h"
#include "Vector.h"
#include "Utils.h"
#include "Exceptions.h"

#include <vector>
#include <algorithm>

NS_BEGIN_NAMESPACE

// Types for dynamic and fixed matrix types
template<typename T, Dimension K1, Dimension K2>
struct DataContainer2D
{};

template<typename T>
struct DataContainer2D<std::vector<T>,0,0>
{
	typedef std::vector<T> container_type;

	container_type Container;
	Dimension RowCount=0;
	Dimension ColumnCount=0;
};

template<typename T, Dimension K1, Dimension K2>
struct DataContainer2D<std::array<T,K1*K2>,K1,K2>
{
	typedef std::array<T,K1*K2> container_type;

	container_type Container;
	static constexpr Dimension RowCount = K1;
	static constexpr Dimension ColumnCount = K2;
};

template<typename T>
using dynamic_container2d_t = DataContainer2D<std::vector<T>,0,0>;

template<typename T, Dimension K1, Dimension K2>
using fixed_container2d_t = DataContainer2D<std::array<T,K1*K2>,K1,K2>;

// -----------------------------------------------

template<typename T, class DC>
class BaseMatrix;

/**
* @brief An Iterator to traverse through the entries of a (dense) matrix.
* @tparam T Internal data type.
* @details This iterator can not be created outside of Matrix.
* @sa Matrix
* @sa MatrixRowIterator
* @sa MatrixColumnIterator
*/
template<typename T, class DC>
class MatrixIterator
{
protected:
	friend BaseMatrix<T,DC>;
	MatrixIterator(const BaseMatrix<T,DC>& m, Index i1, Index i2);

public:
	/**
	* @brief Returns the current row.
	* @details If row is equal to the row count of the matrix,
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
	* @details If column is equal to the column count of the matrix,
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
	* @param other The other iterator. Has to be from the same matrix.
	* @return Returns true if the iterators match, false otherwise.
	* @sa operator!=()
	*/
	bool operator ==(const MatrixIterator& other) const
	{
		return (!isValid() && !other.isValid()) || 
			(mMatrix == other.mMatrix && mIndex1 == other.mIndex1 && mIndex2 == other.mIndex2);
	}

	/**
	* @brief Checks if both iterators are not equal.
	* @param other The other iterator.
	* @return Returns false if the iterators match, true otherwise.
	* @sa operator==()
	*/
	bool operator !=(const MatrixIterator& other) const
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
	MatrixIterator& operator++ ();

	/**
	* @brief Moves the iterator forward.
	* @details The moving direction is from left to right, top to down.\n
	* First all the columns from the current row are traversed and then the next row.
	* @return Copy of `this` iterator before moving forward.
	*/
	MatrixIterator operator++ (int);

protected:
	const BaseMatrix<T,DC>* mMatrix;
	Index mIndex1;
	Index mIndex2;
};

/**
* @brief An Iterator to traverse through one row of a (dense) matrix.
* @tparam T Internal data type.
* @details This iterator can not be created outside of Matrix.
* @sa Matrix
* @sa MatrixIterator
* @sa MatrixColumnIterator
*/
template<typename T, class DC>
class MatrixRowIterator : public MatrixIterator<T,DC>
{
private:
	friend BaseMatrix<T,DC>;
	MatrixRowIterator(const BaseMatrix<T,DC>& m, Index i1, Index i2);

public:
	/**
	* @brief Moves the iterator forward.
	* @details The moving direction is from left to right.\n
	* Only the current row will be traversed.
	* @return Reference to `this` iterator.
	*/
	MatrixRowIterator& operator++ ();

	/**
	* @brief Moves the iterator forward.
	* @details The moving direction is from left to right.\n
	* Only the current row will be traversed.
	* @return Copy of `this` iterator before moving forward.
	*/
	MatrixRowIterator operator++ (int);
};

/**
* @brief An Iterator to traverse through one column of a (dense) matrix.
* @tparam T Internal data type.
* @details This iterator can not be created outside of Matrix.
* @sa Matrix
* @sa MatrixIterator
* @sa MatrixRowIterator
*/
template<typename T, class DC>
class MatrixColumnIterator : public MatrixIterator<T,DC>
{
private:
	friend BaseMatrix<T,DC>;
	MatrixColumnIterator(const BaseMatrix<T,DC>& m, Index i1, Index i2);

public:
	/**
	* @brief Moves the iterator forward.
	* @details The moving direction is from top to down.\n
	* Only the current column will be traversed.
	* @return Reference to `this` iterator.
	*/
	MatrixColumnIterator& operator++ ();

	/**
	* @brief Moves the iterator forward.
	* @details The moving direction is from top to down.\n
	* Only the current column will be traversed.
	* @return Copy of `this` iterator before moving forward.
	*/
	MatrixColumnIterator operator++ (int);
};

/**
* @brief A (dense) matrix class.
* @details A (dense) matrix is only recommended for little sized dimensions.\n
* Use the SparseMatrix class if high dimensions with sparse entries are used.\n
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
* @sa SparseMatrix
*/
template<typename T, class DC>
class BaseMatrix
{
	friend MatrixIterator<T,DC>;
	friend MatrixRowIterator<T,DC>;
	friend MatrixColumnIterator<T,DC>;

protected:
	DC mValues;
	T mEmpty;//Used for references; Could also be static, but that would break the header only approach

public:
	/**
	* @brief The standard iterator.
	* @sa MatrixIterator
	*/
	typedef MatrixIterator<T,DC> iterator;

	/**
	* @brief A const variant of the standard iterator.
	* @sa MatrixIterator
	*/
	typedef const iterator const_iterator;

	/**
	* @brief The row iterator.
	* @sa MatrixRowIterator
	*/
	typedef MatrixRowIterator<T,DC> row_iterator;

	/**
	* @brief A const variant of the row iterator.
	* @sa MatrixRowIterator
	*/
	typedef const row_iterator const_row_iterator;

	/**
	* @brief The column iterator.
	* @sa MatrixIterator
	*/
	typedef MatrixColumnIterator<T,DC> column_iterator;

	/**
	* @brief A const variant of the column iterator.
	* @sa MatrixIterator
	*/
	typedef const column_iterator const_column_iterator;
	
	/**
	* @brief A typedef of the underlying value type.
	*/
	typedef T value_type;

	/**
	* @brief Constructs an empty (dense) matrix
	*/
	BaseMatrix();

	virtual ~BaseMatrix();

	/**
	* @brief Returns an iterator pointing at the first topological entry.
	* @par Complexity
	* Always: \f$ O(1) \f$
	* @return Const matrix iterator.
	*/
	const_iterator begin() const;

	/**
	* @brief Returns an iterator representing the end.
	* @note This iterator can not be dereferenced.
	* @par Complexity
	* Always: \f$ O(1) \f$
	* @return Const matrix iterator.
	*/
	const_iterator end() const;

	/**
	* @brief Returns an iterator pointing at the first topological entry.
	* @par Complexity
	* Always: \f$ O(1) \f$
	* @return Matrix iterator.
	*/
	iterator begin();

	/**
	* @brief Returns an iterator representing the end.
	* @note This iterator can not be dereferenced.
	* @par Complexity
	* Always: \f$ O(1) \f$
	* @return Matrix iterator.
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
	* @return Const matrix row iterator.
	*/
	const_row_iterator row_begin(Index i) const;

	/**
	* @brief Returns a row iterator representing the end of the row.
	* @note This iterator can not be dereferenced.
	* @par Complexity
	* Always: \f$ O(1) \f$
	* @param i Index of the row.
	* @return Const matrix row iterator.
	*/
	const_row_iterator row_end(Index i) const;

	/**
	* @brief Returns a row iterator pointing at the first topological entry at the given row.
	* @par Complexity
	* Always: \f$ O(1) \f$
	* @param i Index of the row.
	* @return Matrix row iterator.
	*/
	row_iterator row_begin(Index i);

	/**
	* @brief Returns a row iterator representing the end.
	* @note This iterator can not be dereferenced.
	* @par Complexity
	* Always: \f$ O(1) \f$
	* @param i Index of the row.
	* @return Matrix row iterator.
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
	* Always: \f$ O(1) \f$
	* @param j Index of the column.
	* @return Const matrix column iterator.
	*/
	const_column_iterator column_begin(Index j) const;

	/**
	* @brief Returns a column iterator representing the end of the column.
	* @note This iterator can not be dereferenced.
	* @par Complexity
	* Always: \f$ O(1) \f$
	* @param j Index of the column.
	* @return Const matrix column iterator.
	*/
	const_column_iterator column_end(Index j) const;

	/**
	* @brief Returns a column iterator pointing at the first topological entry at the given column.
	* @par Complexity
	* Always: \f$ O(1) \f$
	* @param j Index of the column.
	* @return Matrix column iterator.
	*/
	column_iterator column_begin(Index j);

	/**
	* @brief Returns a column iterator representing the end.
	* @note This iterator can not be dereferenced.
	* @par Complexity
	* Always: \f$ O(1) \f$
	* @param j Index of the column.
	* @return Matrix column iterator.
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
	* Always: \f$ O(1) \f$
	* @param i Index of the row.
	* @param j Index of the column.
	* @return Copy of the value at \f$ A_{ij} \f$
	* @sa linear_at
	*/
	T at(Index i, Index j) const;

	/**
	* @brief Sets the value at the respective location.
	* @par Complexity
	* Always: \f$ O(1) \f$
	* @param i Index of the row.
	* @param j Index of the column.
	* @param val New value replacing the old one.
	* @sa linear_set
	*/
	void set(Index i, Index j, const T& val);

	/**
	* @brief Returns a copy of the value at the respective location based on linear coordinates.
	* @details The transformation is based on \f$ lin = i*D_2 + j \f$.
	* @par Complexity
	* Always: \f$ O(1) \f$
	* @param i Linear index.
	* @return Copy of the value.
	* @sa at
	*/
	T linear_at(Index i) const;

	/**
	* @brief Sets the value at the respective location.
	* @details The transformation is based on \f$ lin = i*D_2 + j \f$.
	* @par Complexity
	* Always: \f$ O(1) \f$
	* @param i Linear index.
	* @param val New value replacing the old one.
	* @sa set
	*/
	void linear_set(Index i, const T& val);

	/**
	* @brief Returns true if the entry at the respective location has a value different then 0.
	* @details Same as `at(i,j) != 0`
	* @par Complexity
	* Always: \f$ O(1) \f$
	* @param i Index of the row.
	* @param j Index of the column.
	* @sa at
	* @sa has(Index,Index,T&) const
	*/
	bool has(Index i, Index j) const;

	/**
	* @brief Returns true if the entry at the respective location has a value different then 0.
	* @details Same as `at(i,j) != 0`
	* @par Complexity
	* Always: \f$ O(1) \f$
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
	* Always: \f$ O(1) \f$
	* @param it Standard iterator.
	* @param val The new value.
	* @return The it iterator.
	* @sa erase(const iterator&);
	*/
	iterator set(const iterator& it, const T& val) ;

	/**
	* @brief Sets the entry the iterator is pointing at to 0.
	* @par Complexity
	* Always: \f$ O(1) \f$
	* @param it Standard iterator.
	* @return The it iterator.
	* @sa set(const iterator&, const T&);
	*/
	iterator erase(const iterator& it);

	/**
	* @brief The column count
	* @par Complexity
	* Always: \f$ O(1) \f$
	* @return D2
	*/
	Dimension columns() const;

	/**
	* @brief The row count
	* @par Complexity
	* Always: \f$ O(1) \f$
	* @return D1
	*/
	Dimension rows() const;

	/**
	* @brief The full size of the matrix.
	* @par Complexity
	* Always: \f$ O(1) \f$
	* @return D1*D2
	*/
	Dimension size() const;

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
	* @brief Returns true if matrix has NaN entries
	* @par Complexity
	* Worst case: \f$ O(D1*D2) \f$
	* @return True if matrix has a NaN entry, false otherwise.
	*/
	bool has_nan() const;

	/**
	* @brief Returns true if matrix has infinite entries
	* @par Complexity
	* Worst case: \f$ O(D1*D2) \f$
	* @return True if matrix has a infinite entry, false otherwise.
	*/
	bool has_inf() const;

	/**
	* @brief Returns true if matrix has an entry with 0.
	* @par Complexity
	* Worst case: \f$ O(D1*D2) \f$
	* @return True if matrix has a 0 entry, false otherwise.
	* @sa empty()
	*/
	bool has_zero() const;

	/**
	* @brief Returns true if matrix has only zeros.
	* @par Complexity
	* Always: \f$ O(1) \f$
	* @return True if the matrix is empty, false otherwise.
	* @sa has_zero()
	*/
	bool empty() const;

	/**
	* @brief Returns the trace of the matrix.
	* @details The trace of the matrix is defined as:\n
	* \f[
	* trace := \sum_{i}^{\min(D1,D2)} A_{ii}
	* \f]
	* @par Complexity
	* Worst case: \f$ O(min(D1,D2)) \f$
	* @return The trace of the matrix
	*/
	T trace() const;
};

// Comparison
template<typename T, class DC>
bool operator ==(const BaseMatrix<T,DC>& v1, const BaseMatrix<T,DC>& v2);
template<typename T, class DC>
bool operator !=(const BaseMatrix<T,DC>& v1, const BaseMatrix<T,DC>& v2);

NS_END_NAMESPACE

#define _NS_BASEMATRIX_INL
# include "BaseMatrix.inl"
#undef _NS_BASEMATRIX_INL