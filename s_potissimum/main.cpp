#include <iostream>
#include <sstream>
#include <unistd.h>

#include "../include/include.h"
#include "../include/common_functions.cpp"

#ifdef __QT__
#include <QApplication>
#include <QThread>
#include "widget.h"
#include "log.h"
#include "../include/updater.h"
#endif

#define _sock_ (clients.socket.at(inner_number))  //this is socket to send messages to the particular client
#define _id_ (clients.id.at(inner_number))        //what ? //inner_number -- number of element in clients vector, everywhere.

// using boost::asio::ip::tcp;
using namespace std;
struct clients_list
{ 
  std::vector <int> id;
  std::vector <boost::asio::ip::tcp::socket> socket;
  std::vector <bool> connected;
  std::vector <bool> playing;
  std::vector <int> version;
} clients; //it's impossible to create vector of clients, because it's impossible to declare boost::asio::ip::tcp::socket inside it


#ifdef __QT__
Updater updater;//class to update log window via QApplication::connect
#endif

std::vector <int> active_players /*= {0, 1, 2}*/ ;
std::mutex clients_mutex;        // mutex to avoid undefined behavior with "clients".        some threads can read\write into it simultaniously.
std::mutex active_players_mutex; // mutex to avoid undefined behavior with "active_players". some threads can read\write into it simultaniously.

void test_message_sender(int inner_number = 0, std::string message = "", bool test = true);
void output (std::string string, bool with_new_line = true);
void message_sender(int inner_number, std::string message); //send message to the client
void session(int inner_number);
void server(boost::asio::io_service &io_service, unsigned short port);
void deck_distribution();
// void iffunction(int inner_number, std::string &input_message);

//output to cout and to log window
void output (std::string string, bool with_new_line)
{
  if (with_new_line)
    std::cout << string << std::endl;
  else
    std::cout << string;
  #ifdef __QT__
  updater.send_message_slot(QString::fromStdString(string));
  #endif
}

void handler_action(int signal_number) //this function will be called when server is killed or receive certain SIGnals
{
  for (auto iterator = clients.connected.begin(); iterator != clients.connected.end(); ++iterator)
    if (*iterator)
      message_sender(iterator - clients.connected.begin(), make_message(0, "service", "serverwaskilled"));
    
  std::cout << std::endl << "Server was killed. Bye !" << std::endl << std::endl;
  exit(10);
}

void death_handler()  //this function defines what SIGnals we should handle
{ 
  struct sigaction action;
  action.sa_handler = handler_action; /* Do our own action */
  sigaction(SIGTERM, &action, NULL);  /* killall */
  sigaction(SIGINT,  &action, NULL);  /* ctrl-c  */
}


