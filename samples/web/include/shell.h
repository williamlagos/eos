#ifndef AEROSHELL_H
#define AEROSHELL_H

#define DEBUG_SHELL_RECEIVE 0
#define DEBUG_SHELL_SEND 1
#define STEP_BY_STEP 0
#define DIRECT 1
#define PREVIOUS 0
#define NEXT 1

#include <QMainWindow>
#include <QMessageBox>
#include <QSystemTrayIcon>
#include <QScrollBar>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QKeyEvent>
#include <QTimer>
#include <QString>
#include <QSettings>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QPainter>
#include <QRectF>
#include <QTimeLine>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>

#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexer.h>

#include "_DebugShell.h"
#include "ui_aeroshell.h"

using namespace aer;
using namespace Qt;

class AERoshell;
class AERgraphics;

class GridLines : public QGraphicsItem { 
public:
    GridLines(int width, int height);
    void handleWindowSizeChanged(int w, int h);
private:
    int _width;
    int _height;
    int _space;

    virtual QRectF boundingRect() const; ///< must be re-implemented in this class to provide the diminsions of the box to the QGraphicsView
    virtual void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget); ///< must be re-implemented here to pain the box on the paint-event
    virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
    virtual void mouseMoveEvent(QGraphicsSceneDragDropEvent *event);
    virtual void mousePressEvent (QGraphicsSceneMouseEvent * event );
    virtual void mousePressEvent(QGraphicsSceneDragDropEvent *event);
};

class GraphicsLine : public QGraphicsLineItem {
public:
  int m_nX,m_nY;

  AERgraphics *m_poParent;
  GraphicsLine(QLineF oLine,AERgraphics *poParent);
  ~GraphicsLine();
  void mousePressEvent(QGraphicsSceneMouseEvent *poEvent);
};

class GraphicsDialog : public QDialog {
  Q_OBJECT
public:
  AERoshell *m_poParent;
  AERgraphics *m_poGraphicsView;
  GraphicsDialog(AERgraphics *poGraphicsView,AERoshell *poParent,Qt::WindowFlags flags);
  ~GraphicsDialog();
  void showWindow(AERgraphics *poGraphicsView);
  void resizeEvent(QResizeEvent *event);
  void closeEvent(QCloseEvent *event);
};

class AERgraphics : public QGraphicsView { 
  Q_OBJECT
public:

  QList<GraphicsLine*> m_voSettableLines;
  QList<QGraphicsItem*> m_voClickableLines;
  QWidget *m_poParent;
  QGraphicsScene *m_poScene;
  QGraphicsItemGroup *m_poAxisGroup;
  //GraphicsLine *m_poXAxis,*m_poYAxis
  GraphicsLine *m_poLastLine;
  GridLines *m_poAxis;
  double m_nViewX,m_nViewY,m_nBorderX,m_nBorderY,X,Y,m_nYDiff;
  int m_nCount,m_nYCycle;
  bool m_bFirstTime,m_bFlipTime;
  QTimer *m_poTimer;

  ///constructor
  AERgraphics(QGraphicsScene* poScene,QWidget *poParent);

  ///destructor
  ~AERgraphics();

  void getPlotCoord(bool bResized=false,int nWidth=0,int nHeight=0);

  ///create a grid and axis to graphics view
  void prepareScene();

  ///expand the grid and add a new line
  void updateLines();

  ///receive an event and execute interpreted commands
  void keyPressEvent(QKeyEvent *ke);

  ///reimplementation of resizeEvent
  void resizeEvent(QResizeEvent *event);

public slots:
  void updateScene();
};

class AERconsole : public QsciScintilla, public IDebugShellUpdate { Q_OBJECT public:

  AERoshell *m_poParent;
  string m_strEvent,m_strFound;
  int *m_vnLinesLength,m_nLines;
  bool m_bPauseFlag;

  ///constructor
  AERconsole(bool bDirectionFlag, AERoshell *poShell, QWidget *poParent = 0, Qt::WFlags oFlags = 0);
  
  ///destructor
  ~AERconsole();

  ///receive an event and execute interpreted commands
  void keyPressEvent(QKeyEvent *ke);

  ///search for the term present in the text
  virtual void SearchTerm(bool bChoice);

  ///clean the current indicator on scintilla widget
  virtual void ClearCurrentIndicator();

  ///update the console output
  virtual void NewDebugShellMsg(char* pszMsg);

  ///send a message to app
  virtual char *NewDebugShellEvt();

signals:

  ///trigger to flip the update interface
  void debugEventSent();

public slots:

  ///search for a term in the text
  virtual void SearchForAWord(char* pszWord, int nWordLen, bool bStepFlag);

  ///highlight the current and similar words
  virtual void HighlightCurrentWord();
};

class AERoshell : public QMainWindow { Q_OBJECT public:
  Ui::AERoshellClass ui;
  AERconsole *m_poConsoleIn,*m_poConsoleOut;

  QApplication *m_poQApp;
  QSettings *m_poSettings;
  QTimer m_oTimer;
  QMenu *m_poTrayMenu;
  QSystemTrayIcon *m_poTrayIcon;
  QAction *m_poOpenAction,*m_poCloseAction,*m_poQuitAction;
  QByteArray m_oPlotGeometry;
  GraphicsDialog *m_poPlotWindow;

  string m_strConnAddress;
  bool m_bPlotReady;
  int m_nFrequencyTime;

  ///constructor
  AERoshell(QApplication *poQApp, QWidget *poParent = 0, Qt::WFlags oFlags = 0);

  ///destructor
  ~AERoshell();

  ///receive an event and execute interpreted commands
  void keyPressEvent(QKeyEvent *ke);

  ///create a systray icon to manage aeroshell
  void addSystemTrayIcon();

  ///read the application config file
  void readApplicationConfig();

  ///write the changes in the application config file
  void writeApplicationConfig();

  ///create scintilla widgets to aeroshell main window
  void setScintillaWidgets();

  ///set properties of scintilla widgets
  void setScintillaSettings();

  ///connects the appropriate signals and slots from Qt 
  void setQtConnections();

  ///reimplementation of closeEvent
  void closeEvent(QCloseEvent *poEvent);

  ///reimplementation of resizeEvent
  void resizeEvent(QResizeEvent *poEvent);

  ///reimplementation of hideEvent
  void hideEvent(QHideEvent *poEvent);

  ///reimplementation of showEvent
  void showEvent(QShowEvent *poEvent);

public slots:

  ///function for clicking on the systray icon
  void trayIconClicked(QSystemTrayIcon::ActivationReason reason);

  ///show a performance graphics window
  void showGraphicsWindow();

  ///show a dialog to search a term in console output
  void showFindDialog();

  ///show a dialog with name and version of the program
  void showAboutDialog();

  ///send a debug event
  void postDebugEvent();

  ///update the log receiver widget 
  void getDebugLog();
};

#endif // AEROSHELL_H
