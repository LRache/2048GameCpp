//
// Created by Rache on 2022/4/29.
//

#ifndef INC_2048GAME_GAMEAREAWINWIDGET_H
#define INC_2048GAME_GAMEAREAWINWIDGET_H

#include <QWidget>

class GameAreaWinWidget : public QWidget{
public:
    GameAreaWinWidget(int s, int r);

    void paintEvent(QPaintEvent *event) override;

private:
    int frameRadius;
    int frameSize;

    const QFont textFont = QFont("Bahnschrift SemiBold", 36);
};


#endif //INC_2048GAME_GAMEAREAWINWIDGET_H
