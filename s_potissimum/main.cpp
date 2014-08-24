#include <iostream>
#include <sstream>
#include <unistd.h>
#include "widget.h"
#include "log.h"
#include "../include/updater.h"
#include "../include/include.h"
#include "../include/cin_sender.cpp"
#include <QApplication>

void append_text_box(QTextEdit *textBox, std::string message);
void session(boost::asio::ip::tcp::socket sock, std::string &message, Updater &updater);
void server(boost::asio::io_service& io_service, unsigned short port);
void iffunction(boost::asio::ip::tcp::socket& sock, std::string &input_message);
int get_int_client_id(std::string message);
std::string get_client_id(std::string message);
std::string get_head(std::string message);

using boost::asio::ip::tcp;

void session(tcp::socket sock, std::string &message, Updater &updater)
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
      sock.read_some(boost::asio::buffer(data), error);
      if (error == boost::asio::error::eof)
         break; // Connection closed cleanly by peer.
      else if (error)
         throw boost::system::system_error(error); // Some other error.
         
      ss.str("");      ss << "The original message was \"" << data << "\"";      std::cout << ss.str().c_str() << std::endl;
      message = data;
//       append_text_box(textBox, message);
      std::cout << "__LINE__ = " << __LINE__ << std::endl;
      updater.send_message_slot(QString::fromStdString(message));
      std::cout << "__LINE__ = " << __LINE__ << std::endl;
      std::strcpy (data, ss.str().c_str());
      iffunction(std::ref(sock), message);
//       std::cout << "__LINE__ = " << __LINE__ << std::endl;
      boost::asio::write(sock, boost::asio::buffer(data, max_buffer_length));    //send message back to client
      boost::asio::write(sock, boost::asio::buffer(data, max_buffer_length));    //send message back to client
      std::thread(cin_sender, std::ref(sock)).detach();
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
// //    std::string message = &input_message;
   std::string message = input_message;
   char data[max_buffer_length] = {};
   std::string client_id = get_client_id(message);
   std::string head      = get_head(message);
//    data = input_message.c_str();
//    std::strcpy (data, input_message.c_str());
   
   boost::asio::write(sock, boost::asio::buffer(std::strcpy (data, input_message.c_str()), max_buffer_length));    //send message back to client
   boost::asio::write(sock, boost::asio::buffer(data, max_buffer_length));    //send message back to client
   
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
   return std::atoi(get_client_id(message).c_str());  //atoi transfers c-string (char) into int. (atof-into float)
}

std::string get_head(std::string message)
{
   std::string head = message.substr(_ID_LENGTH_, _HEAD_LENGTH_);
   head = head.substr(0, std::min(_ID_LENGTH_+ _HEAD_LENGTH_, (int)(head.find_first_of(_EMPTY_SYMBOL_))));
   return head;  
}

void append_text_box(QTextEdit *textBox, std::string message)
{
//   textBox.append(QString::fromStdString(message));
  textBox->append(QString::fromStdString(message));
  qApp->processEvents();
}





int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  Widget w;
  //w.show();
  std::stringstream ss;
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

    boost::asio::io_service io_service;
    //start the server
    server(io_service, std::atoi(argv[1]));
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }
  
  return a.exec();
}



void server(boost::asio::io_service& io_service, unsigned short port)
{
  std::cout << "++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
  std::cout << "Server is started " << std::endl;
  std::cout << "++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
  std::string message;
  std::stringstream ss;
  Log log;
  Updater updater;
//   QTextEdit *textBox = log.findChild<QTextEdit *>("textEdit");
  std::cout << "__LINE__ = " << __LINE__ << std::endl;
  QObject::connect(&updater, SIGNAL(send_message_signal(QString)), &log, SLOT(receive_message(QString)));
  std::cout << "__LINE__ = " << __LINE__ << std::endl;
  log.show();
  /*
  for(int i = 0; i < 10; i++  )
  {
      ss.str("");
      ss << i;
      textBox->append(QString::fromStdString(ss.str()));
  }*/

  tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), port));
  for (;;)
  {
    tcp::socket sock(io_service);    //open socket
    acceptor.accept(sock);           //wait for message
    std::cout << "__LINE__ = " << __LINE__ << std::endl;
    log.show();
    qApp->processEvents();
    std::thread(session, std::move(sock), std::ref(message), std::ref(updater)).detach(); //somebody connected; read what they sent to us
    std::cout << "message -> " << message << std::endl;
//     usleep(150);  //there is mistake. message here is still empty, because thread doesn't finish by that time. delay added; to be fixed. 
//     textBox->append(QString::fromStdString(message));
//     log.update();
//     log.repaint();
    log.show();
    qApp->processEvents();
    std::cout << "2message -> " << message << std::endl;
  }
}




