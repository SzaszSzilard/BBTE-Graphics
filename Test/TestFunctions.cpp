#include <cmath>
#include "TestFunctions.h"
#include "../Core/Constants.h"

using namespace cagd;
using namespace std;

GLdouble spiral_on_cone::u_min = -TWO_PI;
GLdouble spiral_on_cone::u_max = +TWO_PI;

DCoordinate3 spiral_on_cone::d0(GLdouble u)
{
    return DCoordinate3(u * cos(u), u * sin(u), u);
}

DCoordinate3 spiral_on_cone::d1(GLdouble u)
{
    GLdouble c = cos(u), s = sin(u);
    return DCoordinate3(c - u * s, s + u * c, 1.0);
}

DCoordinate3 spiral_on_cone::d2(GLdouble u)
{
    GLdouble c = cos(u), s = sin(u);
    return DCoordinate3(-2.0 * s - u * c, 2.0 * c - u * s, 0);
}


GLdouble spiral::u_min = 0;
GLdouble spiral::u_max = 4*PI;

DCoordinate3 spiral::d0(GLdouble u)
{
    return DCoordinate3(cos(u),sin(u),u/4);
}

DCoordinate3 spiral::d1(GLdouble u)
{
    GLdouble c = cos(u), s = sin(u);
    return DCoordinate3(-s,c,1.0/4);
}

DCoordinate3 spiral::d2(GLdouble u)
{
    GLdouble c = cos(u), s = sin(u);
    return DCoordinate3(-c,-s,0);
}


GLdouble hypo::u_min = -3.0;
GLdouble hypo::u_max = +3.0;

DCoordinate3 hypo::d0(GLdouble u)
{
    return DCoordinate3(5*cos(u)+cos(5*u), 5*sin(u)-sin(5*u), 0);
}

DCoordinate3 hypo::d1(GLdouble u)
{
    return DCoordinate3(-5*sin(u)-5*sin(5*u), 5*cos(u)-5*cos(5*u), 0);
}

DCoordinate3 hypo::d2(GLdouble u)
{
    return DCoordinate3(-5*cos(u)-25*cos(5*u), -5*sin(u)+25*sin(5*u), 0);
}

GLdouble cyclo::u_min = 0.0;
GLdouble cyclo::u_max = 125.1324;

DCoordinate3 cyclo::d0(GLdouble u)
{
    return 0.1 * DCoordinate3(2*(u-sin(u)), 2*(1-cos(u)), 0);
}

DCoordinate3 cyclo::d1(GLdouble u)
{
    return 0.1 * DCoordinate3(2-2*cos(u), 2*sin(u), 0);
}

DCoordinate3 cyclo::d2(GLdouble u)
{
    return DCoordinate3(2*sin(u), 2*cos(u), 0);
}


GLdouble ellipse::u_min = -3.141592;
GLdouble ellipse::u_max = +3.141592;

DCoordinate3 ellipse::d0(GLdouble u)
{
    return DCoordinate3(6 * cos(u), 4* sin(u), 0);
}

DCoordinate3 ellipse::d1(GLdouble u)
{
    return DCoordinate3(-6 * sin(u),4* cos(u), 0);
}

DCoordinate3 ellipse::d2(GLdouble u)
{
    return DCoordinate3(-6 * cos(u),-4 * sin(u), 0);
}

GLdouble helix::u_min = 0;
GLdouble helix::u_max = 75.39;

DCoordinate3 helix::d0(GLdouble u)
{
    return 0.2* DCoordinate3(cos(u),sin(u), u/7);
}

DCoordinate3 helix::d1(GLdouble u)
{
    return 0.2* DCoordinate3(-sin(u),cos(u), 1.0/7);
}

DCoordinate3 helix::d2(GLdouble u)
{
    return 0.2* DCoordinate3(-cos(u),-sin(u), 0);
}

GLdouble torus::u_min = 0.0;
GLdouble torus::u_max = 6*PI;

DCoordinate3 torus::d0(GLdouble u)
{
    return DCoordinate3((2+cos(2*u/3))*cos(u), (2+cos(2*u/3))*sin(u), sin(2*u/3));
}

DCoordinate3 torus::d1(GLdouble u)
{
    return DCoordinate3(-(2+cos(2*u/3))*sin(u)-2.0/3*sin(2*u/3)*cos(u), (2+cos(2*u/3))*cos(u)-2.0/3*sin(2*u/3)*sin(u), 2.0/3*cos(2*u/3));
}

DCoordinate3 torus::d2(GLdouble)
{
    return DCoordinate3();
}

//---------------------------Parametric surfaces----------------------------

GLdouble torus_surface::u_min = 0.0;
GLdouble torus_surface::u_max = TWO_PI;
GLdouble torus_surface::v_min = 0.0;
GLdouble torus_surface::v_max = TWO_PI;
GLdouble torus_surface::R = 1.0;
GLdouble torus_surface::r = 0.5;

