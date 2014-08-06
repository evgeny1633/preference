//
// blocking_tcp_echo_server.cpp
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
// const int max_buffer_length = 1024;

void session(tcp::socket sock, std::string &message);
void server(boost::asio::io_service& io_service, unsigned short port);
void iffunction(tcp::socket& sock, std::string &input_message);
// void iffunction(tcp::socket sock, std::string &input_message);
int get_int_client_id(std::string message);
std::string get_client_id(std::string message);
std::string get_head(std::string message);


int main(int argc, char* argv[])
{
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: ./_server <port>\n";
      return 1;
    }

    boost::asio::io_service io_service;
    //start the server
    server(io_service, std::atoi(argv[1]));
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}

void session(tcp::socket sock, std::string &message)
{
  std::stringstream ss;
  ss.str("");
  try
  {
    for (;;)
    {
      char data[max_buffer_length] = {};
      boost::system::error_code error;
      //read message
      size_t length = sock.read_some(boost::asio::buffer(data), error);
      if (error == boost::asio::error::eof)
         break; // Connection closed cleanly by peer.
      else if (error)
         throw boost::system::system_error(error); // Some other error.

      std::thread(cin_sender, std::ref(sock)).detach();
//       cin_sender(std::ref(sock)); 
      ss.str("");
      ss << "The original message was \"" << data << "\"";
      std::cout << ss.str().c_str() << std::endl;
      message = data;
      std::strcpy (data, ss.str().c_str());
      std::cout << "__LINE__ = " << __LINE__ << std::endl;
      iffunction(std::ref(sock), message);
//       iffunction(std::move(sock), message);
      std::cout << "__LINE__ = " << __LINE__ << std::endl;
      boost::asio::write(sock, boost::asio::buffer(data, max_buffer_length));    //send message back to client
      std::cout << "__LINE__ = " << __LINE__ << std::endl;
      boost::asio::write(sock, boost::asio::buffer(data, max_buffer_length));    //send message back to client
      boost::asio::write(sock, boost::asio::buffer(data, max_buffer_length));    //send message back to client
      boost::asio::write(sock, boost::asio::buffer(data, max_buffer_length));    //send message back to client
      std::cout << "__LINE__ = " << __LINE__ << std::endl;
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception in thread: " << e.what() << "\n";
  }
}

void server(boost::asio::io_service& io_service, unsigned short port)
{
  std::cout << "++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
  std::cout << "Server is started " << std::endl;
  std::cout << "++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
  std::string message;
  tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), port));
  for (;;)
  {
    //open socket
    tcp::socket sock(io_service);
    //wait for message
    acceptor.accept(sock);
    //somebody connected; read what they sent to us
    std::thread(session, std::move(sock), std::ref(message)).detach();
    std::cout << "message -> " << message << std::endl;
    usleep(100);  //there is mistake. message here is still empty, because thread doesn't finish by that time. delay added; to be fixed. 
//     iffunction(message);
//     usleep(10);
    std::cout << "2message -> " << message << std::endl;
  }
}

// void iffunction(char input_message)
void iffunction(tcp::socket& sock, std::string &input_message)
{
// //    std::string message = &input_message;
   std::string message = input_message;
   char data[max_buffer_length] = {};
//    std::cout << "message = " << message << std::endl;
//    std::cout << "input_message = " << input_message << std::endl;
   std::string client_id = get_client_id(message);
   std::string head      = get_head(message);
   std::cout << "client_id = " << client_id << std::endl;
   std::cout << "head = " << head << std::endl;
//    data = input_message.c_str();
//    std::strcpy (data, input_message.c_str());
   
   boost::asio::write(sock, boost::asio::buffer(std::strcpy (data, input_message.c_str()), max_buffer_length));    //send message back to client
   std::cout << "__LINE__ = " << __LINE__ << std::endl;
   boost::asio::write(sock, boost::asio::buffer(data, max_buffer_length));    //send message back to client
   std::cout << "__LINE__ = " << __LINE__ << std::endl;
   
   std::vector<std::string> block;
   
//    input_message = head;
   for ( int i = 0; i < 2; i++ )   //10 -- there should be some sane variable, we'll think about this...this is number of blocks
   {
//       block.push_back(message.substr(i * _BLOCK_SIZE_, (i + 1) * _BLOCK_SIZE_));
      block.push_back(" ");
//       block[i] = block[i].substr(0, block[i].find_first_of(_EMPTY_SYMBOL_));
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
//    std::cout << "__LINE__ = " << __LINE__ << std::endl;


}

std::string get_client_id(std::string message)
{
   std::string client_id = message.substr(0, _ID_LENGTH_);
   client_id = message.substr(0, std::min(_ID_LENGTH_, (int)(client_id.find_first_of(_EMPTY_SYMBOL_))));
   return client_id;  
}

int get_int_client_id(std::string message)
{
/*   int int_client_id;
   std::string client_id = message.substr(0, _ID_LENGTH_);
   client_id = message.substr(0, std::min(_ID_LENGTH_, (int)(client_id.find_first_of(_EMPTY_SYMBOL_))));
   int_client_id = std::atoi(client_id.c_str());
   return int_client_id;  */
   return std::atoi(get_client_id(message).c_str());  //atoi transfers c-string (char) into int. (atof-into float)
}

std::string get_head(std::string message)
{
//    std::cout << "__LINE__ = " << __LINE__ << std::endl;
//    std::cout << "_ID_LENGTH_ = " << _ID_LENGTH_ << std::endl;
//    std::cout << "_HEAD_LENGTH_ = " << _HEAD_LENGTH_ << std::endl;
   std::string head = message.substr(_ID_LENGTH_, _HEAD_LENGTH_);
//    std::string head = message;
//    std::cout << "message = " << message << std::endl;
//    std::cout << "1head = " << head << std::endl;
//    std::cout << "head.length() = " << head.length() << std::endl;
//    std::cout << "_ID_LENGTH_+ _HEAD_LENGTH_ = " << _ID_LENGTH_+ _HEAD_LENGTH_ << std::endl;
//    std::cout << "(int)(head.find_first_of(_EMPTY_SYMBOL_)) = " << (int)(head.find_first_of(_EMPTY_SYMBOL_)) << std::endl;
//    std::cout << "(head.find_first_of(_EMPTY_SYMBOL_)) = "      << (head.find_first_of(_EMPTY_SYMBOL_)) << std::endl;
   head = head.substr(0, std::min(_ID_LENGTH_+ _HEAD_LENGTH_, (int)(head.find_first_of(_EMPTY_SYMBOL_))));
//    std::cout << "func head = " << head << std::endl;
   return head;  
}








