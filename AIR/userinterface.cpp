#include "userinterface.hpp"
#include "ui_userinterface.h"
//#include "zipcontroller.hpp"
#include "clocale"
#include "QFileDialog"
#include "QTextCodec"
#include <QDir>

userInterface::userInterface(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::userInterface)
{

    ui->setupUi(this);
    ui->back->hide();
    ui->confirm->hide();
   // ui->label->hide();
    ui->input->hide();
    ui->output->hide();
    ui->zname->hide();
    ui->label_out->hide();
    ui->label_in->hide();
    ui->zname->setText(EName);
    ui->confirm->setEnabled(false);
    ui->line_in->hide();
    ui->line_out->hide();
    ui->warning->hide();
    ui->dir->hide();
    ui->file->hide();
    ui->line_in->setEnabled(false);
    ui->line_out->setEnabled(false);
    ui->list->hide();
//QString test = "да-да";
//ui->list->addItem(test);
//test = "да-";
}

userInterface::~userInterface()
{
  delete ui;
}




void userInterface::on_language_clicked()    //Смена языка
{
  //  QTextCodec::setCodecForLocale(QTextCodec::codecForName("CP1251"));
   //QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());

    if (Lan){

        switch(Case)
        {
            case 1: //dataFromMullZip
            ui->label_in->setText(RIn1);

            break;
            case 2: //dataFromZip
            ui->label_in->setText(RIn2);
            ui->label_out->setText(ROut2);
            break;
            case 3: //dataToZip
            if (chek_dire==1)
                ui->label_in->setText(RIn3F); else ui->label_in->setText(RIn3D);
            ui->label_out->setText(ROut3);
        break;
        }


        ui->dir->setText(RDir);
        ui->file->setText(RFile);
        ui->language->setText(RLanguage);
        ui->mono_unarch->setText(RMono);
        ui->poly_unarch->setText(RPoly);
        ui->arch->setText(RUn);
        ui->input->setText(RIn);
        ui->output->setText(ROut);
       // ui->label->setText(RLab);
        ui->back->setText(RBack);
        ui->confirm->setText(RConf);
        ui->zname->setText(RName);
        ui->warning->setText(RWarn);
        Lan=false;
    }
    else
    {
        switch(Case)
        {
            case 1: //dataFromMullZip
            ui->label_in->setText(EIn1);

            break;
            case 2: //dataFromZip
            ui->label_in->setText(EIn2);
            ui->label_out->setText(EOut2);
            break;
            case 3: //dataToZip
            if (chek_dire==1)
                ui->label_in->setText(EIn3F); else ui->label_in->setText(EIn3D);
            ui->label_out->setText(EOut3);
        break;
        }
        ui->dir->setText(EDir);
        ui->file->setText(EFile);
        ui->warning->setText(EWarn);
        ui->language->setText(ELanguage);
        ui->mono_unarch->setText(EMono);
        ui->poly_unarch->setText(EPoly);
        ui->arch->setText(EUn);
        ui->input->setText(EIn);
        ui->output->setText(EOut);
    //    ui->label->setText(ELab);
        ui->confirm->setText(EConf);
        ui->back->setText(EBack);
        ui->zname->setText(EName);
        Lan=true;
    }

}




