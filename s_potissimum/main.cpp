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

std::mutex clients_mutex; // std::lock_guard<std::mutex> lock(clients_mutex);

Updater updater;

// template<class TYPE>
// void output (TYPE data, Updater &updater);
void test_message_sender(int inner_number = 0, std::string message = "", bool test = true);
void message_sender(int inner_number, std::string message);
void output (std::stringstream ss);
void output (std::string string);
void append_text_box(QTextEdit *textBox, std::string message);
void session(int inner_number);
void server(boost::asio::io_service &io_service, unsigned short port);
void iffunction(boost::asio::ip::tcp::socket& sock, std::string &input_message);
void distribution();
// void rearrangement(std::vector<card> &hand);
int get_int_client_id(std::string message);
std::string get_client_id(std::string message);
std::string get_head(std::string message);



void session(int inner_number)
{
//   std::cout << "session __LINE__ = " << __LINE__ << std::endl;
  std::cout << "inner_number = " << inner_number << std::endl;
  std::unique_lock<std::mutex> session_lock(clients_mutex);
  std::cout << "clients.ids.size() = " << clients.ids.size() << std::endl;
  int client_id = clients.ids.at(inner_number);
  session_lock.unlock();
  std::stringstream ss;
  std::string message;
  ss.str("");
  try
  {
    for (;;) 
    {
      std::cout << "session __LINE__ = " << __LINE__ << std::endl;
      char data[max_buffer_length] = {};
      boost::system::error_code error;
      std::cout << "session __LINE__ = " << __LINE__ << std::endl;
      session_lock.lock();
      std::cout << "session __LINE__ = " << __LINE__ << std::endl;
      _sock_.read_some(boost::asio::buffer(data), error); //read message
      session_lock.unlock();
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
      std::strcpy (data, ss.str().c_str());
      iffunction(std::ref(_sock_), message);
      session_lock.lock();
      boost::asio::write(_sock_, boost::asio::buffer(data, max_buffer_length));    //send message back to client
      session_lock.unlock();
    }
  }
  catch (std::exception& e)
  {
    std::cout << "session __LINE__ = " << __LINE__ << std::endl;
    std::cerr << "Exception in thread: " << e.what() << "\n";
  }
}

void iffunction(tcp::socket& sock, std::string &input_message)
{
//   std::cout << "iffunction __LINE__ = " << __LINE__ << std::endl;
  std::string message = input_message;
  char data[max_buffer_length] = {};
  std::string client_id = get_client_id(message);
  std::string head      = get_head(message);
  std::strcpy (data, input_message.c_str());
  boost::asio::write(sock, boost::asio::buffer(data, max_buffer_length));    //send message back to client
   

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
    std::thread server_thread(server, boost::ref(io_service), std::atoi(argv[1]));//.detach();
    a.exec(); //oh god this is mandatory. nobody told me that this shit is called main thread. stupid qt, i hate it. at last this works now.
    server_thread.join();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }
  
  return 0;
}


