// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainDlg.h"	
	
CMainDlg::CMainDlg() : SHostWnd(_T("LAYOUT:XML_MAINWND"))
{
	int nPixUnit = 30;
	for (int i = 0; i < 8; i++)
	{
		m_vecPix2CM.push_back(i * nPixUnit + 30);
	}
	m_nRatio = 0;
}

CMainDlg::~CMainDlg()
{
}

bool CMainDlg::OnEventScrollViewScroll(EventScroll* pEvt)
{
	SScrollView* pScrollView = FindChildByName2<SScrollView>(L"scrollview");
	SASSERT(pScrollView);

	if (pEvt->bVertical)  //V
	{
		SScrollView* pRulerLeft = FindChildByName2<SScrollView>(L"scrollview_leftruler");
		SASSERT(pRulerLeft);
		pRulerLeft->SetScrollPos(pEvt->bVertical, pEvt->nPos, TRUE);
	}
	else//H
	{
		SScrollView* pRulerTop = FindChildByName2<SScrollView>(L"scrollview_topruler");
		SASSERT(pRulerTop);
		pRulerTop->SetScrollPos(pEvt->bVertical, pEvt->nPos, TRUE);
	}
	return true;
}

BOOL CMainDlg::OnInitDialog(HWND hWnd, LPARAM lParam)
{
	SScrollView* pScrollView = FindChildByName2<SScrollView>(L"scrollview");
	SASSERT(pScrollView);
	pScrollView->GetEventSet()->subscribeEvent(&CMainDlg::OnEventScrollViewScroll, this);

	SScrollView* pRulerLeft = FindChildByName2<SScrollView>(L"scrollview_leftruler");
	SASSERT(pRulerLeft);
	pRulerLeft->Invalidate();

	SScrollView* pRulerTop = FindChildByName2<SScrollView>(L"scrollview_topruler");
	SASSERT(pRulerTop);
	pRulerTop->Invalidate();

	CRulerBox* pRulerbox = FindChildByName2<CRulerBox>(L"rulerbox");
	SASSERT(pRulerbox);


	return 0;
}

void CMainDlg::OnLanguageBtnCN()
{
	OnLanguage(1);
}
void CMainDlg::OnLanguageBtnJP()
{
	OnLanguage(0);
}
void CMainDlg::OnLanguage(int nID)
{
	ITranslatorMgr *pTransMgr = SApplication::getSingletonPtr()->GetTranslator();
	SASSERT(pTransMgr);
	bool bCnLang = nID == 1;

	pugi::xml_document xmlLang;
	if (SApplication::getSingletonPtr()->LoadXmlDocment(xmlLang, bCnLang ? _T("translator:lang_cn") : _T("translator:lang_jp")))
	{
		CAutoRefPtr<ITranslator> lang;
		pTransMgr->CreateTranslator(&lang);
		lang->Load(&xmlLang.child(L"language"), 1);//1=LD_XML
		TCHAR lngName[TR_MAX_NAME_LEN] = {0};
		lang->GetName(lngName);
		pTransMgr->SetLanguage(lngName);
		pTransMgr->InstallTranslator(lang);
		SDispatchMessage(UM_SETLANGUAGE,0,0);
	}
}

//TODO:消息映射
void CMainDlg::OnClose()
{
	SNativeWnd::DestroyWindow();
}

void CMainDlg::OnMaximize()
{
	SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
}
void CMainDlg::OnRestore()
{
	SendMessage(WM_SYSCOMMAND, SC_RESTORE);
}
void CMainDlg::OnMinimize()
{
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);
}

void CMainDlg::OnBnClickRectangle()
{
	CRulerBox* pRulerbox = FindChildByName2<CRulerBox>(L"rulerbox");
	SASSERT(pRulerbox);
	pRulerbox->SetInsertType(RulerTypeRectangle);
}

void CMainDlg::OnBnClickEllipse()
{
	CRulerBox* pRulerbox = FindChildByName2<CRulerBox>(L"rulerbox");
	SASSERT(pRulerbox);
	pRulerbox->SetInsertType(RulerTypeEllipse);
}

void CMainDlg::OnBnClickBarCode()
{
	CRulerBox* pRulerbox = FindChildByName2<CRulerBox>(L"rulerbox");
	SASSERT(pRulerbox);
	pRulerbox->SetInsertType(RulerTypeBarCode);
}

