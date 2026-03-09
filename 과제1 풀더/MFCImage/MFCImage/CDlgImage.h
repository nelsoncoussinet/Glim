#pragma once
#include "afxdialogex.h"


// boîte de dialogue de CDlgImage

class CDlgImage : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgImage)

public:
	CDlgImage(CWnd* pParent = nullptr);   // constructeur standard
	virtual ~CDlgImage();


	BYTE* Pixel(int x, int y);
	bool UpdatePointRadius(int newRadius);
	bool UpdateCircleThickness(int newThickness);
	void RandomPointsMove();
	void ResetDisplay();
	void DisplayPointsPositions();

// CDialogEx datas
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CDlgImage };
#endif
private:
	void InitImage();
	void drawPoint(CPaintDC &);
	bool ComputeCircleThrough3Points(CPoint& center, int &r) const;
	void DrawCircle(void);
	void FillImage(int color);

	// drawing datas
	CImage m_Image;
	int m_ImageWidth;
	int m_ImageHeight;
	int grabbedPointIndex = -1;
	CPoint m_points[3];
	int m_pointCount = 0;
	int m_pointRadius = 1;
	int m_circleThickness = 1;
	bool m_dragging = false;
	int m_dragIndex = -1;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
