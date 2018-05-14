#pragma once

#include <GL/glew.h>
#include <QGLWidget>
#include <QGLFormat>
#include <QTimer>

#include "../Parametric/ParametricCurves3.h"
#include "../Parametric/ParametricSurfaces3.h"
#include "../Cyclic/CyclicCurve3.h"
#include "../Core/ShaderPrograms.h"


namespace cagd
{
    class GLWidget: public QGLWidget
    {
        Q_OBJECT

    private:

        // variables defining the projection matrix
        float       _aspect;            // aspect ratio of the rendering window
        float       _fovy;              // field of view in direction y
        float       _z_near, _z_far;    // distance of near and far clipping planes

        // variables defining the model-view matrix;
        float       _eye[3], _center[3], _up[3];

        // variables needed by transformations;
        int         _angle_x, _angle_y, _angle_z;
        double      _zoom;
        double      _trans_x, _trans_y, _trans_z;

        // your other declarations;
        int         _index, _cc_index,_page_index;
        int         _ps_index, _mo_index, _shader_index;

        // variables needed by parametric curves;
        RowMatrix<ParametricCurve3*> _pc;
        RowMatrix<GenericCurve3*> _image_of_pc;
        GLuint _num_of_pc;

        // variables needed by parametric surfaces;
        RowMatrix<ParametricSurface3*> _ps;
        RowMatrix<TriangulatedMesh3*> _image_of_ps;
        GLuint _num_of_ps;

        // CyclicCurve variables;
        GLuint _n;              // num of cyclic curve poits
        CyclicCurve3* _cc;
        GenericCurve3 *_img_cc;

        // varibles needed by models;
        RowMatrix<TriangulatedMesh3*> _image_of_mo;
        GLuint _num_of_mo;

        GLuint _mod;
        GLuint _div;

        // dynamic vertex buffers;
        QTimer* _timer;
        GLfloat _angle;

        // variables needed by shaders
        ShaderProgram _shader;
        GLfloat _scale_factor = 4.0f;
        GLfloat _smoothing = 2.0f;
        GLfloat _shading = 1.0f;

    public:
        // special and default constructor
        // the format specifies the properties of the rendering window
        GLWidget(QWidget* parent = 0, const QGLFormat& format = QGL::Rgba | QGL::DepthBuffer | QGL::DoubleBuffer);

        // redeclared virtual functions
        void initializeGL();
        void paintGL();
        void resizeGL(int w, int h);

        virtual ~GLWidget();

    public slots:
        // public event handling methods/slots
        void set_angle_x(int value);
        void set_angle_y(int value);
        void set_angle_z(int value);

        void set_zoom_factor(double value);

        void set_trans_x(double value);
        void set_trans_y(double value);
        void set_trans_z(double value);

        void set_parametric_curve_index(int index);
        void set_cyclic_curve_index();
        void set_parametric_surface_index(int index);
        void set_models_index(int index);
        void init_parametric_curves();
        void init_cyclic_curves();
        void init_parametric_surfaces();
        void init_models();
        void render_pc();
        void render_cc();
        void render_ps();
        void render_mo();

        void _animate();
        void set_shader_scale_factor(double value);
        void set_shader_smoothing(double value);
        void set_shader_shading(double value);
        void set_shader_index(int index);
        void init_shader(int index);
    };
}
