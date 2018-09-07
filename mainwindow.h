# ifndef MAINWINDOW_H
# define MAINWINDOW_H

# include <QTimer>
# include <QPoint>
# include <QMainWindow>
# include <QMediaPlayer>

# include "settings.h"
# include "statusengine.h"
# include "communicator.h"

# define boardX 250
# define boardY 88

# define GAME_WAIT_SEC 20
# define REFRESH_INTERVAL 1000

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
    void on_actionNew_Game_triggered();
    void on_action_Disconnect_triggered();
    void on_action_Reset_triggered();
    void on_action_Give_Up_triggered();

    void readyPending();
    void handleMsg();
    void delConnect();
    void countDown();

private:
    enum Status{Unconnected, Pending, Gaming};

    Ui::MainWindow *ui;
    StatusEngine *se;
    Settings *globalSetting;
    CommunicatorDialog *network;
    Status globalStatus;
    bool enReady, amReady;
    bool mouseIsPressed;
    int amPlayer, curPos, timeRemaining;
    QTimer *gameTimer;
    QMediaPlayer *bgm, *soundE;
    bool displayResult, gameResult;

    void paintEvent(QPaintEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);

    void toolBarSet(Status gS);
    void realNewGame();
    void timerReset();
    void setResult(bool result);
    void checkState(int lastPlayer);
};

#endif // MAINWINDOW_H
