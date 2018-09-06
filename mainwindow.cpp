# include <QUrl>
# include <QPoint>
# include <QDebug>
# include <QPainter>
# include <QPaintEvent>
# include <QMouseEvent>
# include <QFileDialog>
# include <QMessageBox>
# include <QMediaPlaylist>

# include "helpme.h"
# include "mainwindow.h"
# include "ui_mainwindow.h"

MainWindow:: MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {

    ui -> setupUi(this);
    ui -> ChessBoard -> hide();

    se = new StatusEngine(); se -> reset();
    globalSetting = new Settings();
    network = new CommunicatorDialog(this);

    toolBarSet(Unconnected);

    /* network */
    connect(network, SIGNAL(connectOK()), this, SLOT(readyPending()));
    connect(network, SIGNAL(gettingData()), this, SLOT(handleMsg()));
    connect(network, SIGNAL(connectLost()), this, SLOT(delConnect()));

    /* Timer */
    gameTimer = new QTimer(this);
    connect(gameTimer, SIGNAL(timeout()), this, SLOT(countDown()));

    /* Player */
    bgm = new QMediaPlayer(this);
    QMediaPlaylist *qpl = new QMediaPlaylist;
    qpl -> addMedia(QUrl(tr("qrc:/BGM/bgm.mp3")));
    qpl -> setCurrentIndex(1);
    qpl -> setPlaybackMode(QMediaPlaylist:: CurrentItemInLoop);
    bgm -> setPlaylist(qpl);
    bgm -> setVolume(0);
    bgm -> play();
}

MainWindow:: ~MainWindow() {
    delete ui;
}

void MainWindow:: countDown() {
    -- timeRemaining;
    if(timeRemaining < 0) timeRemaining = 0;
    ui -> lcdNumber -> display(timeRemaining);
}

void MainWindow:: toolBarSet(Status gS) {
    if(globalStatus == Gaming && gS != Gaming) {
        gameTimer -> stop();
    }

    globalStatus = gS;
    ui -> actionConnect -> setEnabled(gS == Unconnected);
    ui -> action_Disconnect -> setEnabled(gS != Unconnected);
    ui -> actionNew_Game -> setEnabled(gS == Pending);
    // ui -> actionSettings -> setEnabled(true);
    ui -> action_Load_File -> setEnabled(gS != Gaming);
    ui -> action_Reset -> setEnabled(gS != Gaming);
    // ui -> action_Save_File -> setEnabled(true);
    // ui -> action_Help_Me -> setEnabled(true);
    // ui -> action_Quit -> setEnabled(true);
    if(gS == Pending) enReady = false, amReady = false;
    amPlayer = -1;

    if(gS != Gaming) ui -> lcdNumber -> hide();
    else ui -> lcdNumber -> show();

    if(gS != Gaming) se -> setPlayer(0);
}

void MainWindow:: mousePressEvent(QMouseEvent *ev) {
    if(globalStatus == Gaming && curPos != amPlayer) return;
    mouseIsPressed = true;
    QPoint qpos = ev -> pos();
    qpos -= QPoint(boardX, boardY);
    if(se -> mouseClick(qpos, amPlayer)) {
        update();
    }
}

void MainWindow:: mouseMoveEvent(QMouseEvent *ev) {
    if(globalStatus == Gaming && curPos != amPlayer) return;
    if(!mouseIsPressed) return;
    QPoint qpos = ev -> pos();
    qpos -= QPoint(boardX, boardY);
    if(se -> mouseMove(qpos)) {
        // qDebug() << ev -> pos().x() << ev -> pos().y();
        update();
    }
}

void MainWindow:: mouseReleaseEvent(QMouseEvent *ev) {
    if(globalStatus == Gaming && curPos != amPlayer) return;
    mouseIsPressed = false;
    QPoint qpos = ev -> pos();
    qpos -= QPoint(boardX, boardY);
    if(se -> mouseRelease(qpos)){
        if(globalStatus != Unconnected) {
            network -> sendData(se -> toRawData());
            if(globalStatus == Gaming) {
                curPos ^= 1;
                timerReset();
            }
        }
    }
    update();
}

void MainWindow:: paintEvent(QPaintEvent *ev) {
    (void)(ev); // unused warning
    QPainter painter(this);
    painter.save();
    QPixmap image(tr(":/Pics/background.jpg"));
    painter.drawPixmap(0, 0, this -> width(), this -> height(), image);
    if(se != nullptr) se -> draw(&painter, QPoint(boardX, boardY));
    painter.restore();

    if(globalStatus == Pending) {
        QPixmap readyImg(tr(":/Pics/ready.png"));
        if(enReady) {
            painter.drawPixmap(25, 80, readyImg.width(), readyImg.height(), readyImg);
        }
        if(amReady) {
            painter.drawPixmap(810, 580, readyImg.width(), readyImg.height(), readyImg);
        }
    }
}

void MainWindow:: handleMsg() {
    QByteArray msg = network -> getData();
    int tp = *((int *)msg.data());
    if(tp == 0) {
        curPos ^= 1;
        se -> trans(msg);
        if(globalStatus == Gaming) timerReset();
    } else if(tp == 1) {
        enReady ^= 1;
        if(enReady && amReady) realNewGame();
    } else if(tp == 2) {

    }
    update();
}

void MainWindow:: delConnect() {
    network -> disConnect();
    toolBarSet(Unconnected);
}

void MainWindow:: readyPending() {
    toolBarSet(Pending);
    if(!network -> isClient)
        network -> sendData(se -> toRawData());
}

void MainWindow:: on_action_Quit_triggered() {
    this -> close();
}

void MainWindow:: on_action_Help_Me_triggered() {
    HelpMe *hm = new HelpMe(this);
    hm -> show();
}

void MainWindow:: on_actionSettings_triggered() {
    SettingDialog *sd = new SettingDialog(this, globalSetting);
    sd -> exec();
    bgm -> setVolume(globalSetting -> soundEffect ? 100 : 0);
}

void MainWindow:: on_action_Load_File_triggered() {
    QString openFile = QFileDialog:: getOpenFileName(this, "Open File");
    if(openFile.size() == 0) return;
    if(se -> readFile(openFile)) {
        if(globalStatus == Pending)
            network -> sendData(se -> toRawData());
        update();
    } else {
        QMessageBox:: information(this, "ERROR", "Can't Open this file!");
    }
}

void MainWindow:: on_action_Save_File_triggered() {
    se -> saveIntoFile();
}

void MainWindow:: on_actionConnect_triggered() {
    network -> newShow();
}

void MainWindow:: on_actionNew_Game_triggered() {
    amReady ^= 1;
    if(amReady && enReady) realNewGame();
    int iamReady = 1;
    QByteArray command; command.clear();
    command.append((const char *)&iamReady, sizeof(int));
    network -> sendData(command);
    update();
}

void MainWindow:: on_action_Disconnect_triggered() {
    delConnect();
}

void MainWindow:: on_action_Reset_triggered() {
    if(se -> readFile(tr(":/defaultChess.txt"))) {
        if(globalStatus == Pending)
            network -> sendData(se -> toRawData());
        update();
    }
}

void MainWindow:: realNewGame() {
    curPos = 0;
    toolBarSet(Gaming);
    amPlayer = se -> getGeneralPlayer();
    se -> setPlayer(amPlayer);
    timerReset();
}

void MainWindow:: timerReset() {
    if(gameTimer -> isActive())
        gameTimer -> stop();
    timeRemaining = GAME_WAIT_SEC;
    ui -> lcdNumber -> display(GAME_WAIT_SEC);
    gameTimer -> start(REFRESH_INTERVAL);
}
