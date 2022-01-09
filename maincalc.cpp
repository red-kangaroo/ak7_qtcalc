#include "maincalc.h"
#include "ui_maincalc.h"
#include <QJSEngine>


MainCalc::MainCalc(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainCalc)
{
    ui->setupUi(this);
    ui->lineEdit->setText("0.0");

    const int btns = 18;
    QPushButton* numberButtons[btns] = {
        ui->pushButton_15,
        ui->pushButton_11,
        ui->pushButton_14,
        ui->pushButton_17,
        ui->pushButton_10,
        ui->pushButton_13,
        ui->pushButton_16,
        ui->pushButton_2,
        ui->pushButton_3,
        ui->pushButton_4,
        ui->pushButton_18,
        ui->pushButton_28,
        ui->pushButton_5,
        ui->pushButton_6,
        ui->pushButton_19,
        ui->pushButton_22,
        ui->pushButton_20,
        ui->pushButton_23
    };
    for(int i = 0; i < btns; i++) {
        connect(numberButtons[i], SIGNAL(released()), this, SLOT(pressNumber()));
    }
    connect(ui->pushButton_24, SIGNAL(released()), this, SLOT(pressEqual()));

}

MainCalc::~MainCalc()
{
    delete ui;
}

void MainCalc::pressNumber() {
    QPushButton* btn = (QPushButton*)sender();
    QString btnVal = btn->text();
    QString dispVal = ui->lineEdit->text();

//    if(dispVal.toDouble() == 0) {
    if(dispVal.isEmpty() //TODO
       || !QString::compare(dispVal, "0") || !QString::compare(dispVal, "0.0")
//       || !dispVal[0].isDigit() || !dispVal[dispVal.length()].isDigit()
       ) {
        ui->lineEdit->setText(btnVal);
    } else {
        QString newVal = dispVal + btnVal;
        ui->lineEdit->setText(newVal);
    }
}

void MainCalc::pressEqual() {
    QString dispVal = ui->lineEdit->text();
    QJSEngine expression;

    double resultVal = expression.evaluate(dispVal).toNumber();
    ui->lineEdit->setText(QString::number(resultVal, 'g', 16));
}
