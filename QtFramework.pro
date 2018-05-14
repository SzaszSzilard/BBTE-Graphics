QT += core gui widgets opengl
CONFIG += console

win32 {
    message("Windows platform...")

    INCLUDEPATH += $$PWD/Dependencies/Include
    DEPENDPATH += $$PWD/Dependencies/Include

    LIBS += -lopengl32 -lglu32

    CONFIG(release, debug|release): {
        contains(QT_ARCH, i386) {
            message("x86 (i.e., 32-bit) release build")
            LIBS += -L"$$PWD/Dependencies/Lib/GL/x86/" -lglew32
        } else {
            message("x86_64 (i.e., 64-bit) release build")
            LIBS += -L"$$PWD/Dependencies/Lib/GL/x86_64/" -lglew32
        }
    } else: CONFIG(debug, debug|release): {
        contains(QT_ARCH, i386) {
            message("x86 (i.e., 32-bit) debug build")
            LIBS += -L"$$PWD/Dependencies/Lib/GL/x86/" -lglew32
        } else {
            message("x86_64 (i.e., 64-bit) debug build")
            LIBS += -L"$$PWD/Dependencies/Lib/GL/x86_64" -lglew32
        }
    }

    msvc {
      QMAKE_CXXFLAGS += -openmp -arch:AVX -D "_CRT_SECURE_NO_WARNINGS"
      QMAKE_CXXFLAGS_RELEASE *= -O2
    }
}

mac {
    # for GLEW installed into /usr/lib/libGLEW.so or /usr/lib/glew.lib
    LIBS += -lGLEW -lGLU
}

unix {
    # for GLEW installed into /usr/lib/libGLEW.so or /usr/lib/glew.lib
    LIBS += -lGLEW -lGLU
}

HEADERS += \
    Core/Constants.h \
    Core/DCoordinates3.h \
    Core/Exceptions.h \
    Core/GenericCurves3.h \
    Core/LinearCombination3.h \
    Core/Matrices.h \
    Core/RealSquareMatrices.h \
    Cyclic/CyclicCurve3.h \
    Dependencies/Include/GL/glew.h \
    GUI/GLWidget.h \
    GUI/MainWindow.h \
    GUI/SideWidget.h \
    Parametric/ParametricCurves3.h \
    Test/TestFunctions.h \
    Parametric/ParametricSurfaces3.h \
    Core/HCoordinates3.h \
    Core/TCoordinates4.h \
    Core/TriangularFaces.h \
    Core/TriangulatedMeshes3.h \
    Core/Colors4.h \
    Core/Lights.h \
    Core/Materials.h \
    Core/TensorProductSurfaces3.h \
    Core/ShaderPrograms.h \
    Core/Colors4.h \
    Core/Constants.h \
    Core/DCoordinates3.h \
    Core/Exceptions.h \
    Core/GenericCurves3.h \
    Core/HCoordinates3.h \
    Core/Lights.h \
    Core/LinearCombination3.h \
    Core/Materials.h \
    Core/Matrices.h \
    Core/RealSquareMatrices.h \
    Core/ShaderPrograms.h \
    Core/TCoordinates4.h \
    Core/TensorProductSurfaces3.h \
    Core/TriangularFaces.h \
    Core/TriangulatedMeshes3.h \
    Cyclic/CyclicCurve3.h \
    Dependencies/Include/GL/glew.h \
    GUI/GLWidget.h \
    GUI/MainWindow.h \
    GUI/SideWidget.h \
    Parametric/ParametricCurves3.h \
    Parametric/ParametricSurfaces3.h \
    Test/TestFunctions.h \
    B-spline/BicubicBSplinePatch.h

SOURCES += \
    Core/GenericCurves3.cpp \
    Core/LinearCombination3.cpp \
    Core/RealSquareMatrices.cpp \
    Cyclic/CyclicCurve3.cpp \
    GUI/GLWidget.cpp \
    GUI/MainWindow.cpp \
    GUI/SideWidget.cpp \
    Parametric/ParametricCurves3.cpp \
    Test/TestFunctions.cpp \
    main.cpp \
    Parametric/ParametricSurfaces3.cpp \
    Core/TriangulatedMeshes3.cpp \
    Core/Lights.cpp \
    Core/Materials.cpp \
    Core/TensorProductSurfaces3.cpp \
    Core/ShaderPrograms.cpp \
    Core/GenericCurves3.cpp \
    Core/Lights.cpp \
    Core/LinearCombination3.cpp \
    Core/Materials.cpp \
    Core/RealSquareMatrices.cpp \
    Core/ShaderPrograms.cpp \
    Core/TensorProductSurfaces3.cpp \
    Core/TriangulatedMeshes3.cpp \
    Cyclic/CyclicCurve3.cpp \
    GUI/GLWidget.cpp \
    GUI/MainWindow.cpp \
    GUI/SideWidget.cpp \
    Parametric/ParametricCurves3.cpp \
    Parametric/ParametricSurfaces3.cpp \
    Test/TestFunctions.cpp \
    main.cpp \
    B-spline/BicubicBSplinePatch.cpp

FORMS += \
    GUI/MainWindow.ui \
    GUI/SideWidget.ui


