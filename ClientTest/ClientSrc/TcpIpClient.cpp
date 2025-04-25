#include "stdafx.h"
#include "TcpIpClient.h"


CTcpIpClient::CTcpIpClient(CWnd* pParent/*=NULL*/)
{
	::InitializeCriticalSection(&m_sc);

	m_pParent = pParent;
	m_hParentWnd = pParent->GetSafeHwnd();

	m_pReceiveBuffer = new BYTE[16384]; // 1mb
	m_nBufferId = 0;
	m_pCurrentBuffer = new BYTE[16384]; // 1mb
	//m_bReadAck = 0;
	//m_bReadErr = 0;
	m_strReceived = _T("");
}

CTcpIpClient::~CTcpIpClient()
{
	//m_bReadCommData = 0;
	//m_bStop = 1;
	//StopComm();
	//Sleep(100);

	if(m_pReceiveBuffer)
		delete[] m_pReceiveBuffer;
	if (m_pCurrentBuffer)
		delete[] m_pCurrentBuffer;

	::DeleteCriticalSection(&m_sc);
}


void CTcpIpClient::Init(CString strClientIP, CString strServerIP, int nPort)
{
	m_strClientIP = strClientIP;
	m_strServerIP = strServerIP;
	m_nNetworkPort = nPort;
}

void CTcpIpClient::Start()
{
	PrepareThread();
	ResumeThread();
}

int CTcpIpClient::Running()
{
	::EnterCriticalSection(&m_sc);
	CString sMsg;

	if (m_bStop)
	{
		::LeaveCriticalSection(&m_sc);
		return 0; // Terminate Thread
	}

	int i = 0;
	CString strPort;
	strPort.Format(_T("%d"), m_nNetworkPort);

	m_bConnected = ConnectTo(m_strClientIP, m_strServerIP, strPort, AF_INET);

	if (m_bStop)
	{
		::LeaveCriticalSection(&m_sc);
		return 0; // Terminate Thread
	}

	if (m_bConnected == 0)		// Server is failed.
	{
		sMsg = _T("Fail to connect.");
		::SendMessage(m_hParentWnd, WM_CLIENT_RECEIVED, (WPARAM)0, (LPARAM)(LPCTSTR)sMsg);
		::PostMessage(m_hParentWnd, WM_CLIENT_CLOSED, (WPARAM)0, (LPARAM)0);

		::LeaveCriticalSection(&m_sc);
		return 0; // Terminate Thread
	}

	DWORD	dwBytes = 0L;
	DWORD	dwTimeout = 5000;

	while (IsOpen() && !m_bStop)
	{
		fd_set	fdRead = { 0 };
		TIMEVAL	stTime;
		TIMEVAL	*pstTime = NULL;

		//160107 LGH 
		if (INFINITE != dwTimeout) {
			stTime.tv_sec = dwTimeout / 1000;
			stTime.tv_usec = (dwTimeout * 1000) % 1000000;
			pstTime = &stTime;
		}

		SOCKET s = (SOCKET)m_hComm;
		// Set Descriptor
		if (!FD_ISSET(s, &fdRead))
			FD_SET(s, &fdRead);

		// Select function set read timeout
		DWORD dwBytesRead = 0L;
		int res = select(s + 1, &fdRead, NULL, NULL, pstTime);
		if (res > 0)
		{
			BYTE buffer[1024] = { 0, };
			int nIdx = 1024; //protocol의 헤더 길이만큼 딴다

			res = recv(s, (LPSTR)&buffer, nIdx, 0);
			if (res > 0)
			{
				memcpy(&m_pReceiveBuffer[m_nBufferId], &buffer[0], res);
				memset(m_pCurrentBuffer, 0, 16384);
				memcpy(m_pCurrentBuffer, &m_pReceiveBuffer[m_nBufferId], res);
				m_pCurrentBuffer[res] = _T('\0');
				m_nBufferId += res;

				OnDataReceived();

				int nLeftLength = m_nBufferId - (i+1);
				int nStartID = m_nBufferId - i;//3-1= id[2]

				if (nLeftLength > 0)
					memmove(&m_pReceiveBuffer[0], &m_pReceiveBuffer[nStartID], nLeftLength);
				else
				{
					m_nBufferId = 0;
					memset(m_pReceiveBuffer, 0, 16384);
				}

				dwBytesRead = res;
			}
			else
			{ 
				// 연결 상태 끊어짐 확인.
				sMsg = _T("Fail to connect.");
				::SendMessage(m_hParentWnd, WM_CLIENT_RECEIVED, (WPARAM)0, (LPARAM)(LPCTSTR)sMsg);
				::PostMessage(m_hParentWnd, WM_CLIENT_CLOSED, (WPARAM)0, (LPARAM)0);

				::LeaveCriticalSection(&m_sc);
				return 0; // Terminate Thread

			}
		}
		else
			dwBytesRead = (DWORD)(-1L);

		// Error? - need to signal error
		if (dwBytes == (DWORD)-1L)
		{
			// Do not send event if we are closing

			break;
		}

		Sleep(0);

	}

	StopComm();

	::LeaveCriticalSection(&m_sc);
	return 0; // Terminate Thread
}

void CTcpIpClient::OnDataReceived()
{
	m_strReceived = CharToString((char*)m_pCurrentBuffer);

	::SendMessage(m_hParentWnd, WM_CLIENT_RECEIVED, (WPARAM)0, (LPARAM)(LPCTSTR)m_strReceived);
}

BOOL CTcpIpClient::Stop()
{
	m_bStop = 1;

	if (IsOpen())
	{
		StopComm();
		return TRUE;
	}

	return FALSE;
}
