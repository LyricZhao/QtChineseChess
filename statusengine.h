# ifndef STATUSENGINE_H
# define STATUSENGINE_H

# include <QLabel>
# include <QPoint>
# include <QPainter>

# include "rule.h"

class StatusEngine {

private:
    Piece typeArr[m_maxw][m_maxh];
    QRect rectArr[m_maxw][m_maxh];
    int playerArr[m_maxw][m_maxh]; // 0 for red (self), 1 for black (enemy)
    bool emptyState, mouseIsMoving;
    int opX, opY;
    QPoint mouseTrackPos, mouseRealPos;

    int read(const QString &line, int &pos, bool &ok);
    void dataClear();
    bool setLine(QString line, int player, Piece cType);
    QPoint getPosition(int x, int y);
    QString getIconName(Piece chess, int player, bool attack = false);
    void setPosition(int x, int y, int player, Piece chess);
    void drawPosition(int x, int y, int player, Piece chess, QPainter *painter);

public:
    StatusEngine();

    bool existPiece(int x, int y);
    bool isLegal(int x0, int y0, int x1, int y1);
    bool isEnd(int *winner = nullptr);

    void reset();
    bool mouseMove(QPoint qpos);
    bool mouseClick(QPoint qpos);
    bool mouseRelease(QPoint qpos);
    void select(int x, int y);
    void draw(QPainter *painter, QPoint oP);
    void attemptMove(int targetX, int targetY);
    bool readFile(QString filePath);
};

#endif // STATUSENGINE_H