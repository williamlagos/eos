//===========================================================================================================================================
/// \file   DebugShellApp.h
/// \brief  Debug shell application-side functionality.
/// \author Christian Lykawka
/// \date   04.05.2011
//===========================================================================================================================================

#ifndef _DEBUG_SHELL_APP_H_
#define _DEBUG_SHELL_APP_H_

#include "DebugShellBase.h"

namespace aer {

///debug shell application-side functionality
class CDebugShellApp : public CDebugShellBase, public ILogCallback { public:

  CDebugShellClientThread *m_poClientThread;
  CDebugShellServerThread *m_poServerThread;

  ///constructor
  CDebugShellApp();

  ///destructor
  virtual ~CDebugShellApp();

  ///initialize application-side
  BOOL Init(char *pszAddress, char *pszErrMsg = NULL);

  ///update debug shell
  virtual void Update(IDebugShellUpdate *poInterface);

  ///called when a new log message has to be sent
  virtual void SendMessage(char *pszMsg);

  ///called when a new event message has to be received
  virtual void ReceiveMessage(char *pszMsg);
};

}; //end of namespace

#endif
