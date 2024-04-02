#include "stdafx.h"
#include "CRulerRectangle.h"

#include "CRulerBox.h"

namespace SOUI
{
	CRulerRectangle::CRulerRectangle()
	{
		m_bDrawBorder = true;

		m_evtSet.addEvent(EVENTID(EventRectangleLButtonDown));
		m_evtSet.addEvent(EVENTID(EventRectangleLButtonUp));
		m_evtSet.addEvent(EVENTID(EventRectangleMoving));
	}

	CRulerRectangle::~CRulerRectangle()
	{
		//
	}

	void CRulerRectangle::SetDrawBorder(bool bDraw)
	{
		m_bDrawBorder = bDraw;
		Invalidate();
	}

	void CRulerRectangle::SetPosInfo(SStringW& sstrPos)
	{
		m_sstrPos = sstrPos;
	}

	SStringW CRulerRectangle::GetPosInfo()
	{
		return m_sstrPos;
	}

	void CRulerRectangle::SetLeft(int& nLeft)
	{
		m_nLeft = nLeft;
	}

	int CRulerRectangle::GetLeft()
	{
		return m_nLeft;
	}

	void CRulerRectangle::SetTop(int& nTop)
	{
		m_nTop = nTop;
	}

	int CRulerRectangle::GetTop()
	{
		return m_nTop;
	}

	void CRulerRectangle::SetRight(int& nRight)
	{
		m_nRight = nRight;
	}

	int CRulerRectangle::GetRight()
	{
		return m_nRight;
	}

	void CRulerRectangle::SetBottom(int& nBottom)
	{
		m_nBottom = nBottom;
	}

	int CRulerRectangle::GetBottom()
	{
		return m_nBottom;
	}

	void CRulerRectangle::SetWidth(int& nWidth)
	{
		m_nWidth = nWidth;
	}

	int CRulerRectangle::GetWidth()
	{
		return m_nWidth;
	}

	void CRulerRectangle::SetHeight(int& nHeight)
	{
		m_nHeight = nHeight;
	}

	int CRulerRectangle::GetHeight()
	{
		return m_nHeight;
	}

	void CRulerRectangle::CalcPos()
	{
		CRect rc;
		GetClientRect(&rc);

		SOUI::CPoint center = rc.CenterPoint();
		// 上左 方块
		m_rcPos[(int)EcPosType::TopLeft].SetRect(rc.left, rc.top, rc.left + 8, rc.top + 8);
		// 上中 方块
		m_rcPos[(int)EcPosType::TopCenter].SetRect(center.x - 4, rc.top, center.x + 4, rc.top + 8);
		// 上右 方块
		m_rcPos[(int)EcPosType::TopRight].SetRect(rc.right - 8, rc.top, rc.right, rc.top + 8);
		// 右中 方块
		m_rcPos[(int)EcPosType::RightCenter].SetRect(rc.right - 8, center.y - 4, rc.right, center.y + 4);
		// 下右 方块
		m_rcPos[(int)EcPosType::BottomRight].SetRect(rc.right - 8, rc.bottom - 8, rc.right, rc.bottom);
		// 下中 方块
		m_rcPos[(int)EcPosType::BottomCenter].SetRect(center.x - 4, rc.bottom - 8, center.x + 4, rc.bottom);
		// 下左 方块
		m_rcPos[(int)EcPosType::BottomLeft].SetRect(rc.left, rc.bottom - 8, rc.left + 8, rc.bottom);
		// 左中 方块
		m_rcPos[(int)EcPosType::LeftCenter].SetRect(rc.left, center.y - 4, rc.left + 8, center.y + 4);
	}

	EcPosType CRulerRectangle::HitPos(SOUI::CPoint& pt)
	{
		for (int i = 0; i < 8; ++i)
		{
			if (m_rcPos[i].PtInRect(pt))
				return EcPosType(i);
		}

		return EcPosType::Null;
	}	

	LRESULT CRulerRectangle::OnCreate(LPVOID)
	{
		return __super::OnCreate(NULL);
	}

	void CRulerRectangle::OnPaint(IRenderTarget* pRT)
	{
		CRect rc;
		GetClientRect(&rc);

		{
			CAutoRefPtr<IBrush> brush, oldbrush;
			pRT->CreateSolidColorBrush(RGBA(0, 0, 0, 255), &brush);
			pRT->SelectObject(brush, (IRenderObj**)&oldbrush);
			pRT->FillRectangle(&rc);
			pRT->SelectObject(oldbrush, NULL);
		}

		{
			//绘制操作border
			if (m_bDrawBorder)
			{
				CAutoRefPtr<IPen> pen, oldpen;
				pRT->CreatePen(PS_SOLID, RGBA(0, 0, 255, 20), 2, &pen);
				pRT->SelectObject(pen, (IRenderObj**)&oldpen);

				CRect rcBorder(rc);
				pRT->DrawRectangle(rcBorder);
				pRT->SelectObject(oldpen, NULL);
			}
		}

		{
			if (m_bDrawBorder)
			{
				//绘制操作border上的点
				CAutoRefPtr<IPen> pen, oldpen;
				CAutoRefPtr<IBrush> brush, oldbrush;
				pRT->CreatePen(PS_SOLID, RGBA(0, 0, 255, 20), 1, &pen);
				pRT->CreateSolidColorBrush(RGBA(255, 0, 0, 255), &brush);
				pRT->SelectObject(pen, (IRenderObj**)&oldpen);
				pRT->SelectObject(brush, (IRenderObj**)&oldbrush);

				SOUI::CPoint center = rc.CenterPoint();
				// 上左 方块
				m_rcPos[(int)EcPosType::TopLeft].SetRect(rc.left, rc.top, rc.left + 8, rc.top + 8);
				// 上中 方块
				m_rcPos[(int)EcPosType::TopCenter].SetRect(center.x - 4, rc.top, center.x + 4, rc.top + 8);
				// 上右 方块
				m_rcPos[(int)EcPosType::TopRight].SetRect(rc.right - 8, rc.top, rc.right, rc.top + 8);
				// 右中 方块
				m_rcPos[(int)EcPosType::RightCenter].SetRect(rc.right - 8, center.y - 4, rc.right, center.y + 4);
				// 下右 方块
				m_rcPos[(int)EcPosType::BottomRight].SetRect(rc.right - 8, rc.bottom - 8, rc.right, rc.bottom);
				// 下中 方块
				m_rcPos[(int)EcPosType::BottomCenter].SetRect(center.x - 4, rc.bottom - 8, center.x + 4, rc.bottom);
				// 下左 方块
				m_rcPos[(int)EcPosType::BottomLeft].SetRect(rc.left, rc.bottom - 8, rc.left + 8, rc.bottom);
				// 左中 方块
				m_rcPos[(int)EcPosType::LeftCenter].SetRect(rc.left, center.y - 4, rc.left + 8, center.y + 4);

				for (int i = 0; i < 8; ++i)
				{
					CRect rcDot(m_rcPos[i]);
					pRT->FillRectangle(rcDot);
				}

				pRT->SelectObject(oldpen, NULL);
				pRT->SelectObject(oldbrush, NULL);
			}
		}

		SWindow::OnPaint(pRT);
	}
}