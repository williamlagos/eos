//===========================================================================================================================================
/// \file   DebugShellBase.cpp
/// \brief  Base class for both application and hub of debug shell.
/// \author Christian Lykawka
/// \date   04.05.2011
//===========================================================================================================================================

#define _DEBUG_SHELL_BASE_CPP_

#include "_DebugShell.h"

#include "DebugShell/DebugShellBase.h"

namespace aer {

///constructor
CDebugShellWrapper::CDebugShellWrapper()
{
  m_strLogMessage = "";
  m_strEvtMessage = "";
}

///clear event and log messages
void CDebugShellWrapper::CleanMessages()
{
  m_strLogMessage.clear();
  m_strEvtMessage.clear();
}

///receive the new log message
bool CDebugShellWrapper::NewLogMessage(JSONNODE* root, JSONNODE* response) 
{
  m_strLogMessage = string(json_as_string(*json_find(root,"params")));
  json_push_back(response,json_new_a("jsonrpc","2.0"));
  json_push_back(response,json_new_i("id",json_as_int(*json_find(root,"id"))));
  json_push_back(response,json_new_a("result","success"));
  return true;
}

///receive the new event message
bool CDebugShellWrapper::NewEvtMessage(JSONNODE* root, JSONNODE* response) 
{
  m_strEvtMessage = string(json_as_string(*json_find(root,"params")));
  json_push_back(response,json_new_a("jsonrpc","2.0"));
  json_push_back(response,json_new_i("id",json_as_int(*json_find(root,"id"))));
  json_push_back(response,json_new_a("result","success"));
  return true;
}

///constructor
CDebugShellBase::CDebugShellBase() 
{
  ASSERT(!DEBUG_SHELL); //this is a singleton!
  DEBUG_SHELL = this;
}

///destructor
CDebugShellBase::~CDebugShellBase() 
{
  DEBUG_SHELL = NULL;
}

///thread constructor
CDebugShellClientThread::CDebugShellClientThread(CDebugShellBase *poDebugShellBase, string strExecSide) : IThread() 
{
  m_strExecSide = strExecSide;
  m_poBase = poDebugShellBase;
  m_poClient = NULL;
}

///thread destructor
CDebugShellClientThread::~CDebugShellClientThread() 
{
  m_poClient->Close();
  networking::cleanup();
}

///initialize client-side
BOOL CDebugShellClientThread::Init(char *pszHubAddress, char *pszErrMsg) 
{
  char szAddressTmp[256];
  strcpy(szAddressTmp, pszHubAddress);
  char *pszPort = strchr(szAddressTmp, ':');
  if (pszPort) { *pszPort = '\0'; pszPort++; }
  networking::init();
  int iFlipPort = 0;
  if(m_strExecSide == "Hub") iFlipPort = 1;
  m_poClient = new TcpClient(szAddressTmp,(pszPort ? atoi(pszPort) : 4000) + iFlipPort);
  return TRUE;
}

///read messages and send them from thread
void CDebugShellClientThread::SendThreadMessage()
{
  vector<string> vstrMessages;
  vector<string>::iterator it;

  m_poBase->m_oUpdateLock.Enter();
  vstrMessages = m_poBase->m_vstrMessages;
  m_poBase->m_vstrMessages.resize(0);
  m_poBase->m_oUpdateLock.Leave();

  string strMethod = "";
  string strQuery = "";
  string strMessage = "";

  if(m_strExecSide == "Hub") {
    strMethod = "new_evt_message";
  } else if (m_strExecSide == "App") {
    strMethod = "new_log_message";
  } else {
    return;
  }

  for(it = vstrMessages.begin(); it != vstrMessages.end(); it++){
    strMessage = (*it);
    JSONNODE* poQuery = json_new(JSON_NODE);
    json_push_back(poQuery,json_new_a("jsonrpc","2.0"));
    json_push_back(poQuery,json_new_i("id",1));
    json_push_back(poQuery,json_new_a("method",ASCHAR(strMethod)));
    json_push_back(poQuery,json_new_a("params",ASCHAR(strMessage+"!!")));
    strQuery = json_write(poQuery);
    if(m_poClient->Send(strQuery) == -1) {
      while(!m_poClient->Connect()) m_poClient->Connect();
      m_poClient->Send(strQuery);
    }
    json_delete(poQuery);
  }
}

///thread run
void CDebugShellClientThread::ThreadRun() 
{
  while (!m_poClient->Connect()) m_poClient->Connect();
  while (TRUE) {
    SendThreadMessage();
    ReleaseTimeSlice();
  }
}

///get thread name
char *CDebugShellClientThread::GetThreadName() { return "DebugShellClient"; }

///thread constructor
CDebugShellServerThread::CDebugShellServerThread(aer::CDebugShellBase *poDebugShellBase, string strExecSide)
{
  m_poBase = poDebugShellBase;
  m_strExecSide = strExecSide;
  m_poServer = NULL;
}

///thread destructor
CDebugShellServerThread::~CDebugShellServerThread()
{
  m_poServer->Close();
  networking::cleanup();
}

///initialize server side
BOOL CDebugShellServerThread::Init(char *pszAddress, char *pszErrMsg)
{
  char szAddressTmp[256];
  strcpy(szAddressTmp, pszAddress);
  char *pszPort = strchr(szAddressTmp, ':');
  if (pszPort) { *pszPort = '\0'; pszPort++; }
  networking::init();
  int iFlipPort = 0;
  if(m_strExecSide == "App") iFlipPort = 1;
  m_poServer = new TcpServer(szAddressTmp, (pszPort ? atoi(pszPort) : 4000) + iFlipPort);  
  return TRUE;
}

///add a function from wrapper to server
BOOL CDebugShellServerThread::AddFunction(Json::Rpc::RpcMethod<CDebugShellWrapper> *poWrapMethod)
{
  m_poServer->AddMethod(poWrapMethod);
  return TRUE;
}

///wait for a new message received and pass to base class
void CDebugShellServerThread::WaitThreadMessages()
{ 
  string strBufferMessage = "";
  string strMessage;

  for(int iBuffer = 0; iBuffer <= 10; iBuffer++) {
    m_poServer->WaitMessage(1000);
    if (m_strExecSide == "App") {
      strBufferMessage = m_oWrapper.m_strEvtMessage;
      m_oWrapper.CleanMessages();
      break;
    } else if(m_strExecSide == "Hub") {
      strMessage = m_oWrapper.m_strLogMessage;
      strBufferMessage.append(strMessage);
      m_oWrapper.CleanMessages();
    }
  }
    
  //m_poBase->m_oUpdateLock.Enter();
  m_poBase->ReceiveMessage(ASCHAR(strBufferMessage));
  strBufferMessage.clear();
  //m_poBase->m_oUpdateLock.Leave();
}

///thread run
void CDebugShellServerThread::ThreadRun()
{
  m_poServer->Bind();
  m_poServer->Listen();
  while(TRUE) {
    WaitThreadMessages();
    ReleaseTimeSlice();
  }   
}

///get thread name
char *CDebugShellServerThread::GetThreadName() { return "DebugShellServer"; }

}; //end of namespace
