#pragma once

#include "../Core/TensorProductSurfaces3.h"

namespace cagd
{
    class BicubicBSplinePatch: public TensorProductSurface3
    {

        public:
            BicubicBSplinePatch();

            GLboolean UBlendingFunctionValues(GLdouble u_knot, RowMatrix<GLdouble>& blending_values) const;
            GLboolean VBlendingFunctionValues(GLdouble v_knot, RowMatrix<GLdouble>& blending_values) const;
            GLboolean CalculatePartialDerivatives(GLuint maximum_order_of_partial_derivatives,
                                                  GLdouble u, GLdouble v, PartialDerivatives& pd) const;

    };
}
