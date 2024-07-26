#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void on_btnEquals_clicked();

    void on_btnOpenParen_clicked();

    void on_btnCloseParen_clicked();

    void on_btnOpenBracket_clicked();

    void on_btnCloseBrack_clicked();

    void on_btnRemainder_clicked();

    void on_btnFactorial_clicked();

    void on_btn7_clicked();

    void on_btn8_clicked();

    void on_btn9_clicked();

    void on_btnDiv_clicked();

    void on_btnPower_clicked();

    void on_btn4_clicked();

    void on_btn5_clicked();

    void on_btn6_clicked();

    void on_btnMult_clicked();

    void on_btnSqrt_clicked();

    void on_btn1_clicked();

    void on_btn2_clicked();

    void on_btn3_clicked();

    void on_btnSub_clicked();

    void on_btnDeclare_clicked();

    void on_btnDelete_clicked();

    void on_btn0_clicked();

    void on_btnDecimal_clicked();

    void on_btnAdd_clicked();

    void on_btnComma_clicked();

    void on_btnClear_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
