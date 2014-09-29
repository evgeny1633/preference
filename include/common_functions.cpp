
#include "include.h"
#ifndef _COMMON_FUNCTIONS_
#define _COMMON_FUNCTIONS_

struct card
{
  int number;
  std::string suit;
  std::string value;
  std::string name;
};


//*****************************************************************************************//
//the following functions withdraw some information from the message or create message of necessary format:
std::string make_head(std::string head)
{
   head.resize (_HEAD_LENGTH_,_EMPTY_SYMBOL_);
   return head;
}

std::string make_block(std::string block)
{
   block.resize (_BLOCK_LENGTH_,_EMPTY_SYMBOL_);
   return block;
}

std::string make_block(int block)
{
   std::stringstream ss;
   ss << block;
   std::string block_id = ss.str();
   block_id.resize(_BLOCK_LENGTH_, _EMPTY_SYMBOL_);
   return block_id;
}

std::string make_client_id(int int_client_id)
{
   std::stringstream ss;
   ss << int_client_id;
   std::string client_id = ss.str();
   client_id.resize(_ID_LENGTH_, _EMPTY_SYMBOL_);
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
   head = head.substr(0, std::min(_HEAD_LENGTH_, (int)(head.find_first_of(_EMPTY_SYMBOL_))));
   return head;  
}

std::string get_block(std::string message, int block_number)
{
  std::string block = message.substr(_ID_LENGTH_ + _HEAD_LENGTH_ + _BLOCK_LENGTH_ * (block_number - 2), _BLOCK_LENGTH_);
  block = block.substr(0, std::min(_BLOCK_LENGTH_, (int)(block.find_first_of(_EMPTY_SYMBOL_))));
  return block;
}

std::string get_chat_message(std::string message)
{
  return message.substr(_ID_LENGTH_ + _HEAD_LENGTH_, message.size() - (_ID_LENGTH_ + _HEAD_LENGTH_));;
}
//*****************************************************************************************//

std::vector<card> create_deck(bool long_value = false, bool long_suit  = false)
{
  std::vector<card> deck;
  card current_card;
  for ( int i = 0; i < 32; i++ )
  {
    if (!long_value)
    {
      if ( i % 8 == 0 ) current_card.value = "7 ";
      if ( i % 8 == 1 ) current_card.value = "8 ";
      if ( i % 8 == 2 ) current_card.value = "9 ";
      if ( i % 8 == 3 ) current_card.value = "10";
      if ( i % 8 == 4 ) current_card.value = "J ";
      if ( i % 8 == 5 ) current_card.value = "Q ";
      if ( i % 8 == 6 ) current_card.value = "K ";
      if ( i % 8 == 7 ) current_card.value = "A ";
    }
    if (!long_suit) 
    {
      if ( i < 8  )            current_card.suit = "♠";
      if ( i >= 8  && i < 16 ) current_card.suit = "♣";
      if ( i >= 16 && i < 24 ) current_card.suit = "♦";
      if ( i >= 24 )           current_card.suit = "♥";
    }
    
    if (long_value)
    {
      if ( i % 8 == 0 ) current_card.value = "seven";
      if ( i % 8 == 1 ) current_card.value = "eight";
      if ( i % 8 == 2 ) current_card.value = "nine";
      if ( i % 8 == 3 ) current_card.value = "ten";
      if ( i % 8 == 4 ) current_card.value = "jack";
      if ( i % 8 == 5 ) current_card.value = "queen";
      if ( i % 8 == 6 ) current_card.value = "king";
      if ( i % 8 == 7 ) current_card.value = "ace";
    }
    if (long_suit)
    {
      if ( i < 8  )            current_card.suit = "spades";
      if ( i >= 8  && i < 16 ) current_card.suit = "clubs";
      if ( i >= 16 && i < 24 ) current_card.suit = "diamonds";
      if ( i >= 24 )           current_card.suit = "hearts";
    }
    
    current_card.number = i;
    current_card.name   = current_card.value + current_card.suit;
//     current_card.name   = current_card.value + " of " + current_card.suit;
    
    deck.push_back(current_card);
  }
  return deck;
}

