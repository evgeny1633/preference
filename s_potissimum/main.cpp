#include <iostream>
#include <sstream>
#include <unistd.h>
#include "widget.h"
#include "log.h"
#include "../include/updater.h"
#include "../include/include.h"
#include "../include/common_functions.cpp"
#include <QApplication>
#include <QThread>

#define _sock_ (clients.sockets.at(inner_number))

using boost::asio::ip::tcp;
struct client_list
{ 
  std::vector <int> ids;
  std::vector <tcp::socket> sockets;
} clients;  //make it global 

// template<class TYPE>
// void output (TYPE data, Updater &updater);
void message_sender(int inner_number = 0, std::string message = "", bool test = true);
// void message_sender(int inner_number , std::string message , bool test );
void output (std::stringstream ss, Updater &updater);
void output (std::string string, Updater &updater);
void append_text_box(QTextEdit *textBox, std::string message);
// void session(boost::asio::ip::tcp::socket sock, std::string &message, Updater &updater);
void session(int inner_number, std::string &message, Updater &updater);
void server(boost::asio::io_service &io_service, unsigned short port, Updater &updater);
void iffunction(boost::asio::ip::tcp::socket& sock, std::string &input_message);
int get_int_client_id(std::string message);
std::string get_client_id(std::string message);
std::string get_head(std::string message);





// void session(tcp::socket sock, std::string &message, Updater &updater)
// void session(client_list clients, std::string &message, Updater &updater)
void session(int inner_number, std::string &message, Updater &updater)
{
//   std::cout << "session __LINE__ = " << __LINE__ << std::endl;
  std::cout << "inner_number = " << inner_number << std::endl;
  std::cout << "clients.ids.size() = " << clients.ids.size() << std::endl;
  int client_id = clients.ids.at(inner_number);
  std::stringstream ss;
  ss.str("");
  try
  {
    for (;;) 
    {
      std::cout << "session __LINE__ = " << __LINE__ << std::endl;
      char data[max_buffer_length] = {};
      boost::system::error_code error;
      std::cout << "session __LINE__ = " << __LINE__ << std::endl;
      _sock_.read_some(boost::asio::buffer(data), error); //read message
      if (error == boost::asio::error::eof)
      {
        std::cout << "session: break is here" << std::endl;
        break; // Connection closed cleanly by peer.
      }
      else if (error)
      {
        std::cout << "session: some other error" << std::endl;
        throw boost::system::system_error(error); // Some other error.
      }
         
      ss.str("");      ss << "The original message was \"" << data << "\"";  std::cout << ss.str().c_str() << std::endl;
      message = data;

      updater.send_message_slot(QString::fromStdString(message));
//       updater.send_message_signal(QString::fromStdString(message));
      std::strcpy (data, ss.str().c_str());
      iffunction(std::ref(_sock_), message);
      boost::asio::write(_sock_, boost::asio::buffer(data, max_buffer_length));    //send message back to client
//       boost::asio::write(_sock_, boost::asio::buffer(data, max_buffer_length));    //send message back to client
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception in thread: " << e.what() << "\n";
  }
}

// void iffunction(char input_message)
void iffunction(tcp::socket& sock, std::string &input_message)
{
   std::string message = input_message;
   char data[max_buffer_length] = {};
   std::string client_id = get_client_id(message);
   std::string head      = get_head(message);
   boost::asio::write(sock, boost::asio::buffer(std::strcpy (data, input_message.c_str()), max_buffer_length));    //send message back to client
   boost::asio::write(sock, boost::asio::buffer(data, max_buffer_length));    //send message back to client
   std::cout << "iffunction __LINE__ = " << __LINE__ << std::endl;
   
   std::vector<std::string> block;
   /*
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
*/

}

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  Widget w;
  //w.show();
  Log log;
  Updater updater;
  
  QObject::connect(&updater, SIGNAL(send_message_signal(QString)), &log, SLOT(receive_message(QString)));
  
  /*
//   Log log;
//   QTextEdit *textBox = log.findChild<QTextEdit *>("textEdit");  
//   log.show();
//   for(int i = 0; i < 10; i++  )
//   {
//       ss.str("");
//       ss << i;
//       textBox->append(QString::fromStdString(ss.str()));
//   }
//   log.show();
//   sleep(2);
//   for(int i = 11; i < 20; i++)
//   {
//       ss.str("");
//       ss << i;
//       textBox->append(QString::fromStdString(ss.str()));
//       log.repaint();
// //       sleep(1);
//   } */
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: ./_server <port>\n";
      return 1;
    }
    log.show();
    boost::asio::io_service io_service;
    //start the server
/*
//     server(io_service, std::atoi(argv[1]));
// //     server(io_service, std::atoi(argv[1]), updater);
//     std::thread(server, std::ref(io_service), std::atoi(argv[1]), std::ref(updater)).detach();*/
    std::thread server_thread(server, boost::ref(io_service), std::atoi(argv[1]), std::ref(updater));//.detach();
    a.exec(); //oh god this is mandatory. nobody told me that this shit is called main thread. stupid qt, i hate it. at last this works now.
    server_thread.join();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }
  
  return 0;
}


