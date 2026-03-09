
// MFCImageDlg.h : header file
//

#pragma once
#include <atlimage.h>

#include "CDlgImage.h"

#define WM_RANDOM_UPDATE (WM_USER + 1)

// CMFCImageDlg dialog
class CMFCImageDlg : public CDialogEx
{
// Construction
public:
	CMFCImageDlg(CWnd* pParent = nullptr);	// standard constructor
	CDlgImage* m_pDlgImage;
	bool m_randomRunning = false;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCIMAGE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnRandomUpdate(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
public:
	int m_pointRadius;
	int m_circleThickness;
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnRandmv();
	afx_msg void OnKillfocusPointRad();
	afx_msg void OnKillfocusCircleThick();
	afx_msg void OnBnClickedBtnReset();

	void UpdatePointsPositions(const CPoint(&points)[3], int NbToDisplay);
private:
	CStatic m_PointText1;
	CStatic m_PointText2;
	CStatic m_PointText3;
	CStatic m_PointValue1;
	CStatic m_PointValue2;
	CStatic m_PointValue3;
};
