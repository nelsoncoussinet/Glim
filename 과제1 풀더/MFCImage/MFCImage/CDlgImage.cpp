// CDlgImage.cpp : fichier d'implémentation
//

#define _USE_MATH_DEFINES

#include "pch.h"
#include <cmath>
#include <iostream>
#include "MFCImage.h"
#include "afxdialogex.h"
#include "CDlgImage.h"
#include "MFCImageDlg.h"

namespace {
	constexpr double PI = 3.14159265358979323846;
}

// boîte de dialogue de CDlgImage

IMPLEMENT_DYNAMIC(CDlgImage, CDialogEx)

CDlgImage::CDlgImage(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CDlgImage, pParent)
{
}

CDlgImage::~CDlgImage()
{
}

void CDlgImage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgImage, CDialogEx)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// gestionnaires de messages de CDlgImage

BOOL CDlgImage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if (!m_Image.IsNull())
		m_Image.Destroy();

	LONG exStyle = GetWindowLong(m_hWnd, GWL_EXSTYLE);
	SetWindowLong(m_hWnd, GWL_EXSTYLE, exStyle | WS_EX_COMPOSITED);
	InitImage();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION : les pages de propriétés OCX devraient retourner FALSE
}

void CDlgImage::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// N'appelez pas CDialogEx::OnPaint() pour la peinture des messages
	if (m_Image)
	{
		FillImage(0xFF);
		DrawCircle();
		m_Image.Draw(dc, 0, 0);
	}
	drawPoint(dc);
}

void CDlgImage::drawPoint(CPaintDC &dc)
{
	CPen pen(PS_SOLID, m_pointRadius * 2, BLACK);
	CPen* oldPen = dc.SelectObject(&pen);
	for (int i = 0; i < m_pointCount; i++)
	{
		dc.MoveTo(m_points[i].x, m_points[i].y);
		dc.LineTo(m_points[i].x, m_points[i].y);
	}
	dc.SelectObject(oldPen);
}

void CDlgImage::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (cx <= 0 || cy <= 0)
		return;

	if (!m_Image.IsNull())
		m_Image.Destroy();

	InitImage();
}

void CDlgImage::InitImage()
{
	CRect rect;
	GetClientRect(&rect);

    m_ImageWidth = rect.Width();
	m_ImageHeight = rect.Height();

	m_Image.Create(m_ImageWidth, m_ImageHeight, 8);
	static RGBQUAD rgb[256];
	for (int i = 0; i < 256; i++)
	{
		rgb[i].rgbRed = i;
		rgb[i].rgbGreen = i;
		rgb[i].rgbBlue = i;
		rgb[i].rgbReserved = 0;
	}
	m_Image.SetColorTable(0, 256, rgb);

	FillImage(0xFF);
}

void CDlgImage::FillImage(int color)
{
	int pitch = m_Image.GetPitch();
	BYTE* bits = (BYTE*)m_Image.GetBits();

	for (int y = 0; y < m_ImageHeight; y++)
	{
		memset(bits + y * pitch, color, abs(pitch));
	}
}

BYTE* CDlgImage::Pixel(int x, int y)
{
	return (BYTE*)m_Image.GetBits() + y * m_Image.GetPitch() + x;
}

void CDlgImage::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_pointCount < 3)
	{
		m_points[m_pointCount] = point;
		m_pointCount++;
		CMFCImageDlg* parentDlg = dynamic_cast<CMFCImageDlg*>(GetParent());
		if (parentDlg)
			parentDlg->UpdatePointsPositions(m_points, m_pointCount);
	}
	if (m_pointCount == 3)
	{
		for (int i = 0; i < 3; ++i)
		{
			if (abs(point.x - m_points[i].x) <= m_pointRadius &&
				abs(point.y - m_points[i].y) <= m_pointRadius)
			{
				grabbedPointIndex = i; // catching a point for dragging
				break;
			}
		}
	}
	Invalidate();

	CDialogEx::OnLButtonDown(nFlags, point);
}

bool CDlgImage::UpdatePointRadius(int newRadius)
{
	if (newRadius < 1 || newRadius > 100)
		return false;
	m_pointRadius = newRadius;
	Invalidate();
	return true;
}

bool CDlgImage::UpdateCircleThickness(int newThickness)
{
	if (newThickness < 1 || newThickness > 50)
		return false;
	m_circleThickness = newThickness;
	Invalidate();
	return true;
}

