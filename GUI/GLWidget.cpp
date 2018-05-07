#include "GLWidget.h"
#include <iostream>
#include <GL/glu.h>
#include <Core/Exceptions.h>

#include "../Core/Constants.h"
#include "../Core/Matrices.h"
#include "../Test/TestFunctions.h"

using namespace std;
namespace cagd
{
    //--------------------------------
    // special and default constructor
    //--------------------------------
    GLWidget::GLWidget(QWidget *parent, const QGLFormat &format): QGLWidget(format, parent)
    {
    }

    GLWidget::~GLWidget() {
        for (GLuint i = 0; i < _num_of_pc; i++)
            if (_pc[i])
                delete _pc[i], _pc[i] = 0;

        for (GLuint i = 0; i < _num_of_pc; i++)
            if (_image_of_pc[i])
                delete _image_of_pc[i], _image_of_pc[i] = 0;
    }

    //--------------------------------------------------------------------------------------
    // this virtual function is called once before the first call to paintGL() or resizeGL()
    //--------------------------------------------------------------------------------------
    void GLWidget::initializeGL()
    {
        // creating a perspective projection matrix
        glMatrixMode(GL_PROJECTION);

        glLoadIdentity();

        _aspect = (float)width() / (float)height();
        _z_near = 1.0;
        _z_far = 1000.0;
        _fovy = 45.0;

        gluPerspective(_fovy, _aspect, _z_near, _z_far);

        // setting the model view matrix
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        _eye[0] = _eye[1] = 0.0, _eye[2] = 6.0;
        _center[0] = _center[1] = _center[2] = 0.0;
        _up[0] = _up[2] = 0.0, _up[1] = 1.0;

        gluLookAt(_eye[0], _eye[1], _eye[2], _center[0], _center[1], _center[2], _up[0], _up[1], _up[2]);

        // enabling depth test
        glEnable(GL_DEPTH_TEST);

        // setting the color of background
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        // initial values of transformation parameters
        _angle_x = _angle_y = _angle_z = 0.0;
        _trans_x = _trans_y = _trans_z = 0.0;
        _zoom = 1.0;

        try
        {
            // initializing the OpenGL Extension Wrangler library
            GLenum error = glewInit();

            if (error != GLEW_OK)
            {
                throw Exception("Could not initialize the OpenGL Extension Wrangler Library!");
            }

            if (!glewIsSupported("GL_VERSION_2_0"))
            {
                throw Exception("Your graphics card is not compatible with OpenGL 2.0+! "
                                "Try to update your driver or buy a new graphics adapter!");
            }

            // create and store your geometry in display lists or vertex buffer objects
            _index = 0;
            // knim1445
            init_parametric_curves();
            init_cyclic_curves();
            init_parametric_surfaces();
            /*if (!_shadersInstallShaders("Shaders/reflection_lines.vert",
                "Shaders_refelction_lines.frag",GL_TRE))
              throw Exception();
               _shader.Enable();
               _shader.setUniformVariable1f("scale_factor",4.0f);
               _shader.setUniformVariable1f("smoothing",2.0f);
               _shader.setUniformVariable1f("shading",1.0f);*/


        }
        catch (Exception &e)
        {
            cout << e << endl;
        }

        glEnable(GL_POINT_SMOOTH);
        glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
        glEnable(GL_POLYGON_SMOOTH);
        glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

        glEnable(GL_DEPTH_TEST);

        glewInit();     
    }

    //-----------------------
    // the rendering function
    //-----------------------
    void GLWidget::paintGL()
    {
        // clears the color and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // stores/duplicates the original model view matrix
        glPushMatrix();

            // applying transformations
            glRotatef(_angle_x, 1.0, 0.0, 0.0);
            glRotatef(_angle_y, 0.0, 1.0, 0.0);
            glRotatef(_angle_z, 0.0, 0.0, 1.0);
            glTranslated(_trans_x, _trans_y, _trans_z);
            glScaled(_zoom, _zoom, _zoom);

            switch (_page_index) {
            case 1:
                render_pc();
                break;
            case 2:
                render_cc();
                break;
            case 4:
                render_ps();
                break;
            default:
                render_pc();
                break;
            }

        // pops the current matrix stack, replacing the current matrix with the one below it on the stack,
        // i.e., the original model view matrix is restored
        glPopMatrix();
    }

