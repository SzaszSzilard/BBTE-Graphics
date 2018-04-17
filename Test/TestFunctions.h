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

     }
}