//divide message into parts and decide what to do with the message
void iffunction(int inner_number, std::string &input_message)
{
//   std::cout << "iffunction __LINE__ = " << __LINE__ << std::endl;
  std::cout << "iffunction __LINE__ = " << __LINE__ << std::endl;
  if (input_message.size() < _ID_LENGTH_ + _HEAD_LENGTH_)
    return;
  std::stringstream ss;
  std::string message = input_message;
  char data[max_buffer_length] = {};
  
  int block_number = (input_message.size() - _ID_LENGTH_ - _HEAD_LENGTH_) / _BLOCK_LENGTH_ + 2; 
  std::vector<std::string> block;
  block.resize(block_number);
  std::cout << "iffunction __LINE__ = " << __LINE__ << std::endl;
  
  block.at(0) = get_client_id(message);
  block.at(1) = get_head(message);
  std::cout << "iffunction __LINE__ = " << __LINE__ << std::endl;
  for ( int i = 2; i < block_number; i++)
  {
    block.at(i) = get_block(message, i);
    output(block.at(i));
  }
  
  std::lock_guard<std::mutex> iffunction_lock(clients_mutex);  
  std::cout << "iffunction __LINE__ = " << __LINE__ << std::endl;
  if ( block.at(1) == "alive" )
  {
    std::cout << "iffunction __LINE__ = " << __LINE__ << std::endl;
    std::strcpy(data, message.c_str());
    std::cout << "iffunction data = " << data << std::endl;
    std::cout << "iffunction __LINE__ = " << __LINE__ << std::endl;
    message_sender(inner_number, message);
//     boost::asio::write(_sock_, boost::asio::buffer(data, max_buffer_length));    //send message back to client
    std::cout << "iffunction __LINE__ = " << __LINE__ << std::endl;
//     send him back alive signal
//     send others that hi is connected
  }

  if ( block.at(1) == "statistics" )  { /* send him statistics */ }
  if ( block.at(1) == "trade"      )  {  }
  if ( block.at(1) == "game"       )  {  }
  if ( block.at(1) == "offer"      )  {  }
  if ( block.at(1) == "chat"       )
  {
    ss.str("");
    ss << "player " << get_client_id(message) << "[" << ctiming() << "]: " << get_chat_message(message);
    output(ss.str());
    for (auto iterator = clients.connected.begin(); iterator != clients.connected.end(); ++iterator)
    {
      if (*iterator)
      {
        ss.str("");
        ss << make_client_id(get_int_client_id(message)) << make_head("chat") << "player " << get_client_id(message) << "[" << ctiming() << "]: " << get_chat_message(message);
        message_sender(iterator - clients.connected.begin(), ss.str());
      }
    }    //then block[2] is playername
  }
  if ( block.at(1) == "service" )
  {
    std::cout << "iffunction __LINE__ = " << __LINE__ << std::endl;
    if( block.at(2) == "clientwaskilled" )
    {
      std::cout << "Client # " << block.at(0) << " was killed. Let's forget about him..." << std::endl;
      std::cout << "inner_number = " << inner_number << ";\tclients.connected.size() = " << clients.connected.size() << std::endl;
      clients.connected.at(inner_number) = false;
      std::cout << "iffunction __LINE__ = " << __LINE__ << std::endl;
      for (auto iterator = active_players.begin(); iterator != active_players.end(); ++iterator)
      {
        std::cout << "iffunction __LINE__ = " << __LINE__ << std::endl;
        std::cout << "inner_number = " << inner_number << ";\t*iterator = " << *iterator << std::endl;
        if (inner_number == *iterator)
        {
          std::cout << "iterator - active_players.begin() = " << iterator - active_players.begin() << std::endl;
          active_players.erase(iterator);
          break;
        }
      }
    }
  }
  
  clients_mutex.unlock();
  std::cout << "iffunction __LINE__ = " << __LINE__ << std::endl;
}

//probably useless...
void session(int inner_number)
{
//   std::cout << "session __LINE__ = " << __LINE__ << std::endl;
  std::cout << "inner_number = " << inner_number << std::endl;
  std::unique_lock<std::mutex> session_lock(clients_mutex, std::defer_lock);
//   int client_id = clients.id.at(inner_number);
//   session_lock.unlock();
  std::stringstream ss;
  std::string message;
  ss.str("");
  try
  {
    for (;;) 
    {
      char data[max_buffer_length] = {};
      boost::system::error_code error;
      session_lock.lock();
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
      #ifdef __QT__
      updater.send_message_slot(QString::fromStdString(message));
      #endif
      std::strcpy (data, ss.str().c_str());
      iffunction(inner_number, message);
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception in session: " << e.what() << "\n";
    session_lock.unlock();
  }
}

//send messages from cin directly
void test_message_sender(int inner_number, std::string message, bool test)
{
//   std::cout << "test_sender __LINE__ = " << __LINE__ << std::endl;
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
        std::cout << "Enter inner number... " ;
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
        ss.str(""); 
//         ss << (rand() % 80000 + 10000); //rand from 10000 to 90000
        for(int i = 0; i < _ID_LENGTH_ + _HEAD_LENGTH_ + 1; i++) 
          ss << (rand() % 9); //rand from 0 to 9
        message = ss.str();
      }
      std::strcpy (data, message.c_str());
      clients_mutex.lock();
      boost::asio::write(_sock_, boost::asio::buffer(data, max_buffer_length));    //send message back to client
      clients_mutex.unlock();
      std::cout << "Message \"" << data << "\" succesfully sent to client with id " << clients.id.at(inner_number) << std::endl;
    }
    catch (std::exception& e)
    {
      std::cout << "test_sender __LINE__ = " << __LINE__ << std::endl;
      std::cerr << "Exception in cin thread: " << e.what() << "\n";
      clients_mutex.unlock();
    }
  }
}

//send message to the client
void message_sender(int inner_number, std::string message)
{       //probably we don't need mutex here, because ..... stop, why ?
  char data[max_buffer_length] = {};      
  try
  {
    std::strcpy (data, message.c_str());
//     std::cout << "message_sender __LINE__ = " << __LINE__ << std::endl;
//     std::lock_guard<std::mutex> sender_lock(clients_mutex);  // 
//     std::cout << "message_sender __LINE__ = " << __LINE__ << std::endl;
    boost::asio::write(_sock_, boost::asio::buffer(data, max_buffer_length));    //send message back to client
    std::cout << "Message \"" << data << "\" succesfully sent to client with id " << clients.id.at(inner_number) << std::endl;
  }
  catch (std::exception& e)
  {
    std::cout << "message_sender __LINE__ = " << __LINE__ << std::endl;
    std::cerr << "Exception in message sender: " << e.what() << "\n";
  }
}

