#include "aeroshell.h"

///constructor
AERconsole::AERconsole(bool bDirectionFlag, AERoshell *poShell, QWidget *parent, Qt::WFlags flags)
: QsciScintilla()
{
  m_poParent = poShell;
  m_bDirectionFlag = bDirectionFlag;
  m_bPauseFlag = false;
  m_nLines = this->lines();
  m_vnLinesLength = (int*) FastAlloc(m_nLines*sizeof(int));
  m_strEvent = "";
  m_strFound = "";
}

AERconsole::~AERconsole()
{
  FastFree(m_vnLinesLength);
}

///receive an event and execute interpreted commands
void AERconsole::keyPressEvent(QKeyEvent *ke)
{
  int nNativeKey = ke->nativeVirtualKey();
  if (nNativeKey == 13) {
    emit debugEventSent();
  } else if (ke->matches(QKeySequence::Find)) {
    m_poParent->showFindDialog();
  } else if (ke->matches(QKeySequence::FindNext)) {
    if (m_bPauseFlag) SearchTerm(NEXT);
  } else if (ke->matches(QKeySequence::FindPrevious)) {
    if (m_bPauseFlag) SearchTerm(PREVIOUS);
  } else if (ke->key() == Qt::Key_Escape) {
    if (m_bPauseFlag) ClearCurrentIndicator();
  } else {
    QsciScintillaBase::keyPressEvent(ke);
  }
}

///search for the next term in the text
void AERconsole::SearchTerm(bool bChoice)
{
  char* szWord = ASCHAR(m_strFound);
  int nWordLen = m_strFound.length();
  int nSizeDoc = this->text().length();
  int nIndexOf = SendScintilla(SCI_SEARCHINTARGET,nWordLen,szWord);
  long nStartTarget = nIndexOf;
  long nFinalTarget = 0;
  if (bChoice) {
    nStartTarget += nWordLen+1;
    nFinalTarget = nSizeDoc;
  }
  SendScintilla(SCI_SETTARGETSTART,nStartTarget);
  SendScintilla(SCI_SETTARGETEND,nFinalTarget);
  nIndexOf = SendScintilla(SCI_SEARCHINTARGET,nWordLen,szWord);
  SendScintilla(SCI_INDICATORCLEARRANGE,0,nSizeDoc);
  SendScintilla(SCI_INDICATORFILLRANGE,nIndexOf,nWordLen);
  int nCountLi = -1; int nLinePos = nIndexOf;
  for (int nCount = 0; nLinePos >= 0 && nCount < m_nLines; nCount++) {
    nLinePos -= m_vnLinesLength[nCount]; nCountLi++;
  }
  this->setCursorPosition(nCountLi,0);
}

void AERconsole::ClearCurrentIndicator()
{
  int nSizeDoc = this->text().length();
  SendScintilla(SCI_INDICATORCLEARRANGE,0,nSizeDoc);
  m_bPauseFlag = false;
  this->setCursorPosition(m_nLines,0);
}

///update the console output
void AERconsole::NewDebugShellMsg(char *pszMsg)
{
  if ((*pszMsg) != '\0') {
    this->append(QString(pszMsg));
    if (m_nLines != this->lines()) {
      m_nLines = this->lines();
      FastFree(m_vnLinesLength);
      m_vnLinesLength = (int*) FastAlloc(m_nLines*sizeof(int));
      for (int nCount = 0; nCount < m_nLines; nCount++)
        m_vnLinesLength[nCount] = text(nCount).length();
      if (!m_bPauseFlag) this->setCursorPosition(m_nLines,0);
    }
  }
}

///send a message to app
char *AERconsole::NewDebugShellEvt()
{
  m_strEvent = this->text().simplified().toUtf8().data();
  return ASCHAR(m_strEvent);
}

void AERconsole::HighlightCurrentWord() {
  char *pszWord;
  //Get selected text from scintilla.
  int iSelectStart = SendScintilla(SCI_GETSELECTIONSTART);
  int iSelectEnd = SendScintilla(SCI_GETSELECTIONEND);
  int iWordLen = iSelectEnd-iSelectStart;
  if (iWordLen == 0) return;
  pszWord = (char*) FastAlloc(iWordLen+1);
  SendScintilla(SCI_GETSELTEXT,pszWord);
  SearchForAWord(pszWord,iWordLen,DIRECT);
  FastFree(pszWord);
}

void AERconsole::SearchForAWord(char *pszWord, int nWordLen, bool bStepFlag)
{
  m_strFound = pszWord;

  //Configure a scintilla indicator.
  int nSizeDoc = SendScintilla(SCI_GETLENGTH);
  SendScintilla(SCI_SETINDICATORCURRENT,0);
  SendScintilla(SCI_INDICATORCLEARRANGE,0,nSizeDoc);
  SendScintilla(SCI_INDICSETSTYLE,0,INDIC_ROUNDBOX);
  SendScintilla(SCI_INDICSETFORE,0,0x0000ff);
	
  //Configure search settings.
	//SendScintilla(SCI_SETSEARCHFLAGS, SCFIND_MATCHCASE | SCFIND_WHOLEWORD);
	SendScintilla(SCI_SETTARGETSTART, 0);
	SendScintilla(SCI_SETTARGETEND, nSizeDoc);
	int nIndexOf = SendScintilla(SCI_SEARCHINTARGET,nWordLen,pszWord);

  //Find the first occurrence of word.
	while (nIndexOf != -1 && nIndexOf < nSizeDoc) {
    if (bStepFlag) {
		  SendScintilla(SCI_INDICATORFILLRANGE,nIndexOf,nWordLen);
    } else {
      m_bPauseFlag = true;
      SendScintilla(SCI_INDICATORFILLRANGE,nIndexOf,nWordLen);
      int nCountLi = -1; int nLinePos = nIndexOf;
      for (int nCount = 0; nLinePos >= 0 && nCount < m_nLines; nCount++) {
        nLinePos -= m_vnLinesLength[nCount]; nCountLi++;
      }
      this->setCursorPosition(nCountLi,0);
      break;
    }
		SendScintilla(SCI_SETTARGETSTART,(long)nIndexOf+nWordLen+1);
		SendScintilla(SCI_SETTARGETEND,nSizeDoc);
		nIndexOf = SendScintilla(SCI_SEARCHINTARGET,nWordLen,pszWord);
	}
}