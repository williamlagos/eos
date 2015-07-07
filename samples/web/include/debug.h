//===========================================================================================================================================
/// \file   DebugShellBase.h
/// \brief  Base class for both Clientlication and hub of debug shell.
/// \author Christian Lykawka
/// \date   04.05.2011
//===========================================================================================================================================

#ifndef _DEBUG_SHELL_BASE_H_
#define _DEBUG_SHELL_BASE_H_

#include <jsonrpc.h>

using namespace Json;
using namespace Json::Rpc;

namespace aer {

///class with functions to server side
class CDebugShellWrapper { public:

  string m_strLogMessage;
  string m_strEvtMessage;
  
  ///constructor
  CDebugShellWrapper();

  ///clear event and log messages
  void CleanMessages();

  ///receive the new log message as a server
  bool NewLogMessage(JSONNODE* root, JSONNODE* response);

  ///receive the new event message as a client
  bool NewEvtMessage(JSONNODE* root, JSONNODE* response);
};

///base class for both app and hub of debug shell
class CDebugShellBase { public:

  vector<string> m_vstrMessages;
  string         m_strMessage;
  CLock          m_oUpdateLock;
  bool           m_bDirectionFlag;

  ///constructor
  CDebugShellBase();

  ///destructor
  virtual ~CDebugShellBase();

  ///update debug shell
  virtual void Update(IDebugShellUpdate *poInterface = NULL) = 0;

  ///send a log or event message
  virtual void SendMessage(char *pszMsg) = 0;

  ///send a log or event message
  virtual void ReceiveMessage(char *pszMsg) = 0;
};

class CDebugShellClientThread : public IThread { public:

  TcpClient       *m_poClient;
  CDebugShellBase *m_poBase;
  int             m_iFlipPort;
  string          m_strExecSide;

  ///thread constructor
  CDebugShellClientThread(CDebugShellBase *poDebugShellBase, string strExecSide);

  ///thread destructor
  virtual ~CDebugShellClientThread();

  ///initialize client-side
  BOOL Init(char *pszHubAddress, char *pszErrMsg = NULL);
  
  ///read messages and send them from thread
  virtual void SendThreadMessage();

  ///thread run
  virtual void ThreadRun();

  ///get thread name
  virtual char *GetThreadName();
};

class CDebugShellServerThread : public IThread { public:

  TcpServer          *m_poServer;
  CDebugShellBase    *m_poBase;
  CDebugShellWrapper m_oWrapper;
  string             m_strExecSide;
  
  ///thread constructor
  CDebugShellServerThread(CDebugShellBase *poDebugShellBase, string strExecSide);

  ///thread destructor
  virtual ~CDebugShellServerThread();

  ///initialize server-side
  BOOL Init(char *pszAddress, char *pszErrMsg = NULL);

  ///add function to server
  BOOL AddFunction(Rpc::RpcMethod<CDebugShellWrapper> *poWrapMethod);

  ///wait for a new message received and pass to base class
  virtual void WaitThreadMessages();

  ///thread run
  virtual void ThreadRun();

  ///get thread name
  virtual char *GetThreadName();
};

}; //end of namespace

#endif
