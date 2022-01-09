#ifndef MAINCALC_H
#define MAINCALC_H

#include <QMainWindow>

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

private slots:
    void pressNumber();
    void pressEqual();
    void pressDelete();
    void pressBackspace();

    void pressMemoryP();
    void pressMemoryM();
    void pressMemoryRecall();
    void pressMemoryClear();
};
#endif // MAINCALC_H
