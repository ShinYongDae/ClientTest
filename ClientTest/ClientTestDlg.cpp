
// ClientTestDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "ClientTest.h"
#include "ClientTestDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CClientTestDlg 대화 상자



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


// CClientTestDlg 메시지 처리기

BOOL CClientTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	CClientTestApp* pApp = (CClientTestApp*)AfxGetApp();
	GetDlgItem(IDC_SVR_PORT)->SetWindowText(m_strPortSvr = pApp->GetHostPort());
	GetDlgItem(IDC_IPADDRESS_CLI)->SetWindowText(pApp->GetHostAddress());
	GetDlgItem(IDC_IPADDRESS_SVR)->SetWindowText(m_strAddrSvr);

	GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_SEND)->EnableWindow(FALSE);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CClientTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	StopClient();

	CDialogEx::OnOK();
}


void CClientTestDlg::OnDestroy()
{
	StopClient();

	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CClientTestDlg::OnBnClickedBtnClear()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (::IsWindow(m_editMsgList.GetSafeHwnd()))
	{
		int nLen = m_editMsgList.GetWindowTextLength();
		m_editMsgList.SetSel(0, -1);
		m_editMsgList.Clear();
	}
}


void CClientTestDlg::OnBnClickedBtnSend()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
