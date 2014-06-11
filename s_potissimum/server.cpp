//
// blocking_tcp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <iostream>
#include <thread>
#include <utility>
#include <sstream>
#include <boost/asio.hpp>

#define _BLOCK_SIZE_ 16
#define _EMPTY_SYMBOL_ " "

using boost::asio::ip::tcp;

const int max_length = 1024;

void session(tcp::socket sock, std::string &message)
{
  std::stringstream ss;
  ss.str("");
  try
  {
    for (;;)
    {
      char data[max_length] = {};
      boost::system::error_code error;
      size_t length = sock.read_some(boost::asio::buffer(data), error);
      if (error == boost::asio::error::eof)
         break; // Connection closed cleanly by peer.
      else if (error)
         throw boost::system::system_error(error); // Some other error.

      ss.str("");
      ss << "The original message was \"" << data << "\"";
      std::cout << ss.str().c_str() << std::endl;
      std::strcpy (data, ss.str().c_str());
      message = data;
      std::cout << "m ~ " << message << std::endl;
      boost::asio::write(sock, boost::asio::buffer(data, max_length));
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception in thread: " << e.what() << "\n";
  }
}

void server(boost::asio::io_service& io_service, unsigned short port)
{
  std::string message;
  tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), port));
  for (;;)
  {
    tcp::socket sock(io_service);
    a.accept(sock);
    std::thread(session, std::move(sock), std::ref(message)).detach();
    std::cout << "message -> " << message << std::endl;
  }
}

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: blocking_tcp_echo_server <port>\n";
      return 1;
    }

    boost::asio::io_service io_service;

    server(io_service, std::atoi(argv[1]));
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}


void iffunction(char input_message)
{

   std::string message = &input_message;
   std::vector<std::string> block;
   for ( int i = 0; i < 10; i++ )   //10 -- there should be some sane variable, we'll think about this...
   {
      block.push_back(message.substr(i * _BLOCK_SIZE_, (i + 1) * _BLOCK_SIZE_));
      block[i] = block[i].substr(0, block[i].find_first_of(_EMPTY_SYMBOL_));

   }


   if ( block[1] == "alive" )
   {
   //send him back alive signal
   //send others that hi is connected
   }

   if ( block[1] == "statistics" )
   {
   //send him statistics
   }

   if ( block[1] == "trade" )
   {


   }

   if ( block[1] == "game" )
   {


   }

   if ( block[1] == "offer" )
   {


   }

   if ( block[1] == "chat" )
   {
   //then block[2] is playername
   //send a message to everyone

   }

   if ( block[1] == "service" )
   {
   //whatever...
   }


}

