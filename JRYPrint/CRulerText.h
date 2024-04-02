#pragma once

namespace SOUI
{
	class CRulerText : public SWindow
	{
		SOUI_CLASS_NAME(CRulerText, L"rulertext")
	public:
		CRulerText();
		~CRulerText();

	public:
		void SetDrawBorder(bool bDraw);
		void SetPosInfo(SStringW& sstrPos);
		SStringW GetPosInfo();
		void SetLeft(int& nLeft);
		int GetLeft();
		void SetTop(int& nTop);
		int GetTop();
		void SetRight(int& nRight);
		int GetRight();
		void SetBottom(int& nBottom);
		int	GetBottom();
		void SetWidth(int& nWidth);
		int GetWidth();
		void SetHeight(int& nHeight);
		int GetHeight();

		void CalcPos();
		EcPosType HitPos(SOUI::CPoint& pt);

	protected:
		LRESULT OnCreate(LPVOID);
		void OnPaint(IRenderTarget* pRT);

	protected:
		SOUI_MSG_MAP_BEGIN()
			MSG_WM_CREATE(OnCreate)
			MSG_WM_PAINT_EX(OnPaint)
			SOUI_MSG_MAP_END()

	protected:
		bool	m_bDrawBorder;

		SStringW m_sstrPos;
		int		m_nLeft;
		int		m_nTop;
		int		m_nRight;
		int		m_nBottom;
		int		m_nWidth;
		int		m_nHeight;

		SOUI::CRect	m_rcPos[8];

		SStringW m_sstrContent;
	};
}