    //----------------------------------------------------------------------------
    // when the main window is resized one needs to redefine the projection matrix
    //----------------------------------------------------------------------------
    void GLWidget::resizeGL(int w, int h)
    {
        // setting the new size of the rendering context
        glViewport(0, 0, w, h);

        // redefining the projection matrix
        glMatrixMode(GL_PROJECTION);

        glLoadIdentity();

        _aspect = (float)w / (float)h;

        gluPerspective(_fovy, _aspect, _z_near, _z_far);

        // switching back to the model view matrix
        glMatrixMode(GL_MODELVIEW);

        updateGL();
    }

    //-----------------------------------
    // implementation of the public slots
    //-----------------------------------

    void GLWidget::set_angle_x(int value)
    {
        if (_angle_x != value)
        {
            _angle_x = value;
            updateGL();
        }
    }

    void GLWidget::set_angle_y(int value)
    {
        if (_angle_y != value)
        {
            _angle_y = value;
            updateGL();
        }
    }

    void GLWidget::set_angle_z(int value)
    {
        if (_angle_z != value)
        {
            _angle_z = value;
            updateGL();
        }
    }

    void GLWidget::set_zoom_factor(double value)
    {
        if (_zoom != value)
        {
            _zoom = value;
            updateGL();
        }
    }

    void GLWidget::set_trans_x(double value)
    {
        if (_trans_x != value)
        {
            _trans_x = value;
            updateGL();
        }
    }

    void GLWidget::set_trans_y(double value)
    {
        if (_trans_y != value)
        {
            _trans_y = value;
            updateGL();
        }
    }

    void GLWidget::set_trans_z(double value)
    {
        if (_trans_z != value)
        {
            _trans_z = value;
            updateGL();
        }
    }

    void GLWidget::set_parametric_curve_index(int index)
    {
        if (_index != index)
        {
            _index = index;
            _page_index = 1;
            updateGL();
        }
    }

    void GLWidget::set_cyclic_curve_index()
    {
        _page_index = 2;
        updateGL();
    }

    void GLWidget::set_parametric_surface_index(int index)
    {
        if (_ps_index != index)
        {
            _ps_index = index;
            _page_index = 4;
            updateGL();
        }
    }


    // knim1445
    void GLWidget::init_parametric_curves(){
        _num_of_pc = 6;
        _pc.ResizeColumns(_num_of_pc);

        RowMatrix<ParametricCurve3::Derivative> derivative(3);

        derivative(0) = spiral_on_cone::d0;
        derivative(1) = spiral_on_cone::d1;
        derivative(2) = spiral_on_cone::d2;
        _pc[0] = new ParametricCurve3(derivative, spiral_on_cone::u_min, spiral_on_cone::u_max);

        derivative(0) = spiral::d0;
        derivative(1) = spiral::d1;
        derivative(2) = spiral::d2;
        _pc[1] = new ParametricCurve3(derivative, spiral::u_min, spiral::u_max);

        derivative(0) = helix::d0;
        derivative(1) = helix::d1;
        derivative(2) = helix::d2;
        _pc[2] = new ParametricCurve3(derivative, helix::u_min, helix::u_max);

        derivative(0) = ellipse::d0;
        derivative(1) = ellipse::d1;
        derivative(2) = ellipse::d2;
        _pc[3] = new ParametricCurve3(derivative, ellipse::u_min, ellipse::u_max);

        derivative(0) = cyclo::d0;
        derivative(1) = cyclo::d1;
        derivative(2) = cyclo::d2;
        _pc[4] = new ParametricCurve3(derivative, cyclo::u_min, cyclo::u_max);

        derivative(0) = torus::d0;
        derivative(1) = torus::d1;
        derivative(2) = torus::d1;
        _pc[5] = new ParametricCurve3(derivative, cyclo::u_min, cyclo::u_max);

        _image_of_pc.ResizeColumns(_num_of_pc);

        GLuint div_point_count = 500;
        GLenum usage_flag = GL_STATIC_DRAW;
        for (GLuint i = 0; i < _num_of_pc; i++) {
            if (! _pc[i]) {
                cout << "parametric curve wasnt initialized" << endl;
            }

            _image_of_pc[i] = _pc[i]->GenerateImage(div_point_count, usage_flag);

            if (! _image_of_pc[i]) {
                cout << "image of parametric curve wasnt initialized" << endl;
            }

            if (! _image_of_pc[i]->UpdateVertexBufferObjects(usage_flag)) {
                cout << "Could not create the vertex buffer object of the parametrci curve" << endl;
            }
        }
    }

