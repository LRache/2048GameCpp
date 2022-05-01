//
// Created by Rache on 2022/4/30.
//

#include <QPainter>
#include <QDebug>
#include "GameAreaEndWidget.h"

GameAreaEndWidget::GameAreaEndWidget(int s, int csep, int cs, int cr, int fs) {
    cellSize = cs;
    cellSep = csep;
    cellRadius = cr;
    frameSep = fs;
    rectDSize = cellSize * 4 + cellSep * 3 - cellSize;
    tellHerRect = QRect(0, cellSize * 3 + cellSep * 3, cellSize * 4 + cellSep * 3 + frameSep * 2, cellSize / 3);
    tellHerFont = QFont("Microsoft YaHei", 14);

    exitButton = new QPushButton("退出", this);
    exitButton->setFixedSize(cellSize, cellSize / 3);
    exitButton->move(cellSize * 2 + cellSep * 3 + frameSep * 2 + cellSize / 2 + cellSize / 3, cellSize * 3 + cellSep * 3 + frameSep * 2 + cellSize / 2);
    exitButton->setStyleSheet("background-color: #ffffff; border-radius: 5px;");

    setFixedSize(s, s);
    setAttribute(Qt::WA_TranslucentBackground, true);

    connect(&variantAnimation, SIGNAL(valueChanged(QVariant)), this, SLOT(variantAnimationValueChanged(QVariant)));
    connect(&tellHerTextOpacityAnimation, SIGNAL(valueChanged(QVariant)), this, SLOT(tellHerTextOpacityAnimation_valueChanged(QVariant)));
    connect(exitButton, SIGNAL(clicked()), this, SLOT(exitButton_clicked()));
}

void GameAreaEndWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(Qt::NoPen);
    painter.setBrush(cellBgBrush);
    painter.drawRoundedRect(rectX, rectY, rectSize, rectSize, cellRadius, cellRadius);
    painter.setPen(textColor);
    painter.setFont(textFont);
    painter.drawText(rectX, rectY, rectSize, rectSize, Qt::AlignCenter, text);

    painter.setOpacity(tellHerTextOpacity);
    painter.setFont(tellHerFont);
    painter.drawText(tellHerRect, Qt::AlignCenter, tellHerText);

    QWidget::paintEvent(event);
}

void GameAreaEndWidget::start(int r, int c) {
    rectX = rectSx = frameSep + (cellSize + cellSep) * c;
    rectY = rectSy =frameSep + (cellSize + cellSep) * r;
    rectDx = frameSep - rectX;
    rectDy = frameSep - rectY;
    rectSize = cellSize;
    show();

    variantAnimation.setStartValue(0.0);
    variantAnimation.setEndValue(1.0);
    variantAnimation.setDuration(700);
    tellHerTextOpacityAnimation.setStartValue(0.0);
    tellHerTextOpacityAnimation.setEndValue(1.0);
    tellHerTextOpacityAnimation.setDuration(1000);
    auto *sequentialAnimationGroup = new QSequentialAnimationGroup;
    sequentialAnimationGroup->addPause(200);
    sequentialAnimationGroup->addAnimation(&variantAnimation);
    sequentialAnimationGroup->addAnimation(&tellHerTextOpacityAnimation);
    sequentialAnimationGroup->start();
}

void GameAreaEndWidget::load_style(const QString &t, const QFont &tf, const QBrush &bgb,
                                   const QColor &tc) {
    text = t;
    fontSizeStart = tf.pointSize();
    textFont = tf;
    cellBgBrush = bgb;
    textColor = tc;
}

void GameAreaEndWidget::variantAnimationValueChanged(const QVariant &variant) {
    double step = variant.toDouble();
    rectX = rectSx + (int)(step * rectDx);
    rectY = rectSy + (int)(step * rectDy);
    rectSize = cellSize + (int)(step * rectDSize);
    textFont.setPointSize(fontSizeStart * (1 + step * 3));
    repaint();
}

void GameAreaEndWidget::tellHerTextOpacityAnimation_valueChanged(const QVariant &variant) {
    tellHerTextOpacity = variant.toDouble();
    repaint();
}

void GameAreaEndWidget::exitButton_clicked() {
    hide();
    textFont.setPointSize(fontSizeStart);
    tellHerTextOpacity = 0.0;
}
