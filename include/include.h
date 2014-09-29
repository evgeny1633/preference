
#ifndef _INCLUDE_FILE_
#define _INCLUDE_FILE_

#define __std11__
#undef __std11__

#define __QT__
#undef __QT__

#include <cstdlib>
#include <iostream>

#include <utility>
#include <sstream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <algorithm>    /* std::random_shuffle */
#include <cstring>
#include <unistd.h>

#ifdef __std11__
#include <thread>
#include <mutex>          // std::mutex
#else
#include <boost/move/move.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#endif

#include <boost/asio.hpp>
#include <boost/ref.hpp>

#ifdef __QT__
#ifndef _CLIENT_
#include <QThread>
#include <QWidget>
#include <QApplication>
#include <QTextEdit>
#include <QObject>

// #include "widget.h"
// #include "updater.h"
// #include "log.h"
#endif
#endif

//message parts:
#define _BLOCK_LENGTH_ 16
#define _HEAD_LENGTH_ _BLOCK_LENGTH_  // 16
#define _ID_LENGTH_ _BLOCK_LENGTH_    // 4
#define _EMPTY_SYMBOL_ '_'

const int max_buffer_length = 1024;
// #include "common_functions.cpp"

#endif
