# include <cstring>
# include <cassert>

# include "rule.h"

Rule:: Rule(Piece i_typeArr[m_maxw][m_maxh], int i_playerArr[m_maxw][m_maxh], int player) {
    memcpy(&typeArr[0][0], &i_typeArr[0][0], sizeof(Piece) * m_maxw * m_maxh);
    memcpy(&playerArr[0][0], &i_playerArr[0][0], sizeof(int) * m_maxw * m_maxh);
    for(int i = 0; i < m_maxw; ++ i) {
        for(int j = 0; j < m_maxh; ++ j) {
            playerArr[i][j] ^= player;
        }
    }
}

bool Rule:: inGrid(int x, int y, int player) {
    if(player == 0) return IR(3, x, 5) && IR(0, y, 2); // red
    if(player == 1) return IR(3, x, 5) && IR(7, y, 9); // black
    return false;
}

bool Rule:: exist(int x, int y) {
    return typeArr[x][y] != None;
}

bool Rule:: inRange(int x, int y) {
    return IR(0, x, 8) && IR(0, y, 9);
}

bool Rule:: isSame(int x0, int y0, int x1, int y1) {
    if(typeArr[x0][y0] == None || typeArr[x1][y1] == None) return false;
    return playerArr[x0][y0] == playerArr[x1][y1];
}

std:: vector<IPoint> Rule:: reachingGeneral(int x, int y, int p) {
    const int dx[4] = {0,  0,  1, -1};
    const int dy[4] = {1, -1,  0,  0};

    std:: vector<IPoint> ret; ret.clear();
    for(int i = 0; i < 4; ++ i) {
        if(inGrid(x + dx[i], y + dy[i], p) && !isSame(x, y, x + dx[i], y + dy[i])) {
            ret.push_back(MP(x + dx[i], y + dy[i]));
        }
    }

    return ret;
}

std:: vector<IPoint> Rule:: reachingKnight(int x, int y, int p) {
    const int dx[4] = {+1, +1, -1, -1};
    const int dy[4] = {-1, +1, -1, +1};

    std:: vector<IPoint> ret; ret.clear();
    for(int i = 0; i < 4; ++ i) {
        if(inGrid(x + dx[i], y + dy[i], p) && !isSame(x, y, x + dx[i], y + dy[i])) {
            ret.push_back(MP(x + dx[i], y + dy[i]));
        }
    }
    return ret;
}

std:: vector<IPoint> Rule:: reachingElephant(int x, int y, int p) {
    (void)(p);
    const int dx[4] = {+2, +2, -2, -2};
    const int dy[4] = {-2, +2, -2, +2};
    const int cx[4] = {+1, +1, -1, -1};
    const int cy[4] = {-1, +1, -1, +1};

    std:: vector<IPoint> ret; ret.clear();
    for(int i = 0; i < 4; ++ i) {
        if(inRange(x + dx[i], y + dy[i]) && !isSame(x, y, x + dx[i], y + dy[i]) && !exist(x + cx[i], y + cy[i]) && !((y < 5) ^ (y + dy[i] < 5))) {
            ret.push_back(MP(x + dx[i], y + dy[i]));
        }
    }
    return ret;
}

std:: vector<IPoint> Rule:: reachingHorse(int x, int y, int p) {
    (void)(p);
    const int dx[8] = {-1, -1, +1, +1, -2, -2, +2, +2};
    const int dy[8] = {-2, +2, -2, +2, -1, +1, -1, +1};
    const int cx[8] = { 0,  0,  0,  0, -1, -1, +1, +1};
    const int cy[8] = {-1, +1, -1, +1,  0,  0,  0,  0};

    std:: vector<IPoint> ret; ret.clear();
    for(int i = 0; i < 4; ++ i) {
        if(inRange(x + dx[i], y + dy[i]) && !isSame(x, y, x + dx[i], y + dy[i]) && !exist(x + cx[i], y + cy[i])) {
            ret.push_back(MP(x + dx[i], y + dy[i]));
        }
    }
    return ret;
}

# define funcChariot if(!inRange(rx, ry) || isSame(x, y, rx, ry)) break; \
    ret.push_back(MP(rx, ry)); \
    if(exist(rx, ry)) break; \

std:: vector<IPoint> Rule:: reachingChariot(int x, int y, int p) {
    (void)(p);
    std:: vector<IPoint> ret; ret.clear();
    for(int rx = x - 1, ry = y; ; -- rx) { funcChariot; }
    for(int rx = x + 1, ry = y; ; ++ rx) { funcChariot; }
    for(int rx = x, ry = y - 1; ; -- ry) { funcChariot; }
    for(int rx = x, ry = y + 1; ; ++ ry) { funcChariot; }
    return ret;
}

# define funcGun if(!inRange(rx, ry) || (mf && isSame(x, y, rx, ry))) break; \
    if(exist(rx, ry)) {\
        if(mf) { ret.push_back(MP(rx, ry)); break; }\
        else mf = true;\
    } else if(!mf) { ret.push_back(MP(rx, ry)); }

std:: vector<IPoint> Rule:: reachingGun(int x, int y, int p) {
    (void)(p);
    bool mf;
    std:: vector<IPoint> ret; ret.clear();
    mf = false; for(int rx = x - 1, ry = y; ; -- rx) { funcGun; }
    mf = false; for(int rx = x + 1, ry = y; ; ++ rx) { funcGun; }
    mf = false; for(int rx = x, ry = y - 1; ; -- ry) { funcGun; }
    mf = false; for(int rx = x, ry = y + 1; ; ++ ry) { funcGun; }
    return ret;
}

std:: vector<IPoint> Rule:: reachingSoldier(int x, int y, int p) {
    (void)(p);
    const int yL[2] = {0, 5};
    const int yR[2] = {4, 9};
    const int fS[2] = {1, -1};

    // forward
    int dx = x, dy = y + fS[p];
    std:: vector<IPoint> ret; ret.clear();
    if(inRange(dx, dy) && !isSame(x, y, dx, dy)) ret.push_back(MP(dx, dy));

    if(!IR(yL[p], y, yR[p])) {
        const int mx[2] = {-1, 1};
        for(int i = 0; i < 2; ++ i) {
            dx = x + mx[i], dy = y;
            if(inRange(dx, dy) && !isSame(x, y, dx, dy))
                ret.push_back(MP(dx, dy));
        }
    }
    return ret;
}

std:: vector<IPoint> Rule:: reachingPos(int x, int y) {
    Piece chess = typeArr[x][y];
    switch(chess) {
        case  General: return reachingGeneral(x, y, playerArr[x][y]);
        case   Knight: return reachingKnight(x, y, playerArr[x][y]);
        case Elephant: return reachingElephant(x, y, playerArr[x][y]);
        case    Horse: return reachingHorse(x, y, playerArr[x][y]);
        case  Chariot: return reachingChariot(x, y, playerArr[x][y]);
        case      Gun: return reachingGun(x, y, playerArr[x][y]);
        case  Soldier: return reachingSoldier(x, y, playerArr[x][y]);
              default: assert(0); // error
    }
    assert(0);
}

bool Rule:: checkIn(const std:: vector<IPoint> &vec, IPoint point) {
    for(auto it: vec) {
        if(it == point) return true;
    }
    return false;
}