//send message and wait for the answer. dangerous, will be fixed. can wait forever. 
std::string send_receive(int inner_number, std::string message)
{
  char data[max_buffer_length] = {};      
  try
  {
    boost::system::error_code error;
    std::strcpy (data, message.c_str());
    std::lock_guard<std::mutex> sender_lock(clients_mutex);
    boost::asio::write(_sock_, boost::asio::buffer(data, max_buffer_length));    //send message back to client
//     std::cout << "Message \"" << data << "\" succesfully sent to client with id " << clients.id.at(inner_number) << std::endl;
    _sock_.read_some(boost::asio::buffer(data), error); //read message
    message = data;
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception in send_receive: " << e.what() << "\n";
    message = e.what();
  }
  return message;
}

void current_bribe(){}

//send cards to the clients
void deck_distribution()
{
//   std::cout << "deck_distribution __LINE__ = " << __LINE__ << std::endl;
  if(active_players.size() != 3 && active_players.size() != 4 )
  {
    std::cout << "distribution: wrong amount of players " << std::endl;
    return;
  }
  
  std::stringstream ss;
  std::vector<card> deck = create_deck();

  std::random_shuffle(deck.begin(), deck.end());
  std::random_shuffle(deck.begin(), deck.end());
  std::random_shuffle(deck.begin(), deck.end());
  
  std::vector<std::vector<card>> d_player;
  std::vector<card> talon;
  d_player.resize(3);
  
  int talon_place = rand() % 20 + 6;
  talon_place = talon_place + talon_place % 2; //make it even(!)
  std::cout << "deck_distribution __LINE__ = " << __LINE__ << ";\ttalon_place = " << talon_place << std::endl;
  
  for ( int i = 0; i < 32; )
  {
    for (auto i_d_player = d_player.begin(); i_d_player != d_player.end(); i_d_player++)
    {
      (*i_d_player).push_back(deck.at(i++));
      (*i_d_player).push_back(deck.at(i++));
      if (i == talon_place)
      {
        talon.push_back(deck.at(i++));
        talon.push_back(deck.at(i++));
      }
    }
  }
  
  for (auto i_d_player = d_player.begin(); i_d_player != d_player.end(); i_d_player++)
  {
    ss.str(""); ss << "\nd_player.at(" << i_d_player - d_player.begin() << "):";// << (*i_d_player);
    output(ss.str());
    for (auto it = (*i_d_player).begin(); it != (*i_d_player).end(); ++it)     
    {
      output((*it).name);
      ss.str(""); ss << make_client_id(active_players.at(i_d_player - d_player.begin())) << make_head("distribution") << make_block((*it).number);
      message_sender(active_players.at(i_d_player - d_player.begin()), ss.str());
    }
  }
  
  output("\ntalon:"); 
  for (auto it = talon.begin(); it != talon.end(); ++it) output((*it).name);
  /*
  output("pl1:");     ss.str(""); for (auto it = pl1.begin(); it != pl1.end(); ++it)     ss << (*it).suit;  output(ss.str().c_str());
                      ss.str(""); for (auto it = pl1.begin(); it != pl1.end(); ++it)     ss << (*it).value; output(ss.str().c_str());
  output("\npl2:");   ss.str(""); for (auto it = pl2.begin(); it != pl2.end(); ++it)     ss << (*it).suit;  output(ss.str().c_str());
                      ss.str(""); for (auto it = pl2.begin(); it != pl2.end(); ++it)     ss << (*it).value; output(ss.str().c_str());
  output("\npl3:");   ss.str(""); for (auto it = pl3.begin(); it != pl3.end(); ++it)     ss << (*it).suit;  output(ss.str().c_str());
                      ss.str(""); for (auto it = pl3.begin(); it != pl3.end(); ++it)     ss << (*it).value; output(ss.str().c_str());
  output("\ntalon:"); ss.str(""); for (auto it = talon.begin(); it != talon.end(); ++it) output((*it).name);    * /
  rearrangement(std::ref(pl1));
  rearrangement(std::ref(pl2));
  rearrangement(std::ref(pl3));
  
  output("\n\nRearrangemented:\n\n");  
  output("pl1:");     for (auto it = pl1.begin(); it != pl1.end(); ++it)     output((*it).name);
  output("\npl2:");   for (auto it = pl2.begin(); it != pl2.end(); ++it)     output((*it).name);
  output("\npl3:");   for (auto it = pl3.begin(); it != pl3.end(); ++it)     output((*it).name);
  output("\ntalon:"); for (auto it = talon.begin(); it != talon.end(); ++it) output((*it).name);
  */
  
}

