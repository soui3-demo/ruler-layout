#include "stdafx.h"
#include "CRulerDrugCode.h"


namespace SOUI
{
	CRulerDrugCode::CRulerDrugCode()
	{
		m_sstrContent = L"12345670000000012345";
	}

	CRulerDrugCode::~CRulerDrugCode()
	{
		//
	}

	void CRulerDrugCode::SetDrawBorder(bool bDraw)
	{
		m_bDrawBorder = bDraw;
		Invalidate();
	}

	void CRulerDrugCode::SetPosInfo(SStringW& sstrPos)
	{
		m_sstrPos = sstrPos;
	}

	SStringW CRulerDrugCode::GetPosInfo()
	{
		return m_sstrPos;
	}

	void CRulerDrugCode::SetLeft(int& nLeft)
	{
		m_nLeft = nLeft;
	}

	int CRulerDrugCode::GetLeft()
	{
		return m_nLeft;
	}

	void CRulerDrugCode::SetTop(int& nTop)
	{
		m_nTop = nTop;
	}

	int CRulerDrugCode::GetTop()
	{
		return m_nTop;
	}

	void CRulerDrugCode::SetRight(int& nRight)
	{
		m_nRight = nRight;
	}

	int CRulerDrugCode::GetRight()
	{
		return m_nRight;
	}

	void CRulerDrugCode::SetBottom(int& nBottom)
	{
		m_nBottom = nBottom;
	}

	int CRulerDrugCode::GetBottom()
	{
		return m_nBottom;
	}

	void CRulerDrugCode::SetWidth(int& nWidth)
	{
		m_nWidth = nWidth;
	}

	int CRulerDrugCode::GetWidth()
	{
		return m_nWidth;
	}

	void CRulerDrugCode::SetHeight(int& nHeight)
	{
		m_nHeight = nHeight;
	}

	int CRulerDrugCode::GetHeight()
	{
		return m_nHeight;
	}

	void CRulerDrugCode::CalcPos()
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

	EcPosType CRulerDrugCode::HitPos(SOUI::CPoint& pt)
	{
		for (int i = 0; i < 8; ++i)
		{
			if (m_rcPos[i].PtInRect(pt))
				return EcPosType(i);
		}

		return EcPosType::Null;
	}

	LRESULT CRulerDrugCode::OnCreate(LPVOID)
	{
		return __super::OnCreate(NULL);
	}

	void CRulerDrugCode::OnPaint(IRenderTarget* pRT)
	{
		pRT->SetAntiAlias(TRUE);
		CRect rc;
		GetClientRect(&rc);

		{
			//处理需要绘制图片的区域，留出高度的10%用来绘制文本
			CRect rcImageArea(rc);
			rcImageArea.bottom -= rc.Height() * 0.2;
			DWORD mode = MAKELONG(EM_STRETCH, kHigh_FilterLevel);
			SAutoRefPtr<IBitmap> pBmp = SResLoadFromFile::LoadImage(L"CODE_128.png");
			if (pBmp)
			{
				CRect rcSrc(0, 0, pBmp->Width(), pBmp->Height());
				pRT->DrawBitmapEx(rcImageArea, pBmp, &rcSrc, mode, 255);
			}
			pBmp->Release();

			//绘制条码内容区域

			//处理content
			std::string strContent = S_CW2A(m_sstrContent);
			std::string strIdentification;
			std::string strSerial;
			if (strContent.size() > 7)
			{
				strIdentification = strContent.substr(0, 7);
				strSerial = strContent.substr(7);
			}
			else
				strIdentification = strContent.substr(0, strContent.size());

			std::string strDrawText = "药品标识码:" + strIdentification + " 序列号:" + strSerial;
			SStringW sstrDrawText = S_CA2W(strDrawText.c_str());

			SIZE strSize;
			pRT->MeasureText(sstrDrawText, sstrDrawText.GetLength(), &strSize);
			CRect rcContent(rc);
			rcContent.top += rc.Height() * 0.8;
			rcContent.DeflateRect(10, 2, 10, 2);

			float fXScale = (float)(rcContent.Width()) / strSize.cx;
			float fYScale = (float)(rcContent.Height()) / strSize.cy;

			COLORREF crText(RGBA(0, 0, 0, 255));
			pRT->SetTextColor(crText);

			SMatrix matrix;
			matrix.setScale(fXScale, fYScale);
			pRT->SetTransform(matrix.GetData(), NULL);
			pRT->TextOutW(rcContent.left / fXScale, rcContent.top / fYScale, sstrDrawText, sstrDrawText.GetLength());
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