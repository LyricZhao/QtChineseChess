# include "communicator.h"
# include "ui_communicatordialog.h"

CommunicatorDialog:: CommunicatorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CommunicatorDialog) {
    ui -> setupUi(this);
}

CommunicatorDialog:: ~CommunicatorDialog() {
    delete ui;
}

bool CommunicatorDialog:: isOKClicked() {
    return okClicked;
}

bool CommunicatorDialog:: getIsClient() {
    return ui -> clientRB -> isChecked();
}

QString CommunicatorDialog:: getIPAddr() {
    return ui -> ipLine -> text();
}

int CommunicatorDialog:: getPort() {
    return ui -> portSpin -> value();
}

void CommunicatorDialog::on_serverRB_clicked(bool checked) {
    ui -> ipLine -> setEnabled(checked ^ 1);
}

void CommunicatorDialog::on_OKButton_clicked() {
    okClicked = true;
    this -> close();
}

void CommunicatorDialog::on_cancelButton_clicked() {
    okClicked = false;
    this -> close();
}

bool Communicator:: newConnection() {
    if(connected) return false;
    CommunicatorDialog *cd = new CommunicatorDialog;
    cd -> show();
}
