#ifndef _NS_POLYSHAPEFUNCTION_INL
# error PolyShapeFunction.inl should only be included by PolyShapeFunction.h
#endif

NS_BEGIN_NAMESPACE
namespace SF {

	template<typename T, Dimension K, Dimension Order>
    template<class V>
	V PolyShapePolicy<T,K,Order>::value(const V& v) const
    {
        static_assert(K==2, "Currently only two dimensional values implemented.");

        // Not a complete solution!
        V ret = v;
        V pv = v;
        for(Index p = 1; p < Order; ++p)
        {
            pv *= v;
            ret += pv;
        }

        return ret;
    }

	template<typename T, Dimension K, Dimension Order>
    template<class V>
	V PolyShapePolicy<T,K,Order>::gradient(Index localComponent, const V& v) const
    {
        static_assert(K==2, "Currently only two dimensional values implemented.");
        static_assert(Order==1, "Currently only first order is implemented.");

        if(localComponent == 0)
        {
            V pv = v;
            pv.fill(-1);
            return pv;
        }
        else
        {
            V pv = v;
            pv.fill(0);
            pv[localComponent-1] = 1;
            return pv;
        }
    }
}
NS_END_NAMESPACE