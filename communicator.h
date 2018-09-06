# ifndef COMMUNICATOR_H
# define COMMUNICATOR_H

# include <QDialog>
# include <QTcpServer>
# include <QTcpSocket>
# include <QByteArray>

# define DEFAULT_IP "127.0.0.1"
# define DEFAULT_PORT 4723l

namespace Ui {
class CommunicatorDialog;
}

class CommunicatorDialog : public QDialog {
    Q_OBJECT

public:
    bool connected, isClient, connecting;

    explicit CommunicatorDialog(QWidget *parent = 0);
    ~CommunicatorDialog();

    void newShow();
    void createNewConnection();
    void disConnect();
    void resetToDefault();
    bool sendData(const QByteArray &data);
    QByteArray getData();

signals:
    void gettingData();

private slots:
    void on_clientRB_clicked(bool checked);
    void on_serverRB_clicked(bool checked);
    void on_OKButton_clicked();
    void on_cancelButton_clicked();
    void connectedActions();
    void dataPending();

private:
    Ui:: CommunicatorDialog *ui;

    int pendingDataLen;
    QTcpServer *server;
    QTcpSocket *client;
    QByteArray dataBuffer;
    bool checkIPAddrLegal(QString IPAddr);
    void setAllDisable();
};

# endif // COMMUNICATOR_H
