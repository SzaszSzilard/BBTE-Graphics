#include "CyclicCurve3.h"

#include "../Core/Constants.h"

#include <iostream>
#include <cmath>

using namespace std;

namespace cagd
{

    CyclicCurve3::CyclicCurve3(GLuint n, GLenum data_usage_flag):
        LinearCombination3(0.0, TWO_PI, 2 * n + 1, data_usage_flag),
        _n(n),
        _c_n(_CalcuateNormalizingCoefficients(n)),
        _lambda_n(TWO_PI / (2 * n +1))
    {
        _CalculateBinomialCoefficients(2*_n, _bc);
    }

    GLdouble CyclicCurve3::_CalcuateNormalizingCoefficients(GLuint n)
    {
        if (!n)
        {
            return 0.0;
        }

        GLdouble c = 1.0 / 3.0;

        for (GLuint i = 2; i <= n; ++i)
        {
            c*= (GLdouble)i / (GLdouble)(2 * i + 1);
        }

        return c;
    }

    GLvoid CyclicCurve3::_CalculateBinomialCoefficients(GLuint m, TriangularMatrix<GLdouble> &bc)
    {
        bc.ResizeRows(m+1);

        bc(0, 0) = 1.0;

        for (GLuint r = 1; r <= m; ++r)
        {
            bc(r, 0) = 1.0;
            bc(r, r) = 1.0;

            for (GLuint i = 1; i<= r / 2; ++i)
            {
                bc(r, i) = bc(r-1, i-1) + bc(r-1, i);
                bc(r, r-i) = bc(r, i);
            }
        }
    }

    GLboolean CyclicCurve3::BlendingFunctionValues(GLdouble u, RowMatrix<GLdouble>& values) const
    {
        values.ResizeRows(2 * _n + 1);

        for (GLuint i = 0; i < 2 * _n + 1; ++i)
        {
            values[i] = _c_n * pow(1.0 + cos(u - i * _lambda_n), (GLint)_n );
        }

        return GL_TRUE;
    }

    GLboolean CyclicCurve3::CalculateDerivatives(GLuint max_order_of_derivatives, GLdouble u, Derivatives &d) const
    {
        d.ResizeRows( max_order_of_derivatives + 1 );
        d.LoadNullVectors();

        DCoordinate3 centroid;

        for ( GLuint i = 0; i <= 2 * _n; ++i)
        {
            centroid += _data[i];
        }
        centroid /= (GLdouble)(2 * _n + 1);

        for ( GLuint r = 0; r <= max_order_of_derivatives; ++r )
        {
            for (GLuint i = 0; i <= 2 * _n; ++i)
            {
                GLdouble sum_k = 0.0;

                for (GLuint k = 0; k <= _n -1; ++k)
                {
                    sum_k += pow(_n - k, (GLint)r) * _bc(2 * _n, k) * cos((_n - k) * (u - i * _lambda_n) + r * PI / 2.0);
                }

                d[r] += sum_k * _data[i];
            }
            d[r] *= 2.0;
            d[r] /= (GLdouble)(2 * _n + 1);
            d[r] /= _bc(2 * _n, _n);
        }

        d[0] += centroid;

        return GL_TRUE;
    }
}
