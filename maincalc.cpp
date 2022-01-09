#include "maincalc.h"
#include "ui_maincalc.h"
#include <QJSEngine>

/* TODO:
 *  abs, pi+e, %, log, ln
 *
 *  handle keys?
 *  handle Enter as =  -->  https://stackoverflow.com/questions/25637171/qt-key-pressevent-enter
 *  memory not lineEdit
 *  expression invisible as labels?
 *  history gets too long, make history a list
 *  M+, M-; memory a label?
 *
 *  trivia about numbers API??
 */

const QString defVal = "0";


MainCalc::MainCalc(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainCalc)
{
    ui->setupUi(this);
    ui->lineEdit->setText(defVal);

    // Symbol buttons (numbers and math signs):
    const int btns = 19;
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
        ui->pushButton_23,
        ui->pushButton_26
    };
    for(int i = 0; i < btns; i++) {
        connect(numberButtons[i], SIGNAL(released()), this, SLOT(pressNumber()));
    }
    // Individual buttons with special functions:
    connect(ui->pushButton_24, SIGNAL(released()), this, SLOT(pressEqual()));
    connect(ui->pushButton_30, SIGNAL(released()), this, SLOT(pressDelete()));
    connect(ui->pushButton_27, SIGNAL(released()), this, SLOT(pressBackspace()));
    connect(ui->pushButton_7, SIGNAL(released()), this, SLOT(pressMemoryP()));
    connect(ui->pushButton_8, SIGNAL(released()), this, SLOT(pressMemoryM()));
    connect(ui->pushButton, SIGNAL(released()), this, SLOT(pressMemoryRecall()));
    connect(ui->pushButton_9, SIGNAL(released()), this, SLOT(pressMemoryClear()));

}

MainCalc::~MainCalc()
{
    delete ui;
}

void MainCalc::pressNumber() {
    QPushButton* btn = (QPushButton*)sender();
    QString btnVal = btn->text();
    QString dispVal = ui->lineEdit->text();

    if(dispVal.isEmpty()
       || !QString::compare(dispVal, "0") || !QString::compare(dispVal, "0.0")
//       || !dispVal[0].isDigit() || !dispVal[dispVal.length()].isDigit()
       || !QString::compare(dispVal.toLower(), "nan")
       ) {
        ui->lineEdit->setText(btnVal);
    } else if(dispVal.toDouble() && !QString::compare(btnVal, "sqrt(")) {
        QString newVal = btnVal + dispVal;
        ui->lineEdit->setText(newVal);
    } else {
        QString newVal = dispVal + btnVal;
        ui->lineEdit->setText(newVal);
    }
}

void MainCalc::pressEqual() {
    // Evaluate expression:
    QString dispVal = ui->lineEdit->text();
    QString histVal = ui->label->text();
    QJSEngine expression;

    // Prepare history entry:
    histVal = histVal + "\n" + dispVal;
    // Special cases for certain functions:
    dispVal = dispVal.replace("^", "**");
    dispVal = dispVal.replace("sqrt", "Math.sqrt");

    QString resultVal = QString::number(expression.evaluate(dispVal).toNumber(), 'g', 16);
    ui->label->setText(histVal + " = " + resultVal);
    // Only set expression to result if we get a valid result:
    if(QString::compare(resultVal.toLower(), "nan"))
      ui->lineEdit->setText(resultVal);
}

void MainCalc::pressDelete() {
    // Remove whole expression:
    ui->lineEdit->setText(defVal);
}

void MainCalc::pressBackspace() {
    // Remove last character from expression:
    QString dispVal = ui->lineEdit->text();

    dispVal.chop(1);
    if(dispVal.isEmpty())
      dispVal = defVal;

    ui->lineEdit->setText(dispVal);
}

void MainCalc::pressMemoryP() {
    QString dispVal = ui->lineEdit->text();
    QString memVal = ui->lineEdit_2->text();
    // TODO
    ui->lineEdit_2->setText(dispVal);
}

void MainCalc::pressMemoryM() {

}

void MainCalc::pressMemoryRecall() {
    QString dispVal = ui->lineEdit->text();
    QString memVal = ui->lineEdit_2->text();
    ui->lineEdit->setText(dispVal + memVal);
}

void MainCalc::pressMemoryClear() {
    ui->lineEdit_2->setText("");
}
