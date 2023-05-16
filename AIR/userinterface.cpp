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
  ui->zname->hide();
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
        ui->mono_unarch->setText(RMono);
        ui->poly_unarch->setText(RPoly);
        ui->arch->setText(RUn);
        ui->input->setText(RIn);
        ui->output->setText(ROut);
        ui->label->setText(RLab);
        ui->back->setText(RBack);
        ui->confirm->setText(RConf);
        ui->zname->setText(RName);
        Lan=false;
    }
    else
    {
        ui->language->setText(ELanguage);
        ui->mono_unarch->setText(EMono);
        ui->poly_unarch->setText(EPoly);
        ui->arch->setText(EUn);
        ui->input->setText(EIn);
        ui->output->setText(EOut);
        ui->label->setText(ELab);
        ui->confirm->setText(EConf);
        ui->back->setText(EBack);
        ui->zname->setText(EName);
        Lan=true;
    }

}




void userInterface::on_input_clicked()
{
    switch(Case)
    {
        case 1: //dataFromMullZip
        PathOfFiles = QFileDialog::getExistingDirectory(this, tr("Укажите папку с архивами"),"/home",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        break;
        case 2: //dataFromZip
        PathOfZip = QFileDialog::getOpenFileName(0,QObject::tr("Укажите архив"),QDir::homePath(), QObject::tr("Все файлы (*.*)"));
        break;
        case 3: //dataToZip
        PathOfFiles = QFileDialog::getOpenFileName(0,QObject::tr("Укажите файл"),QDir::homePath(), QObject::tr("Все файлы (*.*)"));
        break;
    }
}


void userInterface::on_output_clicked()
{
    switch(Case)
    {
        case 2: //dataFromZip
         PathOfFiles = QFileDialog::getExistingDirectory(this, tr("Укажите папку"),"/home",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        break;
        case 3: //dataToZip
        PathOfZip = QFileDialog::getExistingDirectory(this, tr("Укажите папку"),"/home",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        break;
    }
}


void userInterface::on_mono_unarch_clicked()
{
    Case = 2;
    ui->confirm->show();
    ui->mono_unarch->hide();
    ui->poly_unarch->hide();
    ui->arch->hide();
    ui->input->show();
    ui->output->show();
    ui->label->show();
    ui->back->show();
}


void userInterface::on_arch_clicked()
{
    Case = 3;
    ui->zname->show();
    ui->confirm->show();
    ui->mono_unarch->hide();
    ui->poly_unarch->hide();
    ui->arch->hide();
    ui->back->show();
    ui->input->show();
    ui->output->show();
    ui->label->show();
}


void userInterface::on_back_clicked()
{
    if (Lan) ui->zname->setText(RName); else  ui->zname->setText(RName);
    Case = 0;
    ui->zname->hide();
    ui->confirm->hide();
    ui->mono_unarch->show();
    ui->poly_unarch->show();
    ui->arch->show();
    ui->back->hide();
    ui->input->hide();
    ui->output->hide();
    ui->label->hide();
    PathOfZip.clear();
    PathOfFiles.clear();
}


void userInterface::on_poly_unarch_clicked()
{
    Case = 1;
    ui->confirm->show();
    ui->mono_unarch->hide();
    ui->poly_unarch->hide();
    ui->arch->hide();
    ui->back->show();
    ui->input->show();
    ui->label->show();
}


void userInterface::on_confirm_clicked()
{
    switch(Case)
    {
        case 1: //dataFromMullZip
            zipController::setFiles(PathOfFiles);
            zipController::setZip(PathOfFiles);
            zipController::dataFromMulZip();
        break;
        case 2: //dataFromZip
        zipController::setZip(PathOfZip);
        zipController::setFiles(PathOfFiles);
        zipController::dataFromZip();
        break;
        case 3: //dataToZip
        PathOfZip.push_back("//");
        PathOfZip.push_back(ZName);
        zipController::setZip(PathOfZip);

        zipController::setFiles(PathOfFiles);
        zipController::dataToZip();
        break;
    }
    Case = 0;
    if (Lan) ui->zname->setText(RName); else  ui->zname->setText(RName);

    ui->zname->hide();
    ui->confirm->hide();
    ui->mono_unarch->show();
    ui->poly_unarch->show();
    ui->arch->show();
    ui->back->hide();
    ui->input->hide();
    ui->output->hide();
    ui->label->hide();
    PathOfZip.clear();
    PathOfFiles.clear();
}


void userInterface::on_zname_editingFinished()
{
    ZName = ui->zname->text();
    if (!ZName.contains(".zip"))
        ZName.push_back(".zip");

}

