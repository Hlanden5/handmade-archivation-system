#include "userinterface.hpp"
#include "ui_userinterface.h"
//#include "zipcontroller.hpp"
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
    ui->output_label->hide();
    ui->input_label->hide();
    ui->zname->setText(EName);
    ui->confirm->setEnabled(false);
    ui->l_in->hide();
    ui->l_out->hide();
    ui->warning->hide();
}

userInterface::~userInterface()
{
  delete ui;
}




void userInterface::on_language_clicked()    //Смена языка
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
        ui->l_in->setText(RIn);
        ui->l_out->setText(RIn);
        ui->label->setText(RLab);
        ui->back->setText(RBack);
        ui->confirm->setText(RConf);
        ui->zname->setText(RName);
        ui->warning->setText(RWarn);
        Lan=false;
    }
    else
    {
        ui->warning->setText(EWarn);
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
        ui->l_in->setText(EIn);
        ui->l_out->setText(EIn);
        Lan=true;
    }

}




void userInterface::on_input_clicked()
{
    setlocale(LC_CTYPE,"rus");
    switch(Case)
    {
        case 1: //dataFromMullZip
        PathOfFiles = QFileDialog::getExistingDirectory(this, tr("Specify the folder with the archives"),"/home",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        if (PathOfFiles.size()!=0){
        ui->input_label->setText(PathOfFiles);
        ui->input_label->show();
        ui->l_in->show();
        ui->confirm->setEnabled(true);}

        break;
        case 2: //dataFromZip
        PathOfZip = QFileDialog::getOpenFileName(0,QObject::tr("Select archive"),QDir::homePath(), QObject::tr("Archive (*.zip*)"));
        if (PathOfZip.size()!=0){
        ui->input_label->setText(PathOfZip);
        ui->input_label->show();
        ui->l_in->show();
        if (PathOfFiles.size()!=0) ui->confirm->setEnabled(true);}

        break;
        case 3: //dataToZip
        PathOfFiles = QFileDialog::getOpenFileName(0,QObject::tr("Select a file"),QDir::homePath(), QObject::tr("All Files (*.*)"));
        if (PathOfFiles.size()!=0){
        ui->input_label->setText(PathOfFiles);
        ui->input_label->show();
        ui->l_in->show();
        if (PathOfZip.size()!=0 and ZName.size()!=0) ui->confirm->setEnabled(true);}

        break;
    }
}


void userInterface::on_output_clicked()
{
    switch(Case)
    {
        case 2: //dataFromZip
         PathOfFiles = QFileDialog::getExistingDirectory(this, tr("Select a file"),"/home",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
         if (PathOfFiles.size()!=0){
             ui->output_label->setText(PathOfFiles);
         ui->output_label->show();
         ui->l_out->show();
         if (PathOfZip.size()!=0) ui->confirm->setEnabled(true);}
        break;
        case 3: //dataToZip
        PathOfZip = QFileDialog::getExistingDirectory(this, tr("Select a file"),"/home",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

        if (PathOfZip.size()!=0){ui->output_label->setText(PathOfZip);
        ui->output_label->show();
        ui->l_out->show();
        if (PathOfFiles.size()!=0 and ZName.size()!=0) ui->confirm->setEnabled(true);}

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
    if (Lan) ui->zname->setText(EName); else  ui->zname->setText(RName);
    Case = 0;
    ui->confirm->setEnabled(false);
    ui->input->setEnabled(true);
    ui->output->setEnabled(true);
    ui->zname->setEnabled(true);
    ui->zname->hide();
    ui->confirm->hide();
    ui->mono_unarch->show();
    ui->poly_unarch->show();
    ui->arch->show();
    ui->warning->hide();
    ui->back->hide();
    ui->input->hide();
    ui->output->hide();
    ui->label->hide();
    PathOfZip.clear();
    PathOfFiles.clear();
    ui->input_label->clear();
    ui->input_label->hide();
    ui->output_label->clear();
    ui->output_label->hide();
    ui->l_in->hide();
    ui->l_out->hide();
}


void userInterface::on_poly_unarch_clicked()
{
    Case = 1;
    ui->warning->show();
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
    if (!Lan) ui->zname->setText(RName); else  ui->zname->setText(RName);
    ui->zname->setEnabled(true);
    ui->confirm->setEnabled(false);
    ui->input->setEnabled(true);
    ui->output->setEnabled(true);
    ui->zname->hide();
    ui->warning->hide();
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
    ui->input_label->clear();
    ui->input_label->hide();
    ui->output_label->clear();
    ui->output_label->hide();
    ui->l_in->hide();
    ui->l_out->hide();
}


void userInterface::on_zname_editingFinished()
{

    ZName = ui->zname->text();
    if (!ZName.contains(".zip"))
        ZName.push_back(".zip");
    if (PathOfFiles.size()!=0 and PathOfFiles.size()!=0 and ZName.size()!=0)
        ui->confirm->setEnabled(true);

}

