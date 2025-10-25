#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QLabel>
#include <QTimer>
#include <QtMath>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QList<QPushButton*> digitList = {ui->btn_0,ui->btn_1,ui->btn_2,ui->btn_3,ui->btn_4,ui->btn_5,ui->btn_6,ui->btn_7,ui->btn_8,ui->btn_9};
    QList<QPushButton*> TemperatureDigitList = {ui->btn_temp0,ui->btn_temp1,ui->btn_temp2,ui->btn_temp3,ui->btn_temp4,ui->btn_temp5,ui->btn_temp6,ui->btn_temp7,ui->btn_temp8,ui->btn_temp9};
    QList<QPushButton*> weightDigitList = {ui->btn_w0,ui->btn_w1,ui->btn_w2,ui->btn_w3,ui->btn_w4,ui->btn_w5,ui->btn_w6,ui->btn_w7,ui->btn_w8,ui->btn_w9};
    QList<QPushButton*> timeDigitList = {ui->btn_t0,ui->btn_t1,ui->btn_t2,ui->btn_t3,ui->btn_t4,ui->btn_t5,ui->btn_t6,ui->btn_t7,ui->btn_t8,ui->btn_t9};
    for (auto button : digitList) {
        connect(button, &QPushButton::clicked, this, &MainWindow::DigitClicked);
    }
    for (auto button : TemperatureDigitList) {
        connect(button, &QPushButton::clicked, this, &MainWindow::TemperatureDigitClicked);
    }
    for (auto button : weightDigitList) {
        connect(button, &QPushButton::clicked, this, &MainWindow::WeightDigitClicked);
    }
    for (auto button : timeDigitList) {
        connect(button, &QPushButton::clicked, this, &MainWindow::TimeDigitClicked);
    }

    QList<QPushButton*> operatorList = {ui->btn_add,ui->btn_divide,ui->btn_multiply,ui->btn_subtract,ui->btn_equal};

    for (auto button : operatorList) {
        connect(button, &QPushButton::clicked, this, &MainWindow::OperatorClicked);
    }
    ui->verticalLayout_3->setAlignment(Qt::AlignTop);
    ui->verticalWidget->setVisible(false);

    connect(ui->btn_hamburger, &QPushButton::clicked, this, [=]() {
        bool isVisible = ui->verticalWidget->isVisible();
        ui->verticalWidget->setVisible(!isVisible);
    });

    connect(ui->btn_temperature, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(ui->page_temperature);
    });
    connect(ui->btn_standart, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(ui->page_standart);
    });
    connect(ui->btn_weight, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(ui->page_weight);
    });
    connect(ui->btn_time, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(ui->page_time);
    });

    connect(ui->fromTemp,&QComboBox::currentTextChanged, this,&::MainWindow::convertTemperature);
    connect(ui->fromTempText,&QLineEdit::textChanged, this,&::MainWindow::convertTemperature);
    connect(ui->toTemp,&QComboBox::currentTextChanged, this,&::MainWindow::convertTemperature);

    connect(ui->fromWeight,&QComboBox::currentTextChanged, this,&::MainWindow::convertWeight);
    connect(ui->fromWeightText,&QLineEdit::textChanged, this,&::MainWindow::convertWeight);
    connect(ui->toWeight,&QComboBox::currentTextChanged, this,&::MainWindow::convertWeight);

    connect(ui->fromTime,&QComboBox::currentTextChanged, this,&::MainWindow::convertTime);
    connect(ui->fromTimeText,&QLineEdit::textChanged, this,&::MainWindow::convertTime);
    connect(ui->toTime,&QComboBox::currentTextChanged, this,&::MainWindow::convertTime);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::DigitClicked(){
    QString text = DigitClicked2(ui->lineEdit);
    ui->lineEdit->setText(text);

}

