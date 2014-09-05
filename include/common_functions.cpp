
#ifndef _COMMON_FUNCTIONS_
#define _COMMON_FUNCTIONS_
#include "include.h"

struct card
{
  int number;
  std::string suit;
  std::string value;
  std::string name;
};

std::string make_head(std::string head)
{
   head.resize (_HEAD_LENGTH_,_EMPTY_SYMBOL_);
   return head;
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

void rearrangement(std::vector<card> &hand)
{
  std::vector<int> numbers;
  for (auto it = hand.begin(); it != hand.end(); ++it)
    numbers.push_back((*it).number);
  std::sort(numbers.begin(), numbers.end());
  for (auto nit = numbers.begin(); nit != numbers.end(); ++nit)
  {
    for (auto it = hand.begin(); it != hand.end(); ++it)
    {
      if ((*it).number == (*nit))
      {
        std::swap(hand.at(it - hand.begin()), hand.at(nit - numbers.begin()));
        break;
      }
    }
  }
}




#endif //_COMMON_FUNCTIONS_