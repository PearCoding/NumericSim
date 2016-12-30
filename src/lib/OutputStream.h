#pragma once

#include "Types.h"
#include "Vector.h"
#include "matrix/Matrix.h"

#include <ostream>

NS_BEGIN_NAMESPACE

template<typename T, class DC>
std::ostream& operator<<(std::ostream& out, const NS::Vector<T,DC>& f)
{
	out << "[ ";
	for (auto v : f)
		out << v << " ";
	out << "]";
	return out;
}

template<typename T, class DC>
std::ostream& operator<<(std::ostream& out, const NS::BaseMatrix<T, DC>& f)
{
	out << "[ ";
	for (Index i = 0; i < f.rows(); ++i)
	{
		for (Index j = 0; j < f.columns(); ++j)
			out << f.at(i, j) << " ";

		if (i != f.rows() - 1)
			out << "| ";
	}
	out << "]";
	return out;
}

template<typename T>
std::ostream& operator<<(std::ostream& out, const NS::SparseMatrix<T>& f)
{
	out << "[ ";
	for (auto it = f.begin(); it != f.end(); ++it)
	{
		out << "(" << it.row() << ", " << it.column() << "; " << *it << ") ";
	}
	out << "]";
	return out;
}

NS_END_NAMESPACE