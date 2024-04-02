#include "stdafx.h"
#include "CRulerPDF.h"


namespace SOUI
{
	CRulerPDF::CRulerPDF()
	{
	}

	CRulerPDF::~CRulerPDF()
	{
		//
	}

	void CRulerPDF::SetDrawBorder(bool bDraw)
	{
		m_bDrawBorder = bDraw;
		Invalidate();
	}

	void CRulerPDF::SetPosInfo(SStringW& sstrPos)
	{
		m_sstrPos = sstrPos;
	}

	SStringW CRulerPDF::GetPosInfo()
	{
		return m_sstrPos;
	}

	void CRulerPDF::SetLeft(int& nLeft)
	{
		m_nLeft = nLeft;
	}

	int CRulerPDF::GetLeft()
	{
		return m_nLeft;
	}

	void CRulerPDF::SetTop(int& nTop)
	{
		m_nTop = nTop;
	}

	int CRulerPDF::GetTop()
	{
		return m_nTop;
	}

	void CRulerPDF::SetRight(int& nRight)
	{
		m_nRight = nRight;
	}

	int CRulerPDF::GetRight()
	{
		return m_nRight;
	}

	void CRulerPDF::SetBottom(int& nBottom)
	{
		m_nBottom = nBottom;
	}

	int CRulerPDF::GetBottom()
	{
		return m_nBottom;
	}

	void CRulerPDF::SetWidth(int& nWidth)
	{
		m_nWidth = nWidth;
	}

	int CRulerPDF::GetWidth()
	{
		return m_nWidth;
	}

	void CRulerPDF::SetHeight(int& nHeight)
	{
		m_nHeight = nHeight;
	}

	int CRulerPDF::GetHeight()
	{
		return m_nHeight;
	}

	void CRulerPDF::SetPDFPath(SStringW& sstrPath)
	{
		m_sstrPath = sstrPath;

		//调用mupdf将pdf的第一页生成图片，然后绘制到当前控件中

		fz_context* ctx = fz_new_context(NULL, NULL, FZ_STORE_DEFAULT);
		fz_register_document_handlers(ctx);
		std::string strPath = S_CW2A(m_sstrPath, CP_UTF8);
		fz_document* doc = fz_open_document(ctx, strPath.c_str());
		int nCount = fz_count_pages(ctx, doc);

		//默认600DPI
		float xZoom = (float)100 / (float)72;
		float yZoom = (float)100 / (float)72;
		fz_matrix ctm = fz_scale(xZoom, yZoom);
		if (nCount > 0)  //只取一张
		{
			fz_pixmap* pix = fz_new_pixmap_from_page_number(ctx, doc, 0, ctm, fz_device_gray(ctx), 0);  //灰度图
			fz_save_pixmap_as_png(ctx, pix, "pdf_test.png");
			fz_drop_pixmap(ctx, pix);
		}
		fz_drop_document(ctx, doc);
		fz_drop_context(ctx);

		Invalidate();
	}

	void CRulerPDF::CalcPos()
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

	EcPosType CRulerPDF::HitPos(SOUI::CPoint& pt)
	{
		for (int i = 0; i < 8; ++i)
		{
			if (m_rcPos[i].PtInRect(pt))
				return EcPosType(i);
		}

		return EcPosType::Null;
	}

	LRESULT CRulerPDF::OnCreate(LPVOID)
	{
		return __super::OnCreate(NULL);
	}

	void CRulerPDF::OnPaint(IRenderTarget* pRT)
	{
		CRect rc;
		GetClientRect(&rc);

		{
			DWORD mode = MAKELONG(EM_STRETCH, kHigh_FilterLevel);
			SAutoRefPtr<IBitmap> pBmp = SResLoadFromFile::LoadImage(L"pdf_test.png");
			if (pBmp)
			{
				CRect rcSrc(0, 0, pBmp->Width(), pBmp->Height());
				pRT->DrawBitmapEx(rc, pBmp, &rcSrc, mode, 255);
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