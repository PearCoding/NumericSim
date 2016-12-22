#ifndef _NS_SIMPLEX_INL
# error Simplex.inl should only be included by Simplex.h
#endif

NS_BEGIN_NAMESPACE

template<typename T, Dimension K, Dimension N>
Simplex<T,K,N>::Simplex() :
	Vertices()
{
	static_assert(K <= N, "Simplex of k-order is bigger than parent dimension");
}

template<typename T, Dimension K, Dimension N>
Simplex<T,K,N>::Simplex(std::initializer_list<std::initializer_list<T> > list) :
	Simplex()
{
	NS_ASSERT(list.size() == (K+1));//, "Given list is not of the same size as the Simplex");
	
	Index i = 0;
	for (const auto& v : list)
	{
		Vertices[i] = FixedVector<T,N>(v);
		i++;
	}	
}

template<typename T, Dimension K, Dimension N>
T Simplex<T,K,N>::volume() const
{
	FixedVector<T,N> subs[K];
	Matrix<T> s(K,K);

	for(Index i = 0; i < K; ++i)
		subs[i] = Vertices[i+1]-Vertices[i];
	
	for(Index i = 0; i < K; ++i)
	{
		for(Index j = 0; j < K; ++j)
			s.set(i,j, subs[i].dot(subs[j]));
	}

	return (1.0/Math::factorial(K))*std::sqrt(Operations::determinant(s));
}

NS_END_NAMESPACE