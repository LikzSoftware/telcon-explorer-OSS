/// this header file enables compilation on qt4.8 and qt5 (removes a warning)
#ifndef DECLAREQCLOSEEVENT_H
#define DECLAREQCLOSEEVENT_H

#include <QtGlobal>
#if QT_VERSION >= 0x050000
    class QCloseEvent;
#else
    struct QCloseEvent;
#endif

#endif // DECLAREQCLOSEEVENT_H

