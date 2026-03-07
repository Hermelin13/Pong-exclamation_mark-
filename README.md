# Pong!

A classic Pong arcade game built with C++ and the Qt framework.

## Features

- **Player vs AI** — control the left paddle with your mouse, while the right paddle is controlled by a simple AI opponent
- **Mouse control** — move your mouse up and down to position your paddle
- **Pause/Resume** — press `Space` to pause and resume the game
- **Score tracking** — scores are displayed at the top of the screen
- **~60 FPS game loop** — smooth gameplay powered by `QTimer`

## How to Play

| Control       | Action              |
|---------------|---------------------|
| Mouse (Y-axis)| Move player paddle |
| Space         | Pause / Resume      |

The ball bounces off the top and bottom walls. If it passes your paddle, the AI scores a point — and vice versa. The ball angle changes depending on where it hits the paddle.

## Building

### Prerequisites

- **Qt 5** or **Qt 6** (Widgets module)
- **CMake** 3.16+
- A C++17 compatible compiler

### Build steps

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

### Run

```bash
./pong
```

## Project Structure

| File             | Description                                      |
|------------------|--------------------------------------------------|
| `main.cpp`       | Application entry point                          |
| `mainwindow.h/cpp` | Main window setup, hosts the game widget       |
| `mainwindow.ui`  | Qt Designer UI file for the main window          |
| `gamewidget.h/cpp`| Core game logic — ball physics, collisions, AI, rendering |
| `CMakeLists.txt` | CMake build configuration                        |

## Technical Details

- **Game loop**: A `QTimer` fires every 16 ms (~60 FPS), triggering the `gameLoop()` slot which updates ball position, checks collisions, moves the AI paddle, and repaints the widget
- **Rendering**: All drawing is done in `paintEvent()` using `QPainter`
- **AI**: The AI paddle tracks the ball's vertical center with a limited speed (`3.5 px/tick`), making it beatable
- **Ball physics**: The ball's vertical angle changes based on where it hits the paddle — hitting the edge produces a steeper bounce

## License

This project is provided as-is for educational purposes.