void server(boost::asio::io_service &io_service, unsigned short port, Updater &updater)
{
//   std::cout << "server __LINE__ = " << __LINE__ << std::endl;
  std::cout << "++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
  std::cout << "Server is started " << std::endl;
  std::cout << "++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
  std::string message;
  std::stringstream ss;
  bool reconnected;
  int id;
  int inner_number;
  
  std::cout << "port = " << port << std::endl;
  tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), port));
  std::thread([]{ message_sender();}).detach();
  for (;;)
  {
    tcp::socket sock(io_service);    //open socket
    std::cout << "server __LINE__ = " << __LINE__ << std::endl;
    acceptor.accept(sock);           //wait for message
    
    char data[max_buffer_length] = {};
    boost::system::error_code error;
    std::cout << "server __LINE__ = " << __LINE__ << std::endl;
    sock.read_some(boost::asio::buffer(data), error); //read message
    /*
    if (error == boost::asio::error::eof)
        break; // Connection closed cleanly by peer.
    else if (error)
        throw boost::system::system_error(error); // Some other error.
        */
    message = data;
    if (get_head(message) == "alive")
    {
      reconnected = false;
      id = get_int_client_id(message);
      std::cout << "server __LINE__ = " << __LINE__ << std::endl;
      for (auto iterator = clients.ids.begin(); iterator != clients.ids.end(); ++iterator)
      {
        std::cout << "server __LINE__ = " << __LINE__ << std::endl;
        if (id == (*iterator))
        {
          ss.str(""); ss << "Client # " << id << " reconnected\n"; output(ss.str(), std::ref(updater));
          reconnected = true;
          inner_number = iterator - clients.ids.begin();
          clients.sockets.at(inner_number) = std::move(sock);
        }
      }
      std::cout << "server __LINE__ = " << __LINE__ << std::endl;
      if (!reconnected)
      {
        clients.ids.push_back(id);
        clients.sockets.push_back(std::move(sock));
        inner_number = clients.ids.size() - 1;
      }
      std::cout << "server __LINE__ = " << __LINE__ << std::endl;
    }
    
    std::cout << "server __LINE__ = " << __LINE__ << std::endl;
    std::thread(session, inner_number, std::ref(message), std::ref(updater)).detach(); //somebody connected; read what they sent to us
//     std::thread(session, std::ref(clients), std::ref(message), std::ref(updater)).detach(); //somebody connected; read what they sent to us
  }
}

void message_sender(int inner_number, std::string message, bool test)
{
  std::stringstream ss;
  int check_time = -1;
  int counter = 0;
  srand (time(NULL));  // initialize random seed 
  for (;;)
  {
    try
    {
      char data[max_buffer_length] = {};      
      if (test)
      {
        std::cout << "Enter inner number..." << std::endl;
        std::cin  >> inner_number;
        std::cin.clear();
        
        if (check_time == time(NULL))
        {
          if ( counter > 10 )
          {
            
            std::cout << "Sender happly died..." << std::endl;
            break;  //kill it if cycle goes wild
          }
        }
        else
        {
            counter = 0;
        }
        check_time = time(NULL);
        counter++;
        
        ss.str(""); ss << (rand() % 80000 + 10000); //rand from 10000 to 90000
        message = ss.str();
      }
      std::strcpy (data, message.c_str());
      boost::asio::write(_sock_, boost::asio::buffer(data, max_buffer_length));    //send message back to client
      std::cout << "Message \"" << data << "\" succesfully sent to client with id " << clients.ids.at(inner_number) << std::endl;
    }
    catch (std::exception& e)
    {
      std::cerr << "Exception in cin thread: " << e.what() << "\n";
    }
  }
}

std::string get_client_id(std::string message)
{
   std::string client_id = message.substr(0, _ID_LENGTH_);
   client_id = message.substr(0, std::min(_ID_LENGTH_, (int)(client_id.find_first_of(_EMPTY_SYMBOL_))));
   return client_id;  
}

int get_int_client_id(std::string message)
{
   return std::atoi(get_client_id(message).c_str());  //atoi transfers c-string (char) into int. (atof-into float)
}

std::string get_head(std::string message)
{
   std::string head = message.substr(_ID_LENGTH_, _HEAD_LENGTH_);
   head = head.substr(0, std::min(_ID_LENGTH_+ _HEAD_LENGTH_, (int)(head.find_first_of(_EMPTY_SYMBOL_))));
   return head;  
}


void output (std::stringstream ss, Updater &updater)
{
  std::cout << ss.str() << std::endl;
  updater.send_message_slot(QString::fromStdString(ss.str()));
}

void output (std::string string, Updater &updater)
{
  std::cout << string << std::endl;
  updater.send_message_slot(QString::fromStdString(string));
}

/*
// void output (TYPE data, Updater &updater)
// {
//   std::cout << data << std::endl;
//   stringstream ss; ss << data;
//   updater.send_message_slot(QString::fromStdString(ss.str()));
// }*/




