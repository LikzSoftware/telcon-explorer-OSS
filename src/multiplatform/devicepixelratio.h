/// this header file enables compilation on qt4.8 and qt5
#ifndef DEVICEPIXELRATIO_H
#define DEVICEPIXELRATIO_H

#include <QtGlobal>
#if QT_VERSION < 0x050000
inline float devicePixelRatio() { return 1.0; }
#endif

#endif // DEVICEPIXELRATIO_H

