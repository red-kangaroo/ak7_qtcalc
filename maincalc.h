#ifndef MAINCALC_H
#define MAINCALC_H

#include <QMainWindow>
#include <QJSEngine>


QT_BEGIN_NAMESPACE
namespace Ui { class MainCalc; }
QT_END_NAMESPACE

class MainCalc : public QMainWindow
{
    Q_OBJECT

public:
    MainCalc(QWidget *parent = nullptr);
    ~MainCalc();

private:
    Ui::MainCalc *ui;

    void displayHistory();
    void displayHelp();
    void displayWelcome();

private slots:
    void pressNumber();
    void pressEqual();
    void pressDelete();
    void pressBackspace();
    void pressLast();
    void pressHelp();

    void pressMemoryP();
    void pressMemoryM();
    void pressMemoryRecall();
    void pressMemoryClear();

    void pressBlank();
};
#endif // MAINCALC_H
