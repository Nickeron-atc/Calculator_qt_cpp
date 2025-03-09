#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QVBoxLayout>

#include <vector>
#include <string>

#include <ctime>

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "calculator.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    calculator(""),
    timer1(new QTimer(this)),
    timer2(new QTimer(this))
{
    ui->setupUi(this);

    //Ui::MainWindow *ui;

    int ExpressionMaxSize = 24;
    int ExpressionMaxSize2 = 33;
    int ExpressionMaxSizeResult = 10;
    int ExpressionMaxSizeResult2 = 17;
    expression = "0";

    lastTag = TAGS::NONE;

    //this->timer1->setSingleShot(true);
    //this->timer1->setInterval(4000);
    //this->timer2->setSingleShot(true);
    //this->timer2->setInterval(5000);

    this->divisionByZero = false;
    this->floatingNumber = false;
    this->valueIsZero = true;
    this->operationInserted = false;
    this->equalsButtonPressed = false;
    this->timerInterrupted = false;
    brackets_counter = 0;

    auto update_text_labels = [this,
                               ExpressionMaxSize,
                               ExpressionMaxSize2,
                               ExpressionMaxSizeResult,
                               ExpressionMaxSizeResult2](){
        std::string printable_expression = this->expression;

        /* MULTI BYTE CHARACTER ENCODING REQUIRED
        for (int i = 0; i < printable_expression.length(); i++) {
            if (printable_expression[i] == '*') printable_expression[i] = '×';
            if (printable_expression[i] == '/') printable_expression[i] = '÷';
        }
        //*/

        if (this->expression.length() >= ExpressionMaxSize && this->expression.length() < ExpressionMaxSize2) {
            this->ui->input->setStyleSheet(
                "qproperty-alignment: \"AlignVCenter | AlignRight\";"
                "color: \"white\";"
                "background-color: \"#04BFAD\";"
                "font-family: 'Open Sans', sans-serif;"
                "font-weight: 600;          /* Semibold */"
                "font-size: 15px;           /* Размер шрифта */"
                "line-height: 30px;         /* Межстрочный интервал */"
                "letter-spacing: 0.5px;     /* Межбуквенный интервал */"
                );
        }
        else if (this->expression.length() >= ExpressionMaxSize2) {
            this->ui->input->setStyleSheet(
                "qproperty-alignment: \"AlignVCenter | AlignRight\";"
                "color: \"white\";"
                "background-color: \"#04BFAD\";"
                "font-family: 'Open Sans', sans-serif;"
                "font-weight: 600;          /* Semibold */"
                "font-size: 10px;           /* Размер шрифта */"
                "line-height: 30px;         /* Межстрочный интервал */"
                "letter-spacing: 0.5px;     /* Межбуквенный интервал */"
                );
        }
        if ((this->lastTag == MainWindow::TAGS::EQUALS
             || this->lastTag == MainWindow::TAGS::NONE
             || !this->operationInserted)
            && this->expression.length() >= ExpressionMaxSizeResult
            && this->expression.length() < ExpressionMaxSizeResult2) {
            this->ui->result->setStyleSheet(
                "qproperty-alignment: \"AlignVCenter | AlignRight\";"
                "color: \"white\";"
                "background-color: \"#04BFAD\";"
                "font-family: 'Open Sans', sans-serif;"
                "font-size: 25px;"
                "font-weight: 600;          /* Semibold */"
                "line-height: 60px;         /* Межстрочный интервал */"
                "letter-spacing: 0.5px;     /* Межбуквенный интервал */"
                "border-radius: 36px;"
                );
        }
        if ((this->lastTag == MainWindow::TAGS::EQUALS
             || this->lastTag == MainWindow::TAGS::NONE
             || !this->operationInserted)
            && this->expression.length() >= ExpressionMaxSizeResult2) {
            this->ui->result->setStyleSheet(
                "qproperty-alignment: \"AlignVCenter | AlignRight\";"
                "color: \"white\";"
                "background-color: \"#04BFAD\";"
                "font-family: 'Open Sans', sans-serif;"
                "font-size: 20px;"
                "font-weight: 600;          /* Semibold */"
                "line-height: 60px;         /* Межстрочный интервал */"
                "letter-spacing: 0.5px;     /* Межбуквенный интервал */"
                "border-radius: 36px;"
                );
        }

        if (this->operationInserted && this->lastTag != EQUALS)
            this->ui->input->setText(printable_expression.c_str());
        else
            this->ui->result->setText(printable_expression.c_str());


        return 0;
    };
    update_text_labels();

    connect(ui->pushButton_clear, &QPushButton::clicked, this, [this, update_text_labels] () {
        this->expression = "0";

        this->divisionByZero = false;
        this->valueIsZero = true;
        this->operationInserted = false;
        this->floatingNumber = false;

        this->brackets_counter = 0;

        this->ui->input->setStyleSheet(
            "qproperty-alignment: \"AlignVCenter | AlignRight\";"
            "color: \"white\";"
            "background-color: \"#04BFAD\";"
            "font-family: 'Open Sans', sans-serif;"
            "font-weight: 600;          /* Semibold */"
            "font-size: 20px;           /* Размер шрифта */"
            "line-height: 30px;         /* Межстрочный интервал */"
            "letter-spacing: 0.5px;     /* Межбуквенный интервал */"
        );

        this->lastTag = TAGS::CLEAR;
        this->ui->input->setText("");
        update_text_labels();
    });

    connect(ui->pushButton_0, &QPushButton::clicked, this, [this, update_text_labels] () {
        if (this->lastTag == TAGS::DIVIDE);

        if (this->divisionByZero || this->valueIsZero) { return; }

        if (this->lastTag == TAGS::CLOSING_BRACKET) {
            this->expression += " * 0";
        }
        else if(this->lastTag == TAGS::BINARY_OPERATION){
            this->expression += " 0";
        }
        else {
            this->expression += "0";
        }

        this->lastTag = TAGS::NUMBER;
        update_text_labels();
    });

    connect(ui->pushButton_1, &QPushButton::clicked, this, [this, update_text_labels] () {
        if (this->divisionByZero) { return; }

        if (this->valueIsZero && !this->operationInserted) {
            expression = "";
            this->valueIsZero = false;
        }

        if (this->lastTag == TAGS::CLOSING_BRACKET) {
            this->expression += " * 1";
        }
        else if(this->lastTag == TAGS::BINARY_OPERATION){
            this->expression += " 1";
        }
        else {
            this->expression += "1";
        }

        this->ui->pushButton_1->setStyleSheet(
            "border-radius: 33px;"
            "background: \"#B0D1D8\";"
            "font-family: 'Open Sans', sans-serif;"
            "font-weight: 600;"
            "font-size: 24px;"
            "line-height: 30px;"
            "letter-spacing: 1px;"
        );

        this->lastTag = TAGS::NUMBER;
        update_text_labels();
    });

    connect(ui->pushButton_2, &QPushButton::clicked, this, [this, update_text_labels] () {
        if (this->divisionByZero) { return; }

        if (this->valueIsZero && !this->operationInserted) {
            expression = "";
            this->valueIsZero = false;
        }

        if (this->lastTag == TAGS::CLOSING_BRACKET) {
            this->expression += " * 2";
        }
        else if(this->lastTag == TAGS::BINARY_OPERATION){
            this->expression += " 2";
        }
        else {
            this->expression += "2";
        }

        this->lastTag = TAGS::NUMBER;
        update_text_labels();
    });

    connect(ui->pushButton_3, &QPushButton::clicked, this, [this, update_text_labels] () {
        if (this->divisionByZero) { return; }

        if (this->valueIsZero && !this->operationInserted) {
            expression = "";
            this->valueIsZero = false;
        }

        if (this->lastTag == TAGS::CLOSING_BRACKET) {
            this->expression += " * 3";
        }
        else if(this->lastTag == TAGS::BINARY_OPERATION){
            this->expression += " 3";
        }
        else {
            this->expression += "3";
        }

        this->lastTag = TAGS::NUMBER;
        update_text_labels();
    });

    connect(ui->pushButton_4, &QPushButton::clicked, this, [this, update_text_labels] () {
        if (this->divisionByZero) { return; }

        if (this->valueIsZero && !this->operationInserted) {
            expression = "";
            this->valueIsZero = false;
        }

        if (this->lastTag == TAGS::CLOSING_BRACKET) {
            this->expression += " * 4";
        }
        else if(this->lastTag == TAGS::BINARY_OPERATION){
            this->expression += " 4";
        }
        else {
            this->expression += "4";
        }

        this->lastTag = TAGS::NUMBER;
        update_text_labels();
    });

    connect(ui->pushButton_5, &QPushButton::clicked, this, [this, update_text_labels] () {
        if (this->divisionByZero) { return; }

        if (this->valueIsZero && !this->operationInserted) {
            expression = "";
            this->valueIsZero = false;
        }

        if (this->lastTag == TAGS::CLOSING_BRACKET) {
            this->expression += " * 5";
        }
        else if(this->lastTag == TAGS::BINARY_OPERATION){
            this->expression += " 5";
        }
        else {
            this->expression += "5";
        }

        this->lastTag = TAGS::NUMBER;
        update_text_labels();
    });

    connect(ui->pushButton_6, &QPushButton::clicked, this, [this, update_text_labels] () {
        if (this->divisionByZero) { return; }

        if (this->valueIsZero && !this->operationInserted) {
            expression = "";
            this->valueIsZero = false;
        }

        if (this->lastTag == TAGS::CLOSING_BRACKET) {
            this->expression += " * 6";
        }
        else if(this->lastTag == TAGS::BINARY_OPERATION){
            this->expression += " 6";
        }
        else {
            this->expression += "6";
        }

        this->lastTag = TAGS::NUMBER;
        update_text_labels();
    });

    connect(ui->pushButton_7, &QPushButton::clicked, this, [this, update_text_labels] () {
        if (this->divisionByZero) { return; }

        if (this->valueIsZero && !this->operationInserted) {
            expression = "";
            this->valueIsZero = false;
        }

        if (this->lastTag == TAGS::CLOSING_BRACKET) {
            this->expression += " * 7";
        }
        else if(this->lastTag == TAGS::BINARY_OPERATION){
            this->expression += " 7";
        }
        else {
            this->expression += "7";
        }

        this->lastTag = TAGS::NUMBER;
        update_text_labels();
    });

    connect(ui->pushButton_8, &QPushButton::clicked, this, [this, update_text_labels] () {
        if (this->divisionByZero) { return; }

        if (this->valueIsZero && !this->operationInserted) {
            expression = "";
            this->valueIsZero = false;
        }

        if (this->lastTag == TAGS::CLOSING_BRACKET) {
            this->expression += " * 8";
        }
        else if(this->lastTag == TAGS::BINARY_OPERATION){
            this->expression += " 8";
        }
        else {
            this->expression += "8";
        }

        this->lastTag = TAGS::NUMBER;
        update_text_labels();
    });

    connect(ui->pushButton_9, &QPushButton::clicked, this, [this, update_text_labels] () {
        if (this->divisionByZero) { return; }

        if (this->valueIsZero && !this->operationInserted) {
            expression = "";
            this->valueIsZero = false;
        }

        if (this->lastTag == TAGS::CLOSING_BRACKET) {
            this->expression += " * 9";
        }
        else if(this->lastTag == TAGS::BINARY_OPERATION){
            this->expression += " 9";
        }
        else {
            this->expression += "9";
        }

        this->lastTag = TAGS::NUMBER;
        update_text_labels();
    });

    connect(ui->pushButton_add, &QPushButton::clicked, this, [this, update_text_labels] () {
        if (this->divisionByZero) { return; }

        if (this->valueIsZero) {
            this->valueIsZero = false;
        }

        this->expression += " +";

        this->operationInserted = true;
        this->lastTag = TAGS::BINARY_OPERATION;
        update_text_labels();
    });

    connect(ui->pushButton_sub, &QPushButton::clicked, this, [this, update_text_labels] () {
        if (this->divisionByZero) { return; }

        if (this->valueIsZero) {
            this->valueIsZero = false;
        }

        this->expression += " -";

        this->operationInserted = true;
        this->lastTag = TAGS::BINARY_OPERATION;
        update_text_labels();
    });

    connect(ui->pushButton_mult, &QPushButton::clicked, this, [this, update_text_labels] () {
        if (this->divisionByZero) { return; }

        if (this->valueIsZero) {
            this->valueIsZero = false;
        }

        this->expression += " *";

        this->operationInserted = true;
        this->lastTag = TAGS::BINARY_OPERATION;
        update_text_labels();
    });

    connect(ui->pushButton_div, &QPushButton::clicked, this, [this, update_text_labels] () {
        if (this->divisionByZero) { return; }

        if (this->valueIsZero) {
            this->valueIsZero = false;
        }

        this->expression += " /";

        this->operationInserted = true;
        this->lastTag = TAGS::BINARY_OPERATION;
        update_text_labels();
    });

    connect(ui->pushButton_equals, &QPushButton::clicked, this, [this, update_text_labels] () {
        if (this->expression.length() == 0
            || this->lastTag == TAGS::BINARY_OPERATION
            || this->lastTag == TAGS::DOT
            || this->lastTag == TAGS::OPENNING_BRACKET
            || this->lastTag == TAGS::SIGN) { return;}

        this->equalsButtonPressed = true;
        this->timerInterrupted = false;

        if (this->timer1->isActive()) {
            this->timer1->stop();
            this->timerInterrupted = true;
        }
        else
            this->timer1->start(4000);

        this->calculator.set_input(this->expression);
        double res = this->calculator.calculate();


        auto cut_the_zeros = [](std::string res_str) {
            auto it = std::find(res_str.begin(), res_str.end(), '.');
            if (it != res_str.end()) {
                int pos = it - res_str.begin();
                int last_non_zero = res_str.length() - 1;
                while (last_non_zero > pos && res_str[last_non_zero] == '0') {
                    last_non_zero--;
                }
                if (last_non_zero == pos) {
                    res_str = res_str.substr(0, pos);
                }
                else {
                    res_str = res_str.substr(0, last_non_zero + 1);
                }
            }
            return res_str;
        };


        this->expression = cut_the_zeros(std::to_string(res));
        this->floatingNumber = this->expression.find('.') != std::string::npos;

        this->ui->input->setText("");
        this->ui->input->setStyleSheet(
            "qproperty-alignment: \"AlignVCenter | AlignRight\";"
            "color: \"white\";"
            "background-color: \"#04BFAD\";"
            "font-family: 'Open Sans', sans-serif;"
            "font-weight: 600;          /* Semibold */"
            "font-size: 20px;           /* Размер шрифта */"
            "line-height: 30px;         /* Межстрочный интервал */"
            "letter-spacing: 0.5px;     /* Межбуквенный интервал */"
            );

        if (this->expression == "0") this->valueIsZero = true;
        this->lastTag = TAGS::EQUALS;

        update_text_labels();
    });

    connect(ui->pushButton_equals, &QPushButton::released, this, [this]() {
        this->equalsButtonPressed = false;
        this->timerInterrupted = true;
        if(this->timer1->isActive()) {
            this->timer1->stop();
        }
    });

//TIMERS
    connect(this->timer1, &QTimer::timeout, this, [this]() {
        if (this->equalsButtonPressed && !this->timerInterrupted) {
            this->timer2->start(5000);

            this->ui->pushButton_equals->setStyleSheet(
                "border-radius: 33px;"
                "background: \"yellow\";"
                "font-family: 'Open Sans', sans-serif;"
                "font-weight: 600;          /* Semibold */"
                "font-size: 24px;           /* Размер шрифта */"
                "line-height: 30px;         /* Межстрочный интервал */"
                "letter-spacing: 1px;     /* Межбуквенный интервал */"
                );
        }
    });

    auto showSecretMenu = [this]() {
        if (!this->secretMenu) {
            this->secretMenu = new QWidget();
            this->secretMenu->setWindowTitle("Секретное меню");
            this->secretMenu->setWindowFlags(Qt::Window);
            this->secretMenu->setFixedSize(300, 200);
            this->secretMenu->setVisible(true);

            QVBoxLayout* layout = new QVBoxLayout(this->secretMenu);
            QLabel* label = new QLabel("Секретное меню", this->secretMenu);
            QPushButton* backButton = new QPushButton("Назад", this->secretMenu);

            layout->addWidget(label);
            layout->addWidget(backButton);
        }
        this->secretMenu->show();
    };

    connect(this->timer2, &QTimer::timeout, this, [this]() {
        if (this->expression == "123") {
            // Создаем окно только один раз
            if (!secretMenu) {
                secretMenu = new QWidget(); // Без родителя
                secretMenu->setWindowTitle("Секретная информация");
                secretMenu->setFixedSize(300, 150);

                QVBoxLayout* layout = new QVBoxLayout(secretMenu);
                QLabel* label = new QLabel("Секретные настройки", secretMenu);
                label->setAlignment(Qt::AlignCenter);

                layout->addWidget(label);
            }

            secretMenu->show();
            secretMenu->raise();
            secretMenu->activateWindow();

            this->timer2->stop();
            this->expression.clear();

            this->ui->pushButton_equals->setStyleSheet(
                "border-radius: 33px;"
                "background: \"#0889A6\";"
                "font-family: 'Open Sans', sans-serif;"
                "font-weight: 600;"
                "font-size: 24px;"
                "line-height: 30px;"
                "letter-spacing: 1px;"
                );
        }
    });

    connect(ui->pushButton_sign, &QPushButton::clicked, this, [this, update_text_labels] () {
        if (this->lastTag == TAGS::DOT
            || this->divisionByZero) { return;}

        if (this->lastTag == TAGS::BINARY_OPERATION
        ||  this->lastTag == TAGS::SIGN)
        {
            this->expression += " (-";
            this->brackets_counter++;
        }
        else {
            this->expression += " * (-";
            this->brackets_counter++;
        }

        this->operationInserted = true;
        this->floatingNumber = false;
        this->lastTag = TAGS::SIGN;
        update_text_labels();
    });

    connect(ui->pushButton_dot, &QPushButton::clicked, this, [this, update_text_labels] () {
        if (this->lastTag != TAGS::NUMBER
            && this->lastTag != TAGS::EQUALS
            && this->lastTag != TAGS::CLEAR
            && this->lastTag != TAGS::NONE
            || this->divisionByZero
            || this->floatingNumber
            || this->expression.length() == 0) { return;}

        this->expression += ".";
        this->floatingNumber = true;


        this->lastTag = TAGS::DOT;
        update_text_labels();
    });

    connect(ui->pushButton_bracket, &QPushButton::clicked, this, [this, update_text_labels] () {
        if (this->divisionByZero) return;

        if (this->brackets_counter > 0) {
            if (this->lastTag == TAGS::NUMBER || this->lastTag == TAGS::CLOSING_BRACKET) {
                this->expression += ")";
                this->brackets_counter--;
                this->lastTag = TAGS::CLOSING_BRACKET;
            }
        } else {
            if (this->lastTag == TAGS::NUMBER || this->lastTag == TAGS::CLOSING_BRACKET
                || this->lastTag == TAGS::EQUALS || this->lastTag == TAGS::NONE) {
                this->expression += " * (";
            } else {
                this->expression += " (";
            }
            this->brackets_counter++;
            this->lastTag = TAGS::OPENNING_BRACKET;
        }
        this->operationInserted = true;
        update_text_labels();
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}
