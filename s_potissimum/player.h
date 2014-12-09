#ifndef PLAYER_H
#define PLAYER_H

class player
{
public:
    player();
    
    boost::asio::ip::tcp::socket socket;
    int id;
    bool connected;
    bool playing;
    bool version;
    int bet;
};

#endif // PLAYER_H
