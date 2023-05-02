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
    QString RBack = "Назад";
    QString RLab = "Выберите ваши Файлы";
    QString RIn = "Ввод";
    QString ROut = "Вывод";
    QString RPoly= "Множественная архивация";
    QString RMono= "Архивация";
    QString RLanguage = "Русский";
    QString RUn = "Разархивация";
    QString ELab = "Choose your Files";
    QString EIn = "Input";
    QString EOut = "Output";
    QString EPoly = "Multiple archiving";
    QString EMono = "Archiving";
    QString ELanguage = "English";
    QString EBack = "Back";
    QString EUn = "Unarchiving";
    QString input_file;
    QString output_file;
    bool Lan = true;

private slots:

  void on_language_clicked();


  void on_input_clicked();

  void on_output_clicked();

  void on_mono_arch_clicked();

  void on_unarch_clicked();

  void on_back_clicked();

private:
  Ui::userInterface *ui;
};
#endif // USERINTERFACE_HPP