void server(boost::asio::io_service &io_service, unsigned short port)
{
//   std::cout << "server __LINE__ = " << __LINE__ << std::endl;
  std::cout << "++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
  std::cout << "Server is started " << std::endl;
  std::cout << "++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
  std::string message;
  std::stringstream ss;
  std::unique_lock<std::mutex> server_lock(clients_mutex, std::defer_lock);
  bool reconnected;
  int id;
  int inner_number;
  std::cout << "server __LINE__ = " << __LINE__ << std::endl;  
  distribution();
  std::cout << "server __LINE__ = " << __LINE__ << std::endl;
  
  std::cout << "port = " << port << std::endl;
  tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), port));
  std::thread([]{ test_message_sender();}).detach();
  for (;;)
  {
    tcp::socket sock(io_service);    //open socket
    std::cout << "server __LINE__ = " << __LINE__ << std::endl;
    acceptor.accept(sock);           //wait for message
    
    char data[max_buffer_length] = {};
    boost::system::error_code error;
    std::cout << "server __LINE__ = " << __LINE__ << std::endl;
    sock.read_some(boost::asio::buffer(data), error); //read message
    message = data;
    if (get_head(message) == "alive")
    {
      reconnected = false;
      id = get_int_client_id(message);
      std::cout << "server __LINE__ = " << __LINE__ << std::endl;
      server_lock.lock();
      for (auto iterator = clients.ids.begin(); iterator != clients.ids.end(); ++iterator)
      {
        if (id == (*iterator))
        {
          ss.str(""); ss << "Client # " << id << " reconnected\n"; output(ss.str());
          reconnected = true;
          inner_number = iterator - clients.ids.begin();
          clients.sockets.at(inner_number) = std::move(sock);
        }
      }
      if (!reconnected)
      {
        clients.ids.push_back(id);
        clients.sockets.push_back(std::move(sock));
        inner_number = clients.ids.size() - 1;
      }
      std::cout << "server __LINE__ = " << __LINE__ << std::endl;
      server_lock.unlock();
    }
    
    std::cout << "server __LINE__ = " << __LINE__ << std::endl;
    std::thread(session, inner_number).detach(); //somebody connected; read what they sent to us
//     std::thread(session, std::ref(clients), std::ref(message), std::ref(updater)).detach(); //somebody connected; read what they sent to us
  }
}

void test_message_sender(int inner_number, std::string message, bool test)
{
  std::stringstream ss;
  int check_time = -1;
  int counter = 0;
  
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
        
        srand (time(NULL));  // initialize random seed 
        ss.str(""); ss << (rand() % 80000 + 10000); //rand from 10000 to 90000
        message = ss.str();
      }
      std::strcpy (data, message.c_str());
      clients_mutex.lock();
      boost::asio::write(_sock_, boost::asio::buffer(data, max_buffer_length));    //send message back to client
      clients_mutex.unlock();
      std::cout << "Message \"" << data << "\" succesfully sent to client with id " << clients.ids.at(inner_number) << std::endl;
    }
    catch (std::exception& e)
    {
      std::cerr << "Exception in cin thread: " << e.what() << "\n";
    }
  }
}

void message_sender(int inner_number, std::string message)
{
  std::stringstream ss;
  char data[max_buffer_length] = {};      
  try
  {
    std::strcpy (data, message.c_str());
    std::lock_guard<std::mutex> sender_lock(clients_mutex);
    boost::asio::write(_sock_, boost::asio::buffer(data, max_buffer_length));    //send message back to client
    std::cout << "Message \"" << data << "\" sent to client with id " << clients.ids.at(inner_number) << std::endl;
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception in message sender: " << e.what() << "\n";
  }
}

