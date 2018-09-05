# ifndef RULE_H
# define RULE_H

# define m_maxw 9l
# define m_maxh 10l

# define IR(a, b, c) ((a) <= (b) && (b) <= (c))
# define MP(a, b) std:: make_pair((a), (b))

# include <vector>
# include <iostream>

typedef std:: pair<int, int> IPoint;

enum Piece {None, General, Knight, Elephant, Horse, Chariot, Gun, Soldier};

class Rule {
    Piece typeArr[m_maxw][m_maxh];
    int playerArr[m_maxw][m_maxh];

    bool exist(int x, int y);
    bool inRange(int x, int y);
    bool inGrid(int x, int y, int player);
    bool isSame(int x0, int y0, int x1, int y1);

    std:: vector<IPoint> reachingGeneral(int x, int y, int p);
    std:: vector<IPoint> reachingKnight(int x, int y, int p);
    std:: vector<IPoint> reachingElephant(int x, int y, int p);
    std:: vector<IPoint> reachingHorse(int x, int y, int p);
    std:: vector<IPoint> reachingChariot(int x, int y, int p);
    std:: vector<IPoint> reachingGun(int x, int y, int p);
    std:: vector<IPoint> reachingSoldier(int x, int y, int p);

public:
    Rule(Piece i_typeArr[m_maxw][m_maxh], int i_playerArr[m_maxw][m_maxh]);
    std:: vector<IPoint> reachingPos(int x, int y);
    bool checkIn(std:: vector<IPoint> &vec, IPoint point);
};

#endif // RULE_H
