#include "userinterface.hpp"
#include "ui_userinterface.h"
#include "clocale"
#include "QFileDialog"

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




void userInterface::on_language_clicked()
{
   setlocale(LC_ALL,"rus");

    if (Lan){
        ui->language->setText(RLanguage);
        ui->mono_arch->setText(RMono);
        ui->poly_arch->setText(RPoly);
        ui->unarch->setText(RUn);
        ui->input->setText(RIn);
        ui->output->setText(ROut);
        ui->label->setText(RLab);
        Lan=false;
    }
    else
    {
        ui->language->setText(ELanguage);
        ui->mono_arch->setText(EMono);
        ui->poly_arch->setText(EPoly);
        ui->unarch->setText(EUn);
        ui->input->setText(EIn);
        ui->output->setText(EOut);
        ui->label->setText(ELab);
        Lan=true;
    }

}




void userInterface::on_input_clicked()
{
    QString input_file = QFileDialog::getOpenFileName(0,QObject::tr("Укажите файл"),QDir::homePath(), QObject::tr("Все файлы (*.*)"));
}


void userInterface::on_output_clicked()
{
    QString output_file = QFileDialog::getOpenFileName(0,QObject::tr("Укажите файл"),QDir::homePath(), QObject::tr("Все файлы (*.*)"));
}

