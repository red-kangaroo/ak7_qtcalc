#include "maincalc.h"
#include "ui_maincalc.h"

/* TODO:
 *  dice?, FUN
 *  welcome and help screen text
 *
 *  handle keys?
 *  handle Enter as =  -->  https://stackoverflow.com/questions/25637171/qt-key-pressevent-enter
 *  Space to start editing, enter for =
 *
 *  make expression lineEdit as invisible as labels?
 *
 *  trivia about numbers API??
 */

const QString defVal = "0";
const int calcHistDepth = 6;
QList<QString> calcHistory;


MainCalc::MainCalc(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainCalc)
{
    ui->setupUi(this);

    // Text field setup:
    ui->lineEdit->setText(defVal);
    displayWelcome();

    // Symbol buttons (numbers and math signs):
    const int btns = 25;
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
        ui->pushButton_26,
        ui->pushButton_29,
        ui->pushButton_21,
        ui->pushButton_12,
        ui->pushButton_31,
        ui->pushButton_32,
        ui->pushButton_33
    };
    for(int i = 0; i < btns; i++) {
        connect(numberButtons[i], SIGNAL(released()), this, SLOT(pressNumber()));
    }
    // Individual buttons with special functions:
    connect(ui->pushButton_24, SIGNAL(released()), this, SLOT(pressEqual()));
    connect(ui->pushButton_30, SIGNAL(released()), this, SLOT(pressDelete()));
    connect(ui->pushButton_27, SIGNAL(released()), this, SLOT(pressBackspace()));
    connect(ui->pushButton_35, SIGNAL(released()), this, SLOT(pressHelp()));
    connect(ui->pushButton_25, SIGNAL(released()), this, SLOT(pressLast()));
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
    } else if((dispVal.toDouble() || !QString::compare(btnVal, "pi") || !QString::compare(btnVal, "e")) &&
              (!QString::compare(btnVal, "sqrt(") || !QString::compare(btnVal, "abs(")
               || !QString::compare(btnVal, "log(") || !QString::compare(btnVal, "ln("))) {
        // If there is only a number in expression, function should go before it.
        // Don't handle not-just-number expressions, too tough. ;)
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
    QString newHist;
    QJSEngine expression;

    // CLose brackets that the user has forgotten:
    int bra = dispVal.count(QChar('('));
    int ket = dispVal.count(QChar(')'));
    if(bra > ket) {
        dispVal += QString(")").repeated(bra - ket);
    }

    // Prepare history entry:
    newHist = dispVal;
    // Special cases for certain functions:
    dispVal = dispVal.replace("^", "**");
    dispVal = dispVal.replace("sqrt", "Math.sqrt");
    dispVal = dispVal.replace("abs", "Math.abs");
    dispVal = dispVal.replace("log", "Math.log10");
    dispVal = dispVal.replace("ln", "Math.log");
    dispVal = dispVal.replace("pi", "Math.PI");
    dispVal = dispVal.replace("e", "Math.E");

    QString resultVal = QString::number(expression.evaluate(dispVal).toNumber(), 'g', 16);
    calcHistory.append(newHist + " = " + resultVal);
    displayHistory();

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
    QString memVal = ui->label_2->text();

    if(memVal.isEmpty()
       || !QString::compare(memVal, "0") || !QString::compare(memVal, "0.0")
       || !QString::compare(memVal.toLower(), "nan")
       ) {
        ui->label_2->setText(dispVal);
    } else {
        QString newVal = memVal + "+" + dispVal;
        ui->label_2->setText(newVal);
    }
}

void MainCalc::pressMemoryM() {
    QString dispVal = ui->lineEdit->text();
    QString memVal = ui->label_2->text();

    if(memVal.isEmpty()
       || !QString::compare(memVal, "0") || !QString::compare(memVal, "0.0")
       || !QString::compare(memVal.toLower(), "nan")
       ) {
        ui->label_2->setText(dispVal);
    } else {
        QString newVal = memVal + "-" + dispVal;
        ui->label_2->setText(newVal);
    }
}

void MainCalc::pressMemoryRecall() {
    QString dispVal = ui->lineEdit->text();
    QString memVal = ui->label_2->text();
    ui->lineEdit->setText(dispVal + memVal);
}

void MainCalc::pressMemoryClear() {
    ui->label_2->setText(defVal);
}

void MainCalc::pressLast() {
    if(!calcHistory.isEmpty()) {
        QStringList lstVal = calcHistory.takeLast().split(QString(" = "), Qt::SkipEmptyParts);
        if(!lstVal.isEmpty())
            ui->lineEdit->setText(lstVal[0]);
    }
    displayHistory();
}

void MainCalc::pressHelp() {
    displayHelp();
}

void MainCalc::displayHistory() {
    ui->label->setAlignment(Qt::AlignRight | Qt::AlignBottom);
    QListIterator<QString> riterHist(calcHistory);
    riterHist.toBack();
    QString histVal = "";
    int cnt = 0;

    while(riterHist.hasPrevious()) {
      if(cnt >= calcHistDepth) {
          break;
      } else if(histVal.isEmpty())
          histVal = riterHist.previous();
      else
          histVal = riterHist.previous() + "\n" + histVal;
      cnt++;
    }

    ui->label->setText(histVal);
}

void MainCalc::displayHelp() {
    ui->label->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    QString helpVal = "<pre>[Help file for QtCalculator]\n"
                      "DEL delete all     BCK backspace  LST last expression\n"
                      "MR  memory recall  MC  memory clear\n"
                      "M±  put whole expression to memory with given sign\n"
                      "\n"
                      "Press [Help] again to return to calculation history.</pre>";

    if(!QString::compare(ui->label->text(), helpVal))
        displayHistory();
    else
        ui->label->setText(helpVal);
}

void MainCalc::displayWelcome() {
    ui->label->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    QString startVal = "Welcome to QtCalculator!\n"
                       "TODO";

    ui->label->setText(startVal);
}
