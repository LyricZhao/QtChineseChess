# include <tuple>
# include <cstring>
# include <cassert>
# include <iostream>

# include <QFile>
# include <QRect>
# include <QSize>
# include <QDebug>
# include <QString>
# include <QObject>
# include <QPixmap>
# include <QTextStream>
# include <QPaintEvent>

# include "statusengine.h"

const char pieceMap[8] = {' ', 'j', 's', 'x', 'm', 'c', 'p', 'z'};

StatusEngine:: StatusEngine() {
    dataClear();
}

void StatusEngine:: refreshChecked() {
    if(!mouseIsMoving) {
        memset(checkedArr, 0, sizeof(checkedArr));
        return;
    }
    Rule ruler(typeArr, playerArr);
    std:: vector<IPoint> ret = ruler.reachingPos(opX, opY);
    memset(checkedArr, 0, sizeof(checkedArr));
    for(auto it: ret) {
        checkedArr[it.first][it.second] = true;
    }
}

void StatusEngine:: dataClear() {
    opX = opY = -1;
    emptyState = true;
    mouseIsMoving = false;
    memset(typeArr, 0, sizeof(typeArr));
    memset(playerArr, 0, sizeof(playerArr));

    for(int i = 0; i < m_maxw; ++ i) {
        for(int j = 0; j < m_maxh; ++ j) {
            rectArr[i][j] = QRect(getPosition(i, j), QSize(57, 57));
        }
    }
}

void StatusEngine:: reset() {
    emptyState = false;
    assert(readFile(QObject:: tr(":/defaultChess.txt")) == true);
}

bool StatusEngine:: mouseClick(QPoint qpos) {
    assert(mouseIsMoving == false);
    mouseRealPos = qpos;
    for(int i = 0; i < m_maxw; ++ i) {
        for(int j = 0; j < m_maxh; ++ j) {
            if(rectArr[i][j].contains(qpos)) {
                opX = i, opY = j;
                mouseIsMoving = true;
                mouseTrackPos = qpos - rectArr[i][j].topLeft();
                refreshChecked();
                return true;
            }
        }
    }
    return false;
}

bool StatusEngine:: mouseMove(QPoint qpos) {
    mouseRealPos = qpos;
    return mouseIsMoving;
}

bool StatusEngine:: mouseRelease(QPoint qpos) {
    mouseRealPos = qpos;
    if(mouseIsMoving) {
        mouseIsMoving = false;
        for(int i = 0; i < m_maxw; ++ i) {
            for(int j = 0; j < m_maxh; ++ j) {
                if(rectArr[i][j].contains(qpos)) {
                    bool ret = attemptMove(i, j);
                    refreshChecked();
                    return ret;
                }
            }
        }
        return true;
    }
    refreshChecked();
    return false;
}

bool StatusEngine:: attemptMove(int targetX, int targetY) {
    // if(opX == targetX && opY == targetY) return false;
    if(!checkedArr[targetX][targetY]) return false;
    setPosition(targetX, targetY, playerArr[opX][opY], typeArr[opX][opY]);
    setPosition(opX, opY, 0, None);
    return true;
}

void StatusEngine:: setPosition(int x, int y, int player, Piece chess) {
    assert(0 <= x && x <= 8);
    assert(0 <= y && y <= 9);
    assert(0 <= player && player <= 1);

    typeArr[x][y] = chess;
    playerArr[x][y] = player;
}

QPoint StatusEngine:: getPosition(int x, int y) {
    assert(0 <= x && x <= 8);
    assert(0 <= y && y <= 9);

    y = 9 - y;
    int rx = 29, ry = (y >= 5) ? 318 : 32;
    int cx = 521.0 / 9.0 * x - 57.0 / 2, cy = 57 * ((y >= 5) ? (y - 5) : y) - 57.0 / 2;
    cx += rx, cy += ry;

    return QPoint(cx, cy);
}