    void GLWidget::init_cyclic_curves(){
        _n  = 4;
        _cc = new CyclicCurve3(_n);

        GLdouble step = TWO_PI / (2 * _n + 1);

        for (GLuint i = 0; i <= 2*_n; i++)
        {
            GLdouble u = i * step;

            DCoordinate3 &cp = (*_cc)[i];

            cp[0] = cos(u);
            cp[1] = sin(u);
            cp[2] = -2.0 + 4.0 * (GLdouble)rand() / RAND_MAX;
        }
        _cc->UpdateVertexBufferObjectsOfData();

        _mod = 3;
        _div = 100;
        _img_cc = _cc->GenerateImage(_mod, _div);
        _img_cc->UpdateVertexBufferObjects();
    }

    void GLWidget::init_parametric_surfaces(){
        _num_of_ps = 5;
        _ps.ResizeColumns(_num_of_ps);

        TriangularMatrix<ParametricSurface3::PartialDerivative> derivative(3);

        derivative(0) = torus_surface::d00;
        derivative(1) = torus_surface::d10;
        derivative(2) = torus_surface::d01;
        _ps[0] = new ParametricSurface3(derivative, torus_surface::u_min, torus_surface::u_max,torus_surface::v_min,torus_surface::v_max);

        _image_of_ps.ResizeColumns(_num_of_ps);

        GLuint div_point_count = 500;
        GLuint v_point_count = 500;
        GLenum usage_flag = GL_STATIC_DRAW;
        for (GLuint i = 0; i < _num_of_ps; i++) {
            if (! _ps[i]) {
                cout << "parametric curve wasnt initialized" << endl;
            }

            _image_of_ps[i] = _ps[i]->GenerateImage(div_point_count,v_point_count,usage_flag);

            if (! _image_of_ps[i]) {
                cout << "image of parametric curve wasnt initialized" << endl;
            }

            if (! _image_of_ps[i]->UpdateVertexBufferObjects(usage_flag)) {
                cout << "Could not create the vertex buffer object of the parametrci curve" << endl;
            }
        }
    }

    void GLWidget::render_pc(){
        if (_image_of_pc[_index]) {
            glColor3f(1.0,1.0,1.0);
            _image_of_pc[_index]->RenderDerivatives(0, GL_LINE_STRIP);

            glPointSize(5.0);

                glColor3f(0.0, 0.5, 0.0);
                _image_of_pc[_index]->RenderDerivatives(1, GL_LINES);
                _image_of_pc[_index]->RenderDerivatives(1, GL_POINTS);

                glColor3f(1.0, 0.5, 0.0);
                _image_of_pc[_index]->RenderDerivatives(2, GL_LINES);
                _image_of_pc[_index]->RenderDerivatives(2, GL_POINTS);

            glPointSize(1.0);
        }
    }

    void GLWidget::render_ps(){
        if (_image_of_ps[_ps_index]) {
            glColor3f(1.0,1.0,1.0);
            _image_of_ps[_ps_index]->RenderDerivatives(0, GL_LINE_STRIP);

            glPointSize(5.0);

                glColor3f(0.0, 0.5, 0.0);
                _image_of_ps[_ps_index]->RenderDerivatives(1, GL_LINES);
                _image_of_ps[_ps_index]->RenderDerivatives(1, GL_POINTS);

                glColor3f(1.0, 0.5, 0.0);
                _image_of_ps[_ps_index]->RenderDerivatives(2, GL_LINES);
                _image_of_ps[_ps_index]->RenderDerivatives(2, GL_POINTS);

            glPointSize(1.0);
        }
    }

    void GLWidget::render_cc(){
        if (_cc)
        {
            _cc->RenderData(GL_LINE_LOOP);
        }

        if (_img_cc)
        {
            glColor3f(1.0, 0.0, 0.0);
            _img_cc->RenderDerivatives(0, GL_LINE_LOOP);
            glColor3f(0.0, 0.5, 0.0);
            _img_cc->RenderDerivatives(1, GL_LINES);
            glColor3f(0.1, 0.5, 0.9);
            _img_cc->RenderDerivatives(2, GL_LINES);
        }
    }

}
