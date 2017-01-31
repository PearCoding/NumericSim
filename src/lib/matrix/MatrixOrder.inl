#ifndef _NS_MATRIXORDER_INL
# error MatrixOrder.inl should only be included by MatrixOrder.h
#endif

NS_BEGIN_NAMESPACE

template<class K, class V>
typename std::multimap<K, V>::const_iterator find_pair(const std::multimap<K, V>& map,
	const std::pair<K, V>& pair)
{
    const auto range = map.equal_range(pair.first);
    for (auto it = range.first; it != range.second; ++it)
        if (it->second == pair.second)
            return it;
    return map.end();
}

template<typename T>
DynamicVector<Index> cuthill_mckee(const SparseMatrix<T>& m)
{
	if(m.rows() != m.columns())
		throw NotSquareException();
	
	DynamicVector<Index> R(m.rows());
	Index currentIndex = 0;

	std::queue<Index> Q;
	std::multimap<Dimension, Index> Rows;
	for(Index i = 0; i < m.rows(); ++i)
	{
		Index mc = m.row_filled_count(i);
		if(mc > 0)
			Rows.insert(std::pair<Dimension, Index>(mc, i));
	}
	
	while(!Rows.empty())
	{
		// Find first p with lowest degree
		Index p = Rows.begin()->second;
		R[currentIndex] = p;
		Rows.erase(Rows.begin());
		++currentIndex;

		// Add all adjacent nodes to the queue
		std::multimap<Dimension, Index> adj;
		for(auto it = m.row_cbegin(p); it != m.row_cend(p); ++it)
		{
			if(it.column() != it.row())
			{
				Index mc = m.row_filled_count(it.column());
				if(mc > 0)
					adj.insert(std::pair<Dimension, Index>(mc, it.column()));
			}
		}

		for(std::pair<Dimension, Index> pair : adj)
			Q.push(pair.second);

		// Examine first entry of the queue
		while(!Q.empty())
		{
			Index c = Q.front();
			Q.pop();
			Index mc = m.row_filled_count(c);
			auto rit = find_pair(Rows, std::pair<Dimension, Index>(mc, c));
			if(rit != Rows.end())
			{
				R[currentIndex] = c;
				Rows.erase(rit);
				++currentIndex;

				adj.clear();
				for(auto it = m.row_cbegin(c); it != m.row_cend(c); ++it)
				{
					if(it.column() != it.row())
					{
						Index mc = m.row_filled_count(it.column());
						if(mc > 0)
							adj.insert(std::pair<Dimension, Index>(mc, it.column()));
					}
				}

				for(std::pair<Dimension, Index> pair : adj)
					Q.push(pair.second);
			}
		}
	}

	return R;
}

template<typename T>
DynamicVector<Index> r_cuthill_mckee(const SparseMatrix<T>& m)
{
	auto v = cuthill_mckee(m);

	// Reverse
	for(Index i = 0; i < v.size()/2; ++i)
	{
		auto tmp = v.at(i);
		v.set(i, v.at(v.size()-i-1));
		v.set(v.size()-i-1, tmp);
	}

	return v;
}

template<typename T>
SparseMatrix<T> permutation_matrix(const DynamicVector<Index>& permutation)
{
	SparseMatrix<T> nm(permutation.size(), permutation.size());
	for(Index i = 0; i < permutation.size(); ++i)
		nm.set(i, permutation[i], (T)1);
	return nm;
}

DynamicVector<Index> inverse_permutation(const DynamicVector<Index>& permutation)
{
	DynamicVector<Index> np(permutation.size());
	for(Index i = 0; i < permutation.size(); ++i)
		np.set(permutation[i], i);
	
	return np;
}

template<typename T>
SparseMatrix<T> permutate(const SparseMatrix<T>& m, const DynamicVector<Index>& permutation)
{
	if(m.rows() != m.columns())
		throw NotSquareException();

	if(m.rows() != permutation.size())
		throw MatrixMulMismatchException();

	const auto P = permutation_matrix<T>(permutation);
	return P.mul(m.mul(P.transpose()));
}

template<typename T>
DynamicVector<T> permutate(const DynamicVector<T>& m, const DynamicVector<Index>& permutation)
{
	if(m.size() != permutation.size())
		throw VectorSizeMismatchException();

	DynamicVector<T> nm(m.size());
	for(Index i = 0; i < m.size(); ++i)
		nm.set(permutation[i], m[i]);

	return nm;
}

NS_END_NAMESPACE