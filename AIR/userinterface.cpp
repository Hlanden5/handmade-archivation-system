#include "userinterface.hpp"
#include "ui_userinterface.h"

userInterface::userInterface(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::userInterface)
{
  ui->setupUi(this);
}

userInterface::~userInterface()
{
  delete ui;
}

