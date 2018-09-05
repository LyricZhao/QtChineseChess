# include "helpme.h"
# include "ui_helpme.h"

HelpMe::HelpMe(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpMe) {
    ui->setupUi(this);
}

HelpMe::~HelpMe() {
    delete ui;
}

void HelpMe::on_pushButton_clicked() {
    this -> close();
}