DCoordinate3 torus_surface::d00(GLdouble u, GLdouble v)
{
    return DCoordinate3((R + r * sin(u)) * cos(v), (R + r * sin(u)) * sin(v), r * cos(u));
}

DCoordinate3 torus_surface::d10(GLdouble u, GLdouble v)
{
    return DCoordinate3(r * cos(u) * cos(v), r * cos(u) * sin(v), -r * sin(u));
}

DCoordinate3 torus_surface::d01(GLdouble u, GLdouble v)
{
    return DCoordinate3(-(R + r * sin(u)) * sin(v), (R + r * sin(u)) * cos(v), 0.0);
}

GLdouble alfred_klein_bottle::u_min = 0;
GLdouble alfred_klein_bottle::u_max = TWO_PI;
GLdouble alfred_klein_bottle::v_min = 0;
GLdouble alfred_klein_bottle::v_max = TWO_PI;

DCoordinate3 alfred_klein_bottle::d00(GLdouble u, GLdouble v)
{
    return DCoordinate3(3 * cos(u) + 1.0/2.0 * (1 + cos(2 * u)) * sin(v) - 1.0/2.0 * sin(2 * u) * sin(2 * v),
                        3 * sin(u) + 1.0/2.0 * sin(2 * u) * sin(v) - 1.0/2.0 * (1 - cos(2 * u)) * sin(2 * v),
                        cos(u) * sin(2 * v) + sin(u) * sin(v));
}

DCoordinate3 alfred_klein_bottle::d10(GLdouble u, GLdouble v)
{
    return DCoordinate3(-3 * sin(u) - sin(2 * u) * sin(v) + cos(2 * u) * sin(2 * v),
                        3 * cos(u) + cos(2 * u) * sin(v) - sin(2 * u) * sin(2 * v),
                        -sin(u) * sin(2 * v) + cos(u) * sin(v));
}

DCoordinate3 alfred_klein_bottle::d01(GLdouble u, GLdouble v)
{
    return DCoordinate3(1.0/2.0 * (1 + cos(2 * u)) * cos(v) - sin(2 * u) * cos(2 * v),
                        1.0/2.0 * sin(2 * u) * cos(v) - (1 - cos(2 * u)) * cos(2 * v),
                        2 * cos(u) * cos(2 * v) + sin(u) * cos(v));
}

GLdouble cylindrical_helicoid::u_min = 0;
GLdouble cylindrical_helicoid::u_max = 2;
GLdouble cylindrical_helicoid::v_min = 0;
GLdouble cylindrical_helicoid::v_max = 3 * TWO_PI;

DCoordinate3 cylindrical_helicoid::d00(GLdouble u, GLdouble v)
{
    return DCoordinate3((2 + u) * cos(v), (2 + u) * sin(v), v);
}

DCoordinate3 cylindrical_helicoid::d10(GLdouble u, GLdouble v)
{
    return DCoordinate3(3*cos(v), 3*sin(v), v);
}

DCoordinate3 cylindrical_helicoid::d01(GLdouble u, GLdouble v)
{
    return DCoordinate3(-(2 + u) * sin(v), (2 + u) * cos(v), 1);
}

GLdouble hyperboloid::u_min = 0;
GLdouble hyperboloid::u_max = 3;
GLdouble hyperboloid::v_min = 0;
GLdouble hyperboloid::v_max = TWO_PI;

DCoordinate3 hyperboloid::d00(GLdouble u, GLdouble v)
{
    return DCoordinate3((1 + cosh(u - 3.0/2.0)) * sin(v),
                        (1 + cosh(u - 3.0/2.0)) * cos(v),
                        sinh(u - 3.0/2.0));
}

DCoordinate3 hyperboloid::d10(GLdouble u, GLdouble v)
{
    return DCoordinate3(sinh(u - 3.0/2.0) * sin(v),
                        sinh(u - 3.0/2.0) * cos(v),
                        cosh(u - 3.0/2.0));
}

DCoordinate3 hyperboloid::d01(GLdouble u, GLdouble v)
{
    return DCoordinate3((1 + cosh(u - 3.0/2.0)) * cos(v),
                        (1 + cosh(u - 3.0/2.0)) * sin(v),
                        0);
}

GLdouble sphere::u_min = 0;
GLdouble sphere::u_max = PI;
GLdouble sphere::v_min = 0;
GLdouble sphere::v_max = TWO_PI;
GLdouble sphere::r = 1.0;

DCoordinate3 sphere::d00(GLdouble u, GLdouble v)
{
    return DCoordinate3(r * sin(u) * cos(v), r * sin(u) * sin(v), r * cos(u));
}

DCoordinate3 sphere::d10(GLdouble u, GLdouble v)
{
    return DCoordinate3(r * cos(u) * cos(v), r * cos(u) * sin(v), -r * sin(u));
}

DCoordinate3 sphere::d01(GLdouble u, GLdouble v)
{
    return DCoordinate3(-r * sin(u) * sin(v), r * sin(u) * cos(v), 0);
}
