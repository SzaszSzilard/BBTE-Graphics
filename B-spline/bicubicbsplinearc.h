#ifndef BICUBICBSPLINEARC_H
#define BICUBICBSPLINEARC_H

#pragma once

#include "../Core/LinearCombination3.h"
#include "../Core/Matrices.h"

namespace cagd
{
    class BicubicBSplineArc: public LinearCombination3
    {
    protected:
        GLuint                      _n;         // order
        //GLdouble                    _c_n;       // normalizing constant
        //GLdouble                    _lambda_n;  // phase change

        //TriangularMatrix<GLdouble>  _bc;        // binomial coefficients

        //GLdouble    _CalcuateNormalizingCoefficients(GLuint n);

        //GLvoid      _CalculateBinomialCoefficients(GLuint m, TriangularMatrix<GLdouble> &bc);

    public:
        // special constructor
        //CyclicCurve3(GLuint n, GLenum data_usage_flag = GL_STATIC_DRAW);
        BicubicBSplineArc(GLuint n = 4, GLenum data_usage_flag = GL_STATIC_DRAW);

        // redeclare and define inherited pure virtual methods
        GLboolean BlendingFunctionValues(GLdouble u, RowMatrix<GLdouble>& values) const;

        GLboolean CalculateDerivatives(GLuint max_order_of_derivatives, GLdouble u, Derivatives &d) const;
    };
}


#endif // BICUBICBSPLINEARC_H
