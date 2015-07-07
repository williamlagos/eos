#include "aeroshell.h"

///constructor
AERoshell::AERoshell(QApplication *poQApp, QWidget *parent, Qt::WFlags flags)
{
  Q_INIT_RESOURCE(aeroshell);
  ui.setupUi(this); 
  m_poQApp = poQApp;
  m_poPlotWindow = new GraphicsDialog(NULL,this,Qt::Window);
  m_bPlotReady = false;
  m_poSettings = new QSettings(QSettings::Format::IniFormat,
                               QSettings::Scope::UserScope,
                               "Rockhead Games","AERoshell");
  this->readApplicationConfig();
  DebugShellStartHub(ASCHAR(m_strConnAddress),NULL);
  m_oTimer.start(m_nFrequencyTime);
  this->addSystemTrayIcon();
  this->setScintillaWidgets();
  this->setScintillaSettings();
  this->setQtConnections();
  this->setWindowIcon(QIcon(":/AER/RockheadGames.png"));
}

AERoshell::~AERoshell()
{
  writeApplicationConfig();
  Q_CLEANUP_RESOURCE(aeroshell);
}

void AERoshell::keyPressEvent(QKeyEvent *ke)
{
  if (ke->matches(QKeySequence::Find)         ||
      ke->matches(QKeySequence::FindNext)     ||
      ke->matches(QKeySequence::FindPrevious) ||
      ke->key() == Qt::Key_Escape               ) {
    m_poConsoleOut->keyPressEvent(ke);
  } else {
    QMainWindow::keyPressEvent(ke);
  }
}
///create a systray icon to manage aeroshell
void AERoshell::addSystemTrayIcon()
{
  m_poOpenAction = new QAction(tr("&Open"), this);
  m_poCloseAction = new QAction(tr("&Close"), this);
  m_poQuitAction = new QAction(tr("&Quit"), this);

  m_poTrayMenu = new QMenu(this);
  m_poTrayMenu->addAction(m_poOpenAction);
  m_poTrayMenu->addAction(m_poCloseAction);
  m_poTrayMenu->addSeparator();
  m_poTrayMenu->addAction(m_poQuitAction);

  static QIcon icon(":/AER/RockheadGames.png");
  m_poTrayIcon = new QSystemTrayIcon(this);
  m_poTrayIcon->setIcon(icon);
  m_poTrayIcon->setContextMenu(m_poTrayMenu);
  m_poTrayIcon->setVisible(true);
}

void AERoshell::readApplicationConfig()
{
  setGeometry(m_poSettings->value("Geometry",QVariant(QRect(270,120,800,600))).toRect());
  QVariant oAddress = m_poSettings->value("Address",QVariant("127.0.0.1:4000"));
  QVariant oFrqTime = m_poSettings->value("Frequency",QVariant(1));
  m_oPlotGeometry = m_poSettings->value("DialogGeometry").toByteArray();
  m_strConnAddress = oAddress.toString().toStdString();
  m_nFrequencyTime = oFrqTime.toInt();
}

void AERoshell::writeApplicationConfig()
{
  m_poSettings->setValue("Address",QVariant(ASCHAR(m_strConnAddress)));
  m_poSettings->setValue("Geometry",QVariant(this->geometry()));
  m_poSettings->setValue("DialogGeometry",m_poPlotWindow->saveGeometry());
  m_poSettings->setValue("Frequency",QVariant(m_nFrequencyTime));
  m_poSettings->sync();
}

///create scintilla widgets to aeroshell main window
void AERoshell::setScintillaWidgets()
{
  m_poConsoleOut = new AERconsole(DEBUG_SHELL_RECEIVE,this);
  m_poConsoleIn = new AERconsole(DEBUG_SHELL_SEND,this);
  ui.consoleOutLayout->addWidget(m_poConsoleOut);
  ui.consoleInLayout->addWidget(m_poConsoleIn);
}

///set properties of scintilla widgets
void AERoshell::setScintillaSettings()
{
  m_poConsoleIn->setFont(QFont("Fixedsys"));
  m_poConsoleOut->setFont(QFont("Fixedsys"));
  m_poConsoleIn->setMinimumSize(600,100);
  m_poConsoleOut->setReadOnly(TRUE);
  m_poConsoleIn->SendScintilla(m_poConsoleIn->SCI_SETHSCROLLBAR,0);
  m_poConsoleOut->SendScintilla(m_poConsoleOut->SCI_SETHSCROLLBAR,0);
}

