#include "userinterface.hpp"
#include "ui_userinterface.h"
#include "zipcontroller.hpp"
#include "clocale"
#include "QFileDialog"
#include "QTextCodec"

userInterface::userInterface(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::userInterface)
{
  ui->setupUi(this);
  ui->back->hide();
  ui->confirm->hide();
  ui->label->hide();
  ui->input->hide();
  ui->output->hide();
}

userInterface::~userInterface()
{
  delete ui;
}




void userInterface::on_language_clicked()
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("CP1251"));
   //QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());

    if (Lan){
        ui->language->setText(RLanguage);
        ui->mono_arch->setText(RMono);
        ui->poly_arch->setText(RPoly);
        ui->unarch->setText(RUn);
        ui->input->setText(RIn);
        ui->output->setText(ROut);
        ui->label->setText(RLab);
        ui->back->setText(RBack);
        ui->confirm->setText(RConf);
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
        ui->confirm->setText(EConf);
        ui->back->setText(EBack);
        Lan=true;
    }

}




void userInterface::on_input_clicked()
{
    PathOfZip = QFileDialog::getOpenFileName(0,QObject::tr("Укажите файл"),QDir::homePath(), QObject::tr("Все файлы (*.*)"));
}


void userInterface::on_output_clicked()
{
    PathOfFiles = QFileDialog::getOpenFileName(0,QObject::tr("Укажите файл"),QDir::homePath(), QObject::tr("Все файлы (*.*)"));
}


void userInterface::on_mono_arch_clicked()
{
    Case = 2;
    ui->confirm->show();
    ui->mono_arch->hide();
    ui->poly_arch->hide();
    ui->unarch->hide();
    ui->input->show();
    ui->output->show();
    ui->label->show();
    ui->back->show();
}


void userInterface::on_unarch_clicked()
{
    Case = 3;
    ui->confirm->show();
    ui->mono_arch->hide();
    ui->poly_arch->hide();
    ui->unarch->hide();
    ui->back->show();
    ui->input->show();
    ui->output->show();
    ui->label->show();
}


void userInterface::on_back_clicked()
{
    Case = 0;
    ui->confirm->hide();
    ui->mono_arch->show();
    ui->poly_arch->show();
    ui->unarch->show();
    ui->back->hide();
    ui->input->hide();
    ui->output->hide();
    ui->label->hide();
    PathOfZip.clear();
    PathOfFiles.clear();
}


void userInterface::on_poly_arch_clicked()
{
    Case = 1;
    ui->confirm->show();
    ui->mono_arch->hide();
    ui->poly_arch->hide();
    ui->unarch->hide();
    ui->back->show();
    ui->input->show();
    ui->output->show();
    ui->label->show();
}


void userInterface::on_confirm_clicked()
{
    switch(Case)
    {
        case 1: //PolyArch
            zipController::setZip(PathOfZip);
            zipController::setFiles(PathOfFiles);
        break;
        case 2: //MonoArch
        zipController::setZip(PathOfZip);
        PathOfFiles.push_back("C:\\test\\");
        zipController::setFiles(PathOfFiles);
        zipController::dataToZip();
        break;
        case 3: //unArch
        zipController::setZip(PathOfZip);
        zipController::setFiles(PathOfFiles);
        break;
    }
    Case = 0;
    ui->confirm->hide();
    ui->mono_arch->show();
    ui->poly_arch->show();
    ui->unarch->show();
    ui->back->hide();
    ui->input->hide();
    ui->output->hide();
    ui->label->hide();
    PathOfZip.clear();
    PathOfFiles.clear();
}

