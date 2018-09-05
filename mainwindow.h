# ifndef MAINWINDOW_H
# define MAINWINDOW_H

# include <QPoint>
# include <QMainWindow>

# include "statusengine.h"

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

private:
    Ui::MainWindow *ui;
    StatusEngine *se;

    bool mouseIsPressed;
    void paintEvent(QPaintEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
};

#endif // MAINWINDOW_H