//check if certain client is alive
std::string check_one(int inner_numer)
{
  std::stringstream ss;
  ss.str(""); ss << make_client_id(inner_numer) << make_head("alive");
  return send_receive(inner_numer, ss.str());
}

/*
void check_one_endless(int inner_numer)
{
  std::stringstream ss;
  while(true)
  {
    ss.str(""); ss << make_client_id(inner_numer) << make_head("alive");
    std::cout << "i send " << send_receive(inner_numer, ss.str()) << std::endl;
    usleep(5e6);
  }
}*/

//check all active players
std::vector<int> check_alive()
{
  std::vector<int> check;
  std::stringstream ss;
  std::string message;
  std::cout << "check_alive __LINE__ = " << __LINE__ << std::endl;
  for (auto i_active = active_players.begin(); i_active != active_players.end(); ++i_active)
  {
    ss.str(""); ss << make_client_id(clients.id.at(*i_active)) << make_head("alive");
    std::cout << "check_alive __LINE__ = " << __LINE__ << std::endl;
    message = send_receive(*i_active, ss.str());
    std::cout << "check_alive __LINE__ = " << __LINE__ << std::endl;
    if (message != ss.str())
      check.push_back(*i_active);
  }
  std::cout << "check_alive __LINE__ = " << __LINE__ << std::endl;
  return check;
}

int check_active(int number = 3)
{
  return number - active_players.size();
}

void trade()   {std::cout << "trade    __LINE__ = " << __LINE__ << std::endl; usleep(5e6);}
void booking() {std::cout << "booking  __LINE__ = " << __LINE__ << std::endl; usleep(5e6);}
void vists()   {std::cout << "vists    __LINE__ = " << __LINE__ << std::endl; usleep(5e6);}
void moves()   {std::cout << "moves    __LINE__ = " << __LINE__ << std::endl; usleep(5e6);}
void offers()  {std::cout << "offers   __LINE__ = " << __LINE__ << std::endl; usleep(5e6);}
void result()  {std::cout << "result   __LINE__ = " << __LINE__ << std::endl; usleep(5e6);}
void cleaning(){std::cout << "cleaning __LINE__ = " << __LINE__ << std::endl; usleep(5e6);}

