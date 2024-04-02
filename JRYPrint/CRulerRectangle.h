#pragma once


namespace SOUI
{
	class EventRectangleLButtonDown : public TplEventArgs<EventRectangleLButtonDown>
	{
		SOUI_CLASS_NAME(EventRectangleLButtonDown, L"on_rectangle_lbuttondown")
	public:
		EventRectangleLButtonDown(SObject* pSender)
			: TplEventArgs<EventRectangleLButtonDown>(pSender) {}
		enum { EventID = EVT_EXTERNAL_BEGIN + 51 };
	};
	class EventRectangleLButtonUp : public TplEventArgs<EventRectangleLButtonUp>
	{
		SOUI_CLASS_NAME(EventRectangleLButtonUp, L"on_rectangle_lbuttonup")
	public:
		EventRectangleLButtonUp(SObject* pSender)
			: TplEventArgs<EventRectangleLButtonUp>(pSender) {}
		enum { EventID = EVT_EXTERNAL_BEGIN + 52 };
	};
	class EventRectangleMoving : public TplEventArgs<EventRectangleMoving>
	{
		SOUI_CLASS_NAME(EventRectangleMoving, L"on_rectangle_moving")
	public:
		EventRectangleMoving(SObject* pSender)
			: TplEventArgs<EventRectangleMoving>(pSender) {}
		enum { EventID = EVT_EXTERNAL_BEGIN + 53 };
		int	m_nX;
		int	m_nY;

		SStringW	m_sstrPos;
	};

	class CRulerRectangle : public SWindow
	{
		SOUI_CLASS_NAME(CRulerRectangle, L"rulerrect")

	public:
		CRulerRectangle();
		~CRulerRectangle();

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
	};

}