///connects the appropriate signals and slots from Qt
void AERoshell::setQtConnections()
{
  connect(&m_oTimer,SIGNAL(timeout()),this,SLOT(getDebugLog()));
  connect(&m_oTimer,SIGNAL(timeout()),m_poConsoleOut,SLOT(HighlightCurrentWord()));

  connect(ui.actionQuit,SIGNAL(triggered()),m_poQApp,SLOT(quit()));
  connect(ui.actionPerformance,SIGNAL(triggered()),this,SLOT(showGraphicsWindow()));
  connect(ui.actionOccurrences,SIGNAL(triggered()),this,SLOT(showFindDialog()));
  connect(ui.actionAbout,SIGNAL(triggered()),this,SLOT(showAboutDialog()));
  connect(ui.sendButton,SIGNAL(clicked()),this,SLOT(postDebugEvent()));
  connect(ui.deleteButton,SIGNAL(clicked()),m_poConsoleOut,SLOT(clear()));
  connect(m_poConsoleIn,SIGNAL(debugEventSent()),this,SLOT(postDebugEvent()));

  connect(m_poOpenAction,SIGNAL(triggered()),this,SLOT(show()));
  connect(m_poCloseAction,SIGNAL(triggered()),this,SLOT(hide()));
  connect(m_poQuitAction,SIGNAL(triggered()),m_poQApp,SLOT(quit()));

  connect(m_poTrayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
          this,SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason)));
}

///reimplementation of closeEvent
void AERoshell::closeEvent(QCloseEvent *poEvent)
{
  if(m_poTrayIcon->isVisible()) {
    this->hide();
    poEvent->ignore();
    if(m_poPlotWindow->isVisible() && m_bPlotReady) 
      m_poPlotWindow->hide();
  }
}

///reimplementation of resizeEvent
void AERoshell::resizeEvent(QResizeEvent *poEvent)
{
  m_poConsoleIn->setMaximumSize(poEvent->size().width(),100);
}

void AERoshell::hideEvent(QHideEvent *poEvent)
{
  if(m_poPlotWindow->isVisible() && m_bPlotReady) m_poPlotWindow->hide();
  QMainWindow::hideEvent(poEvent);
}

void AERoshell::showEvent(QShowEvent *poEvent)
{
  if(!m_poPlotWindow->isVisible() && m_bPlotReady) m_poPlotWindow->show();
  QMainWindow::showEvent(poEvent);
}

///function for clicking on the systray icon
void AERoshell::trayIconClicked(QSystemTrayIcon::ActivationReason reason)
{
  bool bPlotVisible = m_poPlotWindow->isVisible();
  if(this->isVisible()) {
    if(bPlotVisible && m_bPlotReady) m_poPlotWindow->hide();
    else if(!bPlotVisible && m_bPlotReady) m_bPlotReady = false;
    this->hide();
  } else if(reason == QSystemTrayIcon::Trigger) {
    if(!bPlotVisible && m_bPlotReady) m_poPlotWindow->show();
    this->show();
  }
}

void AERoshell::showGraphicsWindow()
{
  AERgraphics *poAERgraphics = 
    new AERgraphics(new QGraphicsScene(this),m_poPlotWindow);
  if(!m_poPlotWindow->isVisible()) {
    m_poPlotWindow->showWindow(poAERgraphics);
    m_bPlotReady = true;
  } else {
    m_poPlotWindow->activateWindow();
  }
}

///show a dialog to search a term in console output
void AERoshell::showFindDialog()
{
  QDialog oDialog;
  QVBoxLayout oVLayout(&oDialog);
  QHBoxLayout oHLayout(&oDialog);

  QLabel oLabel("Search for occurrences of the word on the console output:",&oDialog);
  QLineEdit oSearchText(&oDialog);
  QPushButton oFindButton("Find");
  QPushButton oCancelButton("Cancel");

  oVLayout.addWidget(&oLabel);
  oVLayout.addWidget(&oSearchText);
  oHLayout.addWidget(&oFindButton);
  oHLayout.addWidget(&oCancelButton);
  oVLayout.addLayout(&oHLayout);

  connect(&oCancelButton,SIGNAL(clicked()),
          &oDialog,SLOT(reject()));
  connect(&oFindButton,SIGNAL(clicked()),
          &oDialog,SLOT(accept()));

  oDialog.adjustSize();
  oDialog.exec();

  string strWord = oSearchText.text().toStdString();
  int nWordLen = strWord.length();
  char* pszWord = (char*) FastAlloc(nWordLen+1);
  pszWord = ASCHAR(strWord);
  m_poConsoleOut->SearchForAWord(pszWord,nWordLen,STEP_BY_STEP);
}

///show a dialog with name and version of the program
void AERoshell::showAboutDialog()
{
  QMessageBox dialog;
  dialog.about(this,"About this program","AERoshell Console Debugger \nCopyright (C) 2011 Rockhead Games (R)");
}

///send a debug event
void AERoshell::postDebugEvent()
{
  DebugShellUpdate(m_poConsoleIn);
  m_poConsoleIn->clear();
}

///update the log receiver widget
void AERoshell::getDebugLog()
{
  DebugShellUpdate(m_poConsoleOut);
}