void distribution()
{
//   std::cout << "distribution __LINE__ = " << __LINE__ << std::endl;
  std::vector<card> deck;
  card current_card;
  for ( int i = 0; i < 32; i++ )
  {
    if ( i % 8 == 0 ) current_card.value = "7 ";
    if ( i % 8 == 1 ) current_card.value = "8 ";
    if ( i % 8 == 2 ) current_card.value = "9 ";
    if ( i % 8 == 3 ) current_card.value = "10";
    if ( i % 8 == 4 ) current_card.value = "J ";
    if ( i % 8 == 5 ) current_card.value = "Q ";
    if ( i % 8 == 6 ) current_card.value = "K ";
    if ( i % 8 == 7 ) current_card.value = "A ";
    
    if ( i < 8  )            current_card.suit = "♠";
    if ( i >= 8  && i < 16 ) current_card.suit = "♣";
    if ( i >= 16 && i < 24 ) current_card.suit = "♦";
    if ( i >= 24 )           current_card.suit = "♥";
    
    /*
    if ( i % 8 == 0 ) current_card.value = "seven";
    if ( i % 8 == 1 ) current_card.value = "eight";
    if ( i % 8 == 2 ) current_card.value = "nine";
    if ( i % 8 == 3 ) current_card.value = "ten";
    if ( i % 8 == 4 ) current_card.value = "jack";
    if ( i % 8 == 5 ) current_card.value = "queen";
    if ( i % 8 == 6 ) current_card.value = "king";
    if ( i % 8 == 7 ) current_card.value = "ace";
    
    if ( i < 8  )            current_card.suit = "spades";
    if ( i >= 8  && i < 16 ) current_card.suit = "clubs";
    if ( i >= 16 && i < 24 ) current_card.suit = "diamonds";
    if ( i >= 24 )           current_card.suit = "hearts";
     */
    
    current_card.number = i;
    current_card.name   = current_card.value + current_card.suit;
//     current_card.name   = current_card.value + " of " + current_card.suit;
    
    deck.push_back(current_card);
  }
  
  std::random_shuffle(deck.begin(), deck.end());
  std::random_shuffle(deck.begin(), deck.end());
  std::random_shuffle(deck.begin(), deck.end());
  
  std::vector<card> pl1;
  std::vector<card> pl2;
  std::vector<card> pl3;
  
  std::vector<card> talon;
  int talon_place = rand() % 20 + 6;
  talon_place = talon_place - talon_place % 2 + 1; //make it odd 
  
  for ( int i = 0; i < 32; i++ )
  {
    pl1.push_back(deck.at(i));
    pl1.push_back(deck.at(++i));
    if (i == talon_place)
    {
      talon.push_back(deck.at(++i));
      talon.push_back(deck.at(++i));
    }
    pl2.push_back(deck.at(++i));
    pl2.push_back(deck.at(++i));
    if (i == talon_place)
    {
      talon.push_back(deck.at(++i));
      talon.push_back(deck.at(++i));
    }
    pl3.push_back(deck.at(++i));
    pl3.push_back(deck.at(++i));
    if (i == talon_place)
    {
      talon.push_back(deck.at(++i));
      talon.push_back(deck.at(++i));
    }
  }
  
  std::stringstream ss;
  output("pl1:");     for (auto it = pl1.begin(); it != pl1.end(); ++it)     output((*it).name);
  output("\npl2:");   for (auto it = pl2.begin(); it != pl2.end(); ++it)     output((*it).name);
  output("\npl3:");   for (auto it = pl3.begin(); it != pl3.end(); ++it)     output((*it).name);
  output("\ntalon:"); for (auto it = talon.begin(); it != talon.end(); ++it) output((*it).name);
  /*
  output("pl1:");     ss.str(""); for (auto it = pl1.begin(); it != pl1.end(); ++it)     ss << (*it).suit;  output(ss.str().c_str());
                      ss.str(""); for (auto it = pl1.begin(); it != pl1.end(); ++it)     ss << (*it).value; output(ss.str().c_str());
  output("\npl2:");   ss.str(""); for (auto it = pl2.begin(); it != pl2.end(); ++it)     ss << (*it).suit;  output(ss.str().c_str());
                      ss.str(""); for (auto it = pl2.begin(); it != pl2.end(); ++it)     ss << (*it).value; output(ss.str().c_str());
  output("\npl3:");   ss.str(""); for (auto it = pl3.begin(); it != pl3.end(); ++it)     ss << (*it).suit;  output(ss.str().c_str());
                      ss.str(""); for (auto it = pl3.begin(); it != pl3.end(); ++it)     ss << (*it).value; output(ss.str().c_str());
  output("\ntalon:"); ss.str(""); for (auto it = talon.begin(); it != talon.end(); ++it) output((*it).name);*/
  rearrangement(std::ref(pl1));
  rearrangement(std::ref(pl2));
  rearrangement(std::ref(pl3));
  
  output("\n\nRearrangemented:\n\n");  
  output("pl1:");     for (auto it = pl1.begin(); it != pl1.end(); ++it)     output((*it).name);
  output("\npl2:");   for (auto it = pl2.begin(); it != pl2.end(); ++it)     output((*it).name);
  output("\npl3:");   for (auto it = pl3.begin(); it != pl3.end(); ++it)     output((*it).name);
  output("\ntalon:"); for (auto it = talon.begin(); it != talon.end(); ++it) output((*it).name);
}

void current_bribe()
{
 
  
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


void output (std::stringstream ss)
{
  std::cout << ss.str() << std::endl;
  updater.send_message_slot(QString::fromStdString(ss.str()));
}

void output (std::string string)
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