void MainWindow::OperatorClicked() {
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    operatorSign = button->text();
    if(operatorSign == "="){
        operatorSign = "";
    }
    QString currentText = ui->lineEdit->text();

    // Hızlı operatör değiştirme
    if (!currentText.isEmpty() && QString("+-x÷").contains(currentText.back())) {
        currentText.chop(1);
        currentText += operatorSign;
        ui->lineEdit->setText(currentText);
        return; // Burada işlem yapmaya gerek yok, sadece operator değişti
    }

    // Eğer zaten bir işlem yapıldıysa (calculate true ise)
    if (calculate) {
        QString operators = "+-x÷";
        int opIndex = -1;

        for (int i = 1; i < currentText.length(); i++) {
            if (operators.contains(currentText[i])) {
                opIndex = i;
                break;
            }
        }

        if (opIndex != -1) {
            QString left = currentText.left(opIndex);
            QString right = currentText.mid(opIndex + 1);
            input1 = left.toDouble();
            input2 = right.toDouble();

            QString op = currentText[opIndex];

            if(op == "+") input1 += input2;
            else if(op == "-") input1 -= input2;
            else if(op == "x") input1 *= input2;
            else if(op == "÷") {
                if(input2 != 0) input1 /= input2;
                else {
                    ui->lineEdit->setText("Error");
                    return;
                }
            }

            currentText = QString::number(input1); // sonucu göster
        }
    } else {
        input1 = currentText.toDouble();
        calculate = true;
    }

    // Yeni operatör ekle

    currentText += operatorSign;

    ui->lineEdit->setText(currentText);

}



void MainWindow::on_btn_clear_clicked()
{
    Clear(ui->lineEdit);
}


void MainWindow::on_btn_delete_clicked()
{
    Delete(ui->lineEdit);

}





void MainWindow::on_btn_dot_clicked()
{
    QString text = ui->lineEdit->text();
    bool dot =false;
    QString operators = "+-x÷";
    for(int i=0;i<text.length();i++){
        if(text[i] == "."){
            dot = true;
        }
        if(operators.contains(text[i])){
            dot = false;
        }
    }
    if(!dot){
        ui->lineEdit->setText(text+".");
    }



}


void MainWindow::on_btn_changeSign_clicked()
{
    QString currentText = ui->lineEdit->text();
    QString operators = "+-x÷";
    int opIndex = -1;

    for (int i = 1; i<currentText.length(); i++) {
        if (operators.contains(currentText[i])) {
            opIndex = i;
            break;
        }
    }
    if(opIndex != -1){
        QString left = currentText.left(opIndex+1);
        QString right = currentText.mid(opIndex+1);
        if(right.startsWith("-")){
            right.remove(0,1);
        }
        else if(right == ""){
            return;
        }else{
            right = "-"+right;
        }
        currentText = left + right;
    }else{
        double num = currentText.toDouble();
        num *= -1;
        currentText = QString::number(num);
    }
    ui->lineEdit->setText(currentText);
}


