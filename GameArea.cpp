//
// Created by Rache on 2021/12/11.
//

#include "GameArea.h"

#include <QPainter>
#include <QDebug>
#include <iostream>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>

GameArea::GameArea() {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            cellRect[i][j] = QRect(frameSep + (cellSize + cellSep) * j,
                                   frameSep + (cellSize + cellSep) * i,
                                   cellSize, cellSize);
        }
    }
    memset(data, 0, sizeof(data));

    setFixedSize(frameSize, frameSize);

    moveAnimationTimer.setInterval(6);
    spawnAnimationTimer1.setInterval(1);
    spawnAnimationTimer2.setInterval(1);

    connect(&moveAnimationTimer, SIGNAL(timeout()), this, SLOT(moveAnimationTimer_timeout()));
    connect(&spawnAnimationTimer1, SIGNAL(timeout()), this, SLOT(spawnAnimationTimer1_timeout()));
    connect(&spawnAnimationTimer2, SIGNAL(timeout()), this, SLOT(spawnAnimationTimer2_timeout()));

    gameAreaWinWidget = new GameAreaWinWidget(frameSize, frameRadius);
    gameAreaWinWidget->setParent(this);
    gameAreaGraphicsOpacityEffect = new QGraphicsOpacityEffect(this);
    gameAreaEndWidget = new GameAreaEndWidget(frameSize, cellSep, cellSize, cellRadius, frameSep);
    gameAreaEndWidget->setParent(this);
    gameAreaEndWidget->hide();

    gameAreaWinWidget->setGraphicsEffect(gameAreaGraphicsOpacityEffect);
    gameAreaGraphicsOpacityEffect->setOpacity(0);
}

void GameArea::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(Qt::NoPen);
    painter.setBrush(frameBrush);
    painter.drawRoundedRect(frameRect, frameRadius, frameRadius);

    for (int i = 0; i < cellCount; ++i) {
        for (int j = 0; j < cellCount; ++j) {
            int number = data[i][j];
            if (number > 17) {
                number = 18;
            }
            painter.setPen(Qt::NoPen);
            painter.setBrush(cellBgBrushes[number]);
            painter.drawRoundedRect(cellRect[i][j], cellRadius, cellRadius);
            if (number != 0) {
                painter.setFont(cellTextFonts[number]);
                painter.setPen(cellTextColors[number]);
                painter.drawText(cellRect[i][j], Qt::AlignCenter, cellTexts[number]);
            }
        }
    }

    if (moveAnimationRunning) {
        for (int i = 0; i < moveAnimationCount; ++i) {
            auto animation = moveAnimations[i];
            int n = animation.number;
            if (n > 17){
                n = 18;
            }
            painter.setPen(Qt::NoPen);
            painter.setBrush(cellBgBrushes[n]);
            QRect rect(animation.x, animation.y, cellSize, cellSize);
            painter.drawRoundedRect(rect, cellRadius, cellRadius);
            painter.setFont(cellTextFonts[n]);
            painter.setPen(cellTextColors[n]);
            painter.drawText(rect, Qt::AlignCenter, cellTexts[n]);
        }
    }

    if (spawnAnimationRunning) {
        for (int i = 0; i < spawnAnimationCount; ++i) {
            auto animation = spawnAnimations[i];
            int n = animation.number;
            if (n > 17){
                n = 18;
            }
            painter.setPen(Qt::NoPen);
            painter.setBrush(cellBgBrushes[n]);
            QRect rect(animation.x - spawnAnimationProcess / 4, animation.y - spawnAnimationProcess / 4,
                       cellSize + spawnAnimationProcess / 2,
                       cellSize + spawnAnimationProcess / 2);
            painter.drawRoundedRect(rect, cellRadius, cellRadius);
            painter.setFont(cellTextFonts[n]);
            painter.setPen(cellTextColors[n]);
            painter.drawText(rect, Qt::AlignCenter, cellTexts[n]);
        }
    }

    QWidget::paintEvent(event);
}

void GameArea::moveAnimationTimer_timeout() {
    moveAnimationProcess++;
    for (int i = 0; i < moveAnimationCount; ++i) {
        moveAnimations[i].x += moveAnimations[i].xSpeed;
        moveAnimations[i].y += moveAnimations[i].ySpeed;
    }
    repaint();
    if (moveAnimationProcess == moveAnimationEndProcess) {
        end_move_animation();
    }
}

void GameArea::spawnAnimationTimer1_timeout() {
    spawnAnimationProcess++;
    repaint();
    if (spawnAnimationProcess == spawnAnimationEndProcess) {
        end_spawn_animation1();
    }
}

void GameArea::spawnAnimationTimer2_timeout() {
    spawnAnimationProcess--;
    repaint();
    if (spawnAnimationProcess == 0) {
        end_spawn_animation();
    }
}

