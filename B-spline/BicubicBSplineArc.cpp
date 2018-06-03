#include "../B-spline/BicubicBSplineArc.h"
#include "../Core/LinearCombination3.h"
#include "../Core/Matrices.h"

using namespace std;

namespace cagd
{

    BicubicBSplineArc::BicubicBSplineArc(GLuint n, GLenum data_usage_flag):
        LinearCombination3(0.0, 1, n, data_usage_flag),
        _n(n)
    {}

    GLboolean BicubicBSplineArc::BlendingFunctionValues(GLdouble u_knot, RowMatrix<GLdouble> &blending_values) const
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

    GLboolean BicubicBSplineArc::CalculateDerivatives(GLuint max_order_of_derivatives, GLdouble u, Derivatives &d) const
    {
        if (u < 0.0 || u > 1.0 || max_order_of_derivatives > 1)
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

        // calculate partial derivatives
        d.ResizeRows(max_order_of_derivatives+1);
        d.LoadNullVectors();

        for (GLuint i = 0; i<4; ++i)
        {
            d(0) += _data(i) * u_blending_values(i);
            d(1) += _data(i) * d1_u_blending_values(i);
        }

        return GL_TRUE;
    }

}
