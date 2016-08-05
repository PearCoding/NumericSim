#pragma once

#include "Config.h"

#include <initializer_list>
#include <type_traits>
#include <memory>

NS_BEGIN_NAMESPACE

template<typename T, Dimension D>
class Vector
{
public:
	class VectorIterator
	{
	public:
		VectorIterator(const Vector<T, D>& vec, Index pos) :
			mPos(pos), mVec(vec)
		{}

		bool operator ==(const VectorIterator& other) const
		{
			return mPos == other.mPos;
		}

		bool operator !=(const VectorIterator& other) const
		{
			return !(*this == other);
		}

		T operator *() const
		{
			NS_ASSERT(mPos < D);
			return mVec.mData->ptr[mPos];
		}

		const VectorIterator& operator++ ()
		{
			++mPos;
			return *this;
		}

	private:
		Index mPos;
		const Vector<T, D>& mVec;
	};

	typedef VectorIterator iterator;

	Vector();
	explicit Vector(const T& f);
	Vector(std::initializer_list<T> l);
	Vector(const Vector& v);
	Vector(Vector&& v);

	virtual ~Vector();

	// Assignment
	Vector& operator=(const Vector& v);
	Vector& operator=(Vector&& v);

	// Index
	const T& operator()(Index i) const;
	const T& operator[](Index i) const;
	T& operator[](Index i);

	iterator begin() const
	{
		return VectorIterator(*this, 0);
	}

	iterator end() const
	{
		return VectorIterator(*this, D);
	}

	// Element wise operations
	Vector& operator +=(const Vector& v2);
	Vector& operator +=(const T& f);
	Vector& operator -=(const Vector& v2);
	Vector& operator -=(const T& f);
	Vector& operator *=(const Vector& v2);
	Vector& operator *=(const T& f);
	Vector& operator /=(const Vector& v2);
	Vector& operator /=(const T& f);

	// Other
	constexpr Dimension dim() const;
	T sum() const;
	T max() const;
	T min() const;
	T avg() const;
	T dot(const Vector& v) const;
	T mag() const;
	T magSqr() const;

	void inv();

	void fill(const T& f);
	void swap(Vector& v);

	bool hasNaN() const;
	bool hasInf() const;
	bool hasZero() const;

	// Boolean Set
	template<Index I>
	Vector<T, I + 1> left() const;
	template<Dimension MaxD = D>
	Vector<T, MaxD> left(Index i) const;

	template<Index I>
	Vector<T, D - I> right() const;
	template<Dimension MaxD = D>
	Vector<T, MaxD> right(Index i) const;

	template<Index Start, Index End>
	Vector<T, End - Start> mid() const;
	template<Dimension MaxD = D>
	Vector<T, MaxD> mid(Index start, Index end) const;

private:
	void make_unique();

	struct _Data
	{
	public:
		_Data() :
			ptr(new T[D])
		{
		}

		~_Data()
		{
			delete[] ptr;
		}

		T* ptr;
	};

	std::shared_ptr<_Data> mData;
};

// Element wise operations
template<typename T, Dimension D>
Vector<T, D> operator +(const Vector<T, D>& v1, const Vector<T, D>& v2);
template<typename T, Dimension D>
Vector<T, D> operator +(const Vector<T, D>& v1, T f);
template<typename T, Dimension D>
Vector<T, D> operator +(T f, const Vector<T, D>& v1);
template<typename T, Dimension D>
Vector<T, D> operator -(const Vector<T, D>& v1, const Vector<T, D>& v2);
template<typename T, Dimension D>
Vector<T, D> operator -(const Vector<T, D>& v1, T f);
template<typename T, Dimension D>
Vector<T, D> operator -(T f, const Vector<T, D>& v1);
template<typename T, Dimension D>
Vector<T, D> operator -(const Vector<T, D>& v);
template<typename T, Dimension D>
Vector<T, D> operator *(const Vector<T, D>& v1, const Vector<T, D>& v2);
template<typename T, Dimension D>
Vector<T, D> operator *(const Vector<T, D>& v1, T f);
template<typename T, Dimension D>
Vector<T, D> operator *(T f, const Vector<T, D>& v1);
template<typename T, Dimension D>
Vector<T, D> operator /(const Vector<T, D>& v1, const Vector<T, D>& v2);
template<typename T, Dimension D>
Vector<T, D> operator /(const Vector<T, D>& v1, T f);
template<typename T, Dimension D>
Vector<T, D> operator /(T f, const Vector<T, D>& v1);

// Comparison
template<typename T, Dimension D>
bool operator ==(const Vector<T, D>& v1, const Vector<T, D>& v2);
template<typename T, Dimension D>
bool operator !=(const Vector<T, D>& v1, const Vector<T, D>& v2);

NS_END_NAMESPACE

#define _NS_VECTOR_INL
# include "Vector.inl"
#undef _NS_VECTOR_INL