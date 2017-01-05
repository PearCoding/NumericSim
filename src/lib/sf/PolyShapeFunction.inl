#ifndef _NS_POLYSHAPEFUNCTION_INL
# error PolyShapeFunction.inl should only be included by PolyShapeFunction.h
#endif

NS_BEGIN_NAMESPACE

template<typename T, Dimension K, Dimension Order>
T PolyShapePolicy<T,K,Order>::value(Index localComponent, const FixedVector<T,K>& v) const
{
    static_assert(K==2, "Currently only two dimensional values implemented.");
    static_assert(Order==1, "Currently only first order is implemented.");

    // Not a complete solution!
    switch(localComponent)
    {
    default:
    case 0:
        return 1-v[0]-v[1];
    case 1:
        return v[0];
    case 2:
        return v[1];
    }
}

template<typename T, Dimension K, Dimension Order>
FixedVector<T,K> PolyShapePolicy<T,K,Order>::gradient(Index localComponent, const FixedVector<T,K>& local) const
{
    static_assert(K==2, "Currently only two dimensional values implemented.");
    static_assert(Order==1, "Currently only first order is implemented.");

    if(localComponent == 0)
    {
        FixedVector<T,K> pv;
        pv.fill(-1);
        return pv;
    }
    else
    {
        FixedVector<T,K> pv;
        pv.fill(0);
        pv[localComponent-1] = 1;
        return pv;
    }
}

NS_END_NAMESPACE