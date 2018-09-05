# include "settings.h"
# include "ui_settingdialog.h"

SettingDialog::SettingDialog(QWidget *parent, Settings *i_settings) :
    QDialog(parent), settings(i_settings),
    ui(new Ui::SettingDialog) {
    ui -> setupUi(this);
    ui -> username -> setText(username = settings -> userName);
    ui -> seBox -> setChecked(soundEffect = settings -> soundEffect);
}

SettingDialog::~SettingDialog() {
    delete ui;
}

void SettingDialog::on_restoreButton_clicked() {
    ui -> username -> setText(username = settings -> userName);
    ui -> seBox -> setChecked(soundEffect = settings -> soundEffect);
}

void SettingDialog::on_OKButton_clicked() {
    settings -> userName = ui -> username -> text();
    settings -> soundEffect = ui -> seBox -> isChecked();
    this -> close();
}

void SettingDialog::on_cancelButton_clicked() {
    this -> close();
}
