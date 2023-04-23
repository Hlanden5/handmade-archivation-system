#ifndef USERINTERFACE_HPP
#define USERINTERFACE_HPP

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class userInterface; }
QT_END_NAMESPACE

class userInterface : public QMainWindow
{
  Q_OBJECT

public:
  userInterface(QWidget *parent = nullptr);
  ~userInterface();

private:
  Ui::userInterface *ui;
};
#endif // USERINTERFACE_HPP