void GameArea::add_move_animation(int fromRow, int fromColumn, int toRow, int toColumn, int number) {
    NumberMoveAnimation animation;
    animation.number = number;
    animation.x = frameSep + (cellSize + cellSep) * fromColumn;
    animation.y = frameSep + (cellSize + cellSep) * fromRow;
    animation.xSpeed = (cellSize + cellSep) * (toColumn - fromColumn) / moveAnimationEndProcess;
    animation.ySpeed = (cellSize + cellSep) * (toRow - fromRow) / moveAnimationEndProcess;
    animation.fr = fromRow, animation.fc = fromColumn, animation.tr = toRow, animation.tc = toColumn;
    moveAnimations[moveAnimationCount++] = animation;
}

void GameArea::begin_move_animation() {
    if (moveAnimationCount == 0) {
        begin_spawn_animation();
        return;
    }
    moveAnimationProcess = 0;
    for (int i = 0; i < moveAnimationCount; ++i) {
        data[moveAnimations[i].fr][moveAnimations[i].fc] = 0;
    }
    moveAnimationRunning = true;
    moveAnimationTimer.start();
}

void GameArea::end_move_animation() {
    moveAnimationTimer.stop();
    for (int i = 0; i < moveAnimationCount; ++i) {
        data[moveAnimations[i].tr][moveAnimations[i].tc] = moveAnimations[i].number;
    }
    moveAnimationCount = 0;
    moveAnimationRunning = false;
    repaint();
    begin_spawn_animation();
}

void GameArea::add_spawn_animation(int row, int column, int number) {
    NumberSpawnAnimation animation;
    animation.x = frameSep + (cellSize + cellSep) * column;
    animation.y = frameSep + (cellSize + cellSep) * row;
    animation.row = row;
    animation.column = column;
    animation.number = number;
    spawnAnimations[spawnAnimationCount++] = animation;
}

void GameArea::begin_spawn_animation() {
    if (spawnAnimationCount == 0) {
        return;
    }
    spawnAnimationProcess = 0;
    spawnAnimationRunning = true;
    spawnAnimationTimer1.start();
}

void GameArea::end_spawn_animation1() {
    spawnAnimationTimer1.stop();
    spawnAnimationTimer2.start();
}

void GameArea::end_spawn_animation() {
    spawnAnimationTimer2.stop();
    for (int i = 0; i < spawnAnimationCount; ++i) {
        data[spawnAnimations[i].row][spawnAnimations[i].column] = spawnAnimations[i].number;
    }
    spawnAnimationCount = 0;
    spawnAnimationRunning = false;
    update();
}

void GameArea::output() {
    for (auto & i : data) {
        for (int j : i) {
            printf("%3d ", j);
        }
        printf("\n");
    }
    printf("\n");
}

void GameArea::start_animation() {
    begin_move_animation();
    // output();
}

void GameArea::clear() {
    memset(data, 0, sizeof(data));
}

void GameArea::stop_animation() {
    moveAnimationTimer.stop();
    spawnAnimationTimer1.stop();
    spawnAnimationTimer2.stop();

    for (int i = 0; i < moveAnimationCount; ++i) {
        data[moveAnimations[i].tr][moveAnimations[i].tc] = moveAnimations[i].number;
    }
    moveAnimationCount = 0;
    moveAnimationRunning = false;

    for (int i = 0; i < spawnAnimationCount; ++i) {
        data[spawnAnimations[i].row][spawnAnimations[i].column] = spawnAnimations[i].number;
    }
    spawnAnimationCount = 0;
    spawnAnimationRunning = false;
    repaint();
}

void GameArea::play_win_animation() {
    auto *animation1 = new QPropertyAnimation(gameAreaGraphicsOpacityEffect, "opacity");
    animation1->setStartValue(0);
    animation1->setEndValue(1);
    animation1->setDuration(1000);
    auto *animation2 = new QPropertyAnimation(gameAreaGraphicsOpacityEffect, "opacity");
    animation2->setStartValue(1);
    animation2->setEndValue(0);
    animation2->setDuration(1000);

    auto *sequentialAnimationGroup = new QSequentialAnimationGroup();
    sequentialAnimationGroup->addAnimation(animation1);
    sequentialAnimationGroup->addPause(1500);
    sequentialAnimationGroup->addAnimation(animation2);
    sequentialAnimationGroup->start();
}

void GameArea::play_end_animation(int row, int column) {
    gameAreaEndWidget->start(row, column);
}

void GameArea::reload_style() {
    gameAreaEndWidget->load_style(cellTexts[17], cellTextFonts[17], cellBgBrushes[17], cellTextColors[17]);
}

void GameArea::setTellHerText(const QString &text) {
    gameAreaEndWidget->tellHerText = text;
}
