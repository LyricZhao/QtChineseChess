# include "communicator.h"
# include "ui_communicatordialog.h"

# include <QDebug>
# include <QMessageBox>
# include <QHostAddress>

CommunicatorDialog:: CommunicatorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CommunicatorDialog) {
    ui -> setupUi(this);
    resetToDefault();
}

CommunicatorDialog:: ~CommunicatorDialog() {
    disConnect();
    delete ui;
}

void CommunicatorDialog:: resetToDefault() {
    connected = connecting = false;
    server = nullptr;
    client = nullptr;
    ui -> ipLine -> setEnabled(true);
    ui -> ipLine -> setText(tr(DEFAULT_IP));
    ui -> portSpin -> setEnabled(true);
    ui -> portSpin -> setValue(DEFAULT_PORT);
    ui -> clientRB -> setEnabled(true);
    ui -> clientRB -> setChecked(true);
    ui -> serverRB -> setEnabled(true);
    ui -> serverRB -> setChecked(false);
    ui -> label -> hide();
    ui -> OKButton -> setEnabled(true);
    ui -> cancelButton -> setEnabled(true);
}

void CommunicatorDialog:: setAllDisable() {
    ui -> ipLine -> setEnabled(false);
    ui -> portSpin -> setEnabled(false);
    ui -> clientRB -> setEnabled(false);
    ui -> serverRB -> setEnabled(false);
    // ui -> label -> setEnabled(false);
    ui -> OKButton -> setEnabled(false);
}

void CommunicatorDialog:: newShow() {
    resetToDefault();
    show();
}

bool CommunicatorDialog:: checkIPAddrLegal(QString IPAddr) {
    return IPAddr.size() > 0; // to be continued
}

void CommunicatorDialog:: on_OKButton_clicked() {
    isClient = ui -> clientRB -> isChecked();
    QString IP_Addr = ui -> ipLine -> text();
    int port = ui -> portSpin -> value();
    if(isClient && !checkIPAddrLegal(IP_Addr)) {
        QMessageBox:: information(this, tr("ERROR"), tr("There's something wrong with your IP address."));
        return;
    }

    setAllDisable();
    ui -> label -> show();
    if(isClient) {
        ui -> label -> setText(tr("Connecting ..."));
        client = new QTcpSocket();
        client -> connectToHost(QHostAddress(IP_Addr), port);
        connect(client, SIGNAL(connected()), this, SLOT(connectedActions()));
        connecting = true;
        ui -> OKButton -> setEnabled(false);
    } else {
        ui -> label -> setText(tr("Waiting for connection ..."));
        server = new QTcpServer();
        server -> listen(QHostAddress:: Any, port);
        connect(server, SIGNAL(newConnection()), this, SLOT(connectedActions()));
        connecting = true;
        ui -> OKButton -> setEnabled(false);
    }
}

void CommunicatorDialog:: connectedActions() {
    connecting = false;
    connected = true;
    if(!isClient) {
        client = server -> nextPendingConnection();
    }
    connect(client, SIGNAL(readyRead()), this, SLOT(dataPending()));
    connect(client, &QTcpSocket:: disconnected, this, [this](){ emit connectLost(); });
    this -> close();
    pendingDataLen = 0;
    dataBuffer.clear();
    emit connectOK();
}

void CommunicatorDialog:: disConnect() {
    if(connected) {
        // client -> close();
        if(!isClient) {
            server -> close();
            client -> disconnectFromHost();
        } else {
            client -> disconnectFromHost();
        }
        connected = false;
        client = nullptr;
        server = nullptr;
    }
}

void CommunicatorDialog:: on_cancelButton_clicked() {
    if(connecting) {
        connecting = false;
        client -> close();
        if(!isClient)
            server -> close();
        resetToDefault();
    } else {
        this -> close();
    }
}

void CommunicatorDialog:: dataPending() {
    if(dataBuffer.size() > 0) return;
    if(pendingDataLen == 0) {
        if(client -> bytesAvailable() >= (long long)sizeof(int)) {
            client -> read((char *)&pendingDataLen, sizeof(int));
        }
    }

    if(pendingDataLen == 0) return;

    if(client -> bytesAvailable() >= pendingDataLen) {
        dataBuffer.resize(pendingDataLen);
        client -> read(dataBuffer.data(), pendingDataLen);
        pendingDataLen = 0;
        emit gettingData();
    }
}

bool CommunicatorDialog:: sendData(const QByteArray &data) {
    QByteArray transfer; transfer.clear();
    int length = data.size();
    transfer.append((const char *)(&length), sizeof(int));
    transfer.append(data);
    client -> write(transfer);
    return client -> waitForBytesWritten();
}

QByteArray CommunicatorDialog:: getData() {
    QByteArray transfer = dataBuffer;
    dataBuffer.clear();
    return transfer;
}

void CommunicatorDialog::on_clientRB_clicked(bool checked) {
    ui -> ipLine -> setEnabled(checked);
}

void CommunicatorDialog::on_serverRB_clicked(bool checked) {
    ui -> ipLine -> setEnabled(checked ^ 1);
}
