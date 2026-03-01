#include "gamewidget.h"

#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QTimerEvent>
#include <cmath>
#include <random>

GameWidget::GameWidget(QWidget *parent)
    : QWidget(parent)
    , playerY((H - PADDLE_H) / 2.0)
    , aiY((H - PADDLE_H) / 2.0)
    , playerScore(0)
    , aiScore(0)
    , paused(false)
{
    setFixedSize(W, H);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    resetBall();
    gameTimerId = startTimer(16); // ~60 FPS
}

void GameWidget::resetBall()
{
    ballX = W / 2.0 - BALL_SZ / 2.0;
    ballY = H / 2.0 - BALL_SZ / 2.0;
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> coin(0, 1);
    ballDX = (coin(rng) == 0) ? 5.0 : -5.0;
    ballDY = (coin(rng) == 0) ? 3.0 : -3.0;
}

QSize GameWidget::sizeHint() const
{
    return QSize(W, H);
}

void GameWidget::timerEvent(QTimerEvent *event)
{
    if (event->timerId() != gameTimerId)
        return;
    if (paused)
        return;

    // Move ball
    ballX += ballDX;
    ballY += ballDY;

    // Bounce off top and bottom walls
    if (ballY <= 0) {
        ballY = 0;
        ballDY = std::abs(ballDY);
    } else if (ballY + BALL_SZ >= H) {
        ballY = H - BALL_SZ;
        ballDY = -std::abs(ballDY);
    }

    // Collide with player paddle (left)
    if (ballDX < 0
        && ballX <= PLAYER_X + PADDLE_W
        && ballX + BALL_SZ >= PLAYER_X
        && ballY + BALL_SZ >= playerY
        && ballY <= playerY + PADDLE_H) {
        ballX = PLAYER_X + PADDLE_W;
        ballDX = std::abs(ballDX);
        // Add slight angle variation based on hit position
        double relHit = (ballY + BALL_SZ / 2.0 - playerY) / PADDLE_H;
        ballDY = (relHit - 0.5) * 8.0;
    }

    // Collide with AI paddle (right)
    if (ballDX > 0
        && ballX + BALL_SZ >= AI_X
        && ballX <= AI_X + PADDLE_W
        && ballY + BALL_SZ >= aiY
        && ballY <= aiY + PADDLE_H) {
        ballX = AI_X - BALL_SZ;
        ballDX = -std::abs(ballDX);
        double relHit = (ballY + BALL_SZ / 2.0 - aiY) / PADDLE_H;
        ballDY = (relHit - 0.5) * 8.0;
    }

    // Scoring
    if (ballX + BALL_SZ < 0) {
        aiScore++;
        resetBall();
    } else if (ballX > W) {
        playerScore++;
        resetBall();
    }

    // AI movement: track ball center with limited speed and slight imperfection
    double ballCenter = ballY + BALL_SZ / 2.0;
    double aiCenter   = aiY + PADDLE_H / 2.0;
    constexpr double AI_SPEED = 3.5;
    if (aiCenter < ballCenter - 4) {
        aiY = std::min(aiY + AI_SPEED, static_cast<double>(H - PADDLE_H));
    } else if (aiCenter > ballCenter + 4) {
        aiY = std::max(aiY - AI_SPEED, 0.0);
    }

    update();
}

void GameWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    // Black background
    p.fillRect(rect(), Qt::black);

    // Dashed center line
    QPen dashedPen(Qt::white, 2, Qt::DashLine);
    p.setPen(dashedPen);
    p.drawLine(W / 2, 0, W / 2, H);

    p.setPen(Qt::NoPen);
    p.setBrush(Qt::white);

    // Player paddle
    p.drawRect(PLAYER_X, static_cast<int>(playerY), PADDLE_W, PADDLE_H);

    // AI paddle
    p.drawRect(AI_X, static_cast<int>(aiY), PADDLE_W, PADDLE_H);

    // Ball
    p.drawRect(static_cast<int>(ballX), static_cast<int>(ballY), BALL_SZ, BALL_SZ);

    // Score
    QFont scoreFont("Monospace", 36, QFont::Bold);
    scoreFont.setStyleHint(QFont::TypeWriter);
    p.setFont(scoreFont);
    p.setPen(Qt::white);
    QRect leftScoreRect(W / 2 - 120, 20, 100, 60);
    QRect rightScoreRect(W / 2 + 20, 20, 100, 60);
    p.drawText(leftScoreRect, Qt::AlignRight | Qt::AlignVCenter, QString::number(playerScore));
    p.drawText(rightScoreRect, Qt::AlignLeft | Qt::AlignVCenter, QString::number(aiScore));

    // Paused overlay
    if (paused) {
        p.setPen(Qt::white);
        QFont pauseFont("Sans", 28, QFont::Bold);
        p.setFont(pauseFont);
        p.drawText(rect(), Qt::AlignCenter, "PAUSED\nPress Space to continue");
    }
}

void GameWidget::mouseMoveEvent(QMouseEvent *event)
{
    playerY = event->pos().y() - PADDLE_H / 2.0;
    if (playerY < 0)
        playerY = 0;
    if (playerY > H - PADDLE_H)
        playerY = H - PADDLE_H;
    update();
}

void GameWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space) {
        paused = !paused;
        update();
    }
}
