#pragma once

#include "Types.h"

#include <ostream>

NS_BEGIN_NAMESPACE

template<typename T>
std::ostream& operator<<(std::ostream& out, const NS::ComplexNumber<T>& f)
{
	out << "[ " << f.real() << " " << f.imag() << "i ]";
	return out;
}

template<typename T, NS::Dimension D>
std::ostream& operator<<(std::ostream& out, const NS::Vector<T, D>& f)
{
	out << "[ ";
	for (Index i = 0; i < D; ++i)
		out << f.at(i) << " ";
	out << "]";
	return out;
}

template<typename T, NS::Dimension D1, NS::Dimension D2>
std::ostream& operator<<(std::ostream& out, const NS::Matrix<T, D1, D2>& f)
{
	out << "[ ";
	for (Index i = 0; i < D1; ++i)
	{
		for (Index j = 0; j < D2; ++j)
			out << f.at(i, j) << " ";

		if (i != D1 - 1)
			out << "| ";
	}
	out << "]";
	return out;
}

template<typename T, NS::Dimension D1, NS::Dimension D2>
std::ostream& operator<<(std::ostream& out, const NS::SparseMatrix<T, D1, D2>& f)
{
	out << "[ ";
	for (Index i = 0; i < D2; ++i)
	{
		for (Index j = 0; j < D1; ++j)
			out << f.at(j, i) << " ";

		if (i != D2 - 1)
			out << "| ";
	}
	out << "]";
	return out;
}

NS_END_NAMESPACE