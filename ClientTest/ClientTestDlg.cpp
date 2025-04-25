
// ClientTestDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "ClientTest.h"
#include "ClientTestDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CClientTestDlg ��ȭ ����



CClientTestDlg::CClientTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CLIENTTEST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_strPortSvr = _T("12345");
	//m_strPortSvr = _T("2000");
	m_strAddrSvr = _T("127.0.0.1");
	m_strAddrCli = _T("");
	m_pClient = NULL;
	m_nClientID = 0;
}

CClientTestDlg::~CClientTestDlg()
{
	if (m_pClient)
	{
		delete m_pClient;
		m_pClient = NULL;
	}
}

void CClientTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MESSAGE_LIST, m_editMsgList);
}

BEGIN_MESSAGE_MAP(CClientTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_START, &CClientTestDlg::OnBnClickedBtnStart)
	ON_MESSAGE(WM_CLIENT_RECEIVED, wmClientReceived)
	ON_MESSAGE(WM_CLIENT_CLOSED, wmClientClosed)
	ON_BN_CLICKED(IDOK, &CClientTestDlg::OnBnClickedOk)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CClientTestDlg::OnBnClickedBtnClear)
	ON_BN_CLICKED(IDC_BTN_STOP, &CClientTestDlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_SEND, &CClientTestDlg::OnBnClickedBtnSend)
	ON_BN_CLICKED(IDC_CHECK_SR1000W, &CClientTestDlg::OnBnClickedCheckSr1000w)
	ON_BN_CLICKED(IDC_CHECK_MDX2500, &CClientTestDlg::OnBnClickedCheckMdx2500)
END_MESSAGE_MAP()


// CClientTestDlg �޽��� ó����

BOOL CClientTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	CClientTestApp* pApp = (CClientTestApp*)AfxGetApp();
	GetDlgItem(IDC_SVR_PORT)->SetWindowText(m_strPortSvr = pApp->GetHostPort());
	GetDlgItem(IDC_IPADDRESS_CLI)->SetWindowText(pApp->GetHostAddress());
	GetDlgItem(IDC_IPADDRESS_SVR)->SetWindowText(m_strAddrSvr);

	GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_SEND)->EnableWindow(FALSE);

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CClientTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CClientTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CClientTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CClientTestDlg::StartClient(CString sAddrCli, CString sAddrSvr, CString sPortSvr)
{
	if (!m_pClient)
	{
		m_pClient = new CTcpIpClient(this);
		m_pClient->Init(sAddrCli, sAddrSvr, _tstoi(sPortSvr));
		m_pClient->Start();
		m_pClient->SetServer(m_nClientID);
	}
}

void CClientTestDlg::StopClient()
{
	if (m_pClient)
	{
		if (!m_pClient->Stop()) // Called Destroy Function.
		{
			delete m_pClient;
		}
		m_pClient = NULL;
		Sleep(30);
	}
}

void CClientTestDlg::OnBnClickedBtnStart()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CClientTestApp* pApp = (CClientTestApp*)AfxGetApp();

	CString sPortCli, sPortSvr, sAddrCli, sAddrSvr;
	GetDlgItem(IDC_IPADDRESS_CLI)->GetWindowText(sAddrCli);
	GetDlgItem(IDC_SVR_PORT)->GetWindowText(sPortSvr);
	GetDlgItem(IDC_IPADDRESS_SVR)->GetWindowText(sAddrSvr);
	pApp->SetHostAddress(sAddrCli);
	pApp->SetHostPort(sPortCli);
	m_strPortSvr = sPortSvr;
	m_strAddrSvr = sAddrSvr;	
	m_strAddrCli = sAddrCli;

	StartClient(sAddrCli, sAddrSvr, sPortSvr);

	GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_SEND)->EnableWindow(TRUE);
}


void CClientTestDlg::OnBnClickedBtnStop()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sMsg = _T("Is it sure to stop connection?");
	if (AfxMessageBox(sMsg, MB_ICONQUESTION | MB_OKCANCEL, MB_DEFBUTTON2) == IDOK)
	{
		StopClient();
		GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SEND)->EnableWindow(FALSE);		
	}
}

LRESULT CClientTestDlg::wmClientReceived(WPARAM wParam, LPARAM lParam)
{
	CString sReceived = (LPCTSTR)lParam;

	CString sDisp;
	GetDlgItem(IDC_MESSAGE_LIST)->GetWindowText(sDisp);
	sDisp += sReceived + _T("\r\n");
	GetDlgItem(IDC_MESSAGE_LIST)->SetWindowText(sDisp);

	return (LRESULT)1;
}

LRESULT CClientTestDlg::wmClientClosed(WPARAM wParam, LPARAM lParam)
{
	if (m_pClient)
		m_pClient = NULL;

	GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_SEND)->EnableWindow(FALSE);

	return (LRESULT)1;
}


void CClientTestDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	StopClient();

	CDialogEx::OnOK();
}


void CClientTestDlg::OnDestroy()
{
	StopClient();

	CDialogEx::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}


void CClientTestDlg::OnBnClickedBtnClear()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (::IsWindow(m_editMsgList.GetSafeHwnd()))
	{
		int nLen = m_editMsgList.GetWindowTextLength();
		m_editMsgList.SetSel(0, -1);
		m_editMsgList.Clear();
	}
}


void CClientTestDlg::OnBnClickedBtnSend()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sMsg;
	GetDlgItem(IDC_TXT_MESSAGE)->GetWindowText(sMsg);
	if (!sMsg.IsEmpty())
	{
		if (m_pClient)
		{
			m_pClient->WriteComm(sMsg);
		}
	}
}


void CClientTestDlg::OnBnClickedCheckSr1000w()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	BOOL bSr1000w = ((CButton*)GetDlgItem(IDC_CHECK_SR1000W))->GetCheck();
	BOOL bMdx2500 = ((CButton*)GetDlgItem(IDC_CHECK_MDX2500))->GetCheck();
	if (bSr1000w)
	{
		((CButton*)GetDlgItem(IDC_CHECK_MDX2500))->SetCheck(FALSE);

		GetDlgItem(IDC_SVR_PORT)->SetWindowText(_T("9004"));
		GetDlgItem(IDC_IPADDRESS_CLI)->SetWindowText(_T("192.168.100.99"));
		GetDlgItem(IDC_IPADDRESS_SVR)->SetWindowText(_T("192.168.100.100"));

		m_nClientID = ID_SR1000W;
	}

	if (!bSr1000w && !bMdx2500)
		m_nClientID = 0;
}


void CClientTestDlg::OnBnClickedCheckMdx2500()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	BOOL bSr1000w = ((CButton*)GetDlgItem(IDC_CHECK_SR1000W))->GetCheck();
	BOOL bMdx2500 = ((CButton*)GetDlgItem(IDC_CHECK_MDX2500))->GetCheck();
	if (bMdx2500)
	{
		((CButton*)GetDlgItem(IDC_CHECK_SR1000W))->SetCheck(FALSE);

		GetDlgItem(IDC_SVR_PORT)->SetWindowText(_T("50002"));
		GetDlgItem(IDC_IPADDRESS_CLI)->SetWindowText(_T("192.168.100.199"));
		GetDlgItem(IDC_IPADDRESS_SVR)->SetWindowText(_T("192.168.100.200"));

		m_nClientID = ID_MDX2500;
	}

	if (!bSr1000w && !bMdx2500)
		m_nClientID = 0;
}
