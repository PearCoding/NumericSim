#ifndef _NS_SIMPLEX_INL
# error Simplex.inl should only be included by Simplex.h
#endif

NS_BEGIN_NAMESPACE

template<typename T, Dimension K>
Simplex<T,K>::Simplex() :
	mVertices(), mPrepared(false), mDeterminant(0)
{
	static_assert(K > 0, "Simplex of k-order should be bigger than 0");
	static_assert(K==2, "Simplex currently only implemented for 2D!");
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

	mInverseMatrix.set(0,0, mMatrix.at(1,1));
	mInverseMatrix.set(0,1, -mMatrix.at(0,1));
	mInverseMatrix.set(1,1, mMatrix.at(0,0));
	mInverseMatrix.set(1,0, -mMatrix.at(1,0));
	mInverseMatrix *= (T)1/mDeterminant;

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
constexpr T Simplex<T,K>::unitVolume()
{
	return 1.0/Math::factorial(K);
}

template<typename T, Dimension K>
T Simplex<T,K>::volume() const
{
	NS_ASSERT(mPrepared);
	return std::abs((1.0/Math::factorial(K))*mDeterminant);
}

template<typename T, Dimension K>
typename Simplex<T,K>::vertex_t Simplex<T,K>::center() const
{
	static_assert(K==2, "Center currently only implemented for 2D!");
	vertex_t a = mVertices[1]-mVertices[0];
	vertex_t b = mVertices[2]-mVertices[0];
	
	vertex_t v;
	v[0] = b[1]*a.magSqr() - a[1]*b.magSqr();
	v[1] = a[0]*b.magSqr() - b[0]*a.magSqr();
	return v / (T)2 * mDeterminant;
}

template<typename T, Dimension K>
T Simplex<T,K>::outerRadius() const
{
	return (center()-mVertices[0]).mag();
}

template<typename T, Dimension K>
typename Simplex<T,K>::vertex_t Simplex<T,K>::faceCenter(Index i) const
{
	vertex_t v;
	for(Index j = 0; j < K+1; ++j)
	{
		if(j != i)
			v += mVertices[j];
	}
	
	return v / (T)(K);
}

template<typename T, Dimension K>
typename Simplex<T,K>::vertex_t Simplex<T,K>::faceNormal(Index i) const
{
	return (center() - mVertices[i]).normalized();
}

template<typename T, Dimension K>
typename Simplex<T,K>::vertex_t Simplex<T,K>::gradient(Index component) const
{
	NS_ASSERT(mPrepared);
	NS_ASSERT(component < K+1);

	// We have to solve a linear equation to find the transformation vectors.
	// But it is easy to find for the case of 2d:

	static_assert(K==2, "Gradient currently only implemented for 2D!");

	vertex_t d;
	switch(component)
	{
	case 0:
		d[0] = mVertices[1][1] - mVertices[2][1];
		d[1] = mVertices[2][0] - mVertices[1][0];
		break;
	case 1:
		d[0] = mVertices[2][1] - mVertices[0][1];
		d[1] = mVertices[0][0] - mVertices[2][0];
		break;
	case 2:
		d[0] = mVertices[0][1] - mVertices[1][1];
		d[1] = mVertices[1][0] - mVertices[0][0];
		break;
	}
	
	return d * (1/mDeterminant);
}

template<typename T, Dimension K>
typename Simplex<T,K>::vertex_t Simplex<T,K>::toLocal(const vertex_t& global) const
{
	NS_ASSERT(mPrepared);
	return mInverseMatrix.mul(global-mVertices[0]);
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

template<typename T, Dimension K>
const typename Simplex<T,K>::matrix_t& Simplex<T,K>::inverseMatrix() const
{
	NS_ASSERT(mPrepared);
	return mInverseMatrix;
}

template<typename T, Dimension K>
T Simplex<T,K>::determinant() const
{
	NS_ASSERT(mPrepared);
	return mDeterminant;
}

NS_END_NAMESPACE