#ifndef _NS_POLYSHAPEFUNCTION_INL
# error PolyShapeFunction.inl should only be included by PolyShapeFunction.h
#endif

NS_BEGIN_NAMESPACE

/* Precalculated coefficients for polynomial shapefunction.
 * First Index of the Matrix is the component, the second the polynomial coefficent.
 */

// ----------- 1-Simplex (Line) [Univariate Polynomial] -----------
// First Order: x0 ---- x1
constexpr float PSF_Coeff_1D_1[2][2] = {{1, -1}, {0,1}};

// Second Order: x0 --x2-- x1
constexpr float PSF_Coeff_1D_2[3][3] = {{1, -3, 2}, {0, 4, -4}, {0, -1, 2}};

// ----------- 2-Simplex (Triangle) [Multivariate Polynomial] -----------
// First Order:
// x2
// |  \
// |   \
// |    \
// x0 -- x1
constexpr float PSF_Coeff_2D_1[3][3] = {{1, -1, -1}, {0, 1, 0}, {0, 0, 1}};

// Second Order:
// x2
// |  \
// x5  x4
// |    \
// x0 x3 x1
constexpr float PSF_Coeff_2D_2[6][6] = {
    {1, -3, -3,  4,  2,  2},
    {0, -1,  0,  0,  2,  0},
    {0,  0, -1,  0,  0,  2},
    {0,  4,  0, -4, -4,  0},
    {0,  0,  0,  4,  0,  0},
    {0,  0,  4, -4,  0, -4}};

// Third Order:
//  x2
// |   \
// x7   x6
// |  x9 \
// x8     x5
// |       \
// x0 x3 x4 x1
constexpr float PSF_Coeff_2D_3[10][10] = {
    {1, -5.5, -5.5,    18,     9,     9, -13.5, -13.5,  -4.5,  -4.5},
    {0,    1,    0,     0,  -4.5,     0,     0,     0,   4.5,     0},
    {0,    0,    1,     0,     0,  -4.5,     0,     0,     0,   4.5},
    {0,    9,    0, -22.5, -22.5,     0,    27,  13.5,  13.5,     0},
    {0, -4.5,    0,   4.5,    18,     0, -13.5,     0, -13.5,     0},
    {0,    0,    0,  -4.5,     0,     0,  13.5,     0,     0,     0},
    {0,    0,    0,  -4.5,     0,     0,     0,  13.5,     0,     0},
    {0,    0, -4.5,   4.5,     0,    18,     0, -13.5,     0, -13.5},
    {0,    0,    9, -22.5,     0, -22.5,  13.5,    27,     0,  13.5},
    {0,    0,    0,    27,     0,     0,   -27,   -27,     0,     0}};


// ----------- 3-Simplex (Tetrahedron) [Multivariate Polynomial] -----------
// First Order:
constexpr float PSF_Coeff_3D_1[4][4] = {
    {1, -1, -1, -1},
    {0,  1,  0,  0},
    {0,  0,  1,  0},
    {0,  0,  0,  1}};

// Second Order:
constexpr float PSF_Coeff_3D_2[10][10] = {
    {1, -3, -3, -3,  4,  4,  4,  2,  2,  2},
    {0, -1,  0,  0,  0,  0,  0,  2,  0,  0},
    {0,  0, -1,  0,  0,  0,  0,  0,  2,  0},
    {0,  0,  0, -1,  0,  0,  0,  0,  0,  2},
    {0,  4,  0,  0, -4, -4,  0, -4,  0,  0},
    {0,  0,  0,  0,  4,  0,  0,  0,  0,  0},
    {0,  0,  4,  0, -4,  0, -4,  0, -4,  0},
    {0,  0,  0,  0,  0,  4,  0,  0,  0,  0},
    {0,  0,  0,  4,  0, -4, -4,  0,  0, -4},
    {0,  0,  0,  0,  0,  0,  4,  0,  0, 0}};