void CDlgImage::DrawCircle(void)
{
	CPoint center;
	int radius;
	if (ComputeCircleThrough3Points(center, radius) == false)
		return; // not enough points or points aligned

	int halfThick = m_circleThickness / 2;
	int rInner = max(0, radius - halfThick);  // avoid negative inner radius
	int rOuter = radius + m_circleThickness;
	// avoiding freeze when the circle is too big (points out of the screen)
	const int MAX_COORD = 2000; // pixels
	int yMin = max(center.y - rOuter, 0);
	int yMax = min(center.y + rOuter, m_ImageHeight - 1);
	int xMin = max(center.x - rOuter, 0);
	int xMax = min(center.x + rOuter, m_ImageWidth - 1);

	// If the circle is too big, we limit the bounding box to a reasonable size to avoid freezing while trying to draw it
	if (yMin > MAX_COORD) yMin = MAX_COORD;
	if (yMax > MAX_COORD) yMax = MAX_COORD;
	if (xMin > MAX_COORD) xMin = MAX_COORD;
	if (xMax > MAX_COORD) xMax = MAX_COORD;

	for (int y = yMin; y <= yMax; ++y)
	{
		for (int x = xMin; x <= xMax; ++x)
		{
			int dx = x - center.x;
			int dy = y - center.y;
			int dist2 = dx * dx + dy * dy;

			if (dist2 >= rInner * rInner && dist2 <= rOuter * rOuter &&
				x >= 0 && x < m_ImageWidth && y >= 0 && y < m_ImageHeight)
			{
				BYTE* p = Pixel(x, y);
				p[0] = 0; // B
				p[1] = 0; // G
				p[2] = 0; // R
				p[3] = 255; // A
			}
		}
	}
}

// compute circle function
bool CDlgImage::ComputeCircleThrough3Points(CPoint& center, int &radius) const
{
	if (m_pointCount != 3)
		return false; // wrong amount of points
	double x1 = m_points[0].x;
	double y1 = m_points[0].y;

	double x2 = m_points[1].x;
	double y2 = m_points[1].y;

	double x3 = m_points[2].x;
	double y3 = m_points[2].y;

	double D = 2 * (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2));
	if (D == 0) return false; // points aligned avoiding dividing by 0

	double cx = ((x1 * x1 + y1 * y1) * (y2 - y3) + (x2 * x2 + y2 * y2) * (y3 - y1) + (x3 * x3 + y3 * y3) * (y1 - y2)) / D;
	double cy = ((x1 * x1 + y1 * y1) * (x3 - x2) + (x2 * x2 + y2 * y2) * (x1 - x3) + (x3 * x3 + y3 * y3) * (x2 - x1)) / D;

	center.x = (int)(cx + 0.5);
	center.y = (int)(cy + 0.5);
	radius = (int)(sqrt((cx - x1) * (cx - x1) + (cy - y1) * (cy - y1)) + 0.5);
	return true;
}

void CDlgImage::OnMouseMove(UINT nFlags, CPoint point)
{
	if (grabbedPointIndex != -1)
	{
		m_points[grabbedPointIndex] = point; // update point position
		CMFCImageDlg* parentDlg = dynamic_cast<CMFCImageDlg*>(GetParent());
		if (parentDlg)
			parentDlg->UpdatePointsPositions(m_points, m_pointCount);
		Invalidate();
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

void CDlgImage::OnLButtonUp(UINT nFlags, CPoint point)
{
	grabbedPointIndex = -1; // release

	CDialogEx::OnLButtonUp(nFlags, point);
}


void CDlgImage::RandomPointsMove()
{
	int maxX = m_ImageWidth - 1;
	int maxY = m_ImageHeight - 1;

	grabbedPointIndex = -1; // release in case we were dragging a point

	for (int i = 0; i < 3; ++i)
	{
		m_points[i].x = rand() % maxX;  // x between 0 and maxX-1
		m_points[i].y = rand() % maxY;  // y between 0 and maxY-1
	}
	m_pointCount = 3;
	CMFCImageDlg* parentDlg = dynamic_cast<CMFCImageDlg*>(GetParent());
	if (parentDlg)
		parentDlg->UpdatePointsPositions(m_points, m_pointCount);
}

void CDlgImage::ResetDisplay()
{
	m_pointCount = 0;
	CMFCImageDlg* parentDlg = dynamic_cast<CMFCImageDlg*>(GetParent());
	if (parentDlg)
		parentDlg->UpdatePointsPositions(m_points, m_pointCount);
}