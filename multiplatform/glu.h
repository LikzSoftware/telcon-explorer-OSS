/// this header file enables compilation on linux and mac
#ifndef MP_GLU_H
#define MP_GLU_H

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#endif // MP_GLU_H

