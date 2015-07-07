//===========================================================================================================================================
/// \file   DebugShellHub.cpp
/// \brief  Debug shell hub-side functionality.
/// \author Christian Lykawka
/// \date   04.05.2011
//===========================================================================================================================================

#define _DEBUG_SHELL_HUB_CPP_

#include "_DebugShell.h"

#include "DebugShell/DebugShellHub.h"

namespace aer {

///constructor
CDebugShellHub::CDebugShellHub()
{
  m_poServerThread = new CDebugShellServerThread(this,"Hub");
  m_poClientThread = new CDebugShellClientThread(this,"Hub");
}

///destructor
CDebugShellHub::~CDebugShellHub() {
  SafeDelete(m_poServerThread);
  SafeDelete(m_poClientThread);
}

///initialize hub-side
BOOL CDebugShellHub::Init(char *pszAddress, char *pszErrMsg) {
  m_poServerThread->Init(pszAddress,pszErrMsg);
  m_poServerThread->AddFunction(new Rpc::RpcMethod<CDebugShellWrapper>(
      m_poServerThread->m_oWrapper,&CDebugShellWrapper::NewLogMessage,std::string("new_log_message")));
  m_poServerThread->Resume();

  m_poClientThread->Init(pszAddress,NULL);
  m_poClientThread->Resume();
  return TRUE;
}

///update debug shell
void CDebugShellHub::Update(IDebugShellUpdate *poInterface) {
  if (poInterface) {
    if(!poInterface->m_bDirectionFlag) {
      poInterface->NewDebugShellMsg(ASCHAR(this->m_strMessage));
      m_strMessage.clear();
    } else {
      SendMessage(poInterface->NewDebugShellEvt());
    }
  }
}

///called when a new event message has to be sent
void CDebugShellHub::SendMessage(char *pszMsg) {
  m_oUpdateLock.Enter();
  m_vstrMessages.push_back(pszMsg);
  m_oUpdateLock.Leave();
}

///called when a new log message has to be received
void CDebugShellHub::ReceiveMessage(char *pszMsg) {
  m_oUpdateLock.Enter();
  m_strMessage = pszMsg;
  m_oUpdateLock.Leave();
}

}; //end of namespace
