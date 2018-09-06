# ifndef SETTINGS_H
# define SETTINGS_H

# include <QDialog>
# include <QString>
# include <QPainter>

namespace Ui {
class SettingDialog;
}

struct Settings {
    bool soundEffect;
    QString userName;

    Settings() { soundEffect = false; userName = "LyricZ"; }
};

class SettingDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = 0, Settings *i_settings = nullptr);
    ~SettingDialog();

private slots:

    void on_restoreButton_clicked();
    void on_OKButton_clicked();
    void on_cancelButton_clicked();

private:
    Settings *settings;
    Ui:: SettingDialog *ui;
    QString username; bool soundEffect;
};

#endif // SETTINGS_H
