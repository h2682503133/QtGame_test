#include "GameWidget.h"

#include <QRandomGenerator>
#include <QMessageBox>

void GameWidget::keyPressEvent(QKeyEvent *event)
{
    if (gameOver) return;

    switch (event->key()) {
    case Qt::Key_Up:
        upPressed = true;
        break;
    case Qt::Key_Down:
        downPressed = true;
        break;
    case Qt::Key_Left:
        leftPressed = true;
        break;
    case Qt::Key_Right:
        rightPressed = true;
        break;
    case Qt::Key_Z:
        zPressed = true;
        break;
    default:
        QWidget::keyPressEvent(event);
    }
}

void GameWidget::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:
        upPressed = false;
        break;
    case Qt::Key_Down:
        downPressed = false;
        break;
    case Qt::Key_Left:
        leftPressed = false;
        break;
    case Qt::Key_Right:
        rightPressed = false;
        break;
    case Qt::Key_Z:
        zPressed = false;
    default:
         QWidget::keyReleaseEvent(event);
    }
}