//main game cycle
void game_cycle()
{
  std::vector<int> check;
  std::stringstream ss;
  usleep(5e6);
  try
  {
    while (check_active() != 0)
    {
      ss.str("");  ss << "waiting for " << check_active() << " more clients to play";  output (ss.str());
      usleep(5e6);
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception in game_cycle: " << e.what() << "\n";
  }
  
  while(true)
  {
    do
    {
      check.resize(0);
      std::cout << "game_cycle __LINE__ = " << __LINE__ << std::endl;
      check = check_alive();
      std::cout << "game_cycle __LINE__ = " << __LINE__ << std::endl;
      for (auto iterator : check)
      {
        ss.str("");  ss << "waiting for client " << iterator << " to connect";  output (ss.str());
      }
      if (check.size() > 0)
      {
        ss.str("");  ss << "waiting for " << check.size() << " more clients " << " to connect";  output (ss.str());
        usleep(5e6);
      }
    } while ( check.size() != 0 );
    
    deck_distribution();
    trade();
    booking();
    vists();
    moves();
    offers();
    result();
    cleaning();
    
  }
  
  
}


/*
// void output (TYPE data, Updater &updater)
// {
//   std::cout << data << std::endl;
//   stringstream ss; ss << data;
//   updater.send_message_slot(QString::fromStdString(ss.str()));
// }*/

//server can receive and process some certain messages, e.g. chat, alive, active...
void server(boost::asio::io_service &io_service, unsigned short port)
{
//   std::cout << "server __LINE__ = " << __LINE__ << std::endl;
  std::cout << "++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
  std::cout << "Server is started " << std::endl;
  std::cout << "++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
  std::string message;
  std::string chat_message;
  std::stringstream ss;
  std::unique_lock<std::mutex> server_lock(clients_mutex, std::defer_lock);
  bool reconnected;
  int id;
  int inner_number;
  std::cout << "server __LINE__ = " << __LINE__ << std::endl;  
//   deck_distribution();
  std::cout << "port = " << port << std::endl;
  boost::asio::ip::tcp::acceptor acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port));
  std::thread([]{ test_message_sender();}).detach(); //cin sender
  std::thread(game_cycle).detach(); 
  for (;;)
  {
    boost::asio::ip::tcp::socket sock(io_service);    //open socket
    std::cout << "server __LINE__ = " << __LINE__ << std::endl;
    acceptor.accept(sock);           //wait for message
    
    char data[max_buffer_length] = {};
    boost::system::error_code error;
    std::cout << "server __LINE__ = " << __LINE__ << std::endl;
    sock.read_some(boost::asio::buffer(data), error); //read message
//     size_t reply_length = boost::asio::read(sock,boost::asio::buffer(data, max_buffer_length));
    message = data;
    if (message.size() < _ID_LENGTH_ + _HEAD_LENGTH_)      
    {
      std::cerr << "Error: server received short message: \"" << message << "\" of size " << message.size() << std::endl;
      continue;
    }
    std::cout << "server __LINE__ = " << __LINE__ << std::endl;
    if (get_head(message) == "alive")
    {
      reconnected = false;
      id = get_int_client_id(message);
      std::cout << "server (got alive) __LINE__ = " << __LINE__ << std::endl;
      server_lock.lock();
      for (auto iterator = clients.id.begin(); iterator != clients.id.end(); ++iterator)
      {
        if (id == (*iterator))
        {
          ss.str(""); ss << "Client # " << id << " reconnected"; output(ss.str());
          reconnected = true;
          inner_number = iterator - clients.id.begin();
          clients.socket.at(inner_number) = std::move(sock);
          clients.connected.at(inner_number) = true;
//           std::cout << "clients.socket.at(" << inner_number << ") = " << &(clients.socket.at(inner_number)) << std::endl;
        }
      }

      if (!reconnected)
      {
        std::cout << "server (got alive) __LINE__ = " << __LINE__ << std::endl;
        clients.id.push_back(id);
        clients.socket.push_back(std::move(sock));
        clients.connected.push_back(true);
        inner_number = clients.id.size() - 1;
      }
      active_players.push_back(inner_number);
      server_lock.unlock();
    }
    else
    {
      std::cout << "server __LINE__ = " << __LINE__ << std::endl;
      std::cout << "get_int_client_id(message) = " << get_int_client_id(message) << ";\tget_head(message) = " << get_head(message) << ";\tmessage = " << message << std::endl;
      iffunction(inner_number, message);
    }
    
/*    if (get_head(message) == "chat")
    {
      ss.str("");
      ss << "player " << get_client_id(message) << "[" << ctiming() << "]: " << get_chat_message(message);
      chat_message = ss.str();
      output(chat_message);
      for (auto iterator = clients.connected.begin(); iterator != clients.connected.end(); ++iterator)
      {
        if (*iterator)
        {
          ss.str("");
          ss << get_client_id(message) << get_head(message) << chat_message;
          message_sender(iterator - clients.connected.begin(), ss.str());
        }
      }
    }
   */ 

    std::cout << "server __LINE__ = " << __LINE__ << std::endl;
//     std::cout << "sock.is_open() : " << sock.is_open() << std::endl;
//     sock.shutdown(boost::asio::ip::tcp::socket::shutdown_send);
//     std::cout << "server __LINE__ = " << __LINE__ << std::endl;
//     sock.close();
//     std::cout << "sock.is_open() : " << sock.is_open() << std::endl;
//     std::thread(session, inner_number).detach(); //somebody connected; read what they sent to us
//     std::thread(session, std::ref(clients), std::ref(message), std::ref(updater)).detach(); //somebody connected; read what they sent to us
  }
}

int main(int argc, char *argv[])
{
  death_handler();
  #ifdef __QT__
  QApplication a(argc, argv);
  Widget w;
  //w.show();
  Log log;
  
  QObject::connect(&updater, SIGNAL(send_message_signal(QString)), &log, SLOT(receive_message(QString)));
  #endif
  
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
    #ifdef __QT__
    log.show();
    #endif
    boost::asio::io_service io_service;
    //start the server
    std::thread server_thread(server, boost::ref(io_service), std::atoi(argv[1]));//.detach();
    #ifdef __QT__
    a.exec(); //oh god this is mandatory. nobody told me that this shit is called main thread. stupid qt, i hate it. at last this works now.
    #endif
    server_thread.join();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception in server: " << e.what() << "\n";
  }
  
  return 0;
}

