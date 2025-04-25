///////////////////////////////////////////////////////////////////////////////
// FILE : SocketComm.h
// Header file for CSocketComm class
// CSocketComm
//     Generic class for Socket Communication
///////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef _SOCKETCOMM_H_
#define _SOCKETCOMM_H_
#if _MSC_VER > 1000

#endif // _MSC_VER > 1000

#include <vector>
#include <list>
#include <cstdlib>

#define SOCKET_ERROR (-1)


// Event value
#define EVT_CONSUCCESS		0x0000	// Connection established
#define EVT_CONFAILURE		0x0001	// General failure - Wait Connection failed
#define EVT_CONDROP			0x0002	// Connection dropped
#define EVT_ZEROLENGTH		0x0003	// Zero length message
#define EVT_CONSUCCESS_EX		0x0004	// Connection established

#define BUFFER_DATA_SIZE	15752 //170206 lgh expend
#define HOSTNAME_SIZE	    MAX_PATH
#define STRING_LENGTH	    40
#define PROTOCOL_DEFAULT_LENGTH 52

#define ID_SR1000W		1
#define ID_MDX2500		2


struct SockAddrIn : public SOCKADDR_IN 
{
public:
	SockAddrIn() { Clear(); }
	SockAddrIn(const SockAddrIn& sin) { Copy( sin ); }
	~SockAddrIn() { }
	SockAddrIn& Copy(const SockAddrIn& sin);
	void	Clear() { memset(this, 0, sizeof(SOCKADDR_IN)); }
	bool	IsEqual(const SockAddrIn& sin);
	bool	IsGreater(const SockAddrIn& sin);
	bool	IsLower(const SockAddrIn& pm);
	bool	IsNull() const { return ((sin_addr.s_addr==0L)&&(sin_port==0)); }
	ULONG	GetIPAddr() const { return sin_addr.s_addr; }
	short	GetPort() const { return sin_port; }
	bool	CreateFrom(LPCTSTR sAddr, LPCTSTR sService, int nFamily = AF_INET);
	SockAddrIn& operator=(const SockAddrIn& sin) { return Copy( sin ); }
	bool	operator==(const SockAddrIn& sin) { return IsEqual( sin ); }
	bool	operator!=(const SockAddrIn& sin) { return !IsEqual( sin ); }
	bool	operator<(const SockAddrIn& sin)  { return IsLower( sin ); }
	bool	operator>(const SockAddrIn& sin)  { return IsGreater( sin ); }
	bool	operator<=(const SockAddrIn& sin) { return !IsGreater( sin ); }
	bool	operator>=(const SockAddrIn& sin) { return !IsLower( sin ); }
	operator LPSOCKADDR() { return (LPSOCKADDR)(this); }
	size_t	Size() const { return sizeof(SOCKADDR_IN); }
	void	SetAddr(SOCKADDR_IN* psin) { memcpy(this, psin, Size()); }
};

typedef std::list<SockAddrIn> CSockAddrList;

class CSocketComm
{
	BOOL m_bConnected;	// Connection is done.
	CString m_strLastError;
	int m_nServer;


	// Create a Socket - Server side
	bool CreateSocket(LPCTSTR strServiceName, int nProtocol);
	bool CreateSocket(CString strIP, LPCTSTR strServiceName, int nFamily);
	void CloseComm();		// Close Socket
	static bool ShutdownConnection(SOCKET sock);  // Shutdown a connection
	CString GetSockErorMsg(int Code);

public:
	CSocketComm();
	virtual ~CSocketComm();


	BOOL IsConnected();
	void SetConnectedStatus(BOOL bConnected);

	bool IsOpen() const;	// Is Socket valid?
	SOCKET GetSocket() const;	// return socket handle
	bool GetSockName(SockAddrIn& saddr_in);	// Get Socket name - address
	bool GetPeerName(SockAddrIn& saddr_in);	// Get Peer Socket name - address
	void StopComm();		// Stop Socket thread

	bool ListenFrom(CString strIP, LPCTSTR strServiceName, int nFamily);							// use on Server (listen)
	static SOCKET WaitForConnection(SOCKET server_sock, sockaddr* acceptIp, int* lenthOfIp);		// use on Server (accept)
	bool ConnectTo(CString strClientIP, LPCTSTR strServerIP, LPCTSTR strServiceName, int nFamily);	// use on Client (connect)

	virtual void OnDataReceived();
	int WriteComm(const LPBYTE lpBuffer, DWORD dwCount, DWORD dwTimeout = INFINITE);
	int WriteComm(CString sMsg, DWORD dwTimeout = INFINITE);

	static USHORT GetPortNumber(LPCTSTR strServiceName);	// Get service port number
	static ULONG GetIPAddress(LPCTSTR strHostName);	// Get IP address of a host
	static bool GetLocalName(LPTSTR strName, UINT nSize);	// GetLocalName
	static bool GetLocalAddress(LPTSTR strAddress, UINT nSize);	// GetLocalAddress

	void StringToChar(CString str, char *szStr);
	CString CharToString(char *szStr);

protected:
	HANDLE		m_hComm;		// Serial Comm handle
	HANDLE		m_hThread;		// Thread Comm handle

protected:


public:
	void SetServer(int nServerId = 0);

};

#endif // _SOCKETCOMM_H_

