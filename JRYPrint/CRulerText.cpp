#include "stdafx.h"
#include "CRulerText.h"


namespace SOUI
{
	CRulerText::CRulerText()
	{
		m_sstrContent = L"1234567890A";
	}

	CRulerText::~CRulerText()
	{
		//
	}

	void CRulerText::SetDrawBorder(bool bDraw)
	{
		m_bDrawBorder = bDraw;
		Invalidate();
	}

	void CRulerText::SetPosInfo(SStringW& sstrPos)
	{
		m_sstrPos = sstrPos;
	}

	SStringW CRulerText::GetPosInfo()
	{
		return m_sstrPos;
	}

	void CRulerText::SetLeft(int& nLeft)
	{
		m_nLeft = nLeft;
	}

	int CRulerText::GetLeft()
	{
		return m_nLeft;
	}

	void CRulerText::SetTop(int& nTop)
	{
		m_nTop = nTop;
	}

	int CRulerText::GetTop()
	{
		return m_nTop;
	}

	void CRulerText::SetRight(int& nRight)
	{
		m_nRight = nRight;
	}

	int CRulerText::GetRight()
	{
		return m_nRight;
	}

	void CRulerText::SetBottom(int& nBottom)
	{
		m_nBottom = nBottom;
	}

	int CRulerText::GetBottom()
	{
		return m_nBottom;
	}

	void CRulerText::SetWidth(int& nWidth)
	{
		m_nWidth = nWidth;
	}

	int CRulerText::GetWidth()
	{
		return m_nWidth;
	}

	void CRulerText::SetHeight(int& nHeight)
	{
		m_nHeight = nHeight;
	}

	int CRulerText::GetHeight()
	{
		return m_nHeight;
	}

	void CRulerText::CalcPos()
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

	EcPosType CRulerText::HitPos(SOUI::CPoint& pt)
	{
		for (int i = 0; i < 8; ++i)
		{
			if (m_rcPos[i].PtInRect(pt))
				return EcPosType(i);
		}

		return EcPosType::Null;
	}

	LRESULT CRulerText::OnCreate(LPVOID)
	{
		return __super::OnCreate(NULL);
	}

	void CRulerText::OnPaint(IRenderTarget* pRT)
	{
		pRT->SetAntiAlias(TRUE);
		CRect rc;
		GetWindowRect(&rc);

		//绘制文本，需要根据控件大小计算文本大小
		{
			COLORREF crText(RGBA(0, 0, 0, 255));
			pRT->SetTextColor(crText);
			SIZE strSize;
			pRT->MeasureText(m_sstrContent, m_sstrContent.GetLength(), &strSize);

			CRect rcTextArea(rc);
			rcTextArea.left = 0;
			rcTextArea.top = 0;
			rc.right = rc.left + m_nWidth;
			rc.bottom = rc.top + m_nHeight;

			float fXScale = (float)(m_nWidth) / strSize.cx;
			float fYScale = (float)(m_nHeight) / strSize.cy;

			SMatrix matrix;
			matrix.setScale(fXScale, fYScale);
			pRT->SetTransform(matrix.GetData(), NULL);
			pRT->TextOutW(rc.left / fXScale, rc.top / fYScale, m_sstrContent, m_sstrContent.GetLength());
			pRT->SetTransform(SMatrix().GetData());
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

				SOUI::CRect	m_rcPos[8];
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