QString StatusEngine:: getIconName(Piece chess, int player, bool attack) {
    if(chess == None) {
        assert(attack == true);
        return QObject:: tr("non.gif");
    }
    QString iconName = "";
    iconName += (player == 0) ? 'r' : 'b';
    iconName += pieceMap[(int)(chess)];
    if(attack == true) iconName += 's';
    iconName += ".gif";

    return iconName;
}

void StatusEngine:: drawPosition(int x, int y, int player, Piece chess, QPainter *painter) {
    if(chess == None && !checkedArr[x][y]) return;
    if(mouseIsMoving && x == opX && y == opY) return;

    /*
    painter -> save();
    y = 9 - y;
    painter -> translate(QPoint(29, (y >= 5) ? 318 : 32));
    if(y >= 5) y -= 5;
    // if(chess == General) qDebug() << x << y;
    int cX = 521.0 / 9.0 * x, cY = 57 * y;
    */

    QPixmap image;
    image.load(QObject:: tr(":/Pics/piece/") + getIconName(chess, player, checkedArr[x][y]));
    // qDebug() << QObject:: tr(":/Pics/piece/") + iconName;
    // cX -= image.width() / 2, cY -= image.height() / 2;
    painter -> drawPixmap(getPosition(x, y), image);

    // painter -> restore();
}

int StatusEngine:: read(const QString &line, int &pos, bool &ok) {
    if(pos >= line.size()) ok = false;
    while(pos < line.size() && !line.at(pos).isNumber()) ++ pos;
    if(pos == line.size()) ok = false;
    if(!ok) return 0;
    int ret = 0;
    while(pos < line.size() && line.at(pos).isNumber()) {
        ret = ret * 10 + line.at(pos ++).toLatin1() - 48;
    }
    return ret;
}

bool StatusEngine:: setLine(QString line, int player, Piece cType) {
    int pos = 0; bool ok = true;
    int n = read(line, pos, ok);
    for(int i = 0; i < n; ++ i) {
        int x = read(line, pos, ok), y = read(line, pos, ok);
        if(!ok) break;
        setPosition(x, y, player, cType);
    }
    return ok;
}

void StatusEngine:: draw(QPainter *painter, QPoint oP) {
    if(emptyState == true) return;

    painter -> translate(oP);
    QPixmap board;
    board.load(QObject:: tr(":/Pics/board.gif"));
    painter -> drawPixmap(0, 0, board.width(), board.height(), board);
    for(int i = 0; i < m_maxw; ++ i) {
        for(int j = 0; j < m_maxh; ++ j) {
            drawPosition(i, j, playerArr[i][j], typeArr[i][j], painter);
        }
    }

    // draw mouse pos
    if(mouseIsMoving) {
        QPixmap image;
        image.load(QObject:: tr(":/Pics/piece/") + getIconName(typeArr[opX][opY], playerArr[opX][opY]));
        // qDebug() << mouseRealPos.x() << mouseRealPos.y();
        painter -> drawPixmap(mouseRealPos - mouseTrackPos, image);
    }
}

bool StatusEngine:: readFile(QString filePath) {
    QFile *file = new QFile(filePath);
    if(file -> open(QFile:: ReadOnly)) {
        QTextStream text(file);

        int state[2] = {0, 0};
        for(;;) {
            QString line = text.readLine();
            if(line.size() == 0) {
                goto err_label;
            } else if(line == "red" || line == "black") {
                int player = (line == "red") ? 0 : 1;
                if(state[player]) goto err_label;

                for(int i = 0; i < 7; ++ i) {
                    QString subLine = text.readLine();
                    if(subLine.size() == 0 || setLine(subLine, player, Piece(i + 1)) == false)
                        goto err_label;
                }

                state[player] = 1;
                if(state[0] && state[1]) break;
            } else {
                goto err_label;
            }
        }

        file -> close(); delete file;
        return true;

        err_label:
        file -> close();
    }

    delete file;
    return false;
}
