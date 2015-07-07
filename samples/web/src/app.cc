//===========================================================================================================================================
/// \file   DebugShellApp.cpp
/// \brief  Debug shell application-side functionality.
/// \author Christian Lykawka
/// \date   04.05.2011
//===========================================================================================================================================

#define _DEBUG_SHELL_APP_CPP_

#include "_DebugShell.h"

#include "DebugShell/DebugShellApp.h"

namespace aer {

///constructor
CDebugShellApp::CDebugShellApp() : CDebugShellBase() {
  ASSERT(FRAMEWORK); //be sure framework was previously initialized!
  ASSERT(LOG); //set log callback pointer
  ASSERT(LOG->GetLogCallback() == NULL);
  LOG->SetLogCallback(this);

  m_poServerThread = new CDebugShellServerThread(this,"App");
  m_poClientThread = new CDebugShellClientThread(this,"App");
}

///destructor
CDebugShellApp::~CDebugShellApp() {
  SafeDelete(m_poServerThread);
  SafeDelete(m_poClientThread);
  LOG->SetLogCallback(NULL);
}

///initialize application-side
BOOL CDebugShellApp::Init(char *pszAddress, char *pszErrMsg) {
  m_poServerThread->Init(pszAddress,pszErrMsg);
  m_poServerThread->AddFunction(new Rpc::RpcMethod<CDebugShellWrapper>(
      m_poServerThread->m_oWrapper,&CDebugShellWrapper::NewEvtMessage,std::string("new_evt_message")));
  m_poServerThread->Resume();

  m_poClientThread->Init(pszAddress,pszErrMsg);
  m_poClientThread->Resume();
  return TRUE;
}

///update debug shell
void CDebugShellApp::Update(IDebugShellUpdate *poInterface) {
  if (poInterface) {
    poInterface->NewDebugShellMsg(ASCHAR(this->m_strMessage));
    m_strMessage.clear();
  }
}

///called when a new log message has to be sent
void CDebugShellApp::SendMessage(char *pszMsg) {
  m_oUpdateLock.Enter();
  m_vstrMessages.push_back(pszMsg);
  m_oUpdateLock.Leave();
}

///called when a new event message has to be received
void CDebugShellApp::ReceiveMessage(char *pszMsg) {
  m_oUpdateLock.Enter();
  m_strMessage = pszMsg;
  m_oUpdateLock.Leave();
}

}; //end of namespace
