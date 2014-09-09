// clientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "client.h"
#include "clientDlg.h"
#include "set.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

UINT thread(LPVOID v)
{
	char buff[100];
	int s=1,addcount=0;
	CString str,ss;
	CTime tm;
	CClientDlg *dlg=(CClientDlg*) AfxGetApp()->GetMainWnd();//��öԻ���
	dlg->m_connect.EnableWindow(FALSE);
	dlg->m_disconnect.EnableWindow(TRUE);
	if(connect(dlg->sock,(sockaddr*)&(dlg->cli),sizeof(dlg->cli))) //���ӵ�������
	{
		ss.Format(" %s ",inet_ntoa(dlg->cli.sin_addr));
		dlg->Play(ss);
		dlg->Play("���Ӳ��Ϸ�����!!!\n");
		dlg->m_connect.EnableWindow(1);
		dlg->m_disconnect.EnableWindow(0);
		return 0;
	}
	ss.Format(" %s ",inet_ntoa(dlg->cli.sin_addr));
	dlg->Play("���ӳɹ�\n");
	dlg->m_set.EnableWindow(0);
	dlg->m_connect.EnableWindow(0);
	dlg->m_button.EnableWindow(TRUE);//�趨���Ͱ�ťΪ����״̬      
	dlg->SetForegroundWindow();
	//send(dlg->sock,"nihao!",100,0);
	while(s!=SOCKET_ERROR && dlg->cnt!=0) //ѭ���������
	{	
		s=recv(dlg->sock,buff,100,0); //����recv������������
		dlg->SetForegroundWindow();
		if (s!=SOCKET_ERROR && dlg->cnt!=0)//���û�з�������ͬʱû�жϿ����ӣ�����ʾ���յ�������
		{
			str=ss;
			tm=CTime::GetCurrentTime();
			str+=tm.Format(" %X");
			str+="\n";
			dlg->Play(str);
			str=buff;
			str+="\n";
			dlg->Play(str);
		}
	}
	send(dlg->sock,"Disconnected",100,0);//����������󣬷��ͶϿ�����
	dlg->m_button.EnableWindow(FALSE);//�趨��ؿؼ�����
	dlg->m_connect.EnableWindow(TRUE);
	dlg->m_disconnect.EnableWindow(FALSE);
	closesocket(dlg->sock);//�ر�SOCKET
	AfxEndThread(0);//��ֹ���߳�
	return 0;
}

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientDlg dialog

CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CClientDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClientDlg)
	DDX_Control(pDX, IDC_set, m_set);
	DDX_Control(pDX, IDC_con, m_connect);
	DDX_Control(pDX, IDC_close, m_disconnect);
	DDX_Control(pDX, IDC_info, m_info);
	DDX_Control(pDX, IDC_send, m_send);
	DDX_Control(pDX, IDC_button_send, m_button);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialog)
	//{{AFX_MSG_MAP(CClientDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_set, Onset)
	ON_BN_CLICKED(IDC_button_send, Onbuttonsend)
	ON_BN_CLICKED(IDC_con, Oncon)
	ON_BN_CLICKED(IDC_close, Onclose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientDlg message handlers

BOOL CClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	/*
	CFont *m_Font;
    m_Font = new CFont; 
    m_Font->CreateFont(25,10,0,0,50,
    FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_SWISS,"Arial");
    CEdit *m_Edit=(CEdit *)GetDlgItem(IDC_IP1);
    m_Edit->SetFont(m_Font,FALSE);
    GetDlgItem(IDC_IP1)->SetFont(m_Font);

	m_Edit=(CEdit *)GetDlgItem(IDC_IP2);
    m_Edit->SetFont(m_Font,FALSE);
    GetDlgItem(IDC_IP2)->SetFont(m_Font);

	m_Edit=(CEdit *)GetDlgItem(IDC_retport);
    m_Edit->SetFont(m_Font,FALSE);
    GetDlgItem(IDC_retport)->SetFont(m_Font);

	m_list_ip.SetFont(m_Font);

	m_list_ip.InsertColumn(0,"IP");
	m_list_ip.SetColumnWidth(0,255);//*/

	iscon=0;

	WSADATA wsaData;	
	BOOL ret = WSAStartup(MAKEWORD(2,2), &wsaData);
	if(ret != 0)
	{
		MessageBox("��ʼ������Э��ʧ��!");
		return -1;
	}
	UpdateData(false);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CClientDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CClientDlg::Onset() 
{
	// TODO: Add your control notification handler code here
	set dlg;
	port=0;
	ip="";
	if(dlg.DoModal()==IDOK)
	{
		port=dlg.m_port;
		ip=dlg.str;
	}
	UpdateData(0);
}

void CClientDlg::Onbuttonsend() 
{
	// TODO: Add your control notification handler code here
	if(!iscon)
	{
		AfxMessageBox("û�����ӷ�����!!!");
		return ;
	}
	if(m_send.GetWindowTextLength()==0)
	{
		AfxMessageBox("��Ϣ����Ϊ��!!!");
		return ;
	}
	char buff[100];
	CString str;
	CTime tm;
	tm=CTime::GetCurrentTime();
	str=tm.Format(" %X");
	str+="\n";
	Play(str);
	m_send.GetWindowText(buff,99);//��õ�ǰ�ı����е���Ϣ
	m_send.SetWindowText("");//����ı������Ϣ
	str=buff;
	str+="\n";
	Play(str);
	send(sock,buff,100,0);
}

BOOL CClientDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
	{
		Onbuttonsend();
		return 0;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CClientDlg::Play(CString str)
{
	LPCTSTR pDisplay;
	pDisplay = _T(str);//ת����LPCTSTR��������
	int nlength = m_info.GetWindowTextLength();//��ȡ�༭����ַ���
	m_info.SetSel(nlength, nlength);//��λ���
	m_info.ReplaceSel(pDisplay);//��괦��ʾ
}

void CClientDlg::Oncon()
{
	// TODO: Add your control notification handler code here

	UpdateData(1);
	cli.sin_addr.s_addr=inet_addr(ip);//�趨SOCKET��Ҫ���ӵĵ�ַ
	cli.sin_family=AF_INET;
	cli.sin_port=port;//htons(5000);//�趨�������˿�
	sock=socket(AF_INET,SOCK_STREAM,0);
	//����socket
	cnt=1;
	iscon=1;
	AfxBeginThread(thread,0); //�����߳�
}

void CClientDlg::Onclose() 
{
	// TODO: Add your control notification handler code here
	m_set.EnableWindow(1);
	closesocket(sock);
	iscon=0;
	cnt=0;
}
