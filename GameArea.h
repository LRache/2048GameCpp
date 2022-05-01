//
// Created by Rache on 2021/12/11.
//

#ifndef INC_2048GAME_GAMEAREA_H
#define INC_2048GAME_GAMEAREA_H

#include <QWidget>
#include <QTimer>
#include <QGraphicsOpacityEffect>
#include "GameAreaWinWidget.h"
#include "GameAreaEndWidget.h"

struct NumberMoveAnimation {
    int number = 0;
    int x = 0;
    int y = 0;
    int xSpeed = 0;
    int ySpeed = 0;
    int fr = 0, fc = 0, tr = 0, tc = 0;
};

struct NumberSpawnAnimation{
    int number = 0;
    int x = 0;
    int y = 0;
    int row = 0, column = 0;
};

class GameArea : public QWidget{
    Q_OBJECT
public:
    GameArea();
    void paintEvent(QPaintEvent *event) override;

    QString cellTexts[19] = {};
    QFont cellTextFonts[19] = {};
    QBrush cellBgBrushes[19] = {};
    QColor cellTextColors[19] = {};

    int data[4][4];
    void clear();

    void start_animation();
    void stop_animation();
    void add_move_animation(int fromRow, int fromColumn, int toRow, int toColumn, int number);
    void add_spawn_animation(int row, int column, int number);

    void show_win_animation();
    void show_end_animation(int row, int column);
    void reload_style();
    void setTellHerText(const QString &text);

    int cellCount = 4;

public:
    static const int cellSize  = 60;
    static const int cellSep   = 10;
    static const int frameSep  = 8;
    static const int frameRadius= 8;
    static const int cellRadius = 2;
    int frameSize = cellSize * cellCount + cellSep * (cellCount - 1) + frameSep * 2;

public slots:
    void moveAnimationTimer_timeout();
    void spawnAnimationTimer1_timeout();
    void spawnAnimationTimer2_timeout();

private:
    void begin_move_animation();
    void end_move_animation();
    void begin_spawn_animation();
    void end_spawn_animation1();
    void end_spawn_animation();

    void output();

    const QBrush frameBrush = QBrush(QColor(187, 173, 160));
    const QRect frameRect   = QRect(0, 0, frameSize, frameSize);
    QRect cellRect[4][4];

    /*const QBrush cellBgBrushes[19] = {
            QColor(205, 193, 180), // empty
            QColor(238, 228, 218), // 2
            QColor(237, 224, 200), // 4
            QColor(242, 177, 121), // 8
            QColor(245, 149, 99), // 16
            QColor(246, 124, 95), // 32
            QColor(246, 94, 59), // 64
            QColor(237, 207, 114), // 128
            QColor(237, 204, 97), // 256
            QColor(237, 200, 80), // 512
            QColor(237, 197, 63), // 1024
            QColor(237, 196, 45), // 2048
            QColor(186, 215, 112), // 4096
            QColor(198, 218, 93), // 8192
            QColor(106, 183, 241), // 16384
            QColor(66, 164, 247), // 32768
            QColor(42, 141, 222), // 65536
            QColor(151, 115, 207), // 131072
            QColor(104, 58, 185), // max
    };*/

    /*const QColor cellTextColors[19] = {
            QColor(), // empty
            QColor(119, 110, 101), // 2
            QColor(119, 110, 101), // 4
            QColor(249, 246, 242), // 8
            QColor(249, 246, 242), // 16
            QColor(249, 246, 242), // 32
            QColor(249, 246, 242), // 64
            QColor(249, 246, 242), // 128
            QColor(249, 246, 242), // 256
            QColor(249, 246, 242), // 512
            QColor(249, 246, 242), // 1024
            QColor(249, 246, 242), // 2048
            QColor(249, 246, 242), // 4096
            QColor(249, 246, 242), // 8192
            QColor(249, 246, 242), // 16384
            QColor(249, 246, 242), // 32768
            QColor(249, 246, 242), // 65536
            QColor(249, 246, 242), // 131072
            QColor(249, 246, 242) // max
    };*/

    /*const QFont cellTextFonts[19] = {
            QFont(), // empty
            QFont("Microsoft YaHei", 52), // 2
            QFont("Microsoft YaHei", 52), // 4
            QFont("Microsoft YaHei", 52), // 8
            QFont("Microsoft YaHei", 52), // 16
            QFont("Microsoft YaHei", 52), // 32
            QFont("Microsoft YaHei", 52), // 64
            QFont("Microsoft YaHei", 52), // 128
            QFont("Microsoft YaHei", 52), // 256
            QFont("Microsoft YaHei", 52), // 512
            QFont("Microsoft YaHei", 52), // 1024
            QFont("Microsoft YaHei", 52), // 2048
            QFont("Microsoft YaHei", 52), // 4096
            QFont("Microsoft YaHei", 52), // 8192
            QFont("Microsoft YaHei", 52), // 16384
            QFont("Microsoft YaHei", 52), // 32768
            QFont("Microsoft YaHei", 52), // 65536
            QFont("Microsoft YaHei", 20), // 131072
            QFont("Microsoft YaHei", 16), // undefined
    };*/

    QTimer moveAnimationTimer;
    QTimer spawnAnimationTimer1;
    QTimer spawnAnimationTimer2;

    NumberMoveAnimation moveAnimations[16];
    NumberSpawnAnimation spawnAnimations[16];
    int moveAnimationCount      = 0;
    int spawnAnimationCount     = 0;
    int moveAnimationProcess    = 0;
    int spawnAnimationProcess   = 0;
    const int moveAnimationEndProcess = 18;
    const int spawnAnimationEndProcess = 25;
    bool moveAnimationRunning = false;
    bool spawnAnimationRunning = false;

    GameAreaWinWidget *gameAreaWinWidget;
    QGraphicsOpacityEffect *gameAreaGraphicsOpacityEffect;
    GameAreaEndWidget *gameAreaEndWidget;
};


#endif //INC_2048GAME_GAMEAREA_H
