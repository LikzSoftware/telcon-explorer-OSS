/// this header file enables compilation on linux and mac
#ifndef MP_GL_H
#define MP_GL_H

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#endif // MP_GL_H

