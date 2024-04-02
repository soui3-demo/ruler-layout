#include "stdafx.h"
#include "CRulerPicture.h"

namespace SOUI
{
	CRulerPicture::CRulerPicture()
	{
	}

	CRulerPicture::~CRulerPicture()
	{
		//
	}

	void CRulerPicture::SetDrawBorder(bool bDraw)
	{
		m_bDrawBorder = bDraw;
		Invalidate();
	}

	void CRulerPicture::SetPosInfo(SStringW& sstrPos)
	{
		m_sstrPos = sstrPos;
	}

	SStringW CRulerPicture::GetPosInfo()
	{
		return m_sstrPos;
	}

	void CRulerPicture::SetLeft(int& nLeft)
	{
		m_nLeft = nLeft;
	}

	int CRulerPicture::GetLeft()
	{
		return m_nLeft;
	}

	void CRulerPicture::SetTop(int& nTop)
	{
		m_nTop = nTop;
	}

	int CRulerPicture::GetTop()
	{
		return m_nTop;
	}

	void CRulerPicture::SetRight(int& nRight)
	{
		m_nRight = nRight;
	}

	int CRulerPicture::GetRight()
	{
		return m_nRight;
	}

	void CRulerPicture::SetBottom(int& nBottom)
	{
		m_nBottom = nBottom;
	}

	int CRulerPicture::GetBottom()
	{
		return m_nBottom;
	}

	void CRulerPicture::SetWidth(int& nWidth)
	{
		m_nWidth = nWidth;
	}

	int CRulerPicture::GetWidth()
	{
		return m_nWidth;
	}

	void CRulerPicture::SetHeight(int& nHeight)
	{
		m_nHeight = nHeight;
	}

	int CRulerPicture::GetHeight()
	{
		return m_nHeight;
	}

	void CRulerPicture::CalcPos()
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

	EcPosType CRulerPicture::HitPos(SOUI::CPoint& pt)
	{
		for (int i = 0; i < 8; ++i)
		{
			if (m_rcPos[i].PtInRect(pt))
				return EcPosType(i);
		}

		return EcPosType::Null;
	}

	LRESULT CRulerPicture::OnCreate(LPVOID)
	{
		return __super::OnCreate(NULL);
	}

	void CRulerPicture::OnLButtonDblClk(UINT nFlags, CPoint point)
	{
		BringWindowToTop();
	}

	void CRulerPicture::OnPaint(IRenderTarget* pRT)
	{
		CRect rc;
		GetClientRect(&rc);

		{
			CRect rcDrawPic(rc);
			rcDrawPic.bottom -= 0;
			DWORD mode = MAKELONG(EM_STRETCH, kHigh_FilterLevel);
			SAutoRefPtr<IBitmap> pBmp = SResLoadFromFile::LoadImage(L"test.png");
			if (pBmp)
			{
				CRect rcSrc(0, 0, pBmp->Width() , pBmp->Height());
				pRT->DrawBitmapEx(rcDrawPic, pBmp, &rcSrc, mode, 255);
			}
			pBmp->Release();
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