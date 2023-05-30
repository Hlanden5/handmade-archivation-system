#ifndef USERINTERFACE_HPP
#define USERINTERFACE_HPP

#include <QMainWindow>
#include <zipcontroller.hpp>
#include <QDir>

QT_BEGIN_NAMESPACE
namespace Ui { class userInterface; }
QT_END_NAMESPACE

class userInterface : public QMainWindow, public zipController
{
  Q_OBJECT

public:
  userInterface(QWidget *parent = nullptr);
  ~userInterface();
    int chek_dire = 0;
    QDir dir;
    QStringList fileList;
    QStringList zipFiles;
    QString EIn1 = "Select a folder with ZIP archives";
    QString EIn2 = "Select ZIP archive";
    QString EIn3F = "Select the file you want to archive";
    QString EIn3D = "Select the folder you want to archive";

    QString RIn1= "Выберите папку с ZIP архивами";
    QString RIn2= "Выберите ZIP архив";
    QString RIn3F = "Выебрите файл, который хотите архивировать";
    QString RIn3D = "Выберите папку, которую хотите архивировать";


    QString ROut2 = "Выберите папку, для разархивированных файлов";
    QString ROut3 = "Выберите папку, для ZIP архива";

    QString EOut2 = "Select the folder for the unzipped files";
    QString EOut3 = "Select a folder for the ZIP archive";


    QString RWarn = "Будьте осторожны при выборе папки с архивами";
    QString RName = "Введите название ZIP архива";
    QString RConf = "Подтвердить";
    QString RBack = "Назад";
    QString RLab = "Выберите нужные файлы";

    QString RIn = "Обзор";
    QString ROut = "Обзор";
    QString RPoly= "Множественная разархивация";
    QString RMono= "Разархивация";
    QString RLanguage = "Русский";
    QString RUn = "Архивация";
    QString RDir = "Папка";
    QString RFile = "Файл";
    QString EName = "Enter the name of the ZIP archive";
    QString ELab = "Choose your Files";
    QString EIn = "Review";
    QString EOut = "Review";
    QString EPoly = "Multiple unarchiving";
    QString EMono = "Unarchiving";
    QString ELanguage = "English";
    QString EConf = "Confirm";
    QString EBack = "Back";
    QString EUn = "Archiving";
    QString EWarn = "Be careful when choosing a folder with archives";
    QString EDir = "Directory";
    QString EFile = "File";
    QString PathOfZip;
    QString PathOfFiles;
    QString ZName;
    bool Lan = true;
    int Case = 0;
private slots:

  void on_language_clicked();


  void on_input_clicked();

  void on_output_clicked();

  void on_mono_unarch_clicked();

  void on_arch_clicked();

  void on_back_clicked();

  void on_poly_unarch_clicked();

  void on_confirm_clicked();

  void on_zname_editingFinished();

  void on_dir_clicked();

  void on_file_clicked();

private:
  Ui::userInterface *ui;
};
#endif // USERINTERFACE_HPP
