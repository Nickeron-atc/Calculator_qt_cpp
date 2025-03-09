#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include <optional>

#include "calculator.h"

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

private:
    enum TAGS {
        NONE                = 0,

        PLUS                = 1 << 1,
        MINUS               = 1 << 2,
        MULTIPLY            = 1 << 3,
        DIVIDE              = 1 << 4,

        PERCENT             = 1 << 5,
        SIGN                = 1 << 6,

        EQUALS              = 1 << 7,
        DOT                 = 1 << 8,
        CLEAR               = 1 << 9,

        ZERO                = 1 << 10,
        FLOATING_NUMBER     = (1 << 11) | ZERO,
        INTEGER_NUMBER      = (1 << 12) | ZERO,

        OPENNING_BRACKET    = 1 << 13,
        CLOSING_BRACKET     = 1 << 14,

        BINARY_OPERATION    = TAGS::PLUS | TAGS::MINUS | TAGS::MULTIPLY | TAGS::DIVIDE,
        UNARY_OPERATION     = PERCENT | SIGN,
        NUMBER              = FLOATING_NUMBER | INTEGER_NUMBER,
        BRACKET             = OPENNING_BRACKET | CLOSING_BRACKET
    };

    Ui::MainWindow *ui;

    QTimer *timer1, *timer2;

    QWidget *secretMenu;

    Calculator calculator;

    int ExpressionMaxSize;
    std::string expression;

    int brackets_counter;

    TAGS lastTag;

    bool divisionByZero;
    bool floatingNumber;
    bool valueIsZero;
    bool operationInserted;
    bool equalsButtonPressed;
    bool timerInterrupted;

};
#endif // MAINWINDOW_H
