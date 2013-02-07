// FivechessDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Fivechess.h"
#include "FivechessDlg.h"
//#include "ChessInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	// TODO: Add your control notification handler code here
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CFivechessDlg dialog

CFivechessDlg::CFivechessDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFivechessDlg::IDD, pParent)
{
	//TODO: add own code
	chessBoard = new CBitmap();
	whiteMan= new CBitmap();
	blackMan = new CBitmap();
	winnerMan = new CBitmap();

	chessBoard->LoadBitmap( IDB_CHESSBOARD );
	whiteMan->LoadBitmap( IDB_WHITEMAN );
	blackMan->LoadBitmap( IDB_BLACKMAN );
	winnerMan->LoadBitmap( IDB_WINNERMAN );

	gameOver = FALSE;
	myChess.Initialize();

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFivechessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFivechessDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_ABOUT, &CFivechessDlg::OnBnClickedAbout)
END_MESSAGE_MAP()


// CFivechessDlg message handlers

BOOL CFivechessDlg::OnInitDialog()
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

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFivechessDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CFivechessDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
		//// TODO: Add your control notification handler code here
		CDC tempDC;
		CClientDC myDC(this);
		tempDC.CreateCompatibleDC( &myDC );
		tempDC.SelectObject( chessBoard );
		myDC.BitBlt( 0, 0, LENGTH, LENGTH, &tempDC, 0, 0, SRCCOPY );

		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFivechessDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CFivechessDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: Add your control notification handler code here
	CPoint pt;
	GetCursorPos( &pt );
	ScreenToClient( &pt );
	if( (pt.x>=INTERVAL)&&(pt.x<=LENGTH-INTERVAL)&&(pt.y>=INTERVAL)&&(pt.y<=LENGTH-INTERVAL) )
	{
		SetCursor( AfxGetApp()->LoadCursor( IDC_BLACK_CURSOR ) );
		return TRUE;
	}
	return CDialog::OnSetCursor( pWnd, nHitTest, message );
}

void CFivechessDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your control notification handler code here
	CDC tempDC;
	CClientDC myDC(this);
	tempDC.CreateCompatibleDC( &myDC );

	int x = (point.y-INTERVAL)/INTERVAL;//row
	int y = (point.x-INTERVAL)/INTERVAL;//col

	if( myChess.UpdateAfterPerson( x, y )&&!gameOver )
	{
		int tag = myChess.CheckPWin();
		if(tag==-1)	
		{
			// draw
			tempDC.SelectObject(blackMan);
			myDC.BitBlt( (y+1)*INTERVAL, (x+1)*INTERVAL, INTERVAL, INTERVAL, &tempDC, 0, 0, SRCCOPY );
		}else{
			gameOver = TRUE;
			int* a = new int[5];
			int* b = new int[5];
			myChess.GetWinPos(tag, a, b, "human");

			tempDC.SelectObject( winnerMan );
			for(int i=0;i<5;i++)
				myDC.BitBlt( (b[i]+1)*INTERVAL, (a[i]+1)*INTERVAL, INTERVAL, INTERVAL, &tempDC, 0, 0, SRCAND );
			
			delete [] a;
			delete [] b;
			MessageBox("老婆好牛啊！");
		}
			
		if( !gameOver )
		{
			//delay some time
			for(int t=0;t<6666;t++)
				for(int t2=0;t2<6666;t2++)
					t2=t2;
			//computer move
			myChess.CaculateWeight();
			myChess.ComputerSelect(x, y);
			myChess.UpdateAfterComputer(x, y);

			tag = myChess.CheckCWin();
			if(tag==-1)    
			{
				// draw
				tempDC.SelectObject(whiteMan);
				myDC.BitBlt( (y+1)*INTERVAL, (x+1)*INTERVAL, INTERVAL, INTERVAL, &tempDC, 0, 0, SRCCOPY );
			}else{
				gameOver = TRUE;
				int* a = new int[5];
				int* b = new int[5];
				myChess.GetWinPos(tag, a, b, "computer");

				tempDC.SelectObject( winnerMan );
				for(int i=0;i<5;i++)
					myDC.BitBlt( (b[i]+1)*INTERVAL, (a[i]+1)*INTERVAL, INTERVAL, INTERVAL, &tempDC, 0, 0, SRCCOPY );
				
				delete [] a;
				delete [] b;
				MessageBox("老婆水平还不行，需要勤加练习啊！");
			}
		}
	
	}

	CDialog::OnLButtonDown(nFlags, point); //基类的响应鼠标左键函数

}


void CFivechessDlg::OnStart()
{
	// TODO: Add your control notification handler code here
	//MessageBox("骁尧先下吧：） ");
	gameOver = FALSE;
	myChess.Initialize();

	CDC tempDC;
	CClientDC myDC(this);
	tempDC.CreateCompatibleDC( &myDC );
	tempDC.SelectObject( chessBoard );
	myDC.BitBlt( 0, 0, LENGTH, LENGTH, &tempDC, 0, 0, SRCCOPY );

	Invalidate( FALSE );

}


void CFivechessDlg::OnBnClickedAbout()
{
	// TODO: Add your control notification handler code here
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
