#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "gamewidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , gameWidget(new GameWidget(this))
{
    ui->setupUi(this);
    setCentralWidget(gameWidget);
    setWindowTitle("Pong!");
    menuBar()->hide();
    statusBar()->hide();
    adjustSize();
    setFixedSize(size());
}

MainWindow::~MainWindow()
{
    delete ui;
}