//rearrange the deck
void rearrangement(std::vector<card> &hand)
{
  std::vector<int> numbers;
  
  #ifdef __std11__
  for (auto it = hand.begin(); it != hand.end(); ++it)
  #else
  for (std::vector<card>::const_iterator it = hand.begin(); it != hand.end(); ++it)
  #endif
  {
    numbers.push_back((*it).number);
  }
  std::sort(numbers.begin(), numbers.end());
  
  #ifdef __std11__
  for (auto nit = numbers.begin(); nit != numbers.end(); ++nit)
  #else
  for (std::vector<int>::const_iterator nit = numbers.begin(); nit != numbers.end(); ++nit)
  #endif
  {
    #ifdef __std11__
    for (auto it = hand.begin(); it != hand.end(); ++it)
    #else
    for (std::vector<card>::const_iterator it = hand.begin(); it != hand.end(); ++it)
    #endif
    {
      if ((*it).number == (*nit))
      {
        std::swap(hand.at(it - hand.begin()), hand.at(nit - numbers.begin()));
        break;
      }
    }
  }
}

//rearrange the deck
void rearrangement(std::vector<int> &hand)
{
  std::sort(hand.begin(), hand.end());
}

//display current time and date
std::string ctiming(bool cout = false)
{
  int day = 20;
  int month = 9;
  int year = 2014;
  long int start;
  int curday;
  std::stringstream ss;
  while(1)
  {
    curday = 0;
    for (int i = 1; i <= month; i++)
    {
      if (i == 1)     curday += 0;
      if (i == 2)     curday += 31;   //jan
      if (i == 4)     curday += 31;   //mar
      if (i == 5)     curday += 30;   //apr
      if (i == 6)     curday += 31;   //may
      if (i == 7)     curday += 30;   //jun
      if (i == 8)     curday += 31;   //jul
      if (i == 9)     curday += 31;   //aug
      if (i == 10)    curday += 30;   //sep
      if (i == 11)    curday += 31;   //oct
      if (i == 12)    curday += 30;   //nov

      if (i == 3 && year%4 == 0)      curday += 29;
      if (i == 3 && year%4 != 0)      curday += 28;
    }
    curday += day;
    start = time(NULL);
    start = start - ( 365 * 24 * 3600 * (floor((year - 1970)/4)*3 + (year - 1970)%4) ) - 
                    ( 366 * 24 * 3600 * floor((year - 1970)/4) ) - curday * 24 * 3600 + 4 * 3600;

    if ( start < 0 )
    {
      year--;
      continue;
    }

    if( ( floor(start/3600) >= 24 ) || ( day > 31 ) || ( month > 12) )
    {
      if ( floor(start/3600) >= 24 )
      day++;
      if ( day > 31 )
      {
        month++;
        day = 1;
      }
      if ( month > 12)
      {
        year++;
        month = 1;
      }
      continue;
    }
    break;
  }

//   ss<<"Current time and date\t";

  if( floor(start/3600) < 10 ) ss<<"0"<<floor(start/3600);
  else                         ss<<floor(start/3600);

  ss<<":";

  if( (start/60)%60 < 10 )     ss<<"0"<<(start/60)%60;
  else                         ss<<(start/60)%60;

  ss<<":";

  if ((start%60)%60 < 10)      ss<<"0"<<(start%60)%60<<"     ";
  else                         ss<<(start%60)%60<<"  ";

  if( day < 10 )               ss<<"0"<<day;
  else                         ss<<day;

  ss<<"-";

  if( month < 10 )             ss<<"0"<<month;
  else                         ss<<month;
  
  ss<<"-"<<year;
  
  if (cout)
    std::cout << ss.str() << std::endl;
  
  return ss.str();
}



#endif //_COMMON_FUNCTIONS_
