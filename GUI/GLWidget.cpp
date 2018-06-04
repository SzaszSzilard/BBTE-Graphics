#include "GLWidget.h"
#include <iostream>
#include <GL/glu.h>
#include <Core/Exceptions.h>

#include "../Core/Constants.h"
#include "../Core/Matrices.h"
#include "../Test/TestFunctions.h"
#include "../Core/Lights.h"
#include "../Core/Materials.h"

using namespace std;
namespace cagd
{
    //--------------------------------
    // special and default constructor
    //--------------------------------
    GLWidget::GLWidget(QWidget *parent, const QGLFormat &format): QGLWidget(format, parent)
    {
        _timer = new QTimer(this);
        _timer->setInterval(0);

        _angle = 0.0;
    }

    GLWidget::~GLWidget() {
        for (GLuint i = 0; i < _num_of_pc; i++)
            if (_pc[i])
                delete _pc[i], _pc[i] = 0;

        for (GLuint i = 0; i < _num_of_pc; i++)
            if (_image_of_pc[i])
                delete _image_of_pc[i], _image_of_pc[i] = 0;
        if (_before_interpolation)
            delete _before_interpolation, _before_interpolation = 0;

        if (_after_interpolation)
            delete _after_interpolation, _after_interpolation = 0;
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
            _page_index = 0;
            // knim1445
            init_parametric_curves();
            init_cyclic_curves();
            init_parametric_surfaces();
            init_models();
            init_bspline_arc();
            init_patch();
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
                _shader.Disable();
                render_pc();
                break;
            case 2:
                _shader.Disable();
                render_cc();
                break;
            case 3:
                render_mo();
                break;
            case 4:
                _shader.Disable();
                render_ps();
                break;
            case 6:
                //_shader.Disable();
                render_patch();
                break;
            default:
                render_patch();
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

    void GLWidget::set_cyclic_curve_index(int index)
    {
        //cout << index << endl;
        if (_cc_index != index)
        {
            _cc_index = index;
            _page_index = 2;
            updateGL();
        }
    }

    void GLWidget::set_patch_index(int index)
    {
        if (_patch_index != index)
        {
            _patch_index = index;
            _page_index = 6;
            updateGL();
        }
    }

    void GLWidget::set_models_index(int index){
        if (_mo_index != index)
        {
            _timer->stop();
            _mo_index = index;
            _page_index = 3;
            _angle = 0.0;
            _timer->start();
            updateGL();
        }
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

    void GLWidget::init_cyclic_curves(){

        _num_of_cc = 2;
        _cc.ResizeColumns(_num_of_cc);

        _n  = 4;
        _cc[0] = new CyclicCurve3(_n);

        GLdouble step = TWO_PI / (2 * _n + 1);

        for (GLuint i = 0; i <= 2*_n; i++)
        {
            GLdouble u = i * step;

            DCoordinate3 &cp = (*_cc[0])[i];

            cp[0] = cos(u);
            cp[1] = sin(u);
            cp[2] = -2.0 + 4.0 * (GLdouble)rand() / RAND_MAX;
        }
        _cc[0]->UpdateVertexBufferObjectsOfData();

        GLuint _mod = 3;
        GLuint _div = 100;
        _img_cc.ResizeColumns(_num_of_cc);
        _img_cc[0] = _cc[0]->GenerateImage(_mod, _div);
        _img_cc[0]->UpdateVertexBufferObjects();

        // Here we need to add implementation of interpolating
        _cc[1] = new CyclicCurve3(_n);

        _interp_cc_nodes        = ColumnMatrix<GLdouble>(2 * _n + 1);
        _interp_cc_derivatives  = ColumnMatrix<DCoordinate3>(2 * _n + 1);

        GLdouble t = 0;

        for (GLuint i = 0; i < 2 * _n + 1; i++, t += step)
        {
            _interp_cc_nodes[i] = t;

            DCoordinate3 &cp = (*_cc[1])[i];
            cp.x() = cos(t);
            cp.y() = sin(t);
            cp.z() = 0;

            _interp_cc_derivatives[i] = cp;
        }

        _interp_cc_derivatives[0][2] = 3.0;
        _cc[1]->UpdateDataForInterpolation(_interp_cc_nodes, _interp_cc_derivatives);
        _cc[1]->UpdateVertexBufferObjectsOfData();

        _img_cc[1] = _cc[1]->GenerateImage(_mod, _div);
        _img_cc[1]->UpdateVertexBufferObjects();

    }

    void GLWidget::render_cc(){
        if (_cc[_cc_index])
        {
            _cc[_cc_index]->RenderData(GL_LINE_LOOP);
        }

        if (_img_cc[_cc_index])
        {
            glColor3f(1.0, 0.0, 0.0);
            _img_cc[_cc_index]->RenderDerivatives(0, GL_LINE_LOOP);
            glColor3f(0.0, 0.5, 0.0);
            _img_cc[_cc_index]->RenderDerivatives(1, GL_LINES);
            glColor3f(0.1, 0.5, 0.9);
            _img_cc[_cc_index]->RenderDerivatives(2, GL_LINES);

            if (_cc_index == 1)
            {
                glPointSize(25.0);
                glBegin(GL_POINTS);
                    for (GLuint i = 0; i < 2 * _n + 1; i++)
                    {
                            glVertex3f(_interp_cc_derivatives[i].x(), _interp_cc_derivatives[i].y(), _interp_cc_derivatives[i].z());
                    }
                glEnd();
                glPointSize(1.0);
            }
        }
    }

    void GLWidget::init_parametric_surfaces(){
        _num_of_ps = 5;
        _ps.ResizeColumns(_num_of_ps);

        TriangularMatrix<ParametricSurface3::PartialDerivative> pderivative(4);

        pderivative(0,0) = torus_surface::d00;
        pderivative(1,0) = torus_surface::d10;
        pderivative(1,1) = torus_surface::d01;
        _ps[0] = new ParametricSurface3(pderivative, torus_surface::u_min, torus_surface::u_max,torus_surface::v_min,torus_surface::v_max);

        pderivative(0,0) = alfred_klein_bottle::d00;
        pderivative(1,0) = alfred_klein_bottle::d10;
        pderivative(1,1) = alfred_klein_bottle::d01;
        _ps[1] = new ParametricSurface3(pderivative, alfred_klein_bottle::u_min, alfred_klein_bottle::u_max,alfred_klein_bottle::v_min,alfred_klein_bottle::v_max);

        pderivative(0,0) = cylindrical_helicoid::d00;
        pderivative(1,0) = cylindrical_helicoid::d10;
        pderivative(1,1) = cylindrical_helicoid::d01;
        _ps[2] = new ParametricSurface3(pderivative, cylindrical_helicoid::u_min, cylindrical_helicoid::u_max,cylindrical_helicoid::v_min,cylindrical_helicoid::v_max);

        pderivative(0,0) = hyperboloid::d00;
        pderivative(1,0) = hyperboloid::d10;
        pderivative(1,1) = hyperboloid::d01;
        _ps[3] = new ParametricSurface3(pderivative, hyperboloid::u_min, hyperboloid::u_max,hyperboloid::v_min,hyperboloid::v_max);

        pderivative(0,0) = sphere::d00;
        pderivative(1,0) = sphere::d10;
        pderivative(1,1) = sphere::d01;
        _ps[4] = new ParametricSurface3(pderivative, sphere::u_min, sphere::u_max,sphere::v_min,sphere::v_max);

         _image_of_ps.ResizeColumns(_num_of_ps);

        GLuint div_point_count = 500;
        GLuint v_point_count = 500;
        GLenum usage_flag = GL_STATIC_DRAW;

        for (GLuint i = 0; i < _num_of_ps; i++) {
            if (! _ps[i]) {
                cout << "parametric surface wasnt initialized" << endl;
            }

            _image_of_ps[i] = _ps[i]->GenerateImage(div_point_count,v_point_count,usage_flag);

            if (! _image_of_ps[i]) {
                cout << "image of parametric surface wasnt initialized" << endl;
            }

            if (! _image_of_ps[i]->UpdateVertexBufferObjects(usage_flag)) {
                cout << "Could not create the vertex buffer object of the parametrci surface" << endl;
            }
        }
    }

    void GLWidget::render_ps(){
        if (_image_of_ps[_ps_index]) {
            glEnable(GL_LIGHTING);
            glEnable(GL_NORMALIZE);

            DirectionalLight *dl = 0;
            HCoordinate3 direction(0.0, 0.0, 1.0, 0.0);
            Color4 ambient(0.4, 0.4, 0.4, 1.0);
            Color4 diffuse(0.8, 0.8, 0.8, 1.0);
            Color4 specular(1.0, 1.0, 1.0, 1.0);

            dl = new DirectionalLight(GL_LIGHT0,direction,ambient,diffuse,specular);
            if(dl)
            {
                dl->Enable();
                MatFBRuby.Apply();
                _image_of_ps[_ps_index]->Render();
                dl->Disable();
            }
            glDisable(GL_LIGHTING);
            glDisable(GL_NORMALIZE);
        }
    }

    void GLWidget::init_models(){
        _num_of_mo = 3;
        _image_of_mo.ResizeColumns(_num_of_mo);

        _image_of_mo[0] = new TriangulatedMesh3();
        _image_of_mo[0]->LoadFromOFF("Models/mouse.off",true);
        _image_of_mo[1] = new TriangulatedMesh3();
        _image_of_mo[1]->LoadFromOFF("Models/elephant.off",true);
        _image_of_mo[2] = new TriangulatedMesh3();
        _image_of_mo[2]->LoadFromOFF("Models/sphere.off",true);

        for(GLuint i = 0; i < _num_of_mo; i++)
        {
            _image_of_mo[i]->UpdateVertexBufferObjects(GL_DYNAMIC_DRAW);
        }
    }

    void GLWidget::render_mo(){
         if (_image_of_mo[_mo_index]) {

             glEnable(GL_LIGHTING);
             glEnable(GL_NORMALIZE);

             DirectionalLight *dl = 0;
             HCoordinate3 direction(0.0, 0.0, 1.0, 0.0);
             Color4 ambient(0.4, 0.4, 0.4, 1.0);
             Color4 diffuse(0.8, 0.8, 0.8, 1.0);
             Color4 specular(1.0, 1.0, 1.0, 1.0);

             dl = new DirectionalLight(GL_LIGHT0,direction,ambient,diffuse,specular);
             if(dl)
             {
                 dl->Enable();
                 _shader.Enable();
                 MatFBRuby.Apply();
                 _image_of_mo[_mo_index]->Render();
                 dl->Disable();
                 _shader.Disable();
             }
             glDisable(GL_LIGHTING);
             glDisable(GL_NORMALIZE);
         }
    }

    void  GLWidget::_animate()
    {
        _angle += DEG_TO_RADIAN;
        if (_angle >= TWO_PI)
            _angle -= TWO_PI;

        GLfloat* vertex = _image_of_mo[_mo_index]->MapVertexBuffer(GL_READ_WRITE);
        GLfloat* normal = _image_of_mo[_mo_index]->MapNormalBuffer(GL_READ_ONLY);

        if (vertex && normal)
        {
            GLfloat scale = sin(_angle) / 3000.0;

            for (GLuint i = 0; i < _image_of_mo[_mo_index]->VertexCount(); ++i)
            {
                for (GLuint coordinate = 0; coordinate < 3; ++coordinate, ++vertex, ++normal)
                    *vertex += scale * (*normal);
            }

            _image_of_mo[_mo_index]->UnmapVertexBuffer();
            _image_of_mo[_mo_index]->UnmapNormalBuffer();
            updateGL();
        }
    }

    void GLWidget::set_shader_scale_factor(double value)
    {
        _shader.Enable();
        if (_shader_index == 3){
            _scale_factor = value;
            _shader.SetUniformVariable1f("scale_factor",_scale_factor);
            updateGL();
        }
    }

    void GLWidget::set_shader_smoothing(double value)
    {
         _shader.Enable();
        if (_shader_index == 3){
            _smoothing = value;
            _shader.SetUniformVariable1f("smoothing",_smoothing);
            updateGL();
        }
    }

    void GLWidget::set_shader_shading(double value)
    {
         _shader.Enable();
        if (_shader_index == 3){
            _shading = value;
            _shader.SetUniformVariable1f("shading",_shading);
            updateGL();
        }
    }

    void GLWidget::set_shader_index(int index)
    {
        if (_shader_index != index)
        {
            _shader_index = index;
            init_shader(_shader_index);
            updateGL();
        }
    }

    void GLWidget::init_shader(int index)
    {

        switch (index) {
        case 0:
            _shader.InstallShaders("Shaders/directional_light.vert",
                                   "Shaders/directional_light.frag",GL_TRUE);
            break;
        case 1:
            _shader.InstallShaders("Shaders/two_sided_lighting.vert",
                                   "Shaders/two_sided_lighting.frag",GL_TRUE);
            break;
        case 2:
            _shader.InstallShaders("Shaders/toon.vert",
                                   "Shaders/toon.frag",GL_TRUE);
            break;
        case 3:
            _shader.InstallShaders("Shaders/reflection_lines.vert",
                                   "Shaders/reflection_lines.frag",GL_TRUE);
            break;
        default:
            break;
        }
        _shader.Enable();
        if (index == 3)
        {
            _shader.SetUniformVariable1f("scale_factor",_scale_factor);
            _shader.SetUniformVariable1f("smoothing",_smoothing);
            _shader.SetUniformVariable1f("shading",_shading);
        }
    }

    void GLWidget::init_bspline_arc() {
        GLuint _n = 8;
        _num_of_bspa = 8;
        _bspa.ResizeColumns(_num_of_bspa);

        RowMatrix<DCoordinate3> cpt;
        cpt.ResizeColumns(_n);
        GLdouble step = TWO_PI / _n;

        for (GLuint i = 0; i < _n; i++)
        {
            GLdouble u = i * step;
            DCoordinate3 cp;
            cp[0] = cos(u);
            cp[1] = sin(u);

            cpt[i] = cp;
        }

        for (GLuint i = 0; i < _num_of_bspa; ++i )
            _bspa[i] = new BicubicBSplineArc(); // 4 by default


        for (GLuint i = 0 ; i < _num_of_bspa; ++i)
        for (GLuint j = 0 ; j < 4; ++j) {
            (*_bspa[i])[j] = cpt[(i+j)%_n];
        }


        _mod = 1; // 0th, and 1st order derivatves are all we need, if i understood right
        _div = 50;

        for (GLuint i = 0; i < _num_of_bspa; ++i ) {
            _bspa[i]->UpdateVertexBufferObjectsOfData();

            _img_bspa.ResizeColumns(_num_of_bspa);
            _img_bspa[i] = _bspa[i]->GenerateImage(_mod, _div);
            _img_bspa[i]->UpdateVertexBufferObjects();
        }
    }

    void GLWidget::render_bspline_arc(){
        _shader.Disable();
        for ( GLuint i = 0; i <_num_of_bspa; ++i ) {
            if (_bspa[i])
            {
                glColor3f(0.5, 0.5, 0.5);
                _bspa[i]->RenderData(GL_LINE_STRIP);

                glPointSize(10.0);
                _bspa[i]->RenderData(GL_POINTS);
            }

            if (_img_bspa[i])
            {
                glColor3f(1.0, 0.0, 0.0);
                _img_bspa[i]->RenderDerivatives(0, GL_LINE_STRIP);

                glColor3f(0.0, 0.5, 0.0);
                _img_bspa[i]->RenderDerivatives(1, GL_LINES);
            }
        }
        glColor3f(1.0, 1.0, 1.0);
    }

    DCoordinate3 GLWidget::getCylinderPoint(GLuint i, GLuint j, GLuint n, GLuint m, GLuint r, GLfloat a, GLfloat b)
    {
        GLfloat u, v;
        u = a + i*((b-a)/(n+1));
        v = (2*j*PI)/(m+1);

        GLfloat x,y,z;
        x = r*cos(v);
        y = r*sin(v);
        z = u;

        DCoordinate3 cp;
        cp[0] = x;
        cp[1] = y;
        cp[2] = z;

        return cp;
    }

    DCoordinate3 GLWidget::getTorusPoint(GLuint i, GLuint j, GLuint n, GLuint m, GLfloat r, GLfloat R)
    {
        GLfloat u, v;
        u = (2*i*PI)/(n+1);
        v = (2*j*PI)/(m+1);

        GLfloat x,y,z;
        x = (R + r*sin(u))*cos(v);
        y = (R + r*sin(u))*sin(v);
        z = r*cos(u);

        DCoordinate3 cp;
        cp[0] = x;
        cp[1] = y;
        cp[2] = z;

        return cp;
    }

    void GLWidget::init_patch()
    {
        cGridn=10;
        cGridm=12;
        nPatchn=10;
        nPatchm=12;

        GLuint n = cGridn;
        GLuint m = cGridm;

        Matrix<DCoordinate3> cp;
        cp.ResizeRows(n);
        cp.ResizeColumns(m);
        Matrix<DCoordinate3> cp2;
        cp2.ResizeRows(n);
        cp2.ResizeColumns(m);

        for (GLuint i = 0; i < n; ++i)
            for (GLuint j = 0; j < m; ++j){
                cp(i,j) = getTorusPoint(i,j,n-1,m-1);
                cp2(i,j) = getCylinderPoint(i,j,n-1,m-1);
            }


        _patch_toroid.ResizeRows(n);
        _patch_toroid.ResizeColumns(m);
        _patch_cylindric.ResizeRows(n);
        _patch_cylindric.ResizeColumns(m);

        for (GLuint i = 0; i < n; ++i)
            for (GLuint j = 0; j < m; ++j){
                _patch_toroid(i,j) = new BicubicBSplinePatch();
                _patch_cylindric(i,j) = new BicubicBSplinePatch();
            }

        for (GLuint pi = 0; pi < n; ++pi)
            for (GLuint pj = 0; pj < m; ++pj)
                for (GLuint i = 0; i < 4; ++i)
                    for (GLuint j = 0; j < 4; ++j) {
                        _patch_toroid(pi,pj)->SetData(i,j,cp((pi+i)%n,(pj+j)%m)[0],cp((pi+i)%n,(pj+j)%m)[1],cp((pi+i)%n,(pj+j)%m)[2]);
                        _patch_cylindric(pi,pj)->SetData(i,j,cp2((pi+i)%n,(pj+j)%m)[0],cp2((pi+i)%n,(pj+j)%m)[1],cp2((pi+i)%n,(pj+j)%m)[2]);
                    }

        bi_toroid.ResizeRows(n);
        bi_toroid.ResizeColumns(m);

        bi_cylindric.ResizeRows(n);
        bi_cylindric.ResizeColumns(m);

        for (GLuint pi = 0; pi < n; ++pi)
            for (GLuint pj = 0; pj < m; ++pj) {
                _patch_toroid(pi,pj)->UpdateVertexBufferObjectsOfData();
                _patch_cylindric(pi,pj)->UpdateVertexBufferObjectsOfData();
                bi_toroid(pi,pj) = _patch_toroid(pi,pj)->GenerateImage(30,30,GL_STATIC_DRAW);
                bi_cylindric(pi,pj) = _patch_cylindric(pi,pj)->GenerateImage(30,30,GL_STATIC_DRAW);

                if (bi_toroid(pi,pj))
                    bi_toroid(pi,pj)->UpdateVertexBufferObjects();
                if (bi_cylindric(pi,pj))
                    bi_cylindric(pi,pj)->UpdateVertexBufferObjects();
            }

        _patch.SetData(0, 0, -2.0, -2.0, 0.0);
        _patch.SetData(0, 1, -2.0, -1.0, 0.0);
        _patch.SetData(0, 2, -2.0, 1.0, 0.0);
        _patch.SetData(0, 3, -2.0, 2.0, 0.0);

        _patch.SetData(1, 0, -1.0, -2.0, 0.0);
        _patch.SetData(1, 1, -1.0, -1.0, 2.0);
        _patch.SetData(1, 2, -1.0, 1.0, 2.0);
        _patch.SetData(1, 3, -1.0, 2.0, 0.0);

        _patch.SetData(2, 0, 1.0, -2.0, 0.0);
        _patch.SetData(2, 1, 1.0, -1.0, 2.0);
        _patch.SetData(2, 2, 1.0, 1.0, 2.0);
        _patch.SetData(2, 3, 1.0, 2.0, 0.0);

        _patch.SetData(3, 0, 2.0, -2.0, 0.0);
        _patch.SetData(3, 1, 2.0, -1.0, 0.0);
        _patch.SetData(3, 2, 2.0, 1.0, 0.0);
        _patch.SetData(3, 3, 2.0, 2.0, 0.0);

        _patch.UpdateVertexBufferObjectsOfData();


        _before_interpolation = _patch.GenerateImage(30,30,GL_STATIC_DRAW);

        if (_before_interpolation)
            _before_interpolation->UpdateVertexBufferObjects();

        RowMatrix<GLdouble> u_knot_vector(4);
        u_knot_vector(0) = 0.0;
        u_knot_vector(1) = 1.0 / 3.0;
        u_knot_vector(2) = 2.0 / 3.0;
        u_knot_vector(3) = 1.0;

        ColumnMatrix<GLdouble> v_knot_vector(4);
        v_knot_vector(0) = 0.0;
        v_knot_vector(1) = 1.0 / 3.0;
        v_knot_vector(2) = 2.0 / 3.0;
        v_knot_vector(3) = 1.0;



        Matrix<DCoordinate3> data_points_to_interpolate(4,4);
        for (GLuint row=0; row<4; ++row)
            for (GLuint column=0; column<4; ++column)
                _patch.GetData(row,column,data_points_to_interpolate(row,column));

        /*/////////////////////////////////////////////
        _uLine_num = 7;
        _vLine_num = 12;
        GLuint divpoints = 200;
        _uLines = _patch.GenerateUIsoparametricLines(_uLine_num,1,divpoints);
        _vLines = _patch.GenerateVIsoparametricLines(_vLine_num,1,divpoints);
        /////////////////////////////////////////////*/

        if(_patch.UpdateDataForInterpolation(u_knot_vector,v_knot_vector,data_points_to_interpolate))
        {
            _after_interpolation = _patch.GenerateImage(30,30,GL_STATIC_DRAW);

            if (_after_interpolation)
                _after_interpolation->UpdateVertexBufferObjects();
        }

        /*
        _uLine_num = 7;
        _vLine_num = 12;
        GLuint divpoints = 200;
        _uLines = _patch.GenerateUIsoparametricLines(_uLine_num,1,divpoints);
        _vLines = _patch.GenerateVIsoparametricLines(_vLine_num,1,divpoints);
        */

    }

    void GLWidget::render_patch(){

        glEnable(GL_LIGHTING);
        glEnable(GL_NORMALIZE);
        glEnable(GL_LIGHT0);

        GLuint n = cGridn;
        GLuint m = cGridm;

        switch (_patch_index) {
        case 0:
            _patch.RenderData(GL_LINE_STRIP);
            _shader.Enable();
            MatFBRuby.Apply();
            if (_before_interpolation)
                _before_interpolation->Render();
            _shader.Disable();
            if (_after_interpolation)
            {
                glEnable(GL_BLEND);

                glDepthMask(GL_FALSE);
                glBlendFunc(GL_SRC_ALPHA,GL_ONE);
                MatFBTurquoise.Apply();
                _after_interpolation->Render();
                glDepthMask(GL_TRUE);
                glDisable(GL_BLEND);

            }
            /*
            // ulines
            for (GLuint i = 0; i < _uLine_num; i++) {
                (*_uLines)[i]->UpdateVertexBufferObjects();
                glColor3f(1.0, 0.0, 0.0);
                (*_uLines)[i]->RenderDerivatives(0, GL_LINE_STRIP);
            }
            // vlines
            for (GLuint i = 0; i < _vLine_num; i++) {
                (*_vLines)[i]->UpdateVertexBufferObjects();
                glColor3f(0.0, 0.0, 1.0);
                (*_vLines)[i]->RenderDerivatives(0, GL_LINE_STRIP);
            }
            */
            break;
        case 1:
            if (bi_toroid(0,0))
            {
                _shader.Enable();
                MatFBRuby.Apply();


                for (GLuint pi = 0; pi < n; ++pi)
                    for (GLuint pj = 0; pj < m; ++pj) {
                        bi_toroid(pi,pj)->Render();
                    }
                _shader.Disable();
            }
            glDisable(GL_LIGHTING);
            glDisable(GL_NORMALIZE);
            glDisable(GL_LIGHT0);
            for (GLuint pi = 0; pi < n; ++pi)
                for (GLuint pj = 0; pj < m; ++pj) {
                    _patch_toroid(pi,pj)->RenderData(GL_LINE_STRIP);
                }
            break;
        case 2:

            if (bi_cylindric(0,0))
            {
                _shader.Enable();
                MatFBRuby.Apply();


                for (GLuint pi = 0; pi < n; ++pi)
                    for (GLuint pj = 0; pj < m; ++pj) {
                        bi_cylindric(pi,pj)->Render();
                    }
                _shader.Disable();
            }
            glDisable(GL_LIGHTING);
            glDisable(GL_NORMALIZE);
            glDisable(GL_LIGHT0);
            for (GLuint pi = 0; pi < n; ++pi)
                for (GLuint pj = 0; pj < m; ++pj) {
                    _patch_cylindric(pi,pj)->RenderData(GL_LINE_STRIP);
                }
            break;
        case 3:
            glDisable(GL_LIGHTING);
            glDisable(GL_NORMALIZE);
            glDisable(GL_LIGHT0);
            _shader.Disable();
            render_bspline_arc();
            break;
        default:
            break;
        }
    }

    void GLWidget::start_animate()
    {
        connect(_timer, SIGNAL(timeout()), this, SLOT(_animate()));
    }

    void GLWidget::stop_animate()
    {
        disconnect(_timer, SIGNAL(timeout()), this, SLOT(_animate()));
    }
}
