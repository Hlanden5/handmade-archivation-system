#include "userinterface.hpp"

class userInterfaceData : public QSharedData
{
public:

};

userInterface::userInterface() : data(new userInterfaceData)
{

}

userInterface::userInterface(const userInterface &rhs)
  : data{rhs.data}
{

}

userInterface &userInterface::operator=(const userInterface &rhs)
{
  if (this != &rhs)
    data.operator=(rhs.data);
  return *this;
}

userInterface::~userInterface()
{

}
