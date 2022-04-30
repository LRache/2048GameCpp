//
// Created by Rache on 2022/4/30.
//

#ifndef INC_2048GAME_GAMEAREAENDWIDGET_H
#define INC_2048GAME_GAMEAREAENDWIDGET_H

#include <QWidget>
#include <QVariantAnimation>
#include <QSequentialAnimationGroup>
#include <QPushButton>


class GameAreaEndWidget : public QWidget{
    Q_OBJECT
public:
    GameAreaEndWidget(int frameSize, int cellSep, int cellSize, int cellRadius,int frameSep);

    void paintEvent(QPaintEvent *event) override;
    void start(int row, int column);
    void load_style(const QString &t, const QFont &textFont, const QBrush &cellBgBrush, const QColor &textColor);

    QString tellHerText;
    QPushButton *exitButton;

private:
    int cellRadius;
    int cellSize;
    int cellSep;
    int frameSep;
    QRect tellHerRect;
    QString text;
    int fontSizeStart = 0;
    QFont textFont;
    QFont tellHerFont;
    QBrush cellBgBrush;
    QColor textColor;

    QVariantAnimation variantAnimation;
    QVariantAnimation tellHerTextOpacityAnimation;
    int rectDx = 0;
    int rectDy = 0;
    int rectSx = 0;
    int rectSy = 0;
    int rectX = 0;
    int rectY = 0;
    int rectSize = 0;
    int rectDSize = 0;
    double tellHerTextOpacity = 0.0;

private slots:
    void variantAnimationValueChanged(const QVariant &variant);
    void tellHerTextOpacityAnimation_valueChanged(const QVariant &variant);
    void exitButton_clicked();
};


#endif //INC_2048GAME_GAMEAREAENDWIDGET_H