// TODO: Find a general algorithm, or continue list (3D, etc..)
//   This is a whole mess...
template<typename T, Dimension K, Dimension Order>
T PolyShapePolicy<T,K,Order>::value(Index localComponent, const FixedVector<T,K>& v) const
{
    if(K == 1)
    {
        if(Order == 1)
        {
            NS_ASSERT(localComponent < 2);
            return PSF_Coeff_1D_1[localComponent][0] + PSF_Coeff_1D_1[localComponent][1]*v[0];
        }
        else if(Order == 2)
        {
            NS_ASSERT(localComponent < 3);
            return PSF_Coeff_1D_2[localComponent][0] + PSF_Coeff_1D_2[localComponent][1]*v[0] 
                + PSF_Coeff_1D_2[localComponent][2]*v[0]*v[0];
        }
    }
    else if(K == 2)
    {
        if(Order == 1)
        {
            NS_ASSERT(localComponent < 3);
            return PSF_Coeff_2D_1[localComponent][0] 
                + PSF_Coeff_2D_1[localComponent][1]*v[0]
                + PSF_Coeff_2D_1[localComponent][2]*v[1];
        }
        else if(Order == 2)
        {
            NS_ASSERT(localComponent < 6);
            return PSF_Coeff_2D_2[localComponent][0] 
                + PSF_Coeff_2D_2[localComponent][1]*v[0]
                + PSF_Coeff_2D_2[localComponent][2]*v[1]
                + PSF_Coeff_2D_2[localComponent][3]*v[0]*v[1]
                + PSF_Coeff_2D_2[localComponent][4]*v[0]*v[0]
                + PSF_Coeff_2D_2[localComponent][5]*v[1]*v[1];
        }
        else if(Order == 3)
        {
            NS_ASSERT(localComponent < 10);
            return PSF_Coeff_2D_3[localComponent][0] 
                + PSF_Coeff_2D_3[localComponent][1]*v[0]
                + PSF_Coeff_2D_3[localComponent][2]*v[1]
                + PSF_Coeff_2D_3[localComponent][3]*v[0]*v[1]
                + PSF_Coeff_2D_3[localComponent][4]*v[0]*v[0]
                + PSF_Coeff_2D_3[localComponent][5]*v[1]*v[1]
                + PSF_Coeff_2D_3[localComponent][6]*v[0]*v[0]*v[1]
                + PSF_Coeff_2D_3[localComponent][7]*v[0]*v[1]*v[1]
                + PSF_Coeff_2D_3[localComponent][8]*v[0]*v[0]*v[0]
                + PSF_Coeff_2D_3[localComponent][9]*v[1]*v[1]*v[1];
        }
    }
    else if(K == 3)
    {
        if(Order == 1)
        {
            NS_ASSERT(localComponent < 4);
            return PSF_Coeff_3D_1[localComponent][0] 
                + PSF_Coeff_3D_1[localComponent][1]*v[0]
                + PSF_Coeff_3D_1[localComponent][2]*v[1]
                + PSF_Coeff_3D_1[localComponent][3]*v[2];
        }
        else if(Order == 2)
        {
            NS_ASSERT(localComponent < 10);
            return PSF_Coeff_3D_2[localComponent][0] 
                + PSF_Coeff_3D_2[localComponent][1]*v[0]
                + PSF_Coeff_3D_2[localComponent][2]*v[1]
                + PSF_Coeff_3D_2[localComponent][3]*v[2]
                + PSF_Coeff_3D_2[localComponent][4]*v[0]*v[1]
                + PSF_Coeff_3D_2[localComponent][5]*v[0]*v[2]
                + PSF_Coeff_3D_2[localComponent][6]*v[1]*v[2]
                + PSF_Coeff_3D_2[localComponent][7]*v[0]*v[0]
                + PSF_Coeff_3D_2[localComponent][8]*v[1]*v[1]
                + PSF_Coeff_3D_2[localComponent][9]*v[2]*v[2];
        }
    }

    static_assert((K==1 && Order == 1) ||
    (K==1 && Order == 2) ||
    (K==2 && Order == 1) ||
    (K==2 && Order == 2) ||
    (K==2 && Order == 3) ||
    (K==3 && Order == 1) ||
    (K==3 && Order == 2), "Currently only some combinations are implemented.");
}

