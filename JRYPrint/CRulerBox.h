#pragma once

#include <vector>
#include "CGdiDc.h"
#include "CBitmap.h"

#include "CRulerRectangle.h"
#include "CRulerEllipse.h"
#include "CRulerBarCode.h"
#include "CRulerDrugCode.h"
#include "CRulerQRCode.h"
#include "CRulerText.h"
#include "CRulerPicture.h"
#include "CRulerPDF.h"

namespace SOUI
{
	class CRulerBox : public SWindow
	{
		SOUI_CLASS_NAME(CRulerBox, L"rulerbox")
	public:
		CRulerBox(void);
		~CRulerBox(void);

		void SetPix2CM(int& nPix);
		void SetInsertType(int nType);
		void SetBmpResource(CJPBitmap* pBmp);
		void SetDrawCross(bool bDraw);

	protected:
		bool OnEventRectangleLButtonDown(EventRectangleLButtonDown* pEvt);
		bool OnEventRectangleLButtonUp(EventRectangleLButtonUp* pEvt);
		bool OnEventRectangleLMoving(EventRectangleMoving* pEvt);

	protected:
		void OnPaint(IRenderTarget* pRT);
		void OnLButtonDown(UINT nFlags, SOUI::CPoint point);
		void OnLButtonUp(UINT nFlags, SOUI::CPoint point);
		void OnLButtonDblClk(UINT nFlags, SOUI::CPoint point);
		void OnMouseMove(UINT nFlags, SOUI::CPoint point);

		SOUI_MSG_MAP_BEGIN()
			MSG_WM_PAINT_EX(OnPaint)
			MSG_WM_LBUTTONDOWN(OnLButtonDown)
			MSG_WM_LBUTTONUP(OnLButtonUp)
			MSG_WM_LBUTTONDBLCLK(OnLButtonDblClk)
			MSG_WM_MOUSEMOVE(OnMouseMove)
		SOUI_MSG_MAP_END()

		SOUI_ATTRS_BEGIN()
// 			ATTR_SKIN(L"starSkin", m_pStar, TRUE)
// 			ATTR_INT(L"starNum", m_nStars, TRUE)
// 			ATTR_FLOAT(L"value", m_fValue, TRUE)
		SOUI_ATTRS_END()

	protected:
		bool	m_bSelectObject;
		CRulerRectangle* m_pSelectRectangle;
		std::vector<SWindow*> m_vecSelectObjects;

		SOUI::CPoint m_ptObjDown;

		CJPBitmap* m_pBitmap;
		IBitmap* m_pBmp;
		std::vector<CJPBitmap*>		m_vecBitmap;

		bool	m_bDrawLine;
		int		m_nDPI;

		std::vector<int> m_vecPix2MM;
		int		m_nRatio;
		SOUI::CRect	m_rcCross;

		int		m_nAddObjectType;

		SOUI::CPoint	m_ptDown;				//鼠标按下的位置
		SOUI::CRect		m_rcDrawArea;			//鼠标移动区域

		SOUI::CPoint	m_ptObjLT;
		SOUI::CPoint	m_ptObjRB;

		bool	m_bDrawRulerCross;
		bool	m_bCreateChild;

		HCURSOR	m_hCursorLeft, m_hCursorRight, m_hCursorTop, m_hCursorBottom,
				m_hCursorTopLeft, m_hCursorTopRight, m_hCursorBottomLeft, m_hCursorBottomRight,
				m_hCursorHand, m_hCursorArrow, m_hCursorSelect;

		EcPosType m_curEcPosType;

		int		m_nPix2CM;
	public:
		void ShowCursor(EcPosType ePos);

	private:
		HBITMAP CopyCurBitmap(int nx, int ny, int nWidth, int nHeight);
	public:
		void AddRectangle(const SOUI::CRect& rcRectangle);
		void AddEllipse(const SOUI::CRect& rcEllipse);
		void AddBarCode(const SOUI::CRect& rcBarCoed);
		void AddDrugCode(const SOUI::CRect& rcDrugCode);
		void AddQRCode(const SOUI::CRect& rcQRCode);
		void AddText(const SOUI::CRect& rcText);
		void AddPicture(const SOUI::CRect& rcPicture);
		void AddPDF(const SOUI::CRect& rcPDF);
	public:
		//
	};

}

