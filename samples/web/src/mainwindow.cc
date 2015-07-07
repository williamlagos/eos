#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    c = new Codes();
    QObject::connect(ui->pushButton,SIGNAL(clicked()),
                     this,SLOT(getTextCode()));
    QObject::connect(ui->pushButton_2,SIGNAL(clicked()),
                     this,SLOT(getBlackQuantity()));
    quad_size = 200;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::drawQuad(QGraphicsScene *s, int quad, QPen stroke, QBrush fill, int depth)
{
    int x = 0;
    int y = 0;
    int size = quad_size;
    int sum = 0;
    for(int count = 1; count < depth; count++){
        size /= 2; sum += size;
    }
    switch(quad){
        case 1:
                x = y = sum+quad_size;
                break;
        case 2:
                x = sum+quad_size;
                y = sum;
                break;
        case 3:
                x = y = sum;
                break;
        case 4:
                x = sum;
                y = sum+quad_size;
                break;
    }
    s->addRect(x,y,size,size,stroke,fill);
}

void MainWindow::drawQuadBlack(QGraphicsScene *s, int quad, int depth)
{
    drawQuad(s,quad,QPen(Qt::black),QBrush(Qt::black),depth);
}

void MainWindow::drawQuadWhite(QGraphicsScene *s, int quad, int depth)
{
    drawQuad(s,quad,QPen(Qt::white),QBrush(Qt::white),depth);
}

void MainWindow::drawQuadCode(vector<int> v, QGraphicsScene *s)
{
    drawQuadCode(v,s,1,0);
}

void MainWindow::drawQuadCode(vector<int> v, QGraphicsScene *s, int quad, int depth)
{
    vector<int>::iterator it,next;
    for(it = v.begin(); it != v.end(); ++it){
        if(*it != GRAY){
            if((*it) == BLACK) drawQuadBlack(s,quad,depth);
            else if((*it) == WHITE) drawQuadWhite(s,quad,depth);
            quad++; quad %= 5;
        }else if((*it) == GRAY){
            next = it; next++;
            depth++; vector<int> t(next,v.end());
            drawQuadCode(t,s,quad,depth);
        }
    }
}

void MainWindow::getTextCode()
{
    c->clearTree();
    ui->statusBar->showMessage("Convertendo código...");
    QString t = ui->plainTextEdit->toPlainText();
    QGraphicsScene *s = new QGraphicsScene();
    ui->graphicsView->setSceneRect(0,0,400,400);
    ui->graphicsView->setScene(s);
    string msg = c->parseCode(t.toStdString());
    drawQuadCode(c->sourceTree(),s);
    ui->statusBar->showMessage(msg.c_str());
}

void MainWindow::getBlackQuantity()
{
    ostringstream oss;
    oss << "Quantidade de blocos pretos: " << c->blacks();
    string s = oss.str();
    ui->statusBar->showMessage(s.c_str());
    ui->statusBar->showMessage(s.c_str());
}
