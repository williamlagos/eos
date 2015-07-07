#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <cmath>
#include <QMainWindow>
#include <QGraphicsView>
#include "codes.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void drawQuadWhite(QGraphicsScene *s, int quad, int depth);
    void drawQuadBlack(QGraphicsScene *s, int quad, int depth);
    void drawQuad(QGraphicsScene *s, int quad, QPen stroke, QBrush fill, int depth);
    void drawQuadCode(vector<int> v, QGraphicsScene *s);
    void drawQuadCode(vector<int> v, QGraphicsScene *s, int quad, int depth);
private:
    Ui::MainWindow *ui;
    Codes *c;
    int quad_size;

public slots:
    void getTextCode();
    void getBlackQuantity();
};

#endif // MAINWINDOW_H
