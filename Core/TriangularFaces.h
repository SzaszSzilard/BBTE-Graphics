#pragma once

#include <GL/glew.h>
#include <iostream>
#include <vector>

namespace cagd
{
    class TriangularFace
    {
    protected:
        GLuint _node[3];

    public:
        // default constructor
        TriangularFace();

        // homework: copy constructor
        TriangularFace(const TriangularFace& face);

        // homework: assignment operator
        TriangularFace& operator =(const TriangularFace& rhs);

        // homework: get node identifiers by value
        GLuint operator [](GLuint i) const;

        // homework: get node identifiers by reference
        GLuint& operator [](GLuint i);
    };

    // default constructor
    inline TriangularFace::TriangularFace()
    {
        _node[0] = _node[1] = _node[2] = 0;
    }

    // output to stream
    inline std::ostream& operator <<(std::ostream& lhs, const TriangularFace& rhs)
    {
        lhs << 3;
        for (GLuint i = 0; i < 3; ++i)
            lhs  << " " << rhs[i];
        return lhs;
    }

    // homework
    inline std::istream& operator >>(std::istream& lhs, TriangularFace& rhs);
}
