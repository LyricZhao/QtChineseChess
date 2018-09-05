# ifndef COMMUNICATOR_H
# define COMMUNICATOR_H

# include <QDialog>
# include <QTcpServer>
# include <QTcpSocket>

namespace Ui {
class CommunicatorDialog;
}

class CommunicatorDialog : public QDialog {
    Q_OBJECT

public:
    explicit CommunicatorDialog(QWidget *parent = 0);
    ~CommunicatorDialog();
    bool isOKClicked();
    bool getIsClient();
    QString getIPAddr();
    int getPort();

private slots:
    void on_serverRB_clicked(bool checked);
    void on_OKButton_clicked();
    void on_cancelButton_clicked();

private:
    bool okClicked;
    Ui:: CommunicatorDialog *ui;
};

class Communicator {
    Q_OBJECT

public:
    bool connected, isClient;
    Communicator();
    bool newConnection();
    void disconnect();
    bool sendData(const QByteArray &data);
    void getData(QByteArray &data, int len);

private slots:
    void innerDataReceiced();

signals:
    void dataReceived();

private:
    QTcpServer *server;
    QTcpSocket *client;
};

# endif // COMMUNICATOR_H
