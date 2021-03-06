#include "MainWindow.h"

namespace cagd
{
    MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
    {
        setupUi(this);

    /*

      the structure of the main window's central widget

     *---------------------------------------------------*
     |                 central widget                    |
     |                                                   |
     |  *---------------------------*-----------------*  |
     |  |     rendering context     |   scroll area   |  |
     |  |       OpenGL widget       | *-------------* |  |
     |  |                           | | side widget | |  |
     |  |                           | |             | |  |
     |  |                           | |             | |  |
     |  |                           | *-------------* |  |
     |  *---------------------------*-----------------*  |
     |                                                   |
     *---------------------------------------------------*

    */
        _side_widget = new SideWidget(this);

        _scroll_area = new QScrollArea(this);
        _scroll_area->setWidget(_side_widget);
        _scroll_area->setSizePolicy(_side_widget->sizePolicy());
        _scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

        _gl_widget = new GLWidget(this);

        centralWidget()->setLayout(new QHBoxLayout());
        centralWidget()->layout()->addWidget(_gl_widget);
        centralWidget()->layout()->addWidget(_scroll_area);

        // creating a signal slot mechanism between the rendering context and the side widget
        connect(_side_widget->rotate_x_slider, SIGNAL(valueChanged(int)), _gl_widget, SLOT(set_angle_x(int)));
        connect(_side_widget->rotate_y_slider, SIGNAL(valueChanged(int)), _gl_widget, SLOT(set_angle_y(int)));
        connect(_side_widget->rotate_z_slider, SIGNAL(valueChanged(int)), _gl_widget, SLOT(set_angle_z(int)));

        connect(_side_widget->zoom_factor_spin_box, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_zoom_factor(double)));

        connect(_side_widget->trans_x_spin_box, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_trans_x(double)));
        connect(_side_widget->trans_y_spin_box, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_trans_y(double)));
        connect(_side_widget->trans_z_spin_box, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_trans_z(double)));

        _side_widget->toolBox->setCurrentIndex(5);
        // parametric curves
        connect(_side_widget->pc_combo, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(set_parametric_curve_index(int)));

        // cyclic curves
        connect(_side_widget->cc_combo, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(set_cyclic_curve_index(int)));

        // parametric surfaces
        connect(_side_widget->ps_combo, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(set_parametric_surface_index(int)));
        // models
        connect(_side_widget->mo_combo, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(set_models_index(int)));
        connect(_side_widget->start_animate,SIGNAL(pressed()), _gl_widget, SLOT(start_animate()));
        connect(_side_widget->stop_animate,SIGNAL(pressed()), _gl_widget, SLOT(stop_animate()));

        // shaders
        connect(_side_widget->SpinBoxScale,SIGNAL(valueChanged(double)),_gl_widget, SLOT(set_shader_scale_factor(double)));
        connect(_side_widget->SpinBoxSmooth,SIGNAL(valueChanged(double)),_gl_widget, SLOT(set_shader_smoothing(double)));
        connect(_side_widget->SpinBoxShade,SIGNAL(valueChanged(double)),_gl_widget, SLOT(set_shader_shading(double)));
        connect(_side_widget->sh_combo, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(set_shader_index(int)));

        // patches
        connect(_side_widget->patch_comb,SIGNAL(currentIndexChanged(int)),_gl_widget,SLOT(set_patch_index(int)));

        connect(_side_widget->patch_i,SIGNAL(valueChanged(double)),_gl_widget, SLOT(set_patch_i(double)));
        connect(_side_widget->patch_j,SIGNAL(valueChanged(double)),_gl_widget, SLOT(set_patch_j(double)));
        connect(_side_widget->dcoord_i,SIGNAL(valueChanged(double)),_gl_widget, SLOT(set_dcoord_i(double)));
        connect(_side_widget->dcoord_j,SIGNAL(valueChanged(double)),_gl_widget, SLOT(set_dcoord_j(double)));
        connect(_side_widget->modify_x,SIGNAL(valueChanged(double)),_gl_widget, SLOT(set_modify_x(double)));
        connect(_side_widget->modify_y,SIGNAL(valueChanged(double)),_gl_widget, SLOT(set_modify_y(double)));
        connect(_side_widget->modify_z,SIGNAL(valueChanged(double)),_gl_widget, SLOT(set_modify_z(double)));

        connect(_side_widget->load,SIGNAL(pressed()),_gl_widget,SLOT(callload()));
        connect(_side_widget->save,SIGNAL(pressed()),_gl_widget,SLOT(callsave()));
    }

    //--------------------------------
    // implementation of private slots
    //--------------------------------
    void MainWindow::on_action_Quit_triggered()
    {
        qApp->exit(0);
    }
}
