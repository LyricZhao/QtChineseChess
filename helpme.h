#ifndef HELPME_H
#define HELPME_H

#include <QDialog>

namespace Ui {
class HelpMe;
}

class HelpMe : public QDialog {
    Q_OBJECT

public:
    explicit HelpMe(QWidget *parent = 0);
    ~HelpMe();

private slots:
    void on_pushButton_clicked();

private:
    Ui::HelpMe *ui;
};

#endif // HELPME_H
