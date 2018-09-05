# include <QPoint>
# include <QDebug>
# include <QPainter>
# include <QPaintEvent>
# include <QMouseEvent>

# include "helpme.h"
# include "mainwindow.h"
# include "ui_mainwindow.h"

MainWindow:: MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui -> setupUi(this);

    ui -> ChessBoard -> hide();
    se = new StatusEngine();
    se -> reset();

    // this -> setMouseTracking(true);
}

MainWindow:: ~MainWindow() {
    delete ui;
}

void MainWindow:: mousePressEvent(QMouseEvent *ev) {
    mouseIsPressed = true;
    QPoint qpos = ev -> pos();
    qpos -= QPoint(boardX, boardY);
    if(se -> mouseClick(qpos)) {
        update();
    }
}

void MainWindow:: mouseMoveEvent(QMouseEvent *ev) {
    if(!mouseIsPressed) return;
    QPoint qpos = ev -> pos();
    qpos -= QPoint(boardX, boardY);
    if(se -> mouseMove(qpos)) {
        // qDebug() << ev -> pos().x() << ev -> pos().y();
        update();
    }
}

void MainWindow:: mouseReleaseEvent(QMouseEvent *ev) {
    mouseIsPressed = false;
    QPoint qpos = ev -> pos();
    qpos -= QPoint(boardX, boardY);
    bool ret = se -> mouseRelease(qpos);
    update();
}

void MainWindow:: paintEvent(QPaintEvent *ev) {
    (void)(ev); // unused warning
    QPainter painter(this);
    QPixmap image(tr(":/Pics/background.jpg"));
    painter.drawPixmap(0, 0, this -> width(), this -> height(), image);
    if(se != nullptr) se -> draw(&painter, QPoint(boardX, boardY));
}

void MainWindow::on_action_Quit_triggered() {
    this -> close();
}

void MainWindow::on_action_Help_Me_triggered() {
    HelpMe *hm = new HelpMe(this);
    hm -> show();
}
