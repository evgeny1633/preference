
//this script allows to send messages directly from terminal

#ifndef _CIN_SENDER_
#define _CIN_SENDER_

#include "include.h"

void cin_sender(boost::asio::ip::tcp::socket& sock)
{
  std::string message;
//   std::stringstream ss;
//   ss.str("");
  try
  {
    for (;;)
    {
      char data[max_buffer_length] = {};

      std::cout << "Feel free to enter message here...Format is: client_id" << _EMPTY_SYMBOL_ << "head" << _EMPTY_SYMBOL_ << "anything else. _EMPTY_SYMBOL_ = " << _EMPTY_SYMBOL_ << " ; _ID_LENGTH_ = " << _ID_LENGTH_ << " ; _HEAD_LENGTH_ = " << _HEAD_LENGTH_ << std::endl;
      
      getline ( std::cin, message );
      std::cin.clear();
      std::strcpy (data, message.c_str());
      std::cout << "__LINE__ = " << __LINE__ << std::endl;
      boost::asio::write(sock, boost::asio::buffer(data, max_buffer_length));    //send message back to client
      std::cout << "Message succesfully sent ! " << std::endl;
      std::cout << "__LINE__ = " << __LINE__ << std::endl;
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception in thread: " << e.what() << "\n";
  }
}
#endif