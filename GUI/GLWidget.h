#pragma once

#include <GL/glew.h>
#include <QGLWidget>
#include <QGLFormat>

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
        int         _index;


    public:
        // special and default constructor
        // the format specifies the properties of the rendering window
        GLWidget(QWidget* parent = 0, const QGLFormat& format = QGL::Rgba | QGL::DepthBuffer | QGL::DoubleBuffer);

        // redeclared virtual functions
        void initializeGL();
        void paintGL();
        void resizeGL(int w, int h);

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
    };
}
