//
// blocking_tcp_echo_client.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "../include/include.h"
#include "../include/cin_sender.h"

using boost::asio::ip::tcp;

std::string make_head(std::string head);
std::string make_client_id(int int_client_id);

//enum { max_buffer_length = 1024 };
// const int max_buffer_length = 1024;

int main(int argc, char* argv[])
{
   int int_client_id;
   std::stringstream stringstream;
   std::string head; 
   std::string body;
   std::string client_id;
//    stringstream << "00" << int_client_id;
//    client_id = stringstream.str();
   
   int_client_id = 98;
   head = "alive";
   head = make_head(head);
   client_id = make_client_id(int_client_id);
   std::cout << "client_id = " << client_id << "\tint_client_id = " << int_client_id << std::endl;
//    std::cout << "head = \"" << head << "\"" << std::endl;
//    return 0;
   
   try
   {
      if (argc != 3)
      {
         std::cerr << "Usage: ./_client <host> <port>\n";
         return 1;
      }

      boost::asio::io_service io_service;

      tcp::socket socket(io_service);
      tcp::resolver resolver(io_service);
      boost::asio::connect(socket, resolver.resolve({argv[1], argv[2]}));

//       std::cout << "Enter message: ";
      char request[max_buffer_length];
//       std::cin.getline(request, max_buffer_length);
//      std::strcpy(request, "++++++++++");
      stringstream.str("");
      stringstream << client_id << head;
//       request = stringstream.str().c_str();
//       std::cout << "stringstream.str() = " << stringstream.str() << std::endl;
//       std::cout << "stringstream.str().c_str() = " << stringstream.str().c_str() << std::endl;
      stringstream >> request;
      std::cout << "request = \""<< request << "\"" << std::endl;
      size_t request_length = std::strlen(request);
      boost::asio::write(socket, boost::asio::buffer(request, request_length));

      request_length = max_buffer_length;
      std::thread(cin_sender, std::ref(socket)).detach(); 

      char reply[max_buffer_length];
      while(true)
      {
         size_t reply_length = boost::asio::read(socket,boost::asio::buffer(reply, request_length));
         std::cout << "Reply is: " << reply << std::endl;
      }
   }
   catch (std::exception& e)
   {
      std::cerr << "Exception: " << e.what() << "\n";
   }

   return 0;
}


std::string make_head(std::string head)
{
   head.resize (_HEAD_LENGTH_,_EMPTY_SYMBOL_);
//    std::cout << "head = \"" << head << "\"" << std::endl;
   return head;
}

std::string make_client_id(int int_client_id)
{
   std::stringstream ss;
   ss << int_client_id;
   std::string client_id = ss.str();
   client_id.resize(_ID_LENGTH_, _EMPTY_SYMBOL_);
//    std::cout << "client_id = \"" << client_id << "\"" << std::endl;
   return client_id;
}

std::string make_message(int int_client_id, std::string head)
{
   std::stringstream ss;
   std::string client_id = make_client_id(int_client_id);
   ss << client_id;
   for (int i = 0; i < _ID_LENGTH_ - (int)client_id.length(); i++)
      ss << _EMPTY_SYMBOL_;
   head = make_head(head);
   ss << head;
   for (int i = 0; i < _HEAD_LENGTH_ - (int)head.length(); i++)
      ss << _EMPTY_SYMBOL_;
   
   return ss.str();
}
