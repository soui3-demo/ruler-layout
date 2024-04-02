#include "stdafx.h"
#include "CRulerBar.h"


CRulerBar::CRulerBar()
{
	m_nPix = 30;
}

CRulerBar::~CRulerBar()
{
	//
}

void CRulerBar::OnPaint(IRenderTarget* pRT)
{
	pRT->SetAntiAlias(FALSE);
	COLORREF crRulerText(RGBA(50, 50, 50, 100));
	CAutoRefPtr<IPen> pen, oldpen;
	pRT->CreatePen(PS_SOLID, RGBA(0, 0, 0, 255), 1, &pen);
	pRT->SelectObject(pen, (IRenderObj**)&oldpen);
	CRect rcRuler = GetWindowRect();
	switch (m_nBarPosition)
	{
	case 0:
	{//top
		{//下边的线
			double dXStart = rcRuler.left;
			double dYStart = rcRuler.bottom - 1;
			double dXEnd = rcRuler.right;
			double dYEnd = rcRuler.bottom - 1;

			CPoint pts[2];
			pts[0].x = dXStart;
			pts[0].y = dYStart;

			pts[1].x = dXEnd;
			pts[1].y = dYEnd;
			pRT->DrawLines(pts, 2);
		}
		{//刻度线
			for (int i = 0; i < 100; i++)
			{
				double dXStart = rcRuler.left + i * m_nPix;
				double dYStart = rcRuler.top + 5;
				double dXEnd = dXStart;
				double dYEnd = rcRuler.bottom;
				{//长刻度线
					CPoint pts[2];
					pts[0].x = dXStart;
					pts[0].y = dYStart;

					pts[1].x = dXEnd;
					pts[1].y = dYEnd;
					pRT->DrawLines(pts, 2);
				}

				//文本绘制区域
				SOUI::CRect rcText;
				rcText.left = dXStart;
				rcText.top = dYStart;
				rcText.right = dXStart + m_nPix;
				rcText.bottom = dYEnd;

				{//短刻度线
					for (int j = 1; j < 5; j++)
					{
						double startX = rcText.left + j * (rcText.Width() / 5);
						double startY = rcText.top + (rcText.Height() / 2);
						double endX = startX;
						double endY = rcText.bottom;

						CPoint pts[2];
						pts[0].x = startX;
						pts[0].y = startY;

						pts[1].x = endX;
						pts[1].y = endY;
						pRT->DrawLines(pts, 2);
					}
				}

				SStringT sstrContent;
				sstrContent.Format(_T("%d"), i);
				pRT->SetTextColor(crRulerText);
				pRT->DrawText(sstrContent, sstrContent.GetLength(), rcText, DT_SINGLELINE | DT_LEFT | DT_TOP);
			}
		}
	}
	break;
	case 1:
	{
		//right
	}
		break;
	case 2:
	{
		//bottom
	}
	break;
	case 3:
	{//left
		//top
		{//右边的线
			CPoint pts[2];
			pts[0].x = rcRuler.right - 1;
			pts[0].y = rcRuler.top;

			pts[1].x = rcRuler.right - 1;
			pts[1].y = rcRuler.bottom;
			pRT->DrawLines(pts, 2);
		}
		{//刻度线
			for (int i = 0; i < 100; i++)
			{
				double dXStart = rcRuler.left + 5;
				double dYStart = rcRuler.top + i * m_nPix;
				double dXEnd = rcRuler.right;
				double dYEnd = dYStart;
				{//长刻度线
					CPoint pts[2];
					pts[0].x = dXStart;
					pts[0].y = dYStart;

					pts[1].x = dXEnd;
					pts[1].y = dYEnd;
					pRT->DrawLines(pts, 2);
				}

				//文本绘制区域
				SOUI::CRect rcText;
				rcText.left = dXStart;
				rcText.top = dYStart;
				rcText.right = dXEnd;
				rcText.bottom = dYStart + m_nPix;

				{//短刻度线
					for (int j = 1; j < 5; j++)
					{
						double startX = rcText.left + (rcText.Width() / 2);
						double startY = rcText.top + j * (rcText.Height() / 5);

						double endX = rcText.right;
						double endY = startY;

						CPoint pts[2];
						pts[0].x = startX;
						pts[0].y = startY;

						pts[1].x = endX;
						pts[1].y = endY;
						pRT->DrawLines(pts, 2);
					}
				}

				SStringT sstrContent;
				sstrContent.Format(_T("%d"), i);
				pRT->SetTextColor(crRulerText);
				pRT->DrawText(sstrContent, sstrContent.GetLength(), rcText, DT_SINGLELINE | DT_LEFT | DT_TOP);
			}
		}
		pRT->SelectObject(oldpen, NULL);
	}
	break;
	default:
		break;
	}

	SWindow::OnPaint(pRT);
}

void CRulerBar::SetPix2CM(int nPix)
{
	m_nPix = nPix;
	Invalidate();
}