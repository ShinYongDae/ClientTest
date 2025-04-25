
// ClientTestDlg.h : ��� ����
//

#pragma once

#include "ClientSrc\\TcpIpClient.h"
#include "afxwin.h"


// CClientTestDlg ��ȭ ����
class CClientTestDlg : public CDialogEx
{
	CString	m_strPortSvr, m_strAddrSvr;
	CString	m_strAddrCli;
	CTcpIpClient* m_pClient;
	int m_nClientID;

	void StartClient(CString sAddrCli, CString sAddrSvr, CString sPortSvr);
	void StopClient();

// �����Դϴ�.
public:
	CClientTestDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.
	~CClientTestDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENTTEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT wmClientReceived(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT wmClientClosed(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedOk();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnClear();
	CEdit m_editMsgList;
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedBtnSend();
	afx_msg void OnBnClickedCheckSr1000w();
	afx_msg void OnBnClickedCheckMdx2500();
};
