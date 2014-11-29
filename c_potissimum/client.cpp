
#define _CLIENT_

// #include <signal.h>
#include "../include/include.h"
#include "../include/common_functions.cpp"

#define _sock_ (socket_for_read.at(0))

std::vector <boost::asio::ip::tcp::socket> socket_for_read;

std::string make_head(std::string head);
std::string make_client_id(int int_client_id);

//send message to server directly from cin
void cin_sender(boost::asio::ip::tcp::socket& sock)
{
  std::string message;
  try
  {
    for (;;)
    {
      char data[max_buffer_length] = {};

      std::cout << "Feel free to enter message here...Format is: client_id" << _EMPTY_SYMBOL_ << "head" << _EMPTY_SYMBOL_ << "anything else. _EMPTY_SYMBOL_ = " << _EMPTY_SYMBOL_ << " ; _ID_LENGTH_ = " << _ID_LENGTH_ << " ; _HEAD_LENGTH_ = " << _HEAD_LENGTH_ << std::endl;
      
      getline ( std::cin, message );
      std::cin.clear();
      std::strcpy (data, message.c_str());
      boost::asio::write(sock, boost::asio::buffer(data, max_buffer_length));    //send message back to client
      std::cout << "Message succesfully sent ! " << std::endl;
//       std::cout << "__LINE__ = " << __LINE__ << std::endl;
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception in cin thread: " << e.what() << "\n";
  }
}

//send message to server directly from cin
void chat_sender(char* host, char* port, int client_id)
{
  std::string message;
  std::stringstream ss;
  try
  {
    for (;;)
    {
      char data[max_buffer_length] = {};
      boost::asio::io_service io_service;
      boost::asio::ip::tcp::socket socket(io_service);
      boost::asio::ip::tcp::resolver resolver(io_service);
      boost::asio::connect(socket, resolver.resolve({host, port}));

      std::cout << "Feel free to enter message here..." << std::endl;
      
      getline ( std::cin, message );
      std::cin.clear();
      ss.str("");
      ss << make_client_id(client_id) << make_head("chat") << message;
      std::strcpy (data, ss.str().c_str());
      boost::asio::write(socket, boost::asio::buffer(data, max_buffer_length));    //send message back to client
      std::cout << "Message succesfully sent ! " << std::endl;
//       std::cout << "__LINE__ = " << __LINE__ << std::endl;
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception in chat_sender: " << e.what() << "\n";
  }
}
/*
void sig_handler(int sig)
{
  if (sig == SIGSEGV)
  {
    std::cout << "give out a backtrace or something...\n";
  }
  if (sig == SIGTERM)
  {
    std::cout << "terminating...\n";
  }
  else
    std::cout << "wasn't expecting that!\n";
}*/

int main(int argc, char* argv[])
{
//   signal(SIGTERM, sig_handler);
  int int_client_id;
  std::stringstream stringstream;
  std::string head; 
  std::string body;
  std::string client_id;
  std::string string;
//    stringstream << "00" << int_client_id;
//    client_id = stringstream.str();
  char* host = argv[1];
  char* port = argv[2];

  srand (time(NULL));  // initialize random seed: 
//   int_client_id = 18; 
  int_client_id = (int) (rand() % 30 + 3) ; //range 3-32
  head = "alive";
  head = make_head(head);
  client_id = make_client_id(int_client_id);
  std::cout << "client_id = " << client_id << "\tint_client_id = " << int_client_id << std::endl;
//    std::cout << "head = \"" << head << "\"" << std::endl;
  try
  {
    if (argc != 3)
    {
      std::cerr << "Usage: ./_client <host> <port>\n";
      return 1;
    }

//       std::strcpy(argv[1], "localhost");
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::socket socket(io_service);
    boost::asio::ip::tcp::resolver resolver(io_service);
    boost::asio::connect(socket, resolver.resolve({host, port}));

    char request[max_buffer_length];
    stringstream.str("");
    stringstream << client_id << head;
//       request = stringstream.str().c_str();
//       std::cout << "stringstream.str() = " << stringstream.str() << std::endl;
//       std::cout << "stringstream.str().c_str() = " << stringstream.str().c_str() << std::endl;
    stringstream >> request;
    std::cout << "request = \""<< request << "\"" << std::endl;
    boost::asio::write(socket, boost::asio::buffer(request, (size_t)std::strlen(request)));

//     std::thread(cin_sender, std::ref(socket)).detach(); 
    std::thread ([host, port, int_client_id]{chat_sender(host, port, int_client_id);}).detach(); 
    

    char reply[max_buffer_length];
    while(true)
    {
      size_t reply_length = boost::asio::read(socket,boost::asio::buffer(reply, max_buffer_length));
      try
      {
        if ( get_head( (std::string)reply ) == "chat" )
        {
          std::cout << get_chat_message((std::string)reply) << std::endl;
        }
        if ( get_head ((std::string)reply) == "alive" )
        {
          boost::asio::write(socket, boost::asio::buffer(reply, (size_t)std::strlen(reply)));
        }
        else
        {
          std::cout << "Server sent: \"" << reply << "\"" << std::endl;
        }
      }
      catch (std::exception& e)
      {
        std::cerr << "get_head exception: " << e.what() << "\n";
      }
    }
//     chat_sender_thread.join();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }
  

  return 0;
}

/*
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
}*/
