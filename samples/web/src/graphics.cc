#include "aeroshell.h"

GridLines::GridLines( int w, int h ) : QGraphicsItem(),_width(w),_height(h),_space(10) {}

void GridLines::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QColor c (200,200,200,255);
    painter->setPen(c);
    for (int y= 0; y < _height; y+=_space) {
        painter->drawLine(0,y,_width,y);
    }
    for (int x= 0; x < _width; x+=_space) {
        painter->drawLine(x,0,x,_height);
    }
}

QRectF GridLines::boundingRect() const
{
    return QRectF ( static_cast<qreal>(0), static_cast<qreal>(0), static_cast<qreal>(_width), static_cast<qreal>( _height));
}

void GridLines::handleWindowSizeChanged(int w, int h)
{
    _width = w; _height = h;
}

void GridLines::mouseMoveEvent(QGraphicsSceneDragDropEvent *){}
void GridLines::mousePressEvent(QGraphicsSceneDragDropEvent *){}
void GridLines::mousePressEvent (QGraphicsSceneMouseEvent * event){}
void GridLines::mouseMoveEvent (QGraphicsSceneMouseEvent * event){}

GraphicsLine::GraphicsLine(QLineF oLine,AERgraphics *poParent) : QGraphicsLineItem(oLine,NULL)
{
  m_poParent = poParent;
  m_nX = oLine.x1();
  m_nY = oLine.y1();
}

GraphicsLine::~GraphicsLine()
{
  
}

void GraphicsLine::mousePressEvent(QGraphicsSceneMouseEvent *poEvent)
{
  m_poParent->m_poScene->addText("ABC");
}

GraphicsDialog::GraphicsDialog(AERgraphics *poGraphicsView,AERoshell *poParent,Qt::WindowFlags flags) : QDialog(NULL,flags)
{
  m_poGraphicsView = poGraphicsView;
  m_poParent = poParent;
  connect(this,SIGNAL(saved(QRect)),
          m_poParent,SLOT(saveDialogOptions(QRect)));
}

GraphicsDialog::~GraphicsDialog()
{
  delete m_poGraphicsView;
  delete m_poParent;
}

void GraphicsDialog::showWindow(AERgraphics *poGraphicsView)
{
  m_poGraphicsView = poGraphicsView;
  QByteArray oGeometry = m_poParent->m_oPlotGeometry;
  restoreGeometry(m_poParent->m_oPlotGeometry);
  m_poGraphicsView->m_poTimer->start(1000);
  m_poGraphicsView->resize(500,500);
  this->resize(500,500);
  this->show();
}

void GraphicsDialog::resizeEvent(QResizeEvent *event)
{
  m_poGraphicsView->resize(this->width(),this->height());
  QDialog::resizeEvent(event);
}

void GraphicsDialog::closeEvent(QCloseEvent *event)
{
  m_poParent->m_oPlotGeometry = saveGeometry();
  m_poParent->m_bPlotReady = false;
  m_poGraphicsView->m_poTimer->stop();
  QDialog::closeEvent(event);
}
  

AERgraphics::AERgraphics(QGraphicsScene *poScene,QWidget *poParent) : QGraphicsView(poScene,poParent)
{
  resize(poParent->size());
  m_poScene = poScene;
  this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  this->prepareScene();
  Y = 450; X = m_nViewX;
  m_nCount = 0; m_nYCycle = 0;
  m_poTimer = new QTimer();
  connect(this->m_poTimer,SIGNAL(timeout()),
          this,SLOT(updateScene()));
  m_nBorderX = (m_nViewX*0.05);
  m_nBorderY = (m_nViewY*0.05);
  m_bFirstTime = true;
  m_bFlipTime = false;
}

AERgraphics::~AERgraphics()
{

}

void AERgraphics::keyPressEvent(QKeyEvent *ke)
{
  int nNativeKey = ke->nativeVirtualKey();

  if (nNativeKey == 13) {
    m_poTimer->stop();
  }
}

void AERgraphics::resizeEvent(QResizeEvent *poEvent)
{
  m_nYDiff = poEvent->oldSize().height()-poEvent->size().height();
  QSize oNewSize = poEvent->size();
  this->getPlotCoord(true,oNewSize.width(),oNewSize.height());
  m_poAxis->handleWindowSizeChanged(m_nViewX,m_nViewY);
  QList<GraphicsLine*>::iterator it;
  for(it = m_voSettableLines.begin(); it != m_voSettableLines.end(); it++){
    QLineF ol = (*it)->line();
    (*it)->setLine(ol.x1(),ol.y1()-m_nYDiff,ol.x2(),ol.y2()-m_nYDiff);
  }
  if(m_bFirstTime) m_bFirstTime = false;
  else Y -= m_nYDiff;
}

void AERgraphics::getPlotCoord(bool bResized,int nWidth,int nHeight)
{
  if (bResized) {
    m_nViewX = nWidth;
    m_nViewY = nHeight;
  } else {
    m_nViewX = m_poScene->sceneRect().width();
    m_nViewY = m_poScene->sceneRect().height();
  }
};

void AERgraphics::prepareScene()
{
  this->getPlotCoord();
  m_poAxis = new GridLines(500,500);
  m_poScene->addItem(m_poAxis);
}

void AERgraphics::updateLines()
{
  this->getPlotCoord();
  m_poAxis->handleWindowSizeChanged(m_nViewX,m_nViewY);
}

void AERgraphics::updateScene()
{
  int add = 100;
  if (Y < 0 || m_bFlipTime) {
    m_nYCycle = -m_nCount%100;
    m_bFlipTime = true;
  } else if (Y > m_poScene->sceneRect().height()-100) {
    m_nYCycle = m_nCount%100;
    m_bFlipTime = false;
  }
  m_poLastLine = new GraphicsLine(QLineF(X,Y,X+add,Y-m_nYCycle),this);
  Y -= m_nYCycle;
  m_voSettableLines << m_poLastLine;
  m_nCount++; X += add;
  
  m_poScene->addItem(m_poLastLine);
  this->updateLines();
  this->horizontalScrollBar()->setValue(m_poScene->width());
  this->verticalScrollBar()->setValue(verticalScrollBar()->maximum());
  /*m_voClickableLines << m_poLastLine;
  m_voSettableLines << m_poLastLine;
  QGraphicsItemGroup *m_poAxisGroup = 
    m_poScene->createItemGroup(m_voClickableLines);
  if(m_bFirstTime) {
    m_poAxisGroup->setHandlesChildEvents(false);
    m_bFirstTime = false;
  }*/
}