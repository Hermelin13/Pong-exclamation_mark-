#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QTimer>

class GameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameWidget(QWidget *parent = nullptr);
    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void gameLoop();

private:
    void resetBall();

    static constexpr int W = 800;
    static constexpr int H = 600;
    static constexpr int PADDLE_W = 12;
    static constexpr int PADDLE_H = 80;
    static constexpr int BALL_SZ = 14;
    static constexpr int PLAYER_X = 20;
    static constexpr int AI_X = W - 20 - PADDLE_W;

    double ballX, ballY;
    double ballDX, ballDY;
    double playerY;
    double aiY;
    int playerScore;
    int aiScore;

    QTimer *gameTimer;
    bool paused;
};

#endif // GAMEWIDGET_H
