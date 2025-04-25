#pragma once

#include "CEasyThread.h"
#include "SocketComm.h"

class CTcpIpClient : public CSocketComm, public CEasyThread
{
	HWND m_hParentWnd;
	CWnd* m_pParent;
	BOOL m_bConnected;	// Connection is done.
	CString m_strServerIP;
	CString m_strClientIP;
	int m_nBufferId;
	CString m_strReceived;

	BYTE* m_pReceiveBuffer;
	BYTE* m_pCurrentBuffer;

public:
	CTcpIpClient(CWnd* pParent = NULL);
	virtual ~CTcpIpClient();

	CRITICAL_SECTION m_sc;
	int m_nNetworkPort;

	void Init(CString strClientIP, CString strServerIP, int nPort);
	void Start();
	BOOL Stop();

	virtual int Running();
	virtual void OnDataReceived();
};

