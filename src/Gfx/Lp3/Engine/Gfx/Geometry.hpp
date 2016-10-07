#ifndef FILE_LP3_ENGINE_GFX_GEOMETRY_HPP
#define FILE_LP3_ENGINE_GFX_GEOMETRY_HPP


namespace Lp3 { namespace Engine { namespace Gfx { namespace Geometry {

/******************************************************************************
 * 2D
 ******************************************************************************/
template<typename Number>
struct Coordinates2d
{
    typedef Number NumberType;
    Number X;
    Number Y;

    template<typename Archive>
    void serialize(Archive & archive)
    {
        /*
        archive(cereal::make_nvp("X", X));
        archive(cereal::make_nvp("Y", Y));
        */
    }
};

template<typename Number>
struct Size2d
{
    typedef Number NumberType;
    Number Width;
    Number Height;
};


template<typename Number>
struct NormalizedCoordinates2d
{
    typedef Number NumberType;

    template<typename SizeNumberType>
    using SizeType = Size2d<SizeNumberType>;

    typedef Coordinates2d<Number> UnnormalizedCoordType;

    Number X;
    Number Y;

    inline NormalizedCoordinates2d(Number X, Number Y)
    :   X(X), Y(Y)
    {}

    template<typename ArgNumberC, typename ArgNumberS>
    inline NormalizedCoordinates2d(Coordinates2d<ArgNumberC> c,
                                   SizeType<ArgNumberS> d)
    :   X(c.X / (NumberType) d.Width), Y(c.Y / (NumberType) d.Height)
    {}

};

/******************************************************************************
 * 3D
 ******************************************************************************/

template<typename Number>
struct Coordinates3d
{
    typedef Number NumberType;
    Number X;
    Number Y;
    Number Z;
};

template<typename Number>
struct Size3d
{
    typedef Number NumberType;
    Number Width;
    Number Height;
    Number Length;
};



template<typename Number>
struct NormalizedCoordinates3d
{
    typedef Number NumberType;
    typedef Size3d<Number> SizeType;
    typedef Coordinates3d<Number> UnnormalizedCoordType;

    Number X;
    Number Y;
    Number Z;

    inline NormalizedCoordinates3d(Number X, Number Y, Number Z)
    :   X(X), Y(Y), Z(Z)
    {}

    template<typename ArgNumberC, typename ArgNumberS>
    inline NormalizedCoordinates3d(Coordinates3d<ArgNumberC> c,
                                   Size3d<ArgNumberS> d)
    :   X(c.X / (NumberType) d.Width), Y(c.Y / (NumberType) d.Height),
        Z(c.Z / (NumberType) d.Length)
    {}
};


/******************************************************************************
 * Pairs, Triples and Quads
 ******************************************************************************/

template<typename T>
struct CoordinatePair
{
    typedef T CoordType;

    CoordType A;
    CoordType B;

    inline CoordinatePair(CoordType a, CoordType b)
    :   A(a), B(b)
    {}

    inline CoordinatePair()
    :   A(), B()
    {}

    template<typename Archive>
    void serialize(Archive & archive)
    {
        /*
        archive(cereal::make_nvp("A", A));
        archive(cereal::make_nvp("B", B));
        */
    }

    inline void Set(CoordType a, CoordType b)
    {
        this->A = a;
        this->B = b;
    }
};

template<typename T>
struct NormalizedCoordinatePair : public CoordinatePair<T>
{
    inline NormalizedCoordinatePair(T a, T b)
    :   CoordinatePair<T>(a, b)
    {}

    template<typename ArgCordType, typename SizeType>
    inline NormalizedCoordinatePair(ArgCordType a, ArgCordType b, SizeType s)
    :   CoordinatePair<T>(T(a, s), T(b, s))
    {}

    template<typename ArgCordPairType, typename SizeType>
    inline NormalizedCoordinatePair(ArgCordPairType pair, SizeType s)
    :   CoordinatePair<T>(T(pair.A, s), T(pair.B, s))
    {}
};


template<typename T>
struct CoordinateTriple
{
    typedef T CoordType;

    CoordType A;
    CoordType B;
    CoordType C;

    inline CoordinateTriple(CoordType a, CoordType b, CoordType c)
    :   A(a), B(b), C(c)
    {}


    inline void Set(CoordType a, CoordType b, CoordType c)
    {
        this->A = a;
        this->B = b;
        this->C = c;
    }
};

template<typename T>
struct CoordinateQuad
{
    typedef T CoordType;

    CoordType A;
    CoordType B;
    CoordType C;

    inline CoordinateQuad(CoordType a, CoordType b, CoordType c)
    :   A(a), B(b), C(c)
    {}


    inline void Set(CoordType a, CoordType b, CoordType c)
    {
        this->A = a;
        this->B = b;
        this->C = c;
    }
};

template<typename T>
struct NormalizedCoordinateQuad : public CoordinateQuad<T>
{
    inline NormalizedCoordinateQuad(T a, T b)
    :   CoordinateQuad<T>(a, b)
    {}

    template<typename ArgCordType, typename SizeType>
    inline NormalizedCoordinateQuad(ArgCordType a, ArgCordType b, SizeType s)
    :   CoordinateQuad<T>(T(a, s), T(b, s))
    {}

    template<typename ArgCordQuadType, typename SizeType>
    inline NormalizedCoordinateQuad(ArgCordQuadType quad, SizeType s)
    :   CoordinatePair<T>(T(quad.A, s), T(quad.B, s))
    {}
};


/******************************************************************************
 * Aliases
 ******************************************************************************/

template<typename Number>
using CoordinatePair2d = CoordinatePair<Coordinates2d<Number>>;

template<typename Number>
using NormalizedCoordinatePair2d =
    NormalizedCoordinatePair<NormalizedCoordinates2d<Number>>;

template<typename Number>
using CoordinatePair3d = CoordinatePair<Coordinates3d<Number>>;

template<typename Number>
using NormalizedCoordinatePair3d =
    NormalizedCoordinatePair<NormalizedCoordinates3d<Number>>;



using ScreenPoint = Coordinates2d<float>;
using PolyPoint = Coordinates3d<float>;


using SpriteCoordinates = CoordinatePair2d<float>;

using TexturedPoint = Coordinates2d<float>;
using TexturedQuadUV = NormalizedCoordinatePair2d<float>;
using TexturedQuadSrc = CoordinatePair2d<float>;

struct PolyPointT
{
    PolyPoint P;
    TexturedPoint T;
};

#ifdef LP3_COMPILE_TARGET_DREAMCAST
    static_assert(sizeof(TexturedQuadUV) == sizeof(float) * 4,
                  "TexturedQuadUV should be tiny.");
#endif

} } } }

#endif
