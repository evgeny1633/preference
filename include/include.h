
#ifndef _INCLUDE_FILE_

#define _INCLUDE_FILE_

#include <cstdlib>
#include <iostream>
#include <thread>
#include <mutex>          // std::mutex
#include <utility>
#include <sstream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <algorithm>    /* std::random_shuffle */
#include <unistd.h>

#include <boost/asio.hpp>
#include <boost/ref.hpp>

#ifndef _CLIENT_
#include <QThread>

#include <QWidget>
#include <QApplication>
#include <QTextEdit>
#include <QObject>

#include "widget.h"
#include "updater.h"
#include "log.h"
#endif

#include <cstring>

#define _BLOCK_LENGTH_ 16
#define _HEAD_LENGTH_ _BLOCK_LENGTH_  // 16
#define _ID_LENGTH_ _BLOCK_LENGTH_    // 4
#define _EMPTY_SYMBOL_ '_'


const int max_buffer_length = 1024;

#include "common_functions.cpp"




#endif
