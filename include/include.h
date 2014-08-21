
#ifndef _INCLUDE_FILE_

#define _INCLUDE_FILE_

#include <cstdlib>
#include <iostream>
#include <thread>
#include <utility>
#include <sstream>
#include <boost/asio.hpp>

#include <cstring>

#define _BLOCK_SIZE_ 16
#define _HEAD_LENGTH_ 9
#define _ID_LENGTH_ 4
#define _EMPTY_SYMBOL_ '_'


const int max_buffer_length = 1024;

void session(boost::asio::ip::tcp::socket sock, std::string &message);
void server(boost::asio::io_service& io_service, unsigned short port);
void iffunction(boost::asio::ip::tcp::socket& sock, std::string &input_message);
// void iffunction(boost::asio::ip::tcp::socket sock, std::string &input_message);
int get_int_client_id(std::string message);
std::string get_client_id(std::string message);
std::string get_head(std::string message);

#endif
