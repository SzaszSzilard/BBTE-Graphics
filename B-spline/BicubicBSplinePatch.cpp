#include "../B-spline/BicubicBSplinePatch.h"


using namespace cagd;

BicubicBSplinePatch::BicubicBSplinePatch(): TensorProductSurface3(0.0, 1.0, 0.0, 1.0, 4, 4)
{
}

GLboolean BicubicBSplinePatch::UBlendingFunctionValues(GLdouble u_knot, RowMatrix<GLdouble> &blending_values) const
{
    if (u_knot < 0.0 || u_knot > 1.0)
        return GL_FALSE;

    blending_values.ResizeColumns(4);

    GLdouble u = u_knot, u2 = u*u, u3 = u2*u;
    GLdouble w = 1.0 - u, w2 = w*w, w3 = w2*w;

    blending_values(0) = w3/6;
    blending_values(1) = ((3*u*w2) + (3*w) + 1)/6;
    blending_values(2) = (3*u2*w + 3*u + 1)/6;
    blending_values(3) = u3/6;

    return GL_TRUE;
}

GLboolean BicubicBSplinePatch::VBlendingFunctionValues(GLdouble v_knot, RowMatrix<GLdouble> &blending_values) const
{
    if (v_knot < 0.0 || v_knot > 1.0)
        return GL_FALSE;

    blending_values.ResizeColumns(4);

    GLdouble u = v_knot, u2 = u*u, u3 = u2*u;
    GLdouble w = 1.0 - u, w2 = w*w, w3 = w2*w;

    blending_values(0) = w3/6;
    blending_values(1) = ((3*u*w2) + (3*w) + 1)/6;
    blending_values(2) = (3*u2*w + 3*u + 1)/6;
    blending_values(3) = u3/6;

    return GL_TRUE;
}

GLboolean BicubicBSplinePatch::CalculatePartialDerivatives(
        GLuint maximum_order_of_partial_derivatives,
        GLdouble u, GLdouble v, PartialDerivatives &pd) const
{
    if (u < 0.0 || u > 1.0 || v < 0.0 || v > 1.0 || maximum_order_of_partial_derivatives > 1)
        return GL_FALSE;

    // blending function values and derivatives in u-direction
    RowMatrix<GLdouble> u_blending_values(4), d1_u_blending_values(4);

    GLdouble u2 = u*u, u3 = u2*u;
    GLdouble wu = 1.0 - u, wu2 = wu*wu, wu3 = wu2*wu;

    u_blending_values(0) = wu3/6;
    u_blending_values(1) = ((3*u*wu2) + (3*wu) + 1)/6;
    u_blending_values(2) = (3*u2*wu + 3*u + 1)/6;
    u_blending_values(3) = u3/6;

    d1_u_blending_values(0) = -0.5 * wu2;
    d1_u_blending_values(1) = 0.5 * u * (3*u - 4);
    d1_u_blending_values(2) = (-3*u2)/2 + u + 0.5;
    d1_u_blending_values(3) = 0.5 * wu2;

    // blending function values and derivatives in u-direction
    RowMatrix<GLdouble> v_blending_values(4), d1_v_blending_values(4);

    GLdouble v2 = v*v, v3 = v2*v;
    GLdouble wv = 1.0 - v, wv2 = wv*wv, wv3 = wv2*wv;

    v_blending_values(0) = wv3/6;
    v_blending_values(1) = ((3*v*wv2) + (3*wv) + 1)/6;
    v_blending_values(2) = (3*v2*wv + 3*v + 1)/6;
    v_blending_values(3) = v3/6;

    d1_v_blending_values(0) = -0.5 * wv2;
    d1_v_blending_values(1) = 0.5 * v * (3*v - 4);
    d1_v_blending_values(2) = (-3*v2)/2 + v + 0.5;
    d1_v_blending_values(3) =  0.5 * wv2;

    pd.ResizeRows(2);
    pd.LoadNullVectors();
    for (GLuint row = 0; row < 4; ++row)
    {
        DCoordinate3 aux_d0_v, aux_d1_v;
        for (GLuint column = 0; column<4; ++column)
        {
            aux_d0_v += _data(row,column) * v_blending_values(column);
            aux_d1_v += _data(row,column) * d1_v_blending_values(column);
        }
        pd(0,0) += aux_d0_v * u_blending_values(row);
        pd(1,0) += aux_d0_v * d1_u_blending_values(row);
        pd(1,1) += aux_d1_v * u_blending_values(row);
    }
    return GL_TRUE;
}
