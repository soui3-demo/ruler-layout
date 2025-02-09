// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include "CRulerBox.h"
#include "CRulerBar.h"

class CMainDlg : public SHostWnd
{
public:
	CMainDlg();
	~CMainDlg();

	void OnClose();
	void OnMaximize();
	void OnRestore();
	void OnMinimize();
	void OnSize(UINT nType, CSize size);
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);

public:
	bool OnEventScrollViewScroll(EventScroll* pEvt);
public:
	void OnBnClickRectangle();
	void OnBnClickEllipse();
	void OnBnClickBarCode();
	void OnBnClickDrugCode();
	void OnBnClickQRCode();
	void OnBnClickText();
	void OnBnClickPicture();
	void OnBnClickPDF();

	void OnBnClickAddRatio();
	void OnBnClickSubRatio();
protected:
	void OnLanguage(int nID);
	void OnLanguageBtnCN();
	void OnLanguageBtnJP();
	//soui消息
	EVENT_MAP_BEGIN()
		EVENT_NAME_COMMAND(L"btn_close", OnClose)
		EVENT_NAME_COMMAND(L"btn_min", OnMinimize)
		EVENT_NAME_COMMAND(L"btn_max", OnMaximize)
		EVENT_NAME_COMMAND(L"btn_restore", OnRestore)
		EVENT_NAME_COMMAND(L"zh_cn", OnLanguageBtnCN)
		EVENT_NAME_COMMAND(L"jp", OnLanguageBtnJP)

		EVENT_NAME_COMMAND(L"btn_rectangle", OnBnClickRectangle)
		EVENT_NAME_COMMAND(L"btn_ellipse", OnBnClickEllipse)
		EVENT_NAME_COMMAND(L"btn_barcode", OnBnClickBarCode)
		EVENT_NAME_COMMAND(L"btn_drugcode", OnBnClickDrugCode)
		EVENT_NAME_COMMAND(L"btn_qrcode", OnBnClickQRCode)
		EVENT_NAME_COMMAND(L"btn_text", OnBnClickText)
		EVENT_NAME_COMMAND(L"btn_picture", OnBnClickPicture)
		EVENT_NAME_COMMAND(L"btn_pdf", OnBnClickPDF)

		EVENT_NAME_COMMAND(L"btn_addratio", OnBnClickAddRatio)
		EVENT_NAME_COMMAND(L"btn_subratio", OnBnClickSubRatio)
	EVENT_MAP_END()
		
	//HostWnd真实窗口消息处理
	BEGIN_MSG_MAP_EX(CMainDlg)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_CLOSE(OnClose)
		MSG_WM_SIZE(OnSize)
		CHAIN_MSG_MAP(SHostWnd)
		REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()

private:
	STabCtrl tabCtrl;

	std::vector<int> m_vecPix2CM;  //多少像素1cm
	int		m_nRatio;
};