template<typename T, Dimension K, Dimension Order>
FixedVector<T,K> PolyShapePolicy<T,K,Order>::gradient(Index localComponent, const FixedVector<T,K>& local) const
{
    FixedVector<T,K> g;
    if(K == 1)
    {
        if(Order == 1)
        {
            NS_ASSERT(localComponent < 2);
            g[0] = PSF_Coeff_1D_1[localComponent][1];
        }
        else if(Order == 2)
        {
            NS_ASSERT(localComponent < 3);
            g[0] = PSF_Coeff_1D_2[localComponent][1] + 2*PSF_Coeff_1D_2[localComponent][2]*local[0];
        }
    }
    else if(K == 2)
    {
        if(Order == 1)
        {
            NS_ASSERT(localComponent < 3);
            g[0] = PSF_Coeff_2D_1[localComponent][1];
            g[1] = PSF_Coeff_2D_1[localComponent][2];
        }
        else if(Order == 2)
        {
            NS_ASSERT(localComponent < 6);
            g[0] = PSF_Coeff_2D_2[localComponent][1] 
                + PSF_Coeff_2D_2[localComponent][3]*local[1] 
                + 2*PSF_Coeff_2D_2[localComponent][4]*local[0];
            g[1] = PSF_Coeff_2D_2[localComponent][2] 
                + PSF_Coeff_2D_2[localComponent][3]*local[0] 
                + 2*PSF_Coeff_2D_2[localComponent][5]*local[1];
        }
        else if(Order == 3)
        {
            NS_ASSERT(localComponent < 10);
            g[0] = PSF_Coeff_2D_3[localComponent][1]
                + PSF_Coeff_2D_3[localComponent][3]*local[1]
                + 2*PSF_Coeff_2D_3[localComponent][4]*local[0]
                + 2*PSF_Coeff_2D_3[localComponent][6]*local[0]*local[1]
                + PSF_Coeff_2D_3[localComponent][7]*local[1]*local[1]
                + 3*PSF_Coeff_2D_3[localComponent][8]*local[0]*local[0];
            g[1] = PSF_Coeff_2D_3[localComponent][2]
                + PSF_Coeff_2D_3[localComponent][3]*local[0]
                + 2*PSF_Coeff_2D_3[localComponent][5]*local[1]
                + PSF_Coeff_2D_3[localComponent][6]*local[0]*local[0]
                + 2*PSF_Coeff_2D_3[localComponent][7]*local[0]*local[1]
                + 3*PSF_Coeff_2D_3[localComponent][9]*local[1]*local[1];
        }
    }
    else if(K == 3)
    {
        if(Order == 1)
        {
            NS_ASSERT(localComponent < 4);
            g[0] = PSF_Coeff_3D_1[localComponent][1];
            g[1] = PSF_Coeff_3D_1[localComponent][2];
            g[2] = PSF_Coeff_3D_1[localComponent][3];
        }
        else if(Order == 2)
        {
            NS_ASSERT(localComponent < 10);
            g[0] = PSF_Coeff_3D_2[localComponent][1] 
                + PSF_Coeff_3D_2[localComponent][4]*local[1] 
                + PSF_Coeff_3D_2[localComponent][5]*local[2] 
                + 2*PSF_Coeff_3D_2[localComponent][7]*local[0];

            g[1] = PSF_Coeff_3D_2[localComponent][2] 
                + PSF_Coeff_3D_2[localComponent][4]*local[0] 
                + PSF_Coeff_3D_2[localComponent][6]*local[2] 
                + 2*PSF_Coeff_3D_2[localComponent][8]*local[1];

            g[2] = PSF_Coeff_3D_2[localComponent][3] 
                + PSF_Coeff_3D_2[localComponent][5]*local[0] 
                + PSF_Coeff_3D_2[localComponent][6]*local[1] 
                + 2*PSF_Coeff_3D_2[localComponent][9]*local[2];
        }
    }

    static_assert((K==1 && Order == 1) ||
    (K==1 && Order == 2) ||
    (K==2 && Order == 1) ||
    (K==2 && Order == 2) ||
    (K==2 && Order == 3) ||
    (K==3 && Order == 1) ||
    (K==3 && Order == 2), "Currently only some combinations are implemented.");

    return g;
}

template<typename T, Dimension K, Dimension Order>
void PolyShapePolicy<T,K,Order>::prepareMesh(Mesh<T,K>& m)
{
    static_assert(Order == 1 || 
        (K==1 && Order == 2) ||
        (K==2 && Order == 2) , "Currently only some combinations are implemented.");

    // In polynomial scheme all physical nodes are DOF nodes
    for(auto* e : m.elements())
    {
        for(auto* v : e->Vertices)
            e->DOFVertices.push_back(v);
        
        for(Index i = 0; i < K+1; ++i)
        {
            if(!e->Neighbors[i]->DOFVertices.empty())
                continue;
                
            for(auto* v : e->Neighbors[i]->Vertices)
                e->Neighbors[i]->DOFVertices.push_back(v);
        }
    }

    if(Order == 2)
    {
        if(K == 1)// Line [x0 - x2 - x1]
        {
            for(auto* e : m.elements())
            {
                const auto x2 = (e->Vertices[0]->Vertex + e->Vertices[1]->Vertex)/(T)2;
                MeshVertex<T,K>* v2 = new MeshVertex<T,K>(x2);

                v2->Flags |= e->Vertices[0]->Flags | e->Vertices[1]->Flags;
                v2->Flags |= MVF_Implicit;
                m.addVertex(v2);

                v2->Elements.push_back(e);
                e->DOFVertices.push_back(v2);
            }
        }
        else if(K == 2)
        {
            const auto handleEdge = [&](Index i, MeshElement<T,K>* element)
            {
                MeshEdge<T,K>* edge = element->Neighbors[i];
                if(edge->DOFVertices.size() == K+1)// Already set for this
                {
                    edge->DOFVertices.back()->Elements.push_back(element);
                    element->DOFVertices.push_back(edge->DOFVertices.back());
                }
                else
                {
                    const auto x2 = (edge->Vertices[0]->Vertex + edge->Vertices[1]->Vertex)/(T)2;
                    MeshVertex<T,K>* v2 = new MeshVertex<T,K>(x2);

                    v2->Flags |= MVF_Implicit;
                    if(!edge->Elements[0] || !edge->Elements[1])
                        v2->Flags |= MVF_StrongBoundary;

                    m.addVertex(v2);

                    v2->Elements.push_back(element);
                    element->DOFVertices.push_back(v2);
                    edge->DOFVertices.push_back(v2);
                }
            };

            for(auto* e : m.elements())
            {
                // To ensure right order
                handleEdge(2,e);
                handleEdge(0,e);
                handleEdge(1,e);
            }
        }
    }
}

NS_END_NAMESPACE