void MainWindow::on_btn_percent_clicked()
{
    QString currentText = ui->lineEdit->text();
    QString operators = "+-x÷";
    int opIndex = -1;
    QChar op;

    for (int i = 1; i<currentText.length(); i++) {
        if (operators.contains(currentText[i])) {
            opIndex = i;
            op = currentText[i];
            break;
        }
    }

    if(opIndex != -1){
        QString left = currentText.left(opIndex);
        QString right = currentText.mid(opIndex+1);
        double rightNum = right.toDouble();
        if(op == "+" || op=="-"){
            double leftNum = left.toDouble();
            rightNum = leftNum * (rightNum/100);
        }
        else{
            rightNum /= 100;
        }
        currentText = left + op +QString::number(rightNum);
    }else{

        currentText = "Error";
    }
    ui->lineEdit->setText(currentText);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void MainWindow::TemperatureDigitClicked(){
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (!button) return;
    if(ui->fromTempText->text() == "0") ui->fromTempText->setText("");
    ui->fromTempText->setText(ui->fromTempText->text()+button->text());
}

void MainWindow::on_btn_tempClear_clicked()
{
    Clear(ui->fromTempText);
}

void MainWindow::convertTemperature(){
    double value = ui->fromTempText->text().toDouble();
    QString from = ui->fromTemp->currentText();
    QString to = ui->toTemp->currentText();

    double celcius;

    if(from=="Celcius")
        celcius=value;
    else if(from == "Fahrenheit"){
        celcius = (value-32)*5.0/9.0;
    }
    else if (from == "Kelvin"){
        celcius = value - 273.15;
    }
    double result;
    if(to=="Celcius"){
        result = celcius;
    }else if(to=="Fahrenheit"){
        result = celcius * 9.0/5.0+32;
    }else if(to=="Kelvin"){
        result = celcius + 273.15;
    }

    ui->toTempText->setText(QString::number(result));

}








void MainWindow::on_btn_tempDot_clicked()
{
    DotClicked(ui->fromTempText);
}


void MainWindow::on_btn_tempChangeSign_clicked()
{
    double num = ui->fromTempText->text().toDouble();
    num *= -1;
    ui->fromTempText->setText(QString::number(num));

}

void MainWindow::on_btn_tempDelete_clicked()
{
    Delete(ui->fromTempText);
}

////////////////////////////////////////////////////////////////////////////////////////

static const QVector<double> weightFactors = {
    1e-6,    // mg
    1e-5,
    1e-4,
    1e-3,      // g
    1e-2,      // dag
    1e-1,      // hg
    1,         // kg
    1e3,       // ton
    0.0283495, // ounce
    0.453592   // pound
};


void MainWindow::WeightDigitClicked(){

    QString text = DigitClicked2(ui->fromWeightText);
    ui->fromWeightText->setText(text);
}

void MainWindow::on_btn_wClear_clicked()
{
    Clear(ui->fromWeightText);
}




void MainWindow::on_btn_wDelete_clicked()
{
    Delete(ui->fromWeightText);
}

void MainWindow::on_btn_wDot_clicked()
{
    DotClicked(ui->fromWeightText);
}

void MainWindow::convertWeight() {
    double value = ui->fromWeightText->text().toDouble();
    int fromIndex = ui->fromWeight->currentIndex();
    int toIndex   = ui->toWeight->currentIndex();


    double inKg = value * weightFactors[fromIndex];
    double result = inKg / weightFactors[toIndex];

    ui->toWeightText->setText(QString::number(result, 'g',15)); // e li verme durumu var
}

//////////////////////////////////////////////////////////////////////


static const QVector<double> timeFactors = {
    1.0 / 60000000.0, // µs -> minute
    1.0 / 60000.0,    // ms -> minute
    1.0 / 60.0,       // second -> minute
    1,                // minute
    60,               // hour
    1440,             // day
    10080,             // week
    525600            // year
};

void MainWindow::convertTime(){
    double value = ui->fromTimeText->text().toDouble();
    int fromIndex = ui->fromTime->currentIndex();
    int toIndex   = ui->toTime->currentIndex();


    double min = value * timeFactors[fromIndex];
    double result = min / timeFactors[toIndex];

    ui->toTimeText->setText(QString::number(result,'g',10));
}


void MainWindow::TimeDigitClicked(){

    QString text = DigitClicked2(ui->fromTimeText);
    ui->fromTimeText->setText(text);
}

void MainWindow::on_btn_tDelete_clicked()
{
    Delete(ui->fromTimeText);
}


void MainWindow::on_btn_tDot_clicked()
{
    DotClicked(ui->fromTimeText);
}


void MainWindow::on_btn_tClear_clicked()
{
    Clear(ui->fromTimeText);
}

//////////////////////////////////////////////////////////////////////
QString MainWindow::DigitClicked2(QLineEdit *lineEdit){
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (!button) return NULL;
    if(lineEdit->text() == "0" || lineEdit->text() == "Error") return button->text();
    return lineEdit->text()+button->text();

}

void MainWindow::Clear(QLineEdit *lineEdit){
    lineEdit->setText("0");
}

void MainWindow::DotClicked(QLineEdit *lineEdit){
    QString text = lineEdit->text();
    if(!text.contains('.')){
        lineEdit->setText(text+".");
    }else return;

}

void MainWindow::Delete(QLineEdit *lineEdit){
    QString currentText = lineEdit->text();
    if(currentText.length()>1){
        currentText.chop(1);
    }else{
        currentText = "0";
    }
    lineEdit->setText(currentText);
}
