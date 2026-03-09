
// MFCImageDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "MFCImage.h"
#include "MFCImageDlg.h"
#include "afxdialogex.h"
#include "Resource.h"
#include "Constant.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <thread>
#include <chrono>

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CMFCImageDlg dialog



CMFCImageDlg::CMFCImageDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCIMAGE_DIALOG, pParent)
	, m_pointRadius(MIN_RADIUS)
	, m_circleThickness(MIN_THICKNESS)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_pointRadius);
	DDV_MinMaxInt(pDX, m_pointRadius, MIN_RADIUS, MAX_RADIUS);
	DDX_Text(pDX, IDC_EDIT2, m_circleThickness);
	DDV_MinMaxInt(pDX, m_circleThickness, MIN_THICKNESS, MAX_THICKNESS);
	DDX_Control(pDX, IDC_STATICPOINT1, m_PointText1);
	DDX_Control(pDX, IDC_STATICPOINT2, m_PointText2);
	DDX_Control(pDX, IDC_STATICPOINT3, m_PointText3);
	DDX_Control(pDX, IDC_VALUEPOINT1, m_PointValue1);
	DDX_Control(pDX, IDC_VALUEPOINT2, m_PointValue2);
	DDX_Control(pDX, IDC_VALUEPOINT3, m_PointValue3);
}

BEGIN_MESSAGE_MAP(CMFCImageDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_RANDMV, &CMFCImageDlg::OnBnClickedBtnRandmv)
	ON_EN_KILLFOCUS(IDC_POINT_RAD, &CMFCImageDlg::OnKillfocusPointRad)
	ON_EN_KILLFOCUS(IDC_CIRCLE_THICK, &CMFCImageDlg::OnKillfocusCircleThick)
	ON_BN_CLICKED(IDC_BTN_RESET, &CMFCImageDlg::OnBnClickedBtnReset)
	ON_MESSAGE(WM_RANDOM_UPDATE, &CMFCImageDlg::OnRandomUpdate)
END_MESSAGE_MAP()


// CMFCImageDlg message handlers

BOOL CMFCImageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	MoveWindow(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	CRect rcClient;
	GetClientRect(&rcClient); // real size of the main window

	// 150px kept for the UI above
	rcClient.top += WINDOW_UI_HEIGHT;

	m_pDlgImage = new CDlgImage;
	m_pDlgImage->Create(IDD_CDlgImage, this);
	m_pDlgImage->MoveWindow(&rcClient);
	m_pDlgImage->ShowWindow(SW_SHOW);

	m_PointText1.ShowWindow(SW_HIDE);
	m_PointText2.ShowWindow(SW_HIDE);
	m_PointText3.ShowWindow(SW_HIDE);
	m_PointValue1.ShowWindow(SW_HIDE);
	m_PointValue2.ShowWindow(SW_HIDE);
	m_PointValue3.ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMFCImageDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFCImageDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFCImageDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFCImageDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	delete m_pDlgImage;
}

void threadProcess(CWnd* parent)
{
	CMFCImageDlg* dlg = dynamic_cast<CMFCImageDlg*>(parent);
	if (dlg == nullptr)
		return;

	for (int i = 0; i < RANDOM_ITERATIONS; i++)
	{
		dlg->m_pDlgImage->RandomPointsMove();
		dlg->PostMessage(WM_RANDOM_UPDATE);

		std::this_thread::sleep_for(std::chrono::milliseconds(RANDOM_DELAY_MS));
	}
	dlg->m_randomRunning = false;
}

void CMFCImageDlg::OnBnClickedBtnRandmv()
{
	// boolean protecting to create an other thread if one is already running
	if (m_randomRunning)
		return;

	m_randomRunning = true;

	std::thread my_thread(threadProcess, this);
	my_thread.detach();
}

LRESULT CMFCImageDlg::OnRandomUpdate(WPARAM, LPARAM)
{
	m_pDlgImage->DisplayPointsPositions();

	m_pDlgImage->Invalidate();

	return 0;
}

void CMFCImageDlg::UpdatePointsPositions(const CPoint(&points)[3], int NbToDisplay)
{
	if (NbToDisplay == 0)
	{
		m_PointText1.ShowWindow(SW_HIDE);
		m_PointText2.ShowWindow(SW_HIDE);
		m_PointText3.ShowWindow(SW_HIDE);
		m_PointValue1.ShowWindow(SW_HIDE);
		m_PointValue2.ShowWindow(SW_HIDE);
		m_PointValue3.ShowWindow(SW_HIDE);
		return;
	}
	if (NbToDisplay >= 1)
	{
		m_PointText1.ShowWindow(SW_SHOW);
		m_PointValue1.ShowWindow(SW_SHOW);
		CString str;
		str.Format(_T("(%d, %d)"), points[0].x, points[0].y);
		m_PointValue1.SetWindowText(str);
	}
	if (NbToDisplay >= 2)
	{
		m_PointText2.ShowWindow(SW_SHOW);
		m_PointValue2.ShowWindow(SW_SHOW);
		CString str;
		str.Format(_T("(%d, %d)"), points[1].x, points[1].y);
		m_PointValue2.SetWindowText(str);
	}
	if (NbToDisplay == 3)
	{
		m_PointText3.ShowWindow(SW_SHOW);
		m_PointValue3.ShowWindow(SW_SHOW);
		CString str;
		str.Format(_T("(%d, %d)"), points[2].x, points[2].y);
		m_PointValue3.SetWindowText(str);
	}
}

void CMFCImageDlg::OnKillfocusPointRad()
{
	if (!UpdateData(TRUE))
		return;
	m_pDlgImage->UpdatePointRadius(m_pointRadius);
}

void CMFCImageDlg::OnKillfocusCircleThick()
{
	if (!UpdateData(TRUE))
		return;
	m_pDlgImage->UpdateCircleThickness(m_circleThickness);
}

void CMFCImageDlg::OnBnClickedBtnReset()
{
	m_pDlgImage->ResetDisplay();

	m_pDlgImage->Invalidate(); // forcer le redessin de la zone client du CDlgImage
}
