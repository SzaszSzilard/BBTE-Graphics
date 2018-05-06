#pragma once

#include "../Core/DCoordinates3.h"

namespace cagd
{
    namespace spiral_on_cone
    {
        extern GLdouble u_min, u_max;

        DCoordinate3 d0(GLdouble);
        DCoordinate3 d1(GLdouble);
        DCoordinate3 d2(GLdouble);
    }

    namespace spiral
    {
        extern GLdouble u_min, u_max;

        DCoordinate3 d0(GLdouble);
        DCoordinate3 d1(GLdouble);
        DCoordinate3 d2(GLdouble);
    }

    namespace ellipse
    {
        extern GLdouble u_min, u_max;

        DCoordinate3 d0(GLdouble);
        DCoordinate3 d1(GLdouble);
        DCoordinate3 d2(GLdouble);
    }

     namespace helix
     {
         extern GLdouble u_min, u_max;

         DCoordinate3 d0(GLdouble);
         DCoordinate3 d1(GLdouble);
         DCoordinate3 d2(GLdouble);
     }

     namespace cyclo
     {
         extern GLdouble u_min, u_max;

         DCoordinate3 d0(GLdouble);
         DCoordinate3 d1(GLdouble);
         DCoordinate3 d2(GLdouble);
     }

     namespace hypo
     {
         extern GLdouble u_min, u_max;

         DCoordinate3 d0(GLdouble);
         DCoordinate3 d1(GLdouble);
         DCoordinate3 d2(GLdouble);
     }

     namespace torus
     {
         extern GLdouble u_min, u_max;

         DCoordinate3 d0(GLdouble);
         DCoordinate3 d1(GLdouble);
         DCoordinate3 d2(GLdouble);
     }


     namespace torus_surface
     {
         extern GLdouble u_min, u_max,
         v_min, v_max,
         R, r;

         DCoordinate3 d00(GLdouble u, GLdouble v); // zeroth order partial derivative, i.e. surface point
         DCoordinate3 d10(GLdouble u, GLdouble v); // first order partial derivative in direction u
         DCoordinate3 d01(GLdouble u, GLdouble v); // first order partial derivative in direction v
     }

     namespace alfred_klein_bottle
     {
         extern GLdouble u_min, u_max,
         v_min, v_max;

         DCoordinate3 d00(GLdouble u, GLdouble v);
         DCoordinate3 d10(GLdouble u, GLdouble v);
         DCoordinate3 d01(GLdouble u, GLdouble v);
     }

     namespace cylindrical_helicoid
     {
         extern GLdouble u_min, u_max,
         v_min, v_max;

         DCoordinate3 d00(GLdouble u, GLdouble v);
         DCoordinate3 d10(GLdouble u, GLdouble v);
         DCoordinate3 d01(GLdouble u, GLdouble v);
     }

     namespace hyperboloid
     {
         extern GLdouble u_min, u_max,
         v_min, v_max;

         DCoordinate3 d00(GLdouble u, GLdouble v);
         DCoordinate3 d10(GLdouble u, GLdouble v);
         DCoordinate3 d01(GLdouble u, GLdouble v);
     }

     namespace sphere
     {
         extern GLdouble u_min, u_max,
         v_min, v_max,
         r;

         DCoordinate3 d00(GLdouble u, GLdouble v);
         DCoordinate3 d10(GLdouble u, GLdouble v);
         DCoordinate3 d01(GLdouble u, GLdouble v);
     }
}
