# ifndef MAINWINDOW_H
# define MAINWINDOW_H

# include <QPoint>
# include <QMainWindow>

# include "settings.h"
# include "statusengine.h"
# include "communicator.h"

# define boardX 250
# define boardY 88

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_action_Quit_triggered();
    void on_action_Help_Me_triggered();
    void on_actionSettings_triggered();
    void on_action_Load_File_triggered();
    void on_action_Save_File_triggered();
    void on_actionConnect_triggered();

private:
    Ui::MainWindow *ui;
    StatusEngine *se;
    Settings *globalSetting;
    CommunicatorDialog *network;

    bool mouseIsPressed;
    void paintEvent(QPaintEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
};

#endif // MAINWINDOW_H
