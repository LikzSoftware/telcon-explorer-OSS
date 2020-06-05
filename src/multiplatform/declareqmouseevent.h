/// this header file enables compilation on qt4.8 and qt5 (removes a warning)
#ifndef DECLAREQMOUSEEVENT_H
#define DECLAREQMOUSEEVENT_H

#include <QtGlobal>
#if QT_VERSION >= 0x050000
    class QMouseEvent;
#else
    struct QMouseEvent;
#endif

#endif // DECLAREQMOUSEEVENT_H

