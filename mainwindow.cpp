#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "calculator.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QWidget::setWindowTitle("Calculator");
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*------------------*/
/*---ORGANIZATION---*/
/*------------------*/

void MainWindow::on_btnClear_clicked()
{
    QString temp{};
    ui->txtInput->setText(temp);
    ui->txtResult->setText(temp);
}

void MainWindow::on_btnDelete_clicked()
{
    QString temp1 = ui->txtInput->text();
    if (temp1.size() != 0) {
        QString temp2{};
        for (size_t i = 0; i <temp1.size()-1; ++i) {
            temp2 += temp1[i];
        }
        ui->txtInput->setText(temp2);
    }
}

void MainWindow::on_btnOpenParen_clicked()
{
    QString temp = ui->txtInput->text();
    temp += "(";
    ui->txtInput->setText(temp);
}

void MainWindow::on_btnCloseParen_clicked()
{
    QString temp = ui->txtInput->text();
    temp += ")";
    ui->txtInput->setText(temp);
}

void MainWindow::on_btnOpenBracket_clicked()
{
    QString temp = ui->txtInput->text();
    temp += "{";
    ui->txtInput->setText(temp);
}

void MainWindow::on_btnCloseBrack_clicked()
{
    QString temp = ui->txtInput->text();
    temp += "}";
    ui->txtInput->setText(temp);
}

/*------------------*/
/*----OPERATIONS----*/
/*------------------*/

void MainWindow::on_btnEquals_clicked()
{
    QString temp = ui->txtInput->text();
    // temp += "=";
    // ui->txtInput->setText(temp);
    if (temp.size() != 0) {
        // broken
        QString result = calculate(temp);
        ui->txtResult->setText(result);
    }
}

void MainWindow::on_btnRemainder_clicked()
{
    QString temp = ui->txtInput->text();
    temp += "%";
    ui->txtInput->setText(temp);
}

void MainWindow::on_btnDiv_clicked()
{
    QString temp = ui->txtInput->text();
    temp += "/";
    ui->txtInput->setText(temp);
}

void MainWindow::on_btnMult_clicked()
{
    QString temp = ui->txtInput->text();
    temp += "*";
    ui->txtInput->setText(temp);
}

void MainWindow::on_btnSub_clicked()
{
    QString temp = ui->txtInput->text();
    temp += "-";
    ui->txtInput->setText(temp);
}

void MainWindow::on_btnAdd_clicked()
{
    QString temp = ui->txtInput->text();
    temp += "+";
    ui->txtInput->setText(temp);
}

void MainWindow::on_btnFactorial_clicked()
{
    QString temp = ui->txtInput->text();
    temp += "! ";
    ui->txtInput->setText(temp);
}

void MainWindow::on_btnPower_clicked()
{
    QString temp = ui->txtInput->text();
    temp += " pow";
    ui->txtInput->setText(temp);
}

void MainWindow::on_btnSqrt_clicked()
{
    QString temp = ui->txtInput->text();
    temp += " sqrt";
    ui->txtInput->setText(temp);
}

void MainWindow::on_btnDeclare_clicked()
{
    QString temp = ui->txtInput->text();
    temp += " let ";
    ui->txtInput->setText(temp);
}

/*------------------*/
/*-----NUMBERS------*/
/*------------------*/

void MainWindow::on_btnVariable_clicked()
{
    /* TODO: open a drop down menu to select a variable or input new one */
}

void MainWindow::on_btnDecimal_clicked()
{
    QString temp = ui->txtInput->text();
    temp += ".";
    ui->txtInput->setText(temp);
}

void MainWindow::on_btn0_clicked()
{
    QString temp = ui->txtInput->text();
    temp += "0";
    ui->txtInput->setText(temp);
}

void MainWindow::on_btn1_clicked()
{
    QString temp = ui->txtInput->text();
    temp += "1";
    ui->txtInput->setText(temp);
}

void MainWindow::on_btn2_clicked()
{
    QString temp = ui->txtInput->text();
    temp += "2";
    ui->txtInput->setText(temp);
}

void MainWindow::on_btn3_clicked()
{
    QString temp = ui->txtInput->text();
    temp += "3";
    ui->txtInput->setText(temp);
}

void MainWindow::on_btn4_clicked()
{
    QString temp = ui->txtInput->text();
    temp += "4";
    ui->txtInput->setText(temp);
}

void MainWindow::on_btn5_clicked()
{
    QString temp = ui->txtInput->text();
    temp += "5";
    ui->txtInput->setText(temp);
}

void MainWindow::on_btn6_clicked()
{
    QString temp = ui->txtInput->text();
    temp += "6";
    ui->txtInput->setText(temp);
}

void MainWindow::on_btn7_clicked()
{
    QString temp = ui->txtInput->text();
    temp += "7";
    ui->txtInput->setText(temp);
}

void MainWindow::on_btn8_clicked()
{
    QString temp = ui->txtInput->text();
    temp += "8";
    ui->txtInput->setText(temp);
}

void MainWindow::on_btn9_clicked()
{
    QString temp = ui->txtInput->text();
    temp += "9";
    ui->txtInput->setText(temp);
}