void CMainDlg::OnBnClickDrugCode()
{
	CRulerBox* pRulerbox = FindChildByName2<CRulerBox>(L"rulerbox");
	SASSERT(pRulerbox);
	pRulerbox->SetInsertType(RulerTypeDrugCode);
}

void CMainDlg::OnBnClickQRCode()
{
	CRulerBox* pRulerbox = FindChildByName2<CRulerBox>(L"rulerbox");
	SASSERT(pRulerbox);
	pRulerbox->SetInsertType(RulerTypeQRCode);
}

void CMainDlg::OnBnClickText()
{
	CRulerBox* pRulerbox = FindChildByName2<CRulerBox>(L"rulerbox");
	SASSERT(pRulerbox);
	pRulerbox->SetInsertType(RulerTypeText);
}

void CMainDlg::OnBnClickPicture()
{
	CRulerBox* pRulerbox = FindChildByName2<CRulerBox>(L"rulerbox");
	SASSERT(pRulerbox);
	pRulerbox->SetInsertType(RulerTypePicture);
}

void CMainDlg::OnBnClickPDF()
{
	CRulerBox* pRulerbox = FindChildByName2<CRulerBox>(L"rulerbox");
	SASSERT(pRulerbox);
	pRulerbox->SetInsertType(RulerTypePDF);
}

void CMainDlg::OnBnClickAddRatio()
{
	if (m_nRatio == 7)
		return;
	m_nRatio += 1;
	int nPix = m_vecPix2CM[m_nRatio];

	int nWidth = nPix * 100;
	int nHeight = nPix * 100;

	CRulerBox* pRulerbox = FindChildByName2<CRulerBox>(L"rulerbox");
	SASSERT(pRulerbox);
	pRulerbox->SetPix2CM(nPix);
	SStringW sstrPos;
	sstrPos.Format(L"%d,%d", nWidth, nHeight);
	pRulerbox->SetAttribute(L"size", sstrPos);

	sstrPos.Format(L"%d,40", nWidth);
	CRulerBar* pTopRuler = FindChildByName2<CRulerBar>(L"ruler_top");
	SASSERT(pTopRuler);
	pTopRuler->SetPix2CM(nPix);
	pTopRuler->SetAttribute(L"size", sstrPos);

	sstrPos.Format(L"40,%d", nHeight);
	CRulerBar* pLeftRuler = FindChildByName2<CRulerBar>(L"ruler_left");
	SASSERT(pLeftRuler);
	pLeftRuler->SetPix2CM(nPix);
	pLeftRuler->SetAttribute(L"size", sstrPos);
}

void CMainDlg::OnBnClickSubRatio()
{
	if (m_nRatio == 0)
		return;
	m_nRatio -= 1;
	int nPix = m_vecPix2CM[m_nRatio];

	int nWidth = nPix * 100;
	int nHeight = nPix * 100;

	CRulerBox* pRulerbox = FindChildByName2<CRulerBox>(L"rulerbox");
	SASSERT(pRulerbox);
	pRulerbox->SetPix2CM(nPix);
	SStringW sstrPos;
	sstrPos.Format(L"%d,%d", nWidth, nHeight);
	pRulerbox->SetAttribute(L"size", sstrPos);

	sstrPos.Format(L"%d,40", nWidth);
	CRulerBar* pTopRuler = FindChildByName2<CRulerBar>(L"ruler_top");
	SASSERT(pTopRuler);
	pTopRuler->SetPix2CM(nPix);
	pTopRuler->SetAttribute(L"size", sstrPos);

	sstrPos.Format(L"40,%d", nHeight);
	CRulerBar* pLeftRuler = FindChildByName2<CRulerBar>(L"ruler_left");
	SASSERT(pLeftRuler);
	pLeftRuler->SetPix2CM(nPix);
	pLeftRuler->SetAttribute(L"size", sstrPos);
}

void CMainDlg::OnSize(UINT nType, CSize size)
{
	SetMsgHandled(FALSE);	
	SWindow *pBtnMax = FindChildByName(L"btn_max");
	SWindow *pBtnRestore = FindChildByName(L"btn_restore");
	if(!pBtnMax || !pBtnRestore) return;
	
	if (nType == SIZE_MAXIMIZED)
	{
		pBtnRestore->SetVisible(TRUE);
		pBtnMax->SetVisible(FALSE);
	}
	else if (nType == SIZE_RESTORED)
	{
		pBtnRestore->SetVisible(FALSE);
		pBtnMax->SetVisible(TRUE);
	}
}

