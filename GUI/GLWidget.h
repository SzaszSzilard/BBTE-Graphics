#pragma once

#include <GL/glew.h>
#include <QGLWidget>
#include <QGLFormat>

#include "../Parametric/ParametricCurves3.h"
#include "../Parametric/ParametricSurfaces3.h"
#include "../Cyclic/CyclicCurve3.h"


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

        // variables defining the model-view matrix
        float       _eye[3], _center[3], _up[3];

        // variables needed by transformations
        int         _angle_x, _angle_y, _angle_z;
        double      _zoom;
        double      _trans_x, _trans_y, _trans_z;

        // your other declarations
        int         _index, _cc_index,_page_index;
        int         _ps_index;

        // variables needed by parametric curves
        RowMatrix<ParametricCurve3*> _pc;
        RowMatrix<GenericCurve3*> _image_of_pc;
        GLuint _num_of_pc;

        // variables needed by parametric surfaces
        RowMatrix<ParametricSurface3*> _ps;
        RowMatrix<GenericCurve3*> _image_of_ps;
        GLuint _num_of_ps;

        GLuint _n;
        CyclicCurve3* _cc;

        GLuint _mod;
        GLuint _div;
        GenericCurve3 *_img_cc;


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
        //knim1445
        void set_cyclic_curve_index();
        void set_parametric_surface_index(int index);
        void init_parametric_curves();
        void init_cyclic_curves();
        void init_parametric_surfaces();
        void render_pc();
        void render_cc();
        void render_ps();
    };
}