void userInterface::on_input_clicked()
{
    setlocale(LC_CTYPE,"rus");
    ui->list->clear();
    zipFiles.clear();
    fileList.clear();

    switch(Case)
    {
        case 1: //dataFromMullZip
       if (Lan) PathOfFiles = QFileDialog::getExistingDirectory(this, tr("Select the folder with the archives"),"/home",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks); else
           PathOfFiles = QFileDialog::getExistingDirectory(this, tr("Выберите папку с архивами"),"/home",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        if (PathOfFiles.size()!=0){
        ui->line_in->setText(PathOfFiles);
        ui->confirm->setEnabled(true);}
        dir.setPath(PathOfFiles);
        fileList = dir.entryList(QDir::Files);
        //ui->list->addItem(fileList);
        for (const QString& file : fileList) {
            if (file.endsWith(".zip", Qt::CaseInsensitive)) {
                        zipFiles.append(file);
                    }

        }
        for (const QString& file : zipFiles) {
            ui->list->addItem(file);
        }



        break;
        case 2: //dataFromZip
        if (Lan) PathOfZip = QFileDialog::getOpenFileName(0,QObject::tr("Select archive"),QDir::homePath(), QObject::tr("Archive (*.zip*)")); else
        PathOfZip = QFileDialog::getOpenFileName(0,QObject::tr("Выберите архив"),QDir::homePath(), QObject::tr("Архив (*.zip*)"));
            if (PathOfZip.size()!=0){
        ui->line_in->setText(PathOfZip);

        if (PathOfFiles.size()!=0) ui->confirm->setEnabled(true);}

        break;
        case 3: //dataToZip
        if (Lan){
        if (chek_dire == 1) PathOfFiles = QFileDialog::getOpenFileName(0,QObject::tr("Select a file"),QDir::homePath(), QObject::tr("All Files (*.*)"));
        if (chek_dire == 2) PathOfFiles = QFileDialog::getExistingDirectory(this, tr("Select a folder"),"/home",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);} else
        {
            if (chek_dire == 1) PathOfFiles = QFileDialog::getOpenFileName(0,QObject::tr("Выберите файл"),QDir::homePath(), QObject::tr("Все Файлы (*.*)"));
            if (chek_dire == 2) PathOfFiles = QFileDialog::getExistingDirectory(this, tr("Выберите папку"),"/home",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);}


        if (PathOfFiles.size()!=0){
        ui->line_in->setText(PathOfFiles);

        if (PathOfZip.size()!=0 and ZName.size()!=0) ui->confirm->setEnabled(true);}

        break;
    }
}


void userInterface::on_output_clicked()
{
    switch(Case)
    {
        case 2: //dataFromZip
       if (Lan)  PathOfFiles = QFileDialog::getExistingDirectory(this, tr("Select a file"),"/home",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks); else
           PathOfFiles = QFileDialog::getExistingDirectory(this, tr("Выберите файл"),"/home",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
         if (PathOfFiles.size()!=0){
             ui->line_out->setText(PathOfFiles);
         //ui->line_out->show();
         if (PathOfZip.size()!=0) ui->confirm->setEnabled(true);}
        break;
        case 3: //dataToZip
       if (Lan) PathOfZip = QFileDialog::getExistingDirectory(this, tr("Select a file"),"/home",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks); else
        PathOfZip = QFileDialog::getExistingDirectory(this, tr("Выберите файл"),"/home",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        if (PathOfZip.size()!=0){ui->line_out->setText(PathOfZip);
        // ui->line_out->show();

        if (PathOfFiles.size()!=0 and ZName.size()!=0) ui->confirm->setEnabled(true);}

        break;
    }
}

void userInterface::on_mono_unarch_clicked()
{
    Case = 2;
    if (Lan) {ui->label_in->setText(EIn2);ui->label_out->setText(EOut2);} else {ui->label_in->setText(RIn2);ui->label_out->setText(ROut2);}
    ui->line_in->show();
    ui->line_out->show();
    ui->label_in->show();
    ui->label_out->show();
    ui->confirm->show();
    ui->mono_unarch->hide();
    ui->poly_unarch->hide();
    ui->arch->hide();
    ui->input->show();
    ui->output->show();
   // ui->label->show();
    ui->back->show();
}


void userInterface::on_arch_clicked()
{
    Case = 3;
    //ui->zname->show();
    //ui->confirm->show();
    if (Lan) {ui->label_in->setText(EIn2);ui->label_out->setText(EOut2);} else {ui->label_in->setText(RIn2);ui->label_out->setText(ROut2);}
    ui->mono_unarch->hide();
    ui->poly_unarch->hide();
    ui->arch->hide();
    ui->back->show();
    ui->dir->show();
    ui->file->show();
    //ui->input->show();
    //ui->output->show();
    //ui->label->show();
}


void userInterface::on_back_clicked()
{
    if (Lan) ui->zname->setText(EName); else  ui->zname->setText(RName);
    Case = 0;
    chek_dire = 0;
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
   // ui->label->hide();
    PathOfZip.clear();
    PathOfFiles.clear();
    ui->line_in->clear();
    ui->line_in->hide();
    ui->line_out->clear();
    ui->line_out->hide();
    ui->label_in->hide();
    ui->label_out->hide();
    ui->dir->hide();
    ui->file->hide();
    ui->list->clear();
    ui->list->hide();
    zipFiles.clear();
    fileList.clear();
}


void userInterface::on_poly_unarch_clicked()
{
    Case = 1;
    ui->line_in->show();
    if (Lan) {ui->label_in->setText(EIn1);} else {ui->label_in->setText(RIn1);}
    ui->label_in->show();
    ui->warning->show();
    ui->confirm->show();
    ui->mono_unarch->hide();
    ui->poly_unarch->hide();
    ui->arch->hide();
    ui->back->show();
    ui->input->show();
    ui->list->show();
 //   ui->label->show();
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
    chek_dire=0;
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
   // ui->label->hide();
    PathOfZip.clear();
    PathOfFiles.clear();
    ui->line_in->clear();
    ui->line_in->hide();
    ui->line_out->clear();
    ui->line_out->hide();
    ui->label_in->hide();
    ui->label_out->hide();
    ui->list->clear();
    ui->list->hide();
    zipFiles.clear();
    fileList.clear();
}


void userInterface::on_zname_editingFinished()
{

    ZName = ui->zname->text();
    if (!ZName.contains(".zip"))
        ZName.push_back(".zip");
    if (PathOfFiles.size()!=0 and PathOfFiles.size()!=0 and ZName.size()!=0)
        ui->confirm->setEnabled(true);

}


void userInterface::on_dir_clicked()
{
    chek_dire = 2;
    if (Lan) {ui->label_in->setText(EIn3D);ui->label_out->setText(EOut3);} else {ui->label_in->setText(RIn3D);ui->label_out->setText(ROut3);}
    ui->label_in->show();
    ui->label_out->show();
    ui->line_in->show();
    ui->line_out->show();
    ui->zname->show();
    ui->confirm->show();
    ui->input->show();
    ui->output->show();
  //  ui->label->show();
    ui->dir->hide();
    ui->file->hide();
}



void userInterface::on_file_clicked()
{
    chek_dire = 1;
    if (Lan) {ui->label_in->setText(EIn3F);ui->label_out->setText(EOut3);} else {ui->label_in->setText(RIn3D);ui->label_out->setText(ROut3);}

    ui->label_in->show();
    ui->label_out->show();
    ui->line_in->show();
    ui->line_out->show();
    ui->zname->show();
    ui->confirm->show();
    ui->input->show();
    ui->output->show();
  //  ui->label->show();
    ui->dir->hide();
    ui->file->hide();
}

