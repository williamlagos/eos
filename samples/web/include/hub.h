//===========================================================================================================================================
/// \file   DebugShellHub.h
/// \brief  Debug shell hub-side functionality.
/// \author Christian Lykawka
/// \date   04.05.2011
//===========================================================================================================================================

#ifndef _DEBUG_SHELL_HUB_H_
#define _DEBUG_SHELL_HUB_H_

#include "DebugShellBase.h"

namespace aer {

///debug shell hub-side functionality
class CDebugShellHub : public CDebugShellBase { public:

  CDebugShellServerThread *m_poServerThread;
  CDebugShellClientThread *m_poClientThread;

  ///constructor
  CDebugShellHub();

  ///destructor
  virtual ~CDebugShellHub();

  ///initialize hub-side
  BOOL Init(char *pszAddress, char *pszErrMsg = NULL);

  ///update debug shell
  virtual void Update(IDebugShellUpdate *poInterface);

  ///called when a new event message has to be sent
  virtual void SendMessage(char *pszMsg);

  ///called when a new event message has to be received
  virtual void ReceiveMessage(char *pszMsg);
};

}; //end of namespace

#endif
