#ifndef USERINTERFACE_HPP
#define USERINTERFACE_HPP



class userInterfaceData;

class userInterface
{

public:
  userInterface();
  userInterface(const userInterface &);
  userInterface &operator=(const userInterface &);
  ~userInterface();

private:
  QSharedDataPointer<userInterfaceData> data;
};

#endif // USERINTERFACE_HPP
