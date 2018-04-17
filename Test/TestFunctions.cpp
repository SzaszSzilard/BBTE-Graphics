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
    return DCoordinate3(-s,c,1/4);
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
    return 0.1 * DCoordinate3(2*(1-cos(u)), 2*sin(u), 0);
}

DCoordinate3 cyclo::d2(GLdouble u)
{
    return DCoordinate3(sin(u), 2*cos(u), 0);
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
    return 0.2* DCoordinate3(-sin(u),cos(u), 1/7);
}

DCoordinate3 helix::d2(GLdouble u)
{
    return 0.2* DCoordinate3(-sin(u),cos(u), 0);
}


GLdouble torus::u_min = 0.0;
GLdouble torus::u_max = 18.8493;

DCoordinate3 torus::d0(GLdouble u)
{
    return DCoordinate3((2+cos(2*u/3))*cos(u), (2+cos(2*u/3))*sin(u), sin(2*u/3));
}

DCoordinate3 torus::d1(GLdouble u)
{
    return DCoordinate3(-(2+cos(2*u/3))*sin(u)-2.0/3*sin(2*u/3)*cos(u), (2+cos(2*u/3))*cos(u)-2.0/3*sin(2*u/3)*sin(u), 2.0/3*cos(2*u/3));
}
