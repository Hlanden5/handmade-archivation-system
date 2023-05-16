#ifndef USERINTERFACE_HPP
#define USERINTERFACE_HPP

#include <QMainWindow>
#include <zipcontroller.hpp>

QT_BEGIN_NAMESPACE
namespace Ui { class userInterface; }
QT_END_NAMESPACE

class userInterface : public QMainWindow, public zipController
{
  Q_OBJECT

public:
  userInterface(QWidget *parent = nullptr);
  ~userInterface();
    QString RName = "Введите название архива";
    QString RConf = "Подтвердить";
    QString RBack = "Назад";
    QString RLab = "Выберите ваши Файлы";
    QString RIn = "Ввод";
    QString ROut = "Вывод";
    QString RPoly= "Множественная архивация";
    QString RMono= "Архивация";
    QString RLanguage = "Русский";
    QString RUn = "Разархивация";
    QString EName = "Enter the name of the archive";
    QString ELab = "Choose your Files";
    QString EIn = "Input";
    QString EOut = "Output";
    QString EPoly = "Multiple archiving";
    QString EMono = "Archiving";
    QString ELanguage = "English";
    QString EConf = "Confirm";
    QString EBack = "Back";
    QString EUn = "Unarchiving";
    QString PathOfZip;
    QString PathOfFiles;
    QString ZName;
    bool Lan = true;
    int Case = 0;
    int i=0;

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

private:
  Ui::userInterface *ui;
};
#endif // USERINTERFACE_HPP
