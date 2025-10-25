#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QPropertyAnimation>
#include <QLineEdit>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void DigitClicked();

    void on_btn_clear_clicked();

    void on_btn_delete_clicked();

    void OperatorClicked();

    void on_btn_dot_clicked();

    void on_btn_changeSign_clicked();

    void on_btn_percent_clicked();

    void TemperatureDigitClicked();

    void on_btn_tempClear_clicked();

    void convertTemperature();


    void on_btn_tempDot_clicked();

    void on_btn_tempChangeSign_clicked();

    void WeightDigitClicked();


    void on_btn_wClear_clicked();

    void on_btn_tempDelete_clicked();

    void on_btn_wDelete_clicked();

    void convertWeight();

    void on_btn_wDot_clicked();

    void TimeDigitClicked();

    void on_btn_tDelete_clicked();

    void on_btn_tDot_clicked();

    void on_btn_tClear_clicked();

    void convertTime();

    QString DigitClicked2(QLineEdit *lineEdit);

    void Clear(QLineEdit *lineEdit);

    void DotClicked(QLineEdit *lineEdit);

    void Delete(QLineEdit *lineEdit);
private:
    Ui::MainWindow *ui;
    QWidget* menuPanel;         // Hamburger menüsü
    QPropertyAnimation* menuAnim;

    double input1 = 0;
    double input2 = 0;
    QString operatorSign = "";
    bool calculate = false;

};
#endif // MAINWINDOW_H
