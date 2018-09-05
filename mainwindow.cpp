# include <QPoint>
# include <QDebug>
# include <QPainter>
# include <QPaintEvent>
# include <QMouseEvent>

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
    if(se -> mouseRelease(qpos)) {
        update();
    }
}

void MainWindow:: paintEvent(QPaintEvent *ev) {
    (void)(ev); // unused warning
    QPainter painter(this);
    if(se != nullptr) se -> draw(&painter, QPoint(boardX, boardY));
}
