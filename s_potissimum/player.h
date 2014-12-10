#ifndef PLAYER_H
#define PLAYER_H

#include "../include/include.h"

class player
{
  
private:
  boost::asio::ip::tcp::socket *socket;
    
    
public:
  player();
  void reconnect(boost::asio::ip::tcp::socket socket);
  void connect  (boost::asio::ip::tcp::socket socket);
  void connect  (boost::asio::ip::tcp::socket socket, int id);
  
  int write(std::string message);
  std::string read();
  std::string send_receive(std::string message);
  
  
  int id;
  int bet;
  bool connected;
  bool playing;
  
  bool version;
  
};

#endif // PLAYER_H
