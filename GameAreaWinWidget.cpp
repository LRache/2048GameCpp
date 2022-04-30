//
// Created by Rache on 2022/4/29.
//

#include <QPainter>
#include "GameAreaWinWidget.h"

GameAreaWinWidget::GameAreaWinWidget(int s, int r) {
    setAttribute(Qt::WA_TranslucentBackground, true);
    setFixedSize(s, s);
    frameRadius = r;
    frameSize = s;
}

void GameAreaWinWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setBrush(QColor(234, 194, 1, 96));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(0, 0, frameSize, frameSize, frameRadius, frameRadius);

    painter.setPen(Qt::white);
    painter.setFont(textFont);
    painter.drawText(QRect(0, 0, frameSize, frameSize * 3 / 4), Qt::AlignCenter, "YOU WIN!");

    QWidget::paintEvent(event);
}
