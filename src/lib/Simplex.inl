#ifndef _NS_SIMPLEX_INL
# error Simplex.inl should only be included by Simplex.h
#endif

NS_BEGIN_NAMESPACE

template<typename T, Dimension K>
Simplex<T,K>::Simplex() :
	mVertices(), mPrepared(false), mDeterminant(0)
{
	static_assert(K > 0, "Simplex of k-order should be bigger than 0");
}

template<typename T, Dimension K>
Simplex<T,K>::Simplex(std::initializer_list<std::initializer_list<T> > list) :
	Simplex()
{
	NS_ASSERT(list.size() == (K+1));//, "Given list is not of the same size as the Simplex");
	
	Index i = 0;
	for (const auto& v : list)
	{
		mVertices[i] = vertex_t(v);
		i++;
	}	
}

template<typename T, Dimension K>
void Simplex<T,K>::prepare()
{	
	for(Index i = 1; i < K+1; ++i)
	{
		const auto dist = mVertices[i]-mVertices[0];

		for(Index j = 0; j < K; ++j)
			mMatrix.set(j,i-1, dist[j]);
	}

	mDeterminant = Operations::determinant(mMatrix);

	mPrepared = true;
}

template<typename T, Dimension K>
const typename Simplex<T,K>::vertex_t& Simplex<T,K>::operator[](Index i) const
{
	return mVertices[i];
}

template<typename T, Dimension K>
typename Simplex<T,K>::vertex_t& Simplex<T,K>::operator[](Index i)
{
	return mVertices[i];
}

template<typename T, Dimension K>
T Simplex<T,K>::unitVolume()
{
	return 1.0/Math::factorial(K);
}

template<typename T, Dimension K>
T Simplex<T,K>::volume() const
{
	NS_ASSERT(mPrepared);
	return std::abs((1.0/Math::factorial(K))*mDeterminant);
}

// template<typename T, Dimension K>
// typename Simplex<T,K>::vertex_t Simplex<T,K>::toLocal(const vertex_t& global) const
// {
// 	NS_ASSERT(mPrepared);
// }

template<typename T, Dimension K>
typename Simplex<T,K>::matrix_t Simplex<T,K>::gradient() const
{
	NS_ASSERT(mPrepared);

	// We have to solve a linear equation to find the transformation vectors.
	// But it is easy to find for the case of 2d:

	static_assert(K==2, "Gradient currently only implemented for 2D!");

	const vertex_t d1 = mVertices[1] - mVertices[0];
	const vertex_t d2 = mVertices[2] - mVertices[0];

	matrix_t m;
	m.set(0,0, d2[1]);
	m.set(0,1, -d2[0]);
	m.set(1,0, -d1[1]);
	m.set(1,1, d1[0]);
	
	return m * (1/mDeterminant);
}

template<typename T, Dimension K>
typename Simplex<T,K>::vertex_t Simplex<T,K>::toGlobal(const vertex_t& local) const
{
	NS_ASSERT(mPrepared);
	return mMatrix.mul(local) + mVertices[0];
}

template<typename T, Dimension K>
const typename Simplex<T,K>::matrix_t& Simplex<T,K>::matrix() const
{
	NS_ASSERT(mPrepared);
	return mMatrix;
}

// template<typename T, Dimension K>
// const typename Simplex<T,K>::matrix_t& Simplex<T,K>::inverseMatrix() const
// {
// 	NS_ASSERT(mPrepared);
// 	return mInverseMatrix;
// }

template<typename T, Dimension K>
T Simplex<T,K>::determinant() const
{
	NS_ASSERT(mPrepared);
	return mDeterminant;
}

NS_END_NAMESPACE