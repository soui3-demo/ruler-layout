#include "stdafx.h"
#include "CRulerBox.h"
#include "interface/SRender-i.h"

#include <string>
#include <iostream>
#include <stdexcept>

#include "BarcodeFormat.h"
#include "MultiFormatWriter.h"
#include "BitMatrix.h"
#include "ByteMatrix.h"
#include "TextUtfEncoding.h"
#include "ZXStrConvWorkaround.h"
using namespace ZXing;

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

namespace SOUI
{
	CRulerBox::CRulerBox(void)
	{
		m_hCursorSelect = LoadCursor(NULL, IDC_SIZEALL);
		m_hCursorTopRight = LoadCursor(NULL, IDC_SIZENESW);
		m_hCursorBottomRight = LoadCursor(NULL, IDC_SIZENWSE);
		m_hCursorTop = LoadCursor(NULL, IDC_SIZENS);
		m_hCursorBottom = LoadCursor(NULL, IDC_SIZENS);
		m_hCursorLeft = LoadCursor(NULL, IDC_SIZEWE);
		m_hCursorRight = LoadCursor(NULL, IDC_SIZEWE);
		m_hCursorTopLeft = LoadCursor(NULL, IDC_SIZENWSE);
		m_hCursorBottomLeft = LoadCursor(NULL, IDC_SIZENESW);
		m_hCursorHand = LoadCursor(NULL, IDC_HAND);
		m_hCursorArrow = LoadCursor(NULL, IDC_ARROW);

		m_nPix2CM = 30;

		m_bDrawLine = true;

		m_nDPI = 96;

		m_nAddObjectType = RulerTypeNull;

		m_bDrawRulerCross = true;
		m_bCreateChild = false;

		m_bSelectObject = false;

		m_pBitmap = NULL;
	}

	CRulerBox::~CRulerBox(void)
	{
	}

	//test zing library
	void savePng(const BitMatrix& matrix, BarcodeFormat format)
	{
		auto bitmap = ToMatrix<uint8_t>(matrix);

		std::ostringstream os;
		os.str("");
		os << ToString(format);
		os << ".png";
		stbi_write_png(os.str().c_str(), bitmap.width(), bitmap.height(), 1, bitmap.data(), 0);
	}

	void CRulerBox::SetPix2CM(int& nPix)
	{
		bool bTest = false;
		if (bTest)
		{
			//test zing library
			std::wstring text = L"1234567890A";
			for (auto format : {
				BarcodeFormat::AZTEC,
				BarcodeFormat::DATA_MATRIX,
				BarcodeFormat::PDF_417,
				BarcodeFormat::QR_CODE })
			{
				savePng(MultiFormatWriter(format).encode(text, 200, 200), format);
			}
			text = L"01234567890123456789";
			using FormatSpecs = std::vector<std::pair<BarcodeFormat, size_t>>;
			for (int i = 0; i < 9; i++)
			{
				switch (i)
				{
				case 0:
				{
					int length = 0;
					auto input = length > 0 ? text.substr(0, length) : text;
					savePng(MultiFormatWriter(BarcodeFormat::CODABAR).encode(input, 100, 100), BarcodeFormat::CODABAR);
				}
				break;
				case 1:
				{
					int length = 0;
					auto input = length > 0 ? text.substr(0, length) : text;
					savePng(MultiFormatWriter(BarcodeFormat::CODE_39).encode(input, 100, 100), BarcodeFormat::CODE_39);
				}
				break;
				case 2:
				{
					int length = 0;
					auto input = length > 0 ? text.substr(0, length) : text;
					savePng(MultiFormatWriter(BarcodeFormat::CODE_93).encode(input, 100, 100), BarcodeFormat::CODE_93);
				}
				break;
				case 3:
				{
					int length = 0;
					auto input = length > 0 ? text.substr(0, length) : text;
					savePng(MultiFormatWriter(BarcodeFormat::CODE_128).encode(input, 100, 100), BarcodeFormat::CODE_128);
				}
				break;
				case 4:
				{
					int length = 7;
					auto input = length > 0 ? text.substr(0, length) : text;
					savePng(MultiFormatWriter(BarcodeFormat::EAN_8).encode(input, 100, 100), BarcodeFormat::EAN_8);
				}
				break;
				case 5:
				{
					int length = 12;
					auto input = length > 0 ? text.substr(0, length) : text;
					savePng(MultiFormatWriter(BarcodeFormat::EAN_13).encode(input, 100, 100), BarcodeFormat::EAN_13);
				}
				break;
				case 6:
				{
					int length = 0;
					auto input = length > 0 ? text.substr(0, length) : text;
					savePng(MultiFormatWriter(BarcodeFormat::ITF).encode(input, 100, 100), BarcodeFormat::ITF);
				}
				break;
				case 7:
				{
					int length = 11;
					auto input = length > 0 ? text.substr(0, length) : text;
					savePng(MultiFormatWriter(BarcodeFormat::UPC_A).encode(input, 100, 100), BarcodeFormat::UPC_A);
				}
				break;
				case 8:
				{
					int length = 7;
					auto input = length > 0 ? text.substr(0, length) : text;
					savePng(MultiFormatWriter(BarcodeFormat::UPC_E).encode(input, 100, 100), BarcodeFormat::UPC_E);
				}
				break;
				default:
					break;
				}
			}
		}

		int nOldPix = m_nPix2CM;
		m_nPix2CM = nPix;

		int nChildCount = GetChildrenCount();
		for (int i = 0; i < nChildCount; i++)
		{
			SWindow* objChild = GetChild(i + 1);
			SStringW sstrClassName = objChild->GetObjectClass();
			if (sstrClassName == L"rulerrect")
			{
				CRulerRectangle* pObj = sobj_cast<CRulerRectangle>(objChild);
				if (pObj)
				{
					int nLeft = pObj->GetLeft();
					int nTop = pObj->GetTop();
					int nRight = pObj->GetRight();
					int nBotton = pObj->GetBottom();
					int nWidth = pObj->GetWidth();
					int nHeight = pObj->GetHeight();

					double dLeft = (double)nLeft / nOldPix;
					double dTop = (double)nTop / nOldPix;
					double dRight = (double)nRight / nOldPix;
					double dBottom = (double)nBotton / nOldPix;
					double dWidth = (double)nWidth / nOldPix;
					double dHeight = (double)nHeight / nOldPix;

					//新像素位置
					int nNewLeft = (int)(dLeft * m_nPix2CM);
					int nNewTop = (int)(dTop * m_nPix2CM);
					int nNewRight = (int)(dRight * m_nPix2CM);
					int nNewBottom = (int)(dBottom * m_nPix2CM);
					int nNewWidth = (int)(dWidth * m_nPix2CM);
					int nNewHeight = (int)(dHeight * m_nPix2CM);
					pObj->SetLeft(nNewLeft);
					pObj->SetTop(nNewTop);
					pObj->SetRight(nNewRight);
					pObj->SetBottom(nNewBottom);
					pObj->SetWidth(nNewWidth);
					pObj->SetHeight(nNewHeight);

					SStringW sstrPos;
					sstrPos.Format(L"%d,%d,@%d,@%d", nNewLeft, nNewTop, nNewWidth, nNewHeight);
					pObj->SetAttribute(L"pos", sstrPos);

					pObj->Invalidate();
				}
			}
			else if (sstrClassName == L"rulerellipse")
			{
				CRulerEllipse* pObj = sobj_cast<CRulerEllipse>(objChild);
				if (pObj)
				{
					int nLeft = pObj->GetLeft();
					int nTop = pObj->GetTop();
					int nRight = pObj->GetRight();
					int nBotton = pObj->GetBottom();
					int nWidth = pObj->GetWidth();
					int nHeight = pObj->GetHeight();

					double dLeft = (double)nLeft / nOldPix;
					double dTop = (double)nTop / nOldPix;
					double dRight = (double)nRight / nOldPix;
					double dBottom = (double)nBotton / nOldPix;
					double dWidth = (double)nWidth / nOldPix;
					double dHeight = (double)nHeight / nOldPix;

					//新像素位置
					int nNewLeft = (int)(dLeft * m_nPix2CM);
					int nNewTop = (int)(dTop * m_nPix2CM);
					int nNewRight = (int)(dRight * m_nPix2CM);
					int nNewBottom = (int)(dBottom * m_nPix2CM);
					int nNewWidth = (int)(dWidth * m_nPix2CM);
					int nNewHeight = (int)(dHeight * m_nPix2CM);
					pObj->SetLeft(nNewLeft);
					pObj->SetTop(nNewTop);
					pObj->SetRight(nNewRight);
					pObj->SetBottom(nNewBottom);
					pObj->SetWidth(nNewWidth);
					pObj->SetHeight(nNewHeight);

					SStringW sstrPos;
					sstrPos.Format(L"%d,%d,@%d,@%d", nNewLeft, nNewTop, nNewWidth, nNewHeight);
					pObj->SetAttribute(L"pos", sstrPos);

					pObj->Invalidate();
				}
			}
			else if (sstrClassName == L"rulerbarcode")
			{
				CRulerBarCode* pObj = sobj_cast<CRulerBarCode>(objChild);
				if (pObj)
				{
					int nLeft = pObj->GetLeft();
					int nTop = pObj->GetTop();
					int nRight = pObj->GetRight();
					int nBotton = pObj->GetBottom();
					int nWidth = pObj->GetWidth();
					int nHeight = pObj->GetHeight();

					double dLeft = (double)nLeft / nOldPix;
					double dTop = (double)nTop / nOldPix;
					double dRight = (double)nRight / nOldPix;
					double dBottom = (double)nBotton / nOldPix;
					double dWidth = (double)nWidth / nOldPix;
					double dHeight = (double)nHeight / nOldPix;

					//新像素位置
					int nNewLeft = (int)(dLeft * m_nPix2CM);
					int nNewTop = (int)(dTop * m_nPix2CM);
					int nNewRight = (int)(dRight * m_nPix2CM);
					int nNewBottom = (int)(dBottom * m_nPix2CM);
					int nNewWidth = (int)(dWidth * m_nPix2CM);
					int nNewHeight = (int)(dHeight * m_nPix2CM);
					pObj->SetLeft(nNewLeft);
					pObj->SetTop(nNewTop);
					pObj->SetRight(nNewRight);
					pObj->SetBottom(nNewBottom);
					pObj->SetWidth(nNewWidth);
					pObj->SetHeight(nNewHeight);

					SStringW sstrPos;
					sstrPos.Format(L"%d,%d,@%d,@%d", nNewLeft, nNewTop, nNewWidth, nNewHeight);
					pObj->SetAttribute(L"pos", sstrPos);

					pObj->Invalidate();
				}
			}
			else if (sstrClassName == L"rulerdrugcode")
			{
				CRulerDrugCode* pObj = sobj_cast<CRulerDrugCode>(objChild);
				if (pObj)
				{
					int nLeft = pObj->GetLeft();
					int nTop = pObj->GetTop();
					int nRight = pObj->GetRight();
					int nBotton = pObj->GetBottom();
					int nWidth = pObj->GetWidth();
					int nHeight = pObj->GetHeight();

					double dLeft = (double)nLeft / nOldPix;
					double dTop = (double)nTop / nOldPix;
					double dRight = (double)nRight / nOldPix;
					double dBottom = (double)nBotton / nOldPix;
					double dWidth = (double)nWidth / nOldPix;
					double dHeight = (double)nHeight / nOldPix;

					//新像素位置
					int nNewLeft = (int)(dLeft * m_nPix2CM);
					int nNewTop = (int)(dTop * m_nPix2CM);
					int nNewRight = (int)(dRight * m_nPix2CM);
					int nNewBottom = (int)(dBottom * m_nPix2CM);
					int nNewWidth = (int)(dWidth * m_nPix2CM);
					int nNewHeight = (int)(dHeight * m_nPix2CM);
					pObj->SetLeft(nNewLeft);
					pObj->SetTop(nNewTop);
					pObj->SetRight(nNewRight);
					pObj->SetBottom(nNewBottom);
					pObj->SetWidth(nNewWidth);
					pObj->SetHeight(nNewHeight);

					SStringW sstrPos;
					sstrPos.Format(L"%d,%d,@%d,@%d", nNewLeft, nNewTop, nNewWidth, nNewHeight);
					pObj->SetAttribute(L"pos", sstrPos);

					pObj->Invalidate();
				}
			}
			else if (sstrClassName == L"rulerqrcode")
			{
				CRulerQRCode* pObj = sobj_cast<CRulerQRCode>(objChild);
				if (pObj)
				{
					int nLeft = pObj->GetLeft();
					int nTop = pObj->GetTop();
					int nRight = pObj->GetRight();
					int nBotton = pObj->GetBottom();
					int nWidth = pObj->GetWidth();
					int nHeight = pObj->GetHeight();

					double dLeft = (double)nLeft / nOldPix;
					double dTop = (double)nTop / nOldPix;
					double dRight = (double)nRight / nOldPix;
					double dBottom = (double)nBotton / nOldPix;
					double dWidth = (double)nWidth / nOldPix;
					double dHeight = (double)nHeight / nOldPix;

					//新像素位置
					int nNewLeft = (int)(dLeft * m_nPix2CM);
					int nNewTop = (int)(dTop * m_nPix2CM);
					int nNewRight = (int)(dRight * m_nPix2CM);
					int nNewBottom = (int)(dBottom * m_nPix2CM);
					int nNewWidth = (int)(dWidth * m_nPix2CM);
					int nNewHeight = (int)(dHeight * m_nPix2CM);
					pObj->SetLeft(nNewLeft);
					pObj->SetTop(nNewTop);
					pObj->SetRight(nNewRight);
					pObj->SetBottom(nNewBottom);
					pObj->SetWidth(nNewWidth);
					pObj->SetHeight(nNewHeight);

					SStringW sstrPos;
					sstrPos.Format(L"%d,%d,@%d,@%d", nNewLeft, nNewTop, nNewWidth, nNewHeight);
					pObj->SetAttribute(L"pos", sstrPos);

					pObj->Invalidate();
				}
			}
			else if (sstrClassName == L"rulertext")
			{
				CRulerText* pObj = sobj_cast<CRulerText>(objChild);
				if (pObj)
				{
					int nLeft = pObj->GetLeft();
					int nTop = pObj->GetTop();
					int nRight = pObj->GetRight();
					int nBotton = pObj->GetBottom();
					int nWidth = pObj->GetWidth();
					int nHeight = pObj->GetHeight();

					double dLeft = (double)nLeft / nOldPix;
					double dTop = (double)nTop / nOldPix;
					double dRight = (double)nRight / nOldPix;
					double dBottom = (double)nBotton / nOldPix;
					double dWidth = (double)nWidth / nOldPix;
					double dHeight = (double)nHeight / nOldPix;

					//新像素位置
					int nNewLeft = (int)(dLeft * m_nPix2CM);
					int nNewTop = (int)(dTop * m_nPix2CM);
					int nNewRight = (int)(dRight * m_nPix2CM);
					int nNewBottom = (int)(dBottom * m_nPix2CM);
					int nNewWidth = (int)(dWidth * m_nPix2CM);
					int nNewHeight = (int)(dHeight * m_nPix2CM);
					pObj->SetLeft(nNewLeft);
					pObj->SetTop(nNewTop);
					pObj->SetRight(nNewRight);
					pObj->SetBottom(nNewBottom);
					pObj->SetWidth(nNewWidth);
					pObj->SetHeight(nNewHeight);

					SStringW sstrPos;
					sstrPos.Format(L"%d,%d,@%d,@%d", nNewLeft, nNewTop, nNewWidth, nNewHeight);
					pObj->SetAttribute(L"pos", sstrPos);

					pObj->Invalidate();
				}
			}
			else if (sstrClassName == L"rulerpicture")
			{
				CRulerPicture* pObj = sobj_cast<CRulerPicture>(objChild);
				if (pObj)
				{
					int nLeft = pObj->GetLeft();
					int nTop = pObj->GetTop();
					int nRight = pObj->GetRight();
					int nBotton = pObj->GetBottom();
					int nWidth = pObj->GetWidth();
					int nHeight = pObj->GetHeight();

					double dLeft = (double)nLeft / nOldPix;
					double dTop = (double)nTop / nOldPix;
					double dRight = (double)nRight / nOldPix;
					double dBottom = (double)nBotton / nOldPix;
					double dWidth = (double)nWidth / nOldPix;
					double dHeight = (double)nHeight / nOldPix;

					//新像素位置
					int nNewLeft = (int)(dLeft * m_nPix2CM);
					int nNewTop = (int)(dTop * m_nPix2CM);
					int nNewRight = (int)(dRight * m_nPix2CM);
					int nNewBottom = (int)(dBottom * m_nPix2CM);
					int nNewWidth = (int)(dWidth * m_nPix2CM);
					int nNewHeight = (int)(dHeight * m_nPix2CM);
					pObj->SetLeft(nNewLeft);
					pObj->SetTop(nNewTop);
					pObj->SetRight(nNewRight);
					pObj->SetBottom(nNewBottom);
					pObj->SetWidth(nNewWidth);
					pObj->SetHeight(nNewHeight);

					SStringW sstrPos;
					sstrPos.Format(L"%d,%d,@%d,@%d", nNewLeft, nNewTop, nNewWidth, nNewHeight);
					pObj->SetAttribute(L"pos", sstrPos);

					pObj->Invalidate();
				}
			}
			else if (sstrClassName == L"rulerpdf")
			{
				CRulerPDF* pObj = sobj_cast<CRulerPDF>(objChild);
				if (pObj)
				{
					int nLeft = pObj->GetLeft();
					int nTop = pObj->GetTop();
					int nRight = pObj->GetRight();
					int nBotton = pObj->GetBottom();
					int nWidth = pObj->GetWidth();
					int nHeight = pObj->GetHeight();

					double dLeft = (double)nLeft / nOldPix;
					double dTop = (double)nTop / nOldPix;
					double dRight = (double)nRight / nOldPix;
					double dBottom = (double)nBotton / nOldPix;
					double dWidth = (double)nWidth / nOldPix;
					double dHeight = (double)nHeight / nOldPix;

					//新像素位置
					int nNewLeft = (int)(dLeft * m_nPix2CM);
					int nNewTop = (int)(dTop * m_nPix2CM);
					int nNewRight = (int)(dRight * m_nPix2CM);
					int nNewBottom = (int)(dBottom * m_nPix2CM);
					int nNewWidth = (int)(dWidth * m_nPix2CM);
					int nNewHeight = (int)(dHeight * m_nPix2CM);
					pObj->SetLeft(nNewLeft);
					pObj->SetTop(nNewTop);
					pObj->SetRight(nNewRight);
					pObj->SetBottom(nNewBottom);
					pObj->SetWidth(nNewWidth);
					pObj->SetHeight(nNewHeight);

					SStringW sstrPos;
					sstrPos.Format(L"%d,%d,@%d,@%d", nNewLeft, nNewTop, nNewWidth, nNewHeight);
					pObj->SetAttribute(L"pos", sstrPos);

					pObj->Invalidate();
				}
			}
		}
	}

	void CRulerBox::SetInsertType(int nType)
	{
		m_nAddObjectType = nType;
	}


	void CRulerBox::SetBmpResource(CJPBitmap* pBmp)
	{
		m_pBitmap = pBmp;

		m_vecBitmap.push_back(pBmp);
		Invalidate();
	}

	void CRulerBox::SetDrawCross(bool bDraw)
	{
		m_bDrawRulerCross = bDraw;
	}

	void CRulerBox::OnPaint(IRenderTarget* pRT)
	{
		CRect rcClient = GetClientRect();
		pRT->SetAntiAlias(FALSE);

		HDC hDC = pRT->GetDC();
		Graphics graphics(hDC);
		graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);

		if (m_bDrawRulerCross)
		{
			//draw cross lines
			CAutoRefPtr<IPen> pen, oldpen;
			pRT->CreatePen(PS_DASHDOT, RGBA(0, 0, 0, 60), 1, &pen);
			pRT->SelectObject(pen, (IRenderObj**)&oldpen);
			
			//draw h lines
			for (int i = 1; i < 100; i++)
			{
				CPoint pts[2];
				pts[0].x = rcClient.left;
				pts[0].y = rcClient.top + i * m_nPix2CM;

				pts[1].x = rcClient.right;
				pts[1].y = rcClient.top + i * m_nPix2CM;
				pRT->DrawLines(pts, 2);
			}

			//draw v lines
			for (int i = 1; i < 100; i++)
			{
				CPoint pts[2];
				pts[0].x = rcClient.left + i * m_nPix2CM;
				pts[0].y = rcClient.top;

				pts[1].x = rcClient.left + i * m_nPix2CM;
				pts[1].y = rcClient.bottom;
				pRT->DrawLines(pts, 2);
			}
			pRT->SelectObject(oldpen, NULL);
		}

		//绘制选择区域
		if (!m_rcDrawArea.IsRectEmpty())
		{
			SOUI::CRect rcTmp(m_rcDrawArea);
			//rcTmp.InflateRect(1, 1);
			RectF rcBorder;
			rcBorder.X = rcTmp.left;
			rcBorder.Y = rcTmp.top;
			rcBorder.Width = rcTmp.Width();
			rcBorder.Height = rcTmp.Height();

			COLORREF crDrawAreaBorder = RGBA(22, 119, 189, 255);;
			Gdiplus::Color color(GetRValue(crDrawAreaBorder), GetGValue(crDrawAreaBorder), GetBValue(crDrawAreaBorder));
			Pen hPen(color);
			hPen.SetDashStyle(Gdiplus::DashStyle::DashStyleDot);
			hPen.SetWidth(1);
			graphics.DrawRectangle(&hPen, rcBorder);
		}

		pRT->ReleaseDC(hDC);
	}

	void CRulerBox::OnLButtonDown(UINT nFlags, SOUI::CPoint point)
	{
		SetCapture();
		//记录鼠标按下的点
		m_ptDown = point;
		m_bSelectObject = false;

		CRect rcClient;
		this->GetClientRect(&rcClient);
		SOUI::CPoint pt;
		pt.x = point.x - rcClient.left;
		pt.y = point.y - rcClient.top;
		m_ptObjLT = pt;

		if (m_nAddObjectType == RulerTypeNull)  //选中了绘制
		{
			m_vecSelectObjects.clear();
			SWindow* pSelectedObj = NULL;

			//获取当前页面有多少子控件
			int nChildCount = GetChildrenCount();
			for (int i = 0; i < nChildCount; i++)
			{
				SWindow* objChild = GetChild(i + 1);
				CRect rcObj = objChild->GetClientRect();
				if (PtInRect(rcObj, point))
				{
					pSelectedObj = objChild;  //被点击选中的子对象
					//break;
				}
			}

			//点击选中每次只能选中一个，故而需要将选中的绘制border，未选中的不绘制border
			//如果点击在了拉伸的8个点上则记录操作类型
			for (int i = 0; i < nChildCount; i++)
			{
				SWindow* objChild = GetChild(i + 1);
				SStringW sstrClassName = objChild->GetObjectClass();
				if (objChild == pSelectedObj)
				{
					if (sstrClassName == L"rulerrect")
					{
						CRulerRectangle* pRectangle = sobj_cast<CRulerRectangle>(objChild);
						if (pRectangle)
						{
							pRectangle->SetDrawBorder(true);
							m_bSelectObject = true;
							m_vecSelectObjects.push_back(objChild);

							m_curEcPosType = pRectangle->HitPos(point);
							ShowCursor(m_curEcPosType);
						}
					}
					else if (sstrClassName == L"rulerellipse")
					{
						CRulerEllipse* pEllipse = sobj_cast<CRulerEllipse>(objChild);
						if (pEllipse)
						{
							pEllipse->SetDrawBorder(true);
							m_bSelectObject = true;
							m_vecSelectObjects.push_back(objChild);
							m_curEcPosType = pEllipse->HitPos(point);
							ShowCursor(m_curEcPosType);
						}
					}
					else if (sstrClassName == L"rulerbarcode")
					{
						CRulerBarCode* pBarCode = sobj_cast<CRulerBarCode>(objChild);
						if (pBarCode)
						{
							pBarCode->SetDrawBorder(true);
							m_bSelectObject = true;
							m_vecSelectObjects.push_back(objChild);
							m_curEcPosType = pBarCode->HitPos(point);
							ShowCursor(m_curEcPosType);
						}
					}
					else if (sstrClassName == L"rulerdrugcode")
					{
						CRulerDrugCode* pDrugCode = sobj_cast<CRulerDrugCode>(objChild);
						if (pDrugCode)
						{
							pDrugCode->SetDrawBorder(true);
							m_bSelectObject = true;
							m_vecSelectObjects.push_back(objChild);
							m_curEcPosType = pDrugCode->HitPos(point);
							ShowCursor(m_curEcPosType);
						}
					}
					else if (sstrClassName == L"rulerqrcode")
					{
						CRulerQRCode* pQRCode = sobj_cast<CRulerQRCode>(objChild);
						if (pQRCode)
						{
							pQRCode->SetDrawBorder(true);
							m_bSelectObject = true;
							m_vecSelectObjects.push_back(objChild);
							m_curEcPosType = pQRCode->HitPos(point);
							ShowCursor(m_curEcPosType);
						}
					}
					else if (sstrClassName == L"rulertext")
					{
						CRulerText* pText = sobj_cast<CRulerText>(objChild);
						if (pText)
						{
							pText->SetDrawBorder(true);
							m_bSelectObject = true;
							m_vecSelectObjects.push_back(objChild);
							m_curEcPosType = pText->HitPos(point);
							ShowCursor(m_curEcPosType);
						}
					}
					else if (sstrClassName == L"rulerpicture")
					{
						CRulerPicture* pPicture = sobj_cast<CRulerPicture>(objChild);
						if (pPicture)
						{
							pPicture->SetDrawBorder(true);
							m_bSelectObject = true;
							m_vecSelectObjects.push_back(objChild);
							m_curEcPosType = pPicture->HitPos(point);
							ShowCursor(m_curEcPosType);
						}
					}
					else if (sstrClassName == L"rulerpdf")
					{
						CRulerPDF* pPDF = sobj_cast<CRulerPDF>(objChild);
						if (pPDF)
						{
							pPDF->SetDrawBorder(true);
							m_bSelectObject = true;
							m_vecSelectObjects.push_back(objChild);
							m_curEcPosType = pPDF->HitPos(point);
							ShowCursor(m_curEcPosType);
						}
					}
				}
				else
				{
					if (sstrClassName == L"rulerrect")
					{
						CRulerRectangle* pRectangle = sobj_cast<CRulerRectangle>(objChild);
						if (pRectangle)
						{
							pRectangle->SetDrawBorder(false);
						}
					}
					else if (sstrClassName == L"rulerellipse")
					{
						CRulerEllipse* pEllipse = sobj_cast<CRulerEllipse>(objChild);
						if (pEllipse)
						{
							pEllipse->SetDrawBorder(false);
						}
					}
					else if (sstrClassName == L"rulerbarcode")
					{
						CRulerBarCode* pBarCode = sobj_cast<CRulerBarCode>(objChild);
						if (pBarCode)
						{
							pBarCode->SetDrawBorder(false);
						}
					}
					else if (sstrClassName == L"rulerdrugcode")
					{
						CRulerDrugCode* pDrugCode = sobj_cast<CRulerDrugCode>(objChild);
						if (pDrugCode)
						{
							pDrugCode->SetDrawBorder(false);
						}
					}
					else if (sstrClassName == L"rulerqrcode")
					{
						CRulerQRCode* pQRCode = sobj_cast<CRulerQRCode>(objChild);
						if (pQRCode)
						{
							pQRCode->SetDrawBorder(false);
						}
					}
					else if (sstrClassName == L"rulertext")
					{
						CRulerText* pText = sobj_cast<CRulerText>(objChild);
						if (pText)
						{
							pText->SetDrawBorder(false);
						}
					}
					else if (sstrClassName == L"rulerpicture")
					{
						CRulerPicture* pPicture = sobj_cast<CRulerPicture>(objChild);
						if (pPicture)
						{
							pPicture->SetDrawBorder(false);
						}
					}
					else if (sstrClassName == L"rulerpdf")
					{
						CRulerPDF* pPDF = sobj_cast<CRulerPDF>(objChild);
						if (pPDF)
						{
							pPDF->SetDrawBorder(false);
						}
					}
				}
			}
		}
	}

	void CRulerBox::OnLButtonUp(UINT nFlags, SOUI::CPoint point)
	{
		ReleaseCapture();
		//需要根据point计算移动完的控件的left跟top  然后重新设置控件的left跟top
		if (m_bSelectObject)
		{
			if (m_curEcPosType == EcPosType::Null)  //move
			{
				//point跟m_ptDown两点X、Y的差值；用差值跟控件的位置做加减处理然后做移动操作
				for (int i = 0; i < m_vecSelectObjects.size(); i++)
				{
					SStringW sstrClassName = m_vecSelectObjects[i]->GetObjectClass();
					if (sstrClassName == L"rulerrect")
					{
						CRulerRectangle* pObj = sobj_cast<CRulerRectangle>(m_vecSelectObjects[i]);
						if (pObj)
						{
							int nLeft = pObj->GetLeft();
							int nTop = pObj->GetTop();
							int nWidth = pObj->GetWidth();
							int nHeight = pObj->GetHeight();

							if (m_ptDown.x > point.x)
								nLeft -= (m_ptDown.x - point.x);
							else
								nLeft += (point.x - m_ptDown.x);
							if (m_ptDown.y > point.y)
								nTop -= (m_ptDown.y - point.y);
							else
								nTop += (point.y - m_ptDown.y);

							SStringW sstrPos;
							sstrPos.Format(L"%d,%d", nLeft, nTop);
							pObj->SetAttribute(L"pos", sstrPos);

							int nRight = nLeft + nWidth;
							int nBottom = nTop + nHeight;
							pObj->SetLeft(nLeft);
							pObj->SetTop(nTop);
							pObj->SetRight(nRight);
							pObj->SetBottom(nBottom);
							pObj->CalcPos();
						}
					}
					else if (sstrClassName == L"rulerellipse")
					{
						CRulerEllipse* pObj = sobj_cast<CRulerEllipse>(m_vecSelectObjects[i]);
						if (pObj)
						{
							int nLeft = pObj->GetLeft();
							int nTop = pObj->GetTop();
							int nWidth = pObj->GetWidth();
							int nHeight = pObj->GetHeight();

							if (m_ptDown.x > point.x)
								nLeft -= (m_ptDown.x - point.x);
							else
								nLeft += (point.x - m_ptDown.x);
							if (m_ptDown.y > point.y)
								nTop -= (m_ptDown.y - point.y);
							else
								nTop += (point.y - m_ptDown.y);

							SStringW sstrPos;
							sstrPos.Format(L"%d,%d", nLeft, nTop);
							pObj->SetAttribute(L"pos", sstrPos);

							int nRight = nLeft + nWidth;
							int nBottom = nTop + nHeight;
							pObj->SetLeft(nLeft);
							pObj->SetTop(nTop);
							pObj->SetRight(nRight);
							pObj->SetBottom(nBottom);
							pObj->CalcPos();
						}
					}
					else if (sstrClassName == L"rulerbarcode")
					{
						CRulerBarCode* pObj = sobj_cast<CRulerBarCode>(m_vecSelectObjects[i]);
						if (pObj)
						{
							int nLeft = pObj->GetLeft();
							int nTop = pObj->GetTop();
							int nWidth = pObj->GetWidth();
							int nHeight = pObj->GetHeight();

							if (m_ptDown.x > point.x)
								nLeft -= (m_ptDown.x - point.x);
							else
								nLeft += (point.x - m_ptDown.x);
							if (m_ptDown.y > point.y)
								nTop -= (m_ptDown.y - point.y);
							else
								nTop += (point.y - m_ptDown.y);

							SStringW sstrPos;
							sstrPos.Format(L"%d,%d", nLeft, nTop);
							pObj->SetAttribute(L"pos", sstrPos);

							int nRight = nLeft + nWidth;
							int nBottom = nTop + nHeight;
							pObj->SetLeft(nLeft);
							pObj->SetTop(nTop);
							pObj->SetRight(nRight);
							pObj->SetBottom(nBottom);
							pObj->CalcPos();
						}
					}
					else if (sstrClassName == L"rulerdrugcode")
					{
						CRulerDrugCode* pObj = sobj_cast<CRulerDrugCode>(m_vecSelectObjects[i]);
						if (pObj)
						{
							int nLeft = pObj->GetLeft();
							int nTop = pObj->GetTop();
							int nWidth = pObj->GetWidth();
							int nHeight = pObj->GetHeight();

							if (m_ptDown.x > point.x)
								nLeft -= (m_ptDown.x - point.x);
							else
								nLeft += (point.x - m_ptDown.x);
							if (m_ptDown.y > point.y)
								nTop -= (m_ptDown.y - point.y);
							else
								nTop += (point.y - m_ptDown.y);

							SStringW sstrPos;
							sstrPos.Format(L"%d,%d", nLeft, nTop);
							pObj->SetAttribute(L"pos", sstrPos);

							int nRight = nLeft + nWidth;
							int nBottom = nTop + nHeight;
							pObj->SetLeft(nLeft);
							pObj->SetTop(nTop);
							pObj->SetRight(nRight);
							pObj->SetBottom(nBottom);
							pObj->CalcPos();
						}
					}
					else if (sstrClassName == L"rulerqrcode")
					{
						CRulerQRCode* pObj = sobj_cast<CRulerQRCode>(m_vecSelectObjects[i]);
						if (pObj)
						{
							int nLeft = pObj->GetLeft();
							int nTop = pObj->GetTop();
							int nWidth = pObj->GetWidth();
							int nHeight = pObj->GetHeight();

							if (m_ptDown.x > point.x)
								nLeft -= (m_ptDown.x - point.x);
							else
								nLeft += (point.x - m_ptDown.x);
							if (m_ptDown.y > point.y)
								nTop -= (m_ptDown.y - point.y);
							else
								nTop += (point.y - m_ptDown.y);

							SStringW sstrPos;
							sstrPos.Format(L"%d,%d", nLeft, nTop);
							pObj->SetAttribute(L"pos", sstrPos);

							int nRight = nLeft + nWidth;
							int nBottom = nTop + nHeight;
							pObj->SetLeft(nLeft);
							pObj->SetTop(nTop);
							pObj->SetRight(nRight);
							pObj->SetBottom(nBottom);
							pObj->CalcPos();
						}
					}
					else if (sstrClassName == L"rulertext")
					{
						CRulerText* pObj = sobj_cast<CRulerText>(m_vecSelectObjects[i]);
						if (pObj)
						{
							int nLeft = pObj->GetLeft();
							int nTop = pObj->GetTop();
							int nWidth = pObj->GetWidth();
							int nHeight = pObj->GetHeight();

							if (m_ptDown.x > point.x)
								nLeft -= (m_ptDown.x - point.x);
							else
								nLeft += (point.x - m_ptDown.x);
							if (m_ptDown.y > point.y)
								nTop -= (m_ptDown.y - point.y);
							else
								nTop += (point.y - m_ptDown.y);

							SStringW sstrPos;
							sstrPos.Format(L"%d,%d", nLeft, nTop);
							pObj->SetAttribute(L"pos", sstrPos);

							int nRight = nLeft + nWidth;
							int nBottom = nTop + nHeight;
							pObj->SetLeft(nLeft);
							pObj->SetTop(nTop);
							pObj->SetRight(nRight);
							pObj->SetBottom(nBottom);
							pObj->CalcPos();
						}
					}
					else if (sstrClassName == L"rulerpicture")
					{
						CRulerPicture* pObj = sobj_cast<CRulerPicture>(m_vecSelectObjects[i]);
						if (pObj)
						{
							int nLeft = pObj->GetLeft();
							int nTop = pObj->GetTop();
							int nWidth = pObj->GetWidth();
							int nHeight = pObj->GetHeight();

							if (m_ptDown.x > point.x)
								nLeft -= (m_ptDown.x - point.x);
							else
								nLeft += (point.x - m_ptDown.x);
							if (m_ptDown.y > point.y)
								nTop -= (m_ptDown.y - point.y);
							else
								nTop += (point.y - m_ptDown.y);

							SStringW sstrPos;
							sstrPos.Format(L"%d,%d", nLeft, nTop);
							pObj->SetAttribute(L"pos", sstrPos);

							int nRight = nLeft + nWidth;
							int nBottom = nTop + nHeight;
							pObj->SetLeft(nLeft);
							pObj->SetTop(nTop);
							pObj->SetRight(nRight);
							pObj->SetBottom(nBottom);
							pObj->CalcPos();
						}
					}
					else if (sstrClassName == L"rulerpdf")
					{
						CRulerPDF* pObj = sobj_cast<CRulerPDF>(m_vecSelectObjects[i]);
						if (pObj)
						{
							int nLeft = pObj->GetLeft();
							int nTop = pObj->GetTop();
							int nWidth = pObj->GetWidth();
							int nHeight = pObj->GetHeight();

							if (m_ptDown.x > point.x)
								nLeft -= (m_ptDown.x - point.x);
							else
								nLeft += (point.x - m_ptDown.x);
							if (m_ptDown.y > point.y)
								nTop -= (m_ptDown.y - point.y);
							else
								nTop += (point.y - m_ptDown.y);

							SStringW sstrPos;
							sstrPos.Format(L"%d,%d", nLeft, nTop);
							pObj->SetAttribute(L"pos", sstrPos);

							int nRight = nLeft + nWidth;
							int nBottom = nTop + nHeight;
							pObj->SetLeft(nLeft);
							pObj->SetTop(nTop);
							pObj->SetRight(nRight);
							pObj->SetBottom(nBottom);
							pObj->CalcPos();
						}
					}
				}
			}
			else
			{
				//根据八个点做操作
				if (m_vecSelectObjects.size() == 1)
				{
					switch (m_curEcPosType)
					{
					case TopLeft:
					{
						SStringW sstrClassName = m_vecSelectObjects[0]->GetObjectClass();
						if (sstrClassName == L"rulerrect")
						{
							CRulerRectangle* pObj = sobj_cast<CRulerRectangle>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();
								if (m_ptDown.x > point.x)  //x往左
								{
									nLeft -= (m_ptDown.x - point.x);
								}
								else//x往右
								{
									if ((point.x - m_ptDown.x) - 10 >= (nRight - nLeft))
										nLeft = (point.x - m_ptDown.x) - 10;
									else
										nLeft += (point.x - m_ptDown.x);
								}
								if (m_ptDown.y > point.y)//y往上
								{
									nTop -= (m_ptDown.y - point.y);
								}
								else //y往下
								{
									if ((point.y - m_ptDown.y) - 10 >= (nBottom - nTop))
										nTop = (point.y - m_ptDown.y) - 10;
									else
										nTop += (point.y - m_ptDown.y);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetLeft(nLeft);
								pObj->SetTop(nTop);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulerellipse")
						{
							CRulerEllipse* pObj = sobj_cast<CRulerEllipse>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();
								if (m_ptDown.x > point.x)  //x往左
								{
									nLeft -= (m_ptDown.x - point.x);
								}
								else//x往右
								{
									if ((point.x - m_ptDown.x) - 10 >= (nRight - nLeft))
										nLeft = (point.x - m_ptDown.x) - 10;
									else
										nLeft += (point.x - m_ptDown.x);
								}
								if (m_ptDown.y > point.y)//y往上
								{
									nTop -= (m_ptDown.y - point.y);
								}
								else //y往下
								{
									if ((point.y - m_ptDown.y) - 10 >= (nBottom - nTop))
										nTop = (point.y - m_ptDown.y) - 10;
									else
										nTop += (point.y - m_ptDown.y);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetLeft(nLeft);
								pObj->SetTop(nTop);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulerbarcode")
						{
							CRulerBarCode* pObj = sobj_cast<CRulerBarCode>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();
								if (m_ptDown.x > point.x)  //x往左
								{
									nLeft -= (m_ptDown.x - point.x);
								}
								else//x往右
								{
									if ((point.x - m_ptDown.x) - 10 >= (nRight - nLeft))
										nLeft = (point.x - m_ptDown.x) - 10;
									else
										nLeft += (point.x - m_ptDown.x);
								}
								if (m_ptDown.y > point.y)//y往上
								{
									nTop -= (m_ptDown.y - point.y);
								}
								else //y往下
								{
									if ((point.y - m_ptDown.y) - 10 >= (nBottom - nTop))
										nTop = (point.y - m_ptDown.y) - 10;
									else
										nTop += (point.y - m_ptDown.y);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetLeft(nLeft);
								pObj->SetTop(nTop);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulerdrugcode")
						{
							CRulerDrugCode* pObj = sobj_cast<CRulerDrugCode>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();
								if (m_ptDown.x > point.x)  //x往左
								{
									nLeft -= (m_ptDown.x - point.x);
								}
								else//x往右
								{
									if ((point.x - m_ptDown.x) - 10 >= (nRight - nLeft))
										nLeft = (point.x - m_ptDown.x) - 10;
									else
										nLeft += (point.x - m_ptDown.x);
								}
								if (m_ptDown.y > point.y)//y往上
								{
									nTop -= (m_ptDown.y - point.y);
								}
								else //y往下
								{
									if ((point.y - m_ptDown.y) - 10 >= (nBottom - nTop))
										nTop = (point.y - m_ptDown.y) - 10;
									else
										nTop += (point.y - m_ptDown.y);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetLeft(nLeft);
								pObj->SetTop(nTop);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulerqrcode")
						{
							CRulerQRCode* pObj = sobj_cast<CRulerQRCode>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();
								if (m_ptDown.x > point.x)  //x往左
								{
									nLeft -= (m_ptDown.x - point.x);
								}
								else//x往右
								{
									if ((point.x - m_ptDown.x) - 10 >= (nRight - nLeft))
										nLeft = (point.x - m_ptDown.x) - 10;
									else
										nLeft += (point.x - m_ptDown.x);
								}
								if (m_ptDown.y > point.y)//y往上
								{
									nTop -= (m_ptDown.y - point.y);
								}
								else //y往下
								{
									if ((point.y - m_ptDown.y) - 10 >= (nBottom - nTop))
										nTop = (point.y - m_ptDown.y) - 10;
									else
										nTop += (point.y - m_ptDown.y);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetLeft(nLeft);
								pObj->SetTop(nTop);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulertext")
						{
							CRulerText* pObj = sobj_cast<CRulerText>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();
								if (m_ptDown.x > point.x)  //x往左
								{
									nLeft -= (m_ptDown.x - point.x);
								}
								else//x往右
								{
									if ((point.x - m_ptDown.x) - 10 >= (nRight - nLeft))
										nLeft = (point.x - m_ptDown.x) - 10;
									else
										nLeft += (point.x - m_ptDown.x);
								}
								if (m_ptDown.y > point.y)//y往上
								{
									nTop -= (m_ptDown.y - point.y);
								}
								else //y往下
								{
									if ((point.y - m_ptDown.y) - 10 >= (nBottom - nTop))
										nTop = (point.y - m_ptDown.y) - 10;
									else
										nTop += (point.y - m_ptDown.y);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetLeft(nLeft);
								pObj->SetTop(nTop);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulerpicture")
						{
							CRulerPicture* pObj = sobj_cast<CRulerPicture>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();
								if (m_ptDown.x > point.x)  //x往左
								{
									nLeft -= (m_ptDown.x - point.x);
								}
								else//x往右
								{
									if ((point.x - m_ptDown.x) - 10 >= (nRight - nLeft))
										nLeft = (point.x - m_ptDown.x) - 10;
									else
										nLeft += (point.x - m_ptDown.x);
								}
								if (m_ptDown.y > point.y)//y往上
								{
									nTop -= (m_ptDown.y - point.y);
								}
								else //y往下
								{
									if ((point.y - m_ptDown.y) - 10 >= (nBottom - nTop))
										nTop = (point.y - m_ptDown.y) - 10;
									else
										nTop += (point.y - m_ptDown.y);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetLeft(nLeft);
								pObj->SetTop(nTop);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulerpdf")
						{
							CRulerPDF* pObj = sobj_cast<CRulerPDF>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();
								if (m_ptDown.x > point.x)  //x往左
								{
									nLeft -= (m_ptDown.x - point.x);
								}
								else//x往右
								{
									if ((point.x - m_ptDown.x) - 10 >= (nRight - nLeft))
										nLeft = (point.x - m_ptDown.x) - 10;
									else
										nLeft += (point.x - m_ptDown.x);
								}
								if (m_ptDown.y > point.y)//y往上
								{
									nTop -= (m_ptDown.y - point.y);
								}
								else //y往下
								{
									if ((point.y - m_ptDown.y) - 10 >= (nBottom - nTop))
										nTop = (point.y - m_ptDown.y) - 10;
									else
										nTop += (point.y - m_ptDown.y);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetLeft(nLeft);
								pObj->SetTop(nTop);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
					}
					break;
					case TopCenter:
					{
						SStringW sstrClassName = m_vecSelectObjects[0]->GetObjectClass();
						if (sstrClassName == L"rulerrect")
						{
							CRulerRectangle* pObj = sobj_cast<CRulerRectangle>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();
								if (m_ptDown.y > point.y)//y往上
								{
									nTop -= (m_ptDown.y - point.y);
								}
								else //y往下
								{
									if ((point.y - m_ptDown.y) - 10 >= (nBottom - nTop))
										nTop = (point.y - m_ptDown.y) - 10;
									else
										nTop += (point.y - m_ptDown.y);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetTop(nTop);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulerellipse")
						{
							CRulerEllipse* pObj = sobj_cast<CRulerEllipse>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();
								if (m_ptDown.y > point.y)//y往上
								{
									nTop -= (m_ptDown.y - point.y);
								}
								else //y往下
								{
									if ((point.y - m_ptDown.y) - 10 >= (nBottom - nTop))
										nTop = (point.y - m_ptDown.y) - 10;
									else
										nTop += (point.y - m_ptDown.y);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetTop(nTop);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulerbarcode")
						{
							CRulerBarCode* pObj = sobj_cast<CRulerBarCode>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();
								if (m_ptDown.y > point.y)//y往上
								{
									nTop -= (m_ptDown.y - point.y);
								}
								else //y往下
								{
									if ((point.y - m_ptDown.y) - 10 >= (nBottom - nTop))
										nTop = (point.y - m_ptDown.y) - 10;
									else
										nTop += (point.y - m_ptDown.y);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetTop(nTop);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulerdrugcode")
						{
							CRulerDrugCode* pObj = sobj_cast<CRulerDrugCode>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();
								if (m_ptDown.y > point.y)//y往上
								{
									nTop -= (m_ptDown.y - point.y);
								}
								else //y往下
								{
									if ((point.y - m_ptDown.y) - 10 >= (nBottom - nTop))
										nTop = (point.y - m_ptDown.y) - 10;
									else
										nTop += (point.y - m_ptDown.y);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetTop(nTop);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulerqrcode")
						{
							CRulerQRCode* pObj = sobj_cast<CRulerQRCode>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();
								if (m_ptDown.y > point.y)//y往上
								{
									nTop -= (m_ptDown.y - point.y);
								}
								else //y往下
								{
									if ((point.y - m_ptDown.y) - 10 >= (nBottom - nTop))
										nTop = (point.y - m_ptDown.y) - 10;
									else
										nTop += (point.y - m_ptDown.y);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetTop(nTop);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulertext")
						{
							CRulerText* pObj = sobj_cast<CRulerText>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();
								if (m_ptDown.y > point.y)//y往上
								{
									nTop -= (m_ptDown.y - point.y);
								}
								else //y往下
								{
									if ((point.y - m_ptDown.y) - 10 >= (nBottom - nTop))
										nTop = (point.y - m_ptDown.y) - 10;
									else
										nTop += (point.y - m_ptDown.y);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetTop(nTop);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulerpicture")
						{
							CRulerPicture* pObj = sobj_cast<CRulerPicture>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();
								if (m_ptDown.y > point.y)//y往上
								{
									nTop -= (m_ptDown.y - point.y);
								}
								else //y往下
								{
									if ((point.y - m_ptDown.y) - 10 >= (nBottom - nTop))
										nTop = (point.y - m_ptDown.y) - 10;
									else
										nTop += (point.y - m_ptDown.y);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetTop(nTop);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulerpdf")
						{
							CRulerPDF* pObj = sobj_cast<CRulerPDF>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();
								if (m_ptDown.y > point.y)//y往上
								{
									nTop -= (m_ptDown.y - point.y);
								}
								else //y往下
								{
									if ((point.y - m_ptDown.y) - 10 >= (nBottom - nTop))
										nTop = (point.y - m_ptDown.y) - 10;
									else
										nTop += (point.y - m_ptDown.y);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetTop(nTop);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
					}
					break;
					case TopRight:
					{
						SStringW sstrClassName = m_vecSelectObjects[0]->GetObjectClass();
						if (sstrClassName == L"rulerrect")
						{
							CRulerRectangle* pObj = sobj_cast<CRulerRectangle>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();
								if (m_ptDown.x > point.x)  //x to left
								{
									if ((m_ptDown.x - point.x) >= (nRight - nLeft) - 10)
										nRight = nLeft + 10;
									else
										nRight -= (m_ptDown.x - point.x);
								}
								else//x to right
								{
									nRight += (point.x - m_ptDown.x);
								}

								if (m_ptDown.y > point.y)//y to up
								{
									nTop -= (m_ptDown.y - point.y);
								}
								else //y to down
								{
									if ((point.y - m_ptDown.y) - 10 >= (nBottom - nTop))
										nTop = (point.y - m_ptDown.y) - 10;
									else
										nTop += (point.y - m_ptDown.y);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetRight(nRight);
								pObj->SetTop(nTop);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulerellipse")
						{
							CRulerEllipse* pObj = sobj_cast<CRulerEllipse>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();
								if (m_ptDown.x > point.x)  //x to left
								{
									if ((m_ptDown.x - point.x) >= (nRight - nLeft) - 10)
										nRight = nLeft + 10;
									else
										nRight -= (m_ptDown.x - point.x);
								}
								else//x to right
								{
									nRight += (point.x - m_ptDown.x);
								}

								if (m_ptDown.y > point.y)//y to up
								{
									nTop -= (m_ptDown.y - point.y);
								}
								else //y to down
								{
									if ((point.y - m_ptDown.y) - 10 >= (nBottom - nTop))
										nTop = (point.y - m_ptDown.y) - 10;
									else
										nTop += (point.y - m_ptDown.y);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetRight(nRight);
								pObj->SetTop(nTop);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulerbarcode")
						{
							CRulerBarCode* pObj = sobj_cast<CRulerBarCode>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();
								if (m_ptDown.x > point.x)  //x to left
								{
									if ((m_ptDown.x - point.x) >= (nRight - nLeft) - 10)
										nRight = nLeft + 10;
									else
										nRight -= (m_ptDown.x - point.x);
								}
								else//x to right
								{
									nRight += (point.x - m_ptDown.x);
								}

								if (m_ptDown.y > point.y)//y to up
								{
									nTop -= (m_ptDown.y - point.y);
								}
								else //y to down
								{
									if ((point.y - m_ptDown.y) - 10 >= (nBottom - nTop))
										nTop = (point.y - m_ptDown.y) - 10;
									else
										nTop += (point.y - m_ptDown.y);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetRight(nRight);
								pObj->SetTop(nTop);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulerdrugcode")
						{
							CRulerDrugCode* pObj = sobj_cast<CRulerDrugCode>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();
								if (m_ptDown.x > point.x)  //x to left
								{
									if ((m_ptDown.x - point.x) >= (nRight - nLeft) - 10)
										nRight = nLeft + 10;
									else
										nRight -= (m_ptDown.x - point.x);
								}
								else//x to right
								{
									nRight += (point.x - m_ptDown.x);
								}

								if (m_ptDown.y > point.y)//y to up
								{
									nTop -= (m_ptDown.y - point.y);
								}
								else //y to down
								{
									if ((point.y - m_ptDown.y) - 10 >= (nBottom - nTop))
										nTop = (point.y - m_ptDown.y) - 10;
									else
										nTop += (point.y - m_ptDown.y);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetRight(nRight);
								pObj->SetTop(nTop);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulerqrcode")
						{
							CRulerQRCode* pObj = sobj_cast<CRulerQRCode>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();
								if (m_ptDown.x > point.x)  //x to left
								{
									if ((m_ptDown.x - point.x) >= (nRight - nLeft) - 10)
										nRight = nLeft + 10;
									else
										nRight -= (m_ptDown.x - point.x);
								}
								else//x to right
								{
									nRight += (point.x - m_ptDown.x);
								}

								if (m_ptDown.y > point.y)//y to up
								{
									nTop -= (m_ptDown.y - point.y);
								}
								else //y to down
								{
									if ((point.y - m_ptDown.y) - 10 >= (nBottom - nTop))
										nTop = (point.y - m_ptDown.y) - 10;
									else
										nTop += (point.y - m_ptDown.y);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetRight(nRight);
								pObj->SetTop(nTop);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulertext")
						{
							CRulerText* pObj = sobj_cast<CRulerText>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();
								if (m_ptDown.x > point.x)  //x to left
								{
									if ((m_ptDown.x - point.x) >= (nRight - nLeft) - 10)
										nRight = nLeft + 10;
									else
										nRight -= (m_ptDown.x - point.x);
								}
								else//x to right
								{
									nRight += (point.x - m_ptDown.x);
								}

								if (m_ptDown.y > point.y)//y to up
								{
									nTop -= (m_ptDown.y - point.y);
								}
								else //y to down
								{
									if ((point.y - m_ptDown.y) - 10 >= (nBottom - nTop))
										nTop = (point.y - m_ptDown.y) - 10;
									else
										nTop += (point.y - m_ptDown.y);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetRight(nRight);
								pObj->SetTop(nTop);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulerpicture")
						{
							CRulerPicture* pObj = sobj_cast<CRulerPicture>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();
								if (m_ptDown.x > point.x)  //x to left
								{
									if ((m_ptDown.x - point.x) >= (nRight - nLeft) - 10)
										nRight = nLeft + 10;
									else
										nRight -= (m_ptDown.x - point.x);
								}
								else//x to right
								{
									nRight += (point.x - m_ptDown.x);
								}

								if (m_ptDown.y > point.y)//y to up
								{
									nTop -= (m_ptDown.y - point.y);
								}
								else //y to down
								{
									if ((point.y - m_ptDown.y) - 10 >= (nBottom - nTop))
										nTop = (point.y - m_ptDown.y) - 10;
									else
										nTop += (point.y - m_ptDown.y);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetRight(nRight);
								pObj->SetTop(nTop);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulerpdf")
						{
							CRulerPDF* pObj = sobj_cast<CRulerPDF>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();
								if (m_ptDown.x > point.x)  //x to left
								{
									if ((m_ptDown.x - point.x) >= (nRight - nLeft) - 10)
										nRight = nLeft + 10;
									else
										nRight -= (m_ptDown.x - point.x);
								}
								else//x to right
								{
									nRight += (point.x - m_ptDown.x);
								}

								if (m_ptDown.y > point.y)//y to up
								{
									nTop -= (m_ptDown.y - point.y);
								}
								else //y to down
								{
									if ((point.y - m_ptDown.y) - 10 >= (nBottom - nTop))
										nTop = (point.y - m_ptDown.y) - 10;
									else
										nTop += (point.y - m_ptDown.y);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetRight(nRight);
								pObj->SetTop(nTop);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
					}
					break;
					case RightCenter:
					{
						SStringW sstrClassName = m_vecSelectObjects[0]->GetObjectClass();
						if (sstrClassName == L"rulerrect")
						{
							CRulerRectangle* pObj = sobj_cast<CRulerRectangle>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();
								if (m_ptDown.x > point.x)  //x to left
								{
									if ((m_ptDown.x - point.x) >= (nRight - nLeft) - 10)
										nRight = nLeft + 10;
									else
										nRight -= (m_ptDown.x - point.x);
								}
								else//x to right
								{
									nRight += (point.x - m_ptDown.x);
								}
								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetRight(nRight);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulerellipse")
						{
							CRulerEllipse* pObj = sobj_cast<CRulerEllipse>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();
								if (m_ptDown.x > point.x)  //x to left
								{
									if ((m_ptDown.x - point.x) >= (nRight - nLeft) - 10)
										nRight = nLeft + 10;
									else
										nRight -= (m_ptDown.x - point.x);
								}
								else//x to right
								{
									nRight += (point.x - m_ptDown.x);
								}
								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetRight(nRight);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulerbarcode")
						{
							CRulerBarCode* pObj = sobj_cast<CRulerBarCode>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();
								if (m_ptDown.x > point.x)  //x to left
								{
									if ((m_ptDown.x - point.x) >= (nRight - nLeft) - 10)
										nRight = nLeft + 10;
									else
										nRight -= (m_ptDown.x - point.x);
								}
								else//x to right
								{
									nRight += (point.x - m_ptDown.x);
								}
								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetRight(nRight);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulerdrugcode")
						{
							CRulerDrugCode* pObj = sobj_cast<CRulerDrugCode>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();
								if (m_ptDown.x > point.x)  //x to left
								{
									if ((m_ptDown.x - point.x) >= (nRight - nLeft) - 10)
										nRight = nLeft + 10;
									else
										nRight -= (m_ptDown.x - point.x);
								}
								else//x to right
								{
									nRight += (point.x - m_ptDown.x);
								}
								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetRight(nRight);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulerqrcode")
						{
							CRulerQRCode* pObj = sobj_cast<CRulerQRCode>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();
								if (m_ptDown.x > point.x)  //x to left
								{
									if ((m_ptDown.x - point.x) >= (nRight - nLeft) - 10)
										nRight = nLeft + 10;
									else
										nRight -= (m_ptDown.x - point.x);
								}
								else//x to right
								{
									nRight += (point.x - m_ptDown.x);
								}
								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetRight(nRight);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulertext")
						{
							CRulerText* pObj = sobj_cast<CRulerText>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();
								if (m_ptDown.x > point.x)  //x to left
								{
									if ((m_ptDown.x - point.x) >= (nRight - nLeft) - 10)
										nRight = nLeft + 10;
									else
										nRight -= (m_ptDown.x - point.x);
								}
								else//x to right
								{
									nRight += (point.x - m_ptDown.x);
								}
								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetRight(nRight);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulerpicture")
						{
							CRulerPicture* pObj = sobj_cast<CRulerPicture>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();
								if (m_ptDown.x > point.x)  //x to left
								{
									if ((m_ptDown.x - point.x) >= (nRight - nLeft) - 10)
										nRight = nLeft + 10;
									else
										nRight -= (m_ptDown.x - point.x);
								}
								else//x to right
								{
									nRight += (point.x - m_ptDown.x);
								}
								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetRight(nRight);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulerpdf")
						{
							CRulerPDF* pObj = sobj_cast<CRulerPDF>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();
								if (m_ptDown.x > point.x)  //x to left
								{
									if ((m_ptDown.x - point.x) >= (nRight - nLeft) - 10)
										nRight = nLeft + 10;
									else
										nRight -= (m_ptDown.x - point.x);
								}
								else//x to right
								{
									nRight += (point.x - m_ptDown.x);
								}
								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetRight(nRight);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
					}
					break;
					case BottomRight:
					{
						SStringW sstrClassName = m_vecSelectObjects[0]->GetObjectClass();
						if (sstrClassName == L"rulerrect")
						{
							CRulerRectangle* pObj = sobj_cast<CRulerRectangle>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();
								if (m_ptDown.x > point.x)  //x to left
								{
									if ((m_ptDown.x - point.x) >= (nRight - nLeft) - 10)
										nRight = nLeft + 10;
									else
										nRight -= (m_ptDown.x - point.x);
								}
								else//x to right
								{
									nRight += (point.x - m_ptDown.x);
								}

								if (m_ptDown.y > point.y)//y to up
								{
									if ((m_ptDown.y - point.y) - 10 >= (nBottom - nTop))
										nBottom = nTop + 10;
									else
										nBottom -= (m_ptDown.y - point.y);
								}
								else //y to down
								{
									nBottom += (point.y - m_ptDown.y);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetRight(nRight);
								pObj->SetBottom(nBottom);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulerellipse")
						{
							CRulerEllipse* pObj = sobj_cast<CRulerEllipse>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();
								if (m_ptDown.x > point.x)  //x to left
								{
									if ((m_ptDown.x - point.x) >= (nRight - nLeft) - 10)
										nRight = nLeft + 10;
									else
										nRight -= (m_ptDown.x - point.x);
								}
								else//x to right
								{
									nRight += (point.x - m_ptDown.x);
								}

								if (m_ptDown.y > point.y)//y to up
								{
									if ((m_ptDown.y - point.y) - 10 >= (nBottom - nTop))
										nBottom = nTop + 10;
									else
										nBottom -= (m_ptDown.y - point.y);
								}
								else //y to down
								{
									nBottom += (point.y - m_ptDown.y);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetRight(nRight);
								pObj->SetBottom(nBottom);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulerbarcode")
						{
							CRulerBarCode* pObj = sobj_cast<CRulerBarCode>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();
								if (m_ptDown.x > point.x)  //x to left
								{
									if ((m_ptDown.x - point.x) >= (nRight - nLeft) - 10)
										nRight = nLeft + 10;
									else
										nRight -= (m_ptDown.x - point.x);
								}
								else//x to right
								{
									nRight += (point.x - m_ptDown.x);
								}

								if (m_ptDown.y > point.y)//y to up
								{
									if ((m_ptDown.y - point.y) - 10 >= (nBottom - nTop))
										nBottom = nTop + 10;
									else
										nBottom -= (m_ptDown.y - point.y);
								}
								else //y to down
								{
									nBottom += (point.y - m_ptDown.y);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetRight(nRight);
								pObj->SetBottom(nBottom);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulerdrugcode")
						{
							CRulerDrugCode* pObj = sobj_cast<CRulerDrugCode>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();
								if (m_ptDown.x > point.x)  //x to left
								{
									if ((m_ptDown.x - point.x) >= (nRight - nLeft) - 10)
										nRight = nLeft + 10;
									else
										nRight -= (m_ptDown.x - point.x);
								}
								else//x to right
								{
									nRight += (point.x - m_ptDown.x);
								}

								if (m_ptDown.y > point.y)//y to up
								{
									if ((m_ptDown.y - point.y) - 10 >= (nBottom - nTop))
										nBottom = nTop + 10;
									else
										nBottom -= (m_ptDown.y - point.y);
								}
								else //y to down
								{
									nBottom += (point.y - m_ptDown.y);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetRight(nRight);
								pObj->SetBottom(nBottom);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulerqrcode")
						{
							CRulerQRCode* pObj = sobj_cast<CRulerQRCode>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();
								if (m_ptDown.x > point.x)  //x to left
								{
									if ((m_ptDown.x - point.x) >= (nRight - nLeft) - 10)
										nRight = nLeft + 10;
									else
										nRight -= (m_ptDown.x - point.x);
								}
								else//x to right
								{
									nRight += (point.x - m_ptDown.x);
								}

								if (m_ptDown.y > point.y)//y to up
								{
									if ((m_ptDown.y - point.y) - 10 >= (nBottom - nTop))
										nBottom = nTop + 10;
									else
										nBottom -= (m_ptDown.y - point.y);
								}
								else //y to down
								{
									nBottom += (point.y - m_ptDown.y);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetRight(nRight);
								pObj->SetBottom(nBottom);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulertext")
						{
							CRulerText* pObj = sobj_cast<CRulerText>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();
								if (m_ptDown.x > point.x)  //x to left
								{
									if ((m_ptDown.x - point.x) >= (nRight - nLeft) - 10)
										nRight = nLeft + 10;
									else
										nRight -= (m_ptDown.x - point.x);
								}
								else//x to right
								{
									nRight += (point.x - m_ptDown.x);
								}

								if (m_ptDown.y > point.y)//y to up
								{
									if ((m_ptDown.y - point.y) - 10 >= (nBottom - nTop))
										nBottom = nTop + 10;
									else
										nBottom -= (m_ptDown.y - point.y);
								}
								else //y to down
								{
									nBottom += (point.y - m_ptDown.y);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetRight(nRight);
								pObj->SetBottom(nBottom);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulerpicture")
						{
							CRulerPicture* pObj = sobj_cast<CRulerPicture>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();
								if (m_ptDown.x > point.x)  //x to left
								{
									if ((m_ptDown.x - point.x) >= (nRight - nLeft) - 10)
										nRight = nLeft + 10;
									else
										nRight -= (m_ptDown.x - point.x);
								}
								else//x to right
								{
									nRight += (point.x - m_ptDown.x);
								}

								if (m_ptDown.y > point.y)//y to up
								{
									if ((m_ptDown.y - point.y) - 10 >= (nBottom - nTop))
										nBottom = nTop + 10;
									else
										nBottom -= (m_ptDown.y - point.y);
								}
								else //y to down
								{
									nBottom += (point.y - m_ptDown.y);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetRight(nRight);
								pObj->SetBottom(nBottom);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulerpdf")
						{
							CRulerPDF* pObj = sobj_cast<CRulerPDF>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();
								if (m_ptDown.x > point.x)  //x to left
								{
									if ((m_ptDown.x - point.x) >= (nRight - nLeft) - 10)
										nRight = nLeft + 10;
									else
										nRight -= (m_ptDown.x - point.x);
								}
								else//x to right
								{
									nRight += (point.x - m_ptDown.x);
								}

								if (m_ptDown.y > point.y)//y to up
								{
									if ((m_ptDown.y - point.y) - 10 >= (nBottom - nTop))
										nBottom = nTop + 10;
									else
										nBottom -= (m_ptDown.y - point.y);
								}
								else //y to down
								{
									nBottom += (point.y - m_ptDown.y);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetRight(nRight);
								pObj->SetBottom(nBottom);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
					}
					break;
					case BottomCenter:
					{
						SStringW sstrClassName = m_vecSelectObjects[0]->GetObjectClass();
						if (sstrClassName == L"rulerrect")
						{
							CRulerRectangle* pObj = sobj_cast<CRulerRectangle>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();

								if (m_ptDown.y > point.y)//y to up
								{
									if ((m_ptDown.y - point.y) - 10 >= (nBottom - nTop))
										nBottom = nTop + 10;
									else
										nBottom -= (m_ptDown.y - point.y);
								}
								else //y to down
								{
									nBottom += (point.y - m_ptDown.y);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetBottom(nBottom);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulerellipse")
						{
							CRulerEllipse* pObj = sobj_cast<CRulerEllipse>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();

								if (m_ptDown.y > point.y)//y to up
								{
									if ((m_ptDown.y - point.y) - 10 >= (nBottom - nTop))
										nBottom = nTop + 10;
									else
										nBottom -= (m_ptDown.y - point.y);
								}
								else //y to down
								{
									nBottom += (point.y - m_ptDown.y);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetBottom(nBottom);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulerbarcode")
						{
							CRulerBarCode* pObj = sobj_cast<CRulerBarCode>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();

								if (m_ptDown.y > point.y)//y to up
								{
									if ((m_ptDown.y - point.y) - 10 >= (nBottom - nTop))
										nBottom = nTop + 10;
									else
										nBottom -= (m_ptDown.y - point.y);
								}
								else //y to down
								{
									nBottom += (point.y - m_ptDown.y);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetBottom(nBottom);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulerdrugcode")
						{
							CRulerDrugCode* pObj = sobj_cast<CRulerDrugCode>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();

								if (m_ptDown.y > point.y)//y to up
								{
									if ((m_ptDown.y - point.y) - 10 >= (nBottom - nTop))
										nBottom = nTop + 10;
									else
										nBottom -= (m_ptDown.y - point.y);
								}
								else //y to down
								{
									nBottom += (point.y - m_ptDown.y);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetBottom(nBottom);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulerqrcode")
						{
							CRulerQRCode* pObj = sobj_cast<CRulerQRCode>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();

								if (m_ptDown.y > point.y)//y to up
								{
									if ((m_ptDown.y - point.y) - 10 >= (nBottom - nTop))
										nBottom = nTop + 10;
									else
										nBottom -= (m_ptDown.y - point.y);
								}
								else //y to down
								{
									nBottom += (point.y - m_ptDown.y);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetBottom(nBottom);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulertext")
						{
							CRulerText* pObj = sobj_cast<CRulerText>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();

								if (m_ptDown.y > point.y)//y to up
								{
									if ((m_ptDown.y - point.y) - 10 >= (nBottom - nTop))
										nBottom = nTop + 10;
									else
										nBottom -= (m_ptDown.y - point.y);
								}
								else //y to down
								{
									nBottom += (point.y - m_ptDown.y);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetBottom(nBottom);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulerpicture")
						{
							CRulerPicture* pObj = sobj_cast<CRulerPicture>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();

								if (m_ptDown.y > point.y)//y to up
								{
									if ((m_ptDown.y - point.y) - 10 >= (nBottom - nTop))
										nBottom = nTop + 10;
									else
										nBottom -= (m_ptDown.y - point.y);
								}
								else //y to down
								{
									nBottom += (point.y - m_ptDown.y);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetBottom(nBottom);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulerpdf")
						{
							CRulerPDF* pObj = sobj_cast<CRulerPDF>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();

								if (m_ptDown.y > point.y)//y to up
								{
									if ((m_ptDown.y - point.y) - 10 >= (nBottom - nTop))
										nBottom = nTop + 10;
									else
										nBottom -= (m_ptDown.y - point.y);
								}
								else //y to down
								{
									nBottom += (point.y - m_ptDown.y);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetBottom(nBottom);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
					}
					break;
					case BottomLeft:
					{
						SStringW sstrClassName = m_vecSelectObjects[0]->GetObjectClass();
						if (sstrClassName == L"rulerrect")
						{
							CRulerRectangle* pObj = sobj_cast<CRulerRectangle>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();

								if (m_ptDown.x > point.x)  //x to left
								{
									nLeft -= (m_ptDown.x - point.x);
								}
								else //x to right
								{
									if ((point.x - m_ptDown.x) - 10 > (nRight - nLeft))
									{
										nLeft = nRight - 10;
									}
									else
									{
										nLeft += (point.x - m_ptDown.x);
									}
								}

								if (m_ptDown.y > point.y)//y to up
								{
									nBottom -= (m_ptDown.y - point.y);
								}
								else //y to down
								{
									nBottom += (point.y - m_ptDown.y);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetBottom(nBottom);
								pObj->SetLeft(nLeft);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulerellipse")
						{
							CRulerEllipse* pObj = sobj_cast<CRulerEllipse>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();

								if (m_ptDown.x > point.x)  //x to left
								{
									nLeft -= (m_ptDown.x - point.x);
								}
								else //x to right
								{
									if ((point.x - m_ptDown.x) - 10 > (nRight - nLeft))
									{
										nLeft = nRight - 10;
									}
									else
									{
										nLeft += (point.x - m_ptDown.x);
									}
								}

								if (m_ptDown.y > point.y)//y to up
								{
									nBottom -= (m_ptDown.y - point.y);
								}
								else //y to down
								{
									nBottom += (point.y - m_ptDown.y);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetBottom(nBottom);
								pObj->SetLeft(nLeft);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulerbarcode")
						{
							CRulerBarCode* pObj = sobj_cast<CRulerBarCode>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();

								if (m_ptDown.x > point.x)  //x to left
								{
									nLeft -= (m_ptDown.x - point.x);
								}
								else //x to right
								{
									if ((point.x - m_ptDown.x) - 10 > (nRight - nLeft))
									{
										nLeft = nRight - 10;
									}
									else
									{
										nLeft += (point.x - m_ptDown.x);
									}
								}

								if (m_ptDown.y > point.y)//y to up
								{
									nBottom -= (m_ptDown.y - point.y);
								}
								else //y to down
								{
									nBottom += (point.y - m_ptDown.y);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetBottom(nBottom);
								pObj->SetLeft(nLeft);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulerdrugcode")
						{
							CRulerDrugCode* pObj = sobj_cast<CRulerDrugCode>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();

								if (m_ptDown.x > point.x)  //x to left
								{
									nLeft -= (m_ptDown.x - point.x);
								}
								else //x to right
								{
									if ((point.x - m_ptDown.x) - 10 > (nRight - nLeft))
									{
										nLeft = nRight - 10;
									}
									else
									{
										nLeft += (point.x - m_ptDown.x);
									}
								}

								if (m_ptDown.y > point.y)//y to up
								{
									nBottom -= (m_ptDown.y - point.y);
								}
								else //y to down
								{
									nBottom += (point.y - m_ptDown.y);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetBottom(nBottom);
								pObj->SetLeft(nLeft);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulerqrcode")
						{
							CRulerQRCode* pObj = sobj_cast<CRulerQRCode>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();

								if (m_ptDown.x > point.x)  //x to left
								{
									nLeft -= (m_ptDown.x - point.x);
								}
								else //x to right
								{
									if ((point.x - m_ptDown.x) - 10 > (nRight - nLeft))
									{
										nLeft = nRight - 10;
									}
									else
									{
										nLeft += (point.x - m_ptDown.x);
									}
								}

								if (m_ptDown.y > point.y)//y to up
								{
									nBottom -= (m_ptDown.y - point.y);
								}
								else //y to down
								{
									nBottom += (point.y - m_ptDown.y);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetBottom(nBottom);
								pObj->SetLeft(nLeft);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulertext")
						{
							CRulerText* pObj = sobj_cast<CRulerText>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();

								if (m_ptDown.x > point.x)  //x to left
								{
									nLeft -= (m_ptDown.x - point.x);
								}
								else //x to right
								{
									if ((point.x - m_ptDown.x) - 10 > (nRight - nLeft))
									{
										nLeft = nRight - 10;
									}
									else
									{
										nLeft += (point.x - m_ptDown.x);
									}
								}

								if (m_ptDown.y > point.y)//y to up
								{
									nBottom -= (m_ptDown.y - point.y);
								}
								else //y to down
								{
									nBottom += (point.y - m_ptDown.y);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetBottom(nBottom);
								pObj->SetLeft(nLeft);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulerpicture")
						{
							CRulerPicture* pObj = sobj_cast<CRulerPicture>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();

								if (m_ptDown.x > point.x)  //x to left
								{
									nLeft -= (m_ptDown.x - point.x);
								}
								else //x to right
								{
									if ((point.x - m_ptDown.x) - 10 > (nRight - nLeft))
									{
										nLeft = nRight - 10;
									}
									else
									{
										nLeft += (point.x - m_ptDown.x);
									}
								}

								if (m_ptDown.y > point.y)//y to up
								{
									nBottom -= (m_ptDown.y - point.y);
								}
								else //y to down
								{
									nBottom += (point.y - m_ptDown.y);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetBottom(nBottom);
								pObj->SetLeft(nLeft);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulerpdf")
						{
							CRulerPDF* pObj = sobj_cast<CRulerPDF>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();

								if (m_ptDown.x > point.x)  //x to left
								{
									nLeft -= (m_ptDown.x - point.x);
								}
								else //x to right
								{
									if ((point.x - m_ptDown.x) - 10 > (nRight - nLeft))
									{
										nLeft = nRight - 10;
									}
									else
									{
										nLeft += (point.x - m_ptDown.x);
									}
								}

								if (m_ptDown.y > point.y)//y to up
								{
									nBottom -= (m_ptDown.y - point.y);
								}
								else //y to down
								{
									nBottom += (point.y - m_ptDown.y);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetBottom(nBottom);
								pObj->SetLeft(nLeft);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
					}
					break;
					case LeftCenter:
					{
						SStringW sstrClassName = m_vecSelectObjects[0]->GetObjectClass();
						if (sstrClassName == L"rulerrect")
						{
							CRulerRectangle* pObj = sobj_cast<CRulerRectangle>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();

								if (m_ptDown.x > point.x)  //x to left
								{
									nLeft -= (m_ptDown.x - point.x);
								}
								else //x to right
								{
									if ((point.x - m_ptDown.x) - 10 > (nRight - nLeft))
										nLeft = nRight - 10;
									else
										nLeft += (point.x - m_ptDown.x);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetLeft(nLeft);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulerellipse")
						{
							CRulerRectangle* pObj = sobj_cast<CRulerRectangle>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();

								if (m_ptDown.x > point.x)  //x to left
								{
									nLeft -= (m_ptDown.x - point.x);
								}
								else //x to right
								{
									if ((point.x - m_ptDown.x) - 10 > (nRight - nLeft))
										nLeft = nRight - 10;
									else
										nLeft += (point.x - m_ptDown.x);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetLeft(nLeft);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulerbarcode")
						{
							CRulerBarCode* pObj = sobj_cast<CRulerBarCode>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();

								if (m_ptDown.x > point.x)  //x to left
								{
									nLeft -= (m_ptDown.x - point.x);
								}
								else //x to right
								{
									if ((point.x - m_ptDown.x) - 10 > (nRight - nLeft))
										nLeft = nRight - 10;
									else
										nLeft += (point.x - m_ptDown.x);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetLeft(nLeft);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulerdrugcode")
						{
							CRulerDrugCode* pObj = sobj_cast<CRulerDrugCode>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();

								if (m_ptDown.x > point.x)  //x to left
								{
									nLeft -= (m_ptDown.x - point.x);
								}
								else //x to right
								{
									if ((point.x - m_ptDown.x) - 10 > (nRight - nLeft))
										nLeft = nRight - 10;
									else
										nLeft += (point.x - m_ptDown.x);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetLeft(nLeft);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulerqrcode")
						{
							CRulerQRCode* pObj = sobj_cast<CRulerQRCode>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();

								if (m_ptDown.x > point.x)  //x to left
								{
									nLeft -= (m_ptDown.x - point.x);
								}
								else //x to right
								{
									if ((point.x - m_ptDown.x) - 10 > (nRight - nLeft))
										nLeft = nRight - 10;
									else
										nLeft += (point.x - m_ptDown.x);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetLeft(nLeft);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulertext")
						{
							CRulerText* pObj = sobj_cast<CRulerText>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();

								if (m_ptDown.x > point.x)  //x to left
								{
									nLeft -= (m_ptDown.x - point.x);
								}
								else //x to right
								{
									if ((point.x - m_ptDown.x) - 10 > (nRight - nLeft))
										nLeft = nRight - 10;
									else
										nLeft += (point.x - m_ptDown.x);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetLeft(nLeft);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulerpicture")
						{
							CRulerPicture* pObj = sobj_cast<CRulerPicture>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();

								if (m_ptDown.x > point.x)  //x to left
								{
									nLeft -= (m_ptDown.x - point.x);
								}
								else //x to right
								{
									if ((point.x - m_ptDown.x) - 10 > (nRight - nLeft))
										nLeft = nRight - 10;
									else
										nLeft += (point.x - m_ptDown.x);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetLeft(nLeft);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulerpdf")
						{
							CRulerPDF* pObj = sobj_cast<CRulerPDF>(m_vecSelectObjects[0]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								int nRight = pObj->GetRight();
								int nBottom = pObj->GetBottom();

								if (m_ptDown.x > point.x)  //x to left
								{
									nLeft -= (m_ptDown.x - point.x);
								}
								else //x to right
								{
									if ((point.x - m_ptDown.x) - 10 > (nRight - nLeft))
										nLeft = nRight - 10;
									else
										nLeft += (point.x - m_ptDown.x);
								}

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->SetLeft(nLeft);
								int nWidth = nRight - nLeft;
								int nHeight = nBottom - nTop;
								pObj->SetWidth(nWidth);
								pObj->SetHeight(nHeight);
								pObj->CalcPos();
							}
						}
					}
					break;
					case SelectRect:
						break;
					default:
						break;
					}
				}
			}
		}

		m_bSelectObject = false;

		m_rcDrawArea.SetRectEmpty();

		CRect rcClient;
		this->GetClientRect(&rcClient);
		SOUI::CPoint pt;
		pt.x = point.x - rcClient.left;
		pt.y = point.y - rcClient.top;
		m_ptObjRB = pt;

		//鼠标弹起时在选择的区域内创建相应的object
		CRect rc;
		rc.SetRect(m_ptObjLT, m_ptObjRB);
		rc.NormalizeRect();

		switch (m_nAddObjectType)
		{
		case RulerTypeRectangle:
		{
			AddRectangle(rc);
		}
		break;
		case RulerTypeEllipse:
		{
			AddEllipse(rc);
		}
		break;
		case RulerTypeBarCode:
		{
			AddBarCode(rc);
		}
		break;
		case RulerTypeDrugCode:
		{
			AddDrugCode(rc);
		}
		break;
		case RulerTypeQRCode:
		{
			AddQRCode(rc);
		}
		break;
		case RulerTypeText:
		{
			AddText(rc);
		}
		break;
		case RulerTypePicture:
		{
			AddPicture(rc);
		}
		break;
		case RulerTypePDF:
		{
			AddPDF(rc);
		}
		break;
		default:
			break;
		}

		m_nAddObjectType = RulerTypeNull;
		Invalidate();
	}

	void CRulerBox::OnLButtonDblClk(UINT nFlags, SOUI::CPoint point)
	{
		//演示将下层的控件提到上层
		//begin
		for (int i = 0; i < m_vecSelectObjects.size(); i++)
		{
			SStringW sstrClassName = m_vecSelectObjects[i]->GetObjectClass();
			if (sstrClassName == L"rulerpicture")
			{
				CRulerPicture* pObj = sobj_cast<CRulerPicture>(m_vecSelectObjects[i]);
				if (pObj)
				{
					CRect rcObj = pObj->GetClientRect();
					if (PtInRect(rcObj, point))
					{
						pObj->SSendMessage(WM_LBUTTONDBLCLK);
					}
				}
			}
		}
		//end

		CRect rcClient;
		this->GetClientRect(&rcClient);

		SOUI::CPoint pt;
		pt.x = point.x - rcClient.left;
		pt.y = point.y - rcClient.top;

		if (m_ptObjLT == pt)
		{
			return;
		}

		switch (m_nAddObjectType)
		{
		case RulerTypeRectangle:
		{
			CRect rcRectangle;
			rcRectangle.left = pt.x;
			rcRectangle.top = pt.y;
			rcRectangle.right = pt.x + 200;
			rcRectangle.bottom = pt.y + 100;

			AddRectangle(rcRectangle);
		}
		break;
		default:
			break;
		}

		m_bCreateChild = false;
	}

	void CRulerBox::OnMouseMove(UINT nFlags, SOUI::CPoint point)
	{
		if ((nFlags & MK_LBUTTON))
		{
			if (!m_bSelectObject)
			{
				if (!m_rcDrawArea.IsRectEmpty())
					m_rcDrawArea.SetRectEmpty();
				if (m_ptDown == point)
					return;

				m_rcDrawArea.SetRect(m_ptDown, point);
				m_rcDrawArea.NormalizeRect();


				//需要将框选中的object全部设置被选中状态
				//TODO:
			}
			else
			{
				if (m_curEcPosType == EcPosType::Null)  //move
				{
					//point跟m_ptDown两点X、Y的差值；用差值跟控件的位置做加减处理然后做移动操作
					for (int i = 0; i < m_vecSelectObjects.size(); i++)
					{
						SStringW sstrClassName = m_vecSelectObjects[i]->GetObjectClass();
						if (sstrClassName == L"rulerrect")
						{
							CRulerRectangle* pObj = sobj_cast<CRulerRectangle>(m_vecSelectObjects[i]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();
								if (m_ptDown.x > point.x)
									nLeft -= (m_ptDown.x - point.x);
								else
									nLeft += (point.x - m_ptDown.x);
								if (m_ptDown.y > point.y)
									nTop -= (m_ptDown.y - point.y);
								else
									nTop += (point.y - m_ptDown.y);
								SStringW sstrPos;
								sstrPos.Format(L"%d,%d", nLeft, nTop);
								pObj->SetAttribute(L"pos", sstrPos);
								pObj->CalcPos();
							}
						}
						else if (sstrClassName == L"rulerellipse")
						{
							CRulerEllipse* pObj = sobj_cast<CRulerEllipse>(m_vecSelectObjects[i]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();

								if (m_ptDown.x > point.x)
									nLeft -= (m_ptDown.x - point.x);
								else
									nLeft += (point.x - m_ptDown.x);

								if (m_ptDown.y > point.y)
									nTop -= (m_ptDown.y - point.y);
								else
									nTop += (point.y - m_ptDown.y);

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d", nLeft, nTop);
								pObj->SetAttribute(L"pos", sstrPos);
							}
						}
						else if (sstrClassName == L"rulerbarcode")
						{
							CRulerBarCode* pObj = sobj_cast<CRulerBarCode>(m_vecSelectObjects[i]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();

								if (m_ptDown.x > point.x)
									nLeft -= (m_ptDown.x - point.x);
								else
									nLeft += (point.x - m_ptDown.x);

								if (m_ptDown.y > point.y)
									nTop -= (m_ptDown.y - point.y);
								else
									nTop += (point.y - m_ptDown.y);

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d", nLeft, nTop);
								pObj->SetAttribute(L"pos", sstrPos);
							}
						}
						else if (sstrClassName == L"rulerdrugcode")
						{
							CRulerDrugCode* pObj = sobj_cast<CRulerDrugCode>(m_vecSelectObjects[i]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();

								if (m_ptDown.x > point.x)
									nLeft -= (m_ptDown.x - point.x);
								else
									nLeft += (point.x - m_ptDown.x);

								if (m_ptDown.y > point.y)
									nTop -= (m_ptDown.y - point.y);
								else
									nTop += (point.y - m_ptDown.y);

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d", nLeft, nTop);
								pObj->SetAttribute(L"pos", sstrPos);
							}
						}
						else if (sstrClassName == L"rulerqrcode")
						{
							CRulerQRCode* pObj = sobj_cast<CRulerQRCode>(m_vecSelectObjects[i]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();

								if (m_ptDown.x > point.x)
									nLeft -= (m_ptDown.x - point.x);
								else
									nLeft += (point.x - m_ptDown.x);

								if (m_ptDown.y > point.y)
									nTop -= (m_ptDown.y - point.y);
								else
									nTop += (point.y - m_ptDown.y);

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d", nLeft, nTop);
								pObj->SetAttribute(L"pos", sstrPos);
							}
						}
						else if (sstrClassName == L"rulertext")
						{
							CRulerText* pObj = sobj_cast<CRulerText>(m_vecSelectObjects[i]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();

								if (m_ptDown.x > point.x)
									nLeft -= (m_ptDown.x - point.x);
								else
									nLeft += (point.x - m_ptDown.x);

								if (m_ptDown.y > point.y)
									nTop -= (m_ptDown.y - point.y);
								else
									nTop += (point.y - m_ptDown.y);

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d", nLeft, nTop);
								pObj->SetAttribute(L"pos", sstrPos);
							}
						}
						else if (sstrClassName == L"rulerpicture")
						{
							CRulerPicture* pObj = sobj_cast<CRulerPicture>(m_vecSelectObjects[i]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();

								if (m_ptDown.x > point.x)
									nLeft -= (m_ptDown.x - point.x);
								else
									nLeft += (point.x - m_ptDown.x);

								if (m_ptDown.y > point.y)
									nTop -= (m_ptDown.y - point.y);
								else
									nTop += (point.y - m_ptDown.y);

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d", nLeft, nTop);
								pObj->SetAttribute(L"pos", sstrPos);
							}
						}
						else if (sstrClassName == L"rulerpdf")
						{
							CRulerPDF* pObj = sobj_cast<CRulerPDF>(m_vecSelectObjects[i]);
							if (pObj)
							{
								int nLeft = pObj->GetLeft();
								int nTop = pObj->GetTop();

								if (m_ptDown.x > point.x)
									nLeft -= (m_ptDown.x - point.x);
								else
									nLeft += (point.x - m_ptDown.x);

								if (m_ptDown.y > point.y)
									nTop -= (m_ptDown.y - point.y);
								else
									nTop += (point.y - m_ptDown.y);

								SStringW sstrPos;
								sstrPos.Format(L"%d,%d", nLeft, nTop);
								pObj->SetAttribute(L"pos", sstrPos);
							}
						}
					}
				}
				else
				{
					if (m_vecSelectObjects.size() == 1)
					{
						switch (m_curEcPosType)
						{
						case TopLeft:
						{
							SStringW sstrClassName = m_vecSelectObjects[0]->GetObjectClass();
							if (sstrClassName == L"rulerrect")
							{
								CRulerRectangle* pObj = sobj_cast<CRulerRectangle>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();
									if (m_ptDown.x > point.x)  //x往左
									{
										nLeft -= (m_ptDown.x - point.x);
									}
									else//x往右
									{
										if ((point.x - m_ptDown.x) - 10 >= (nRight - nLeft))
											nLeft = (point.x - m_ptDown.x) - 10;
										else
											nLeft += (point.x - m_ptDown.x);
									}
									if (m_ptDown.y > point.y)//y往上
									{
										nTop -= (m_ptDown.y - point.y);
									}
									else //y往下
									{
										if ((point.y - m_ptDown.y) - 10 >= (nBottom - nTop))
											nTop = (point.y - m_ptDown.y) - 10;
										else
											nTop += (point.y - m_ptDown.y);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulerellipse")
							{
								CRulerEllipse* pObj = sobj_cast<CRulerEllipse>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();
									if (m_ptDown.x > point.x)  //x往左
									{
										nLeft -= (m_ptDown.x - point.x);
									}
									else//x往右
									{
										if ((point.x - m_ptDown.x) - 10 >= (nRight - nLeft))
											nLeft = (point.x - m_ptDown.x) - 10;
										else
											nLeft += (point.x - m_ptDown.x);
									}
									if (m_ptDown.y > point.y)//y往上
									{
										nTop -= (m_ptDown.y - point.y);
									}
									else //y往下
									{
										if ((point.y - m_ptDown.y) - 10 >= (nBottom - nTop))
											nTop = (point.y - m_ptDown.y) - 10;
										else
											nTop += (point.y - m_ptDown.y);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulerbarcode")
							{
								CRulerBarCode* pObj = sobj_cast<CRulerBarCode>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();
									if (m_ptDown.x > point.x)  //x往左
									{
										nLeft -= (m_ptDown.x - point.x);
									}
									else//x往右
									{
										if ((point.x - m_ptDown.x) - 10 >= (nRight - nLeft))
											nLeft = (point.x - m_ptDown.x) - 10;
										else
											nLeft += (point.x - m_ptDown.x);
									}
									if (m_ptDown.y > point.y)//y往上
									{
										nTop -= (m_ptDown.y - point.y);
									}
									else //y往下
									{
										if ((point.y - m_ptDown.y) - 10 >= (nBottom - nTop))
											nTop = (point.y - m_ptDown.y) - 10;
										else
											nTop += (point.y - m_ptDown.y);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulerdrugcode")
							{
								CRulerDrugCode* pObj = sobj_cast<CRulerDrugCode>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();
									if (m_ptDown.x > point.x)  //x往左
									{
										nLeft -= (m_ptDown.x - point.x);
									}
									else//x往右
									{
										if ((point.x - m_ptDown.x) - 10 >= (nRight - nLeft))
											nLeft = (point.x - m_ptDown.x) - 10;
										else
											nLeft += (point.x - m_ptDown.x);
									}
									if (m_ptDown.y > point.y)//y往上
									{
										nTop -= (m_ptDown.y - point.y);
									}
									else //y往下
									{
										if ((point.y - m_ptDown.y) - 10 >= (nBottom - nTop))
											nTop = (point.y - m_ptDown.y) - 10;
										else
											nTop += (point.y - m_ptDown.y);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulerqrcode")
							{
								CRulerQRCode* pObj = sobj_cast<CRulerQRCode>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();
									if (m_ptDown.x > point.x)  //x往左
									{
										nLeft -= (m_ptDown.x - point.x);
									}
									else//x往右
									{
										if ((point.x - m_ptDown.x) - 10 >= (nRight - nLeft))
											nLeft = (point.x - m_ptDown.x) - 10;
										else
											nLeft += (point.x - m_ptDown.x);
									}
									if (m_ptDown.y > point.y)//y往上
									{
										nTop -= (m_ptDown.y - point.y);
									}
									else //y往下
									{
										if ((point.y - m_ptDown.y) - 10 >= (nBottom - nTop))
											nTop = (point.y - m_ptDown.y) - 10;
										else
											nTop += (point.y - m_ptDown.y);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulertext")
							{
								CRulerText* pObj = sobj_cast<CRulerText>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();
									if (m_ptDown.x > point.x)  //x往左
									{
										nLeft -= (m_ptDown.x - point.x);
									}
									else//x往右
									{
										if ((point.x - m_ptDown.x) - 10 >= (nRight - nLeft))
											nLeft = (point.x - m_ptDown.x) - 10;
										else
											nLeft += (point.x - m_ptDown.x);
									}
									if (m_ptDown.y > point.y)//y往上
									{
										nTop -= (m_ptDown.y - point.y);
									}
									else //y往下
									{
										if ((point.y - m_ptDown.y) - 10 >= (nBottom - nTop))
											nTop = (point.y - m_ptDown.y) - 10;
										else
											nTop += (point.y - m_ptDown.y);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulerpicture")
							{
								CRulerPicture* pObj = sobj_cast<CRulerPicture>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();
									if (m_ptDown.x > point.x)  //x往左
									{
										nLeft -= (m_ptDown.x - point.x);
									}
									else//x往右
									{
										if ((point.x - m_ptDown.x) - 10 >= (nRight - nLeft))
											nLeft = (point.x - m_ptDown.x) - 10;
										else
											nLeft += (point.x - m_ptDown.x);
									}
									if (m_ptDown.y > point.y)//y往上
									{
										nTop -= (m_ptDown.y - point.y);
									}
									else //y往下
									{
										if ((point.y - m_ptDown.y) - 10 >= (nBottom - nTop))
											nTop = (point.y - m_ptDown.y) - 10;
										else
											nTop += (point.y - m_ptDown.y);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulerpdf")
							{
								CRulerPDF* pObj = sobj_cast<CRulerPDF>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();
									if (m_ptDown.x > point.x)  //x往左
									{
										nLeft -= (m_ptDown.x - point.x);
									}
									else//x往右
									{
										if ((point.x - m_ptDown.x) - 10 >= (nRight - nLeft))
											nLeft = (point.x - m_ptDown.x) - 10;
										else
											nLeft += (point.x - m_ptDown.x);
									}
									if (m_ptDown.y > point.y)//y往上
									{
										nTop -= (m_ptDown.y - point.y);
									}
									else //y往下
									{
										if ((point.y - m_ptDown.y) - 10 >= (nBottom - nTop))
											nTop = (point.y - m_ptDown.y) - 10;
										else
											nTop += (point.y - m_ptDown.y);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
						}
						break;
						case TopCenter:
						{
							SStringW sstrClassName = m_vecSelectObjects[0]->GetObjectClass();
							if (sstrClassName == L"rulerrect")
							{
								CRulerRectangle* pObj = sobj_cast<CRulerRectangle>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();

									if (m_ptDown.y > point.y)//y往上
									{
										nTop -= (m_ptDown.y - point.y);
									}
									else //y往下
									{
										if ((point.y - m_ptDown.y) - 10 >= (nBottom - nTop))
											nTop = (point.y - m_ptDown.y) - 10;
										else
											nTop += (point.y - m_ptDown.y);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulerellipse")
							{
								CRulerEllipse* pObj = sobj_cast<CRulerEllipse>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();

									if (m_ptDown.y > point.y)//y往上
									{
										nTop -= (m_ptDown.y - point.y);
									}
									else //y往下
									{
										if ((point.y - m_ptDown.y) - 10 >= (nBottom - nTop))
											nTop = (point.y - m_ptDown.y) - 10;
										else
											nTop += (point.y - m_ptDown.y);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulerbarcode")
							{
								CRulerBarCode* pObj = sobj_cast<CRulerBarCode>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();

									if (m_ptDown.y > point.y)//y往上
									{
										nTop -= (m_ptDown.y - point.y);
									}
									else //y往下
									{
										if ((point.y - m_ptDown.y) - 10 >= (nBottom - nTop))
											nTop = (point.y - m_ptDown.y) - 10;
										else
											nTop += (point.y - m_ptDown.y);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulerdrugcode")
							{
								CRulerDrugCode* pObj = sobj_cast<CRulerDrugCode>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();

									if (m_ptDown.y > point.y)//y往上
									{
										nTop -= (m_ptDown.y - point.y);
									}
									else //y往下
									{
										if ((point.y - m_ptDown.y) - 10 >= (nBottom - nTop))
											nTop = (point.y - m_ptDown.y) - 10;
										else
											nTop += (point.y - m_ptDown.y);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulerqrcode")
							{
								CRulerQRCode* pObj = sobj_cast<CRulerQRCode>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();

									if (m_ptDown.y > point.y)//y往上
									{
										nTop -= (m_ptDown.y - point.y);
									}
									else //y往下
									{
										if ((point.y - m_ptDown.y) - 10 >= (nBottom - nTop))
											nTop = (point.y - m_ptDown.y) - 10;
										else
											nTop += (point.y - m_ptDown.y);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulertext")
							{
								CRulerText* pObj = sobj_cast<CRulerText>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();

									if (m_ptDown.y > point.y)//y往上
									{
										nTop -= (m_ptDown.y - point.y);
									}
									else //y往下
									{
										if ((point.y - m_ptDown.y) - 10 >= (nBottom - nTop))
											nTop = (point.y - m_ptDown.y) - 10;
										else
											nTop += (point.y - m_ptDown.y);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulerpicture")
							{
								CRulerPicture* pObj = sobj_cast<CRulerPicture>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();

									if (m_ptDown.y > point.y)//y往上
									{
										nTop -= (m_ptDown.y - point.y);
									}
									else //y往下
									{
										if ((point.y - m_ptDown.y) - 10 >= (nBottom - nTop))
											nTop = (point.y - m_ptDown.y) - 10;
										else
											nTop += (point.y - m_ptDown.y);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulerpdf")
							{
								CRulerPDF* pObj = sobj_cast<CRulerPDF>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();

									if (m_ptDown.y > point.y)//y往上
									{
										nTop -= (m_ptDown.y - point.y);
									}
									else //y往下
									{
										if ((point.y - m_ptDown.y) - 10 >= (nBottom - nTop))
											nTop = (point.y - m_ptDown.y) - 10;
										else
											nTop += (point.y - m_ptDown.y);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
						}
						break;
						case TopRight:
						{
							SStringW sstrClassName = m_vecSelectObjects[0]->GetObjectClass();
							if (sstrClassName == L"rulerrect")
							{
								CRulerRectangle* pObj = sobj_cast<CRulerRectangle>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();
									if (m_ptDown.x > point.x)  //x to left
									{
										if ((m_ptDown.x - point.x) >= (nRight - nLeft) - 10)
											nRight = nLeft + 10;
										else
											nRight -= (m_ptDown.x - point.x);
									}
									else//x to right
									{
										nRight += (point.x - m_ptDown.x);
									}

									if (m_ptDown.y > point.y)//y to up
									{
										nTop -= (m_ptDown.y - point.y);
									}
									else //y to down
									{
										if ((point.y - m_ptDown.y) - 10 >= (nBottom - nTop))
											nTop = (point.y - m_ptDown.y) - 10;
										else
											nTop += (point.y - m_ptDown.y);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulerellipse")
							{
								CRulerEllipse* pObj = sobj_cast<CRulerEllipse>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();
									if (m_ptDown.x > point.x)  //x to left
									{
										if ((m_ptDown.x - point.x) >= (nRight - nLeft) - 10)
											nRight = nLeft + 10;
										else
											nRight -= (m_ptDown.x - point.x);
									}
									else//x to right
									{
										nRight += (point.x - m_ptDown.x);
									}

									if (m_ptDown.y > point.y)//y to up
									{
										nTop -= (m_ptDown.y - point.y);
									}
									else //y to down
									{
										if ((point.y - m_ptDown.y) - 10 >= (nBottom - nTop))
											nTop = (point.y - m_ptDown.y) - 10;
										else
											nTop += (point.y - m_ptDown.y);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulerbarcode")
							{
								CRulerBarCode* pObj = sobj_cast<CRulerBarCode>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();
									if (m_ptDown.x > point.x)  //x to left
									{
										if ((m_ptDown.x - point.x) >= (nRight - nLeft) - 10)
											nRight = nLeft + 10;
										else
											nRight -= (m_ptDown.x - point.x);
									}
									else//x to right
									{
										nRight += (point.x - m_ptDown.x);
									}

									if (m_ptDown.y > point.y)//y to up
									{
										nTop -= (m_ptDown.y - point.y);
									}
									else //y to down
									{
										if ((point.y - m_ptDown.y) - 10 >= (nBottom - nTop))
											nTop = (point.y - m_ptDown.y) - 10;
										else
											nTop += (point.y - m_ptDown.y);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulerdrugcode")
							{
								CRulerDrugCode* pObj = sobj_cast<CRulerDrugCode>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();
									if (m_ptDown.x > point.x)  //x to left
									{
										if ((m_ptDown.x - point.x) >= (nRight - nLeft) - 10)
											nRight = nLeft + 10;
										else
											nRight -= (m_ptDown.x - point.x);
									}
									else//x to right
									{
										nRight += (point.x - m_ptDown.x);
									}

									if (m_ptDown.y > point.y)//y to up
									{
										nTop -= (m_ptDown.y - point.y);
									}
									else //y to down
									{
										if ((point.y - m_ptDown.y) - 10 >= (nBottom - nTop))
											nTop = (point.y - m_ptDown.y) - 10;
										else
											nTop += (point.y - m_ptDown.y);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulerqrcode")
							{
								CRulerQRCode* pObj = sobj_cast<CRulerQRCode>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();
									if (m_ptDown.x > point.x)  //x to left
									{
										if ((m_ptDown.x - point.x) >= (nRight - nLeft) - 10)
											nRight = nLeft + 10;
										else
											nRight -= (m_ptDown.x - point.x);
									}
									else//x to right
									{
										nRight += (point.x - m_ptDown.x);
									}

									if (m_ptDown.y > point.y)//y to up
									{
										nTop -= (m_ptDown.y - point.y);
									}
									else //y to down
									{
										if ((point.y - m_ptDown.y) - 10 >= (nBottom - nTop))
											nTop = (point.y - m_ptDown.y) - 10;
										else
											nTop += (point.y - m_ptDown.y);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulertext")
							{
								CRulerText* pObj = sobj_cast<CRulerText>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();
									if (m_ptDown.x > point.x)  //x to left
									{
										if ((m_ptDown.x - point.x) >= (nRight - nLeft) - 10)
											nRight = nLeft + 10;
										else
											nRight -= (m_ptDown.x - point.x);
									}
									else//x to right
									{
										nRight += (point.x - m_ptDown.x);
									}

									if (m_ptDown.y > point.y)//y to up
									{
										nTop -= (m_ptDown.y - point.y);
									}
									else //y to down
									{
										if ((point.y - m_ptDown.y) - 10 >= (nBottom - nTop))
											nTop = (point.y - m_ptDown.y) - 10;
										else
											nTop += (point.y - m_ptDown.y);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulerpicture")
							{
								CRulerPicture* pObj = sobj_cast<CRulerPicture>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();
									if (m_ptDown.x > point.x)  //x to left
									{
										if ((m_ptDown.x - point.x) >= (nRight - nLeft) - 10)
											nRight = nLeft + 10;
										else
											nRight -= (m_ptDown.x - point.x);
									}
									else//x to right
									{
										nRight += (point.x - m_ptDown.x);
									}

									if (m_ptDown.y > point.y)//y to up
									{
										nTop -= (m_ptDown.y - point.y);
									}
									else //y to down
									{
										if ((point.y - m_ptDown.y) - 10 >= (nBottom - nTop))
											nTop = (point.y - m_ptDown.y) - 10;
										else
											nTop += (point.y - m_ptDown.y);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulerpdf")
							{
								CRulerPDF* pObj = sobj_cast<CRulerPDF>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();
									if (m_ptDown.x > point.x)  //x to left
									{
										if ((m_ptDown.x - point.x) >= (nRight - nLeft) - 10)
											nRight = nLeft + 10;
										else
											nRight -= (m_ptDown.x - point.x);
									}
									else//x to right
									{
										nRight += (point.x - m_ptDown.x);
									}

									if (m_ptDown.y > point.y)//y to up
									{
										nTop -= (m_ptDown.y - point.y);
									}
									else //y to down
									{
										if ((point.y - m_ptDown.y) - 10 >= (nBottom - nTop))
											nTop = (point.y - m_ptDown.y) - 10;
										else
											nTop += (point.y - m_ptDown.y);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
						}
						break;
						case RightCenter:
						{
							SStringW sstrClassName = m_vecSelectObjects[0]->GetObjectClass();
							if (sstrClassName == L"rulerrect")
							{
								CRulerRectangle* pObj = sobj_cast<CRulerRectangle>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();
									if (m_ptDown.x > point.x)  //x to left
									{
										if ((m_ptDown.x - point.x) >= (nRight - nLeft) - 10)
											nRight = nLeft + 10;
										else
											nRight -= (m_ptDown.x - point.x);
									}
									else//x to right
									{
										nRight += (point.x - m_ptDown.x);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulerellipse")
							{
								CRulerEllipse* pObj = sobj_cast<CRulerEllipse>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();
									if (m_ptDown.x > point.x)  //x to left
									{
										if ((m_ptDown.x - point.x) >= (nRight - nLeft) - 10)
											nRight = nLeft + 10;
										else
											nRight -= (m_ptDown.x - point.x);
									}
									else//x to right
									{
										nRight += (point.x - m_ptDown.x);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulerbarcode")
							{
								CRulerBarCode* pObj = sobj_cast<CRulerBarCode>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();
									if (m_ptDown.x > point.x)  //x to left
									{
										if ((m_ptDown.x - point.x) >= (nRight - nLeft) - 10)
											nRight = nLeft + 10;
										else
											nRight -= (m_ptDown.x - point.x);
									}
									else//x to right
									{
										nRight += (point.x - m_ptDown.x);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulerdrugcode")
							{
								CRulerDrugCode* pObj = sobj_cast<CRulerDrugCode>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();
									if (m_ptDown.x > point.x)  //x to left
									{
										if ((m_ptDown.x - point.x) >= (nRight - nLeft) - 10)
											nRight = nLeft + 10;
										else
											nRight -= (m_ptDown.x - point.x);
									}
									else//x to right
									{
										nRight += (point.x - m_ptDown.x);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulerqrcode")
							{
								CRulerQRCode* pObj = sobj_cast<CRulerQRCode>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();
									if (m_ptDown.x > point.x)  //x to left
									{
										if ((m_ptDown.x - point.x) >= (nRight - nLeft) - 10)
											nRight = nLeft + 10;
										else
											nRight -= (m_ptDown.x - point.x);
									}
									else//x to right
									{
										nRight += (point.x - m_ptDown.x);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulertext")
							{
								CRulerText* pObj = sobj_cast<CRulerText>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();
									if (m_ptDown.x > point.x)  //x to left
									{
										if ((m_ptDown.x - point.x) >= (nRight - nLeft) - 10)
											nRight = nLeft + 10;
										else
											nRight -= (m_ptDown.x - point.x);
									}
									else//x to right
									{
										nRight += (point.x - m_ptDown.x);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulerpicture")
							{
								CRulerPicture* pObj = sobj_cast<CRulerPicture>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();
									if (m_ptDown.x > point.x)  //x to left
									{
										if ((m_ptDown.x - point.x) >= (nRight - nLeft) - 10)
											nRight = nLeft + 10;
										else
											nRight -= (m_ptDown.x - point.x);
									}
									else//x to right
									{
										nRight += (point.x - m_ptDown.x);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulerpdf")
							{
								CRulerPDF* pObj = sobj_cast<CRulerPDF>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();
									if (m_ptDown.x > point.x)  //x to left
									{
										if ((m_ptDown.x - point.x) >= (nRight - nLeft) - 10)
											nRight = nLeft + 10;
										else
											nRight -= (m_ptDown.x - point.x);
									}
									else//x to right
									{
										nRight += (point.x - m_ptDown.x);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
						}
						break;
						case BottomRight:
						{
							SStringW sstrClassName = m_vecSelectObjects[0]->GetObjectClass();
							if (sstrClassName == L"rulerrect")
							{
								CRulerRectangle* pObj = sobj_cast<CRulerRectangle>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();
									if (m_ptDown.x > point.x)  //x to left
									{
										if ((m_ptDown.x - point.x) >= (nRight - nLeft) - 10)
											nRight = nLeft + 10;
										else
											nRight -= (m_ptDown.x - point.x);
									}
									else//x to right
									{
										nRight += (point.x - m_ptDown.x);
									}

									if (m_ptDown.y > point.y)//y to up
									{
										if ((m_ptDown.y - point.y) - 10 >= (nBottom - nTop))
											nBottom = nTop + 10;
										else
											nBottom -= (m_ptDown.y - point.y);
									}
									else //y to down
									{
										nBottom += (point.y - m_ptDown.y);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulerellipse")
							{
								CRulerEllipse* pObj = sobj_cast<CRulerEllipse>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();
									if (m_ptDown.x > point.x)  //x to left
									{
										if ((m_ptDown.x - point.x) >= (nRight - nLeft) - 10)
											nRight = nLeft + 10;
										else
											nRight -= (m_ptDown.x - point.x);
									}
									else//x to right
									{
										nRight += (point.x - m_ptDown.x);
									}

									if (m_ptDown.y > point.y)//y to up
									{
										if ((m_ptDown.y - point.y) - 10 >= (nBottom - nTop))
											nBottom = nTop + 10;
										else
											nBottom -= (m_ptDown.y - point.y);
									}
									else //y to down
									{
										nBottom += (point.y - m_ptDown.y);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulerbarcode")
							{
								CRulerBarCode* pObj = sobj_cast<CRulerBarCode>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();
									if (m_ptDown.x > point.x)  //x to left
									{
										if ((m_ptDown.x - point.x) >= (nRight - nLeft) - 10)
											nRight = nLeft + 10;
										else
											nRight -= (m_ptDown.x - point.x);
									}
									else//x to right
									{
										nRight += (point.x - m_ptDown.x);
									}

									if (m_ptDown.y > point.y)//y to up
									{
										if ((m_ptDown.y - point.y) - 10 >= (nBottom - nTop))
											nBottom = nTop + 10;
										else
											nBottom -= (m_ptDown.y - point.y);
									}
									else //y to down
									{
										nBottom += (point.y - m_ptDown.y);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulerdrugcode")
							{
								CRulerDrugCode* pObj = sobj_cast<CRulerDrugCode>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();
									if (m_ptDown.x > point.x)  //x to left
									{
										if ((m_ptDown.x - point.x) >= (nRight - nLeft) - 10)
											nRight = nLeft + 10;
										else
											nRight -= (m_ptDown.x - point.x);
									}
									else//x to right
									{
										nRight += (point.x - m_ptDown.x);
									}

									if (m_ptDown.y > point.y)//y to up
									{
										if ((m_ptDown.y - point.y) - 10 >= (nBottom - nTop))
											nBottom = nTop + 10;
										else
											nBottom -= (m_ptDown.y - point.y);
									}
									else //y to down
									{
										nBottom += (point.y - m_ptDown.y);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulerqrcode")
							{
								CRulerQRCode* pObj = sobj_cast<CRulerQRCode>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();
									if (m_ptDown.x > point.x)  //x to left
									{
										if ((m_ptDown.x - point.x) >= (nRight - nLeft) - 10)
											nRight = nLeft + 10;
										else
											nRight -= (m_ptDown.x - point.x);
									}
									else//x to right
									{
										nRight += (point.x - m_ptDown.x);
									}

									if (m_ptDown.y > point.y)//y to up
									{
										if ((m_ptDown.y - point.y) - 10 >= (nBottom - nTop))
											nBottom = nTop + 10;
										else
											nBottom -= (m_ptDown.y - point.y);
									}
									else //y to down
									{
										nBottom += (point.y - m_ptDown.y);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulertext")
							{
								CRulerText* pObj = sobj_cast<CRulerText>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();
									if (m_ptDown.x > point.x)  //x to left
									{
										if ((m_ptDown.x - point.x) >= (nRight - nLeft) - 10)
											nRight = nLeft + 10;
										else
											nRight -= (m_ptDown.x - point.x);
									}
									else//x to right
									{
										nRight += (point.x - m_ptDown.x);
									}

									if (m_ptDown.y > point.y)//y to up
									{
										if ((m_ptDown.y - point.y) - 10 >= (nBottom - nTop))
											nBottom = nTop + 10;
										else
											nBottom -= (m_ptDown.y - point.y);
									}
									else //y to down
									{
										nBottom += (point.y - m_ptDown.y);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulerpicture")
							{
								CRulerPicture* pObj = sobj_cast<CRulerPicture>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();
									if (m_ptDown.x > point.x)  //x to left
									{
										if ((m_ptDown.x - point.x) >= (nRight - nLeft) - 10)
											nRight = nLeft + 10;
										else
											nRight -= (m_ptDown.x - point.x);
									}
									else//x to right
									{
										nRight += (point.x - m_ptDown.x);
									}

									if (m_ptDown.y > point.y)//y to up
									{
										if ((m_ptDown.y - point.y) - 10 >= (nBottom - nTop))
											nBottom = nTop + 10;
										else
											nBottom -= (m_ptDown.y - point.y);
									}
									else //y to down
									{
										nBottom += (point.y - m_ptDown.y);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulerpdf")
							{
								CRulerPDF* pObj = sobj_cast<CRulerPDF>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();
									if (m_ptDown.x > point.x)  //x to left
									{
										if ((m_ptDown.x - point.x) >= (nRight - nLeft) - 10)
											nRight = nLeft + 10;
										else
											nRight -= (m_ptDown.x - point.x);
									}
									else//x to right
									{
										nRight += (point.x - m_ptDown.x);
									}

									if (m_ptDown.y > point.y)//y to up
									{
										if ((m_ptDown.y - point.y) - 10 >= (nBottom - nTop))
											nBottom = nTop + 10;
										else
											nBottom -= (m_ptDown.y - point.y);
									}
									else //y to down
									{
										nBottom += (point.y - m_ptDown.y);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
						}
						break;
						case BottomCenter:
						{
							SStringW sstrClassName = m_vecSelectObjects[0]->GetObjectClass();
							if (sstrClassName == L"rulerrect")
							{
								CRulerRectangle* pObj = sobj_cast<CRulerRectangle>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();

									if (m_ptDown.y > point.y)//y to up
									{
										if ((m_ptDown.y - point.y) - 10 >= (nBottom - nTop))
											nBottom = nTop + 10;
										else
											nBottom -= (m_ptDown.y - point.y);
									}
									else //y to down
									{
										nBottom += (point.y - m_ptDown.y);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulerellipse")
							{
								CRulerEllipse* pObj = sobj_cast<CRulerEllipse>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();

									if (m_ptDown.y > point.y)//y to up
									{
										if ((m_ptDown.y - point.y) - 10 >= (nBottom - nTop))
											nBottom = nTop + 10;
										else
											nBottom -= (m_ptDown.y - point.y);
									}
									else //y to down
									{
										nBottom += (point.y - m_ptDown.y);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulerbarcode")
							{
								CRulerBarCode* pObj = sobj_cast<CRulerBarCode>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();

									if (m_ptDown.y > point.y)//y to up
									{
										if ((m_ptDown.y - point.y) - 10 >= (nBottom - nTop))
											nBottom = nTop + 10;
										else
											nBottom -= (m_ptDown.y - point.y);
									}
									else //y to down
									{
										nBottom += (point.y - m_ptDown.y);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulerdrugcode")
							{
								CRulerDrugCode* pObj = sobj_cast<CRulerDrugCode>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();

									if (m_ptDown.y > point.y)//y to up
									{
										if ((m_ptDown.y - point.y) - 10 >= (nBottom - nTop))
											nBottom = nTop + 10;
										else
											nBottom -= (m_ptDown.y - point.y);
									}
									else //y to down
									{
										nBottom += (point.y - m_ptDown.y);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulerqrcode")
							{
								CRulerQRCode* pObj = sobj_cast<CRulerQRCode>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();

									if (m_ptDown.y > point.y)//y to up
									{
										if ((m_ptDown.y - point.y) - 10 >= (nBottom - nTop))
											nBottom = nTop + 10;
										else
											nBottom -= (m_ptDown.y - point.y);
									}
									else //y to down
									{
										nBottom += (point.y - m_ptDown.y);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulertext")
							{
								CRulerText* pObj = sobj_cast<CRulerText>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();

									if (m_ptDown.y > point.y)//y to up
									{
										if ((m_ptDown.y - point.y) - 10 >= (nBottom - nTop))
											nBottom = nTop + 10;
										else
											nBottom -= (m_ptDown.y - point.y);
									}
									else //y to down
									{
										nBottom += (point.y - m_ptDown.y);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulerpicture")
							{
								CRulerPicture* pObj = sobj_cast<CRulerPicture>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();

									if (m_ptDown.y > point.y)//y to up
									{
										if ((m_ptDown.y - point.y) - 10 >= (nBottom - nTop))
											nBottom = nTop + 10;
										else
											nBottom -= (m_ptDown.y - point.y);
									}
									else //y to down
									{
										nBottom += (point.y - m_ptDown.y);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulerpdf")
							{
								CRulerPDF* pObj = sobj_cast<CRulerPDF>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();

									if (m_ptDown.y > point.y)//y to up
									{
										if ((m_ptDown.y - point.y) - 10 >= (nBottom - nTop))
											nBottom = nTop + 10;
										else
											nBottom -= (m_ptDown.y - point.y);
									}
									else //y to down
									{
										nBottom += (point.y - m_ptDown.y);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
						}
						break;
						case BottomLeft:
						{
							SStringW sstrClassName = m_vecSelectObjects[0]->GetObjectClass();
							if (sstrClassName == L"rulerrect")
							{
								CRulerRectangle* pObj = sobj_cast<CRulerRectangle>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();

									if (m_ptDown.x > point.x)  //x to left
									{
										nLeft -= (m_ptDown.x - point.x);
									}
									else //x to right
									{
										if ((point.x - m_ptDown.x) - 10 > (nRight - nLeft))
										{
											nLeft = nRight - 10;
										}
										else
										{
											nLeft += (point.x - m_ptDown.x);
										}
									}

									if (m_ptDown.y > point.y)//y to up
									{
										nBottom -= (m_ptDown.y - point.y);
									}
									else //y to down
									{
										nBottom += (point.y - m_ptDown.y);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulerellipse")
							{
								CRulerEllipse* pObj = sobj_cast<CRulerEllipse>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();

									if (m_ptDown.x > point.x)  //x to left
									{
										nLeft -= (m_ptDown.x - point.x);
									}
									else //x to right
									{
										if ((point.x - m_ptDown.x) - 10 > (nRight - nLeft))
										{
											nLeft = nRight - 10;
										}
										else
										{
											nLeft += (point.x - m_ptDown.x);
										}
									}

									if (m_ptDown.y > point.y)//y to up
									{
										nBottom -= (m_ptDown.y - point.y);
									}
									else //y to down
									{
										nBottom += (point.y - m_ptDown.y);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulerbarcode")
							{
								CRulerBarCode* pObj = sobj_cast<CRulerBarCode>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();

									if (m_ptDown.x > point.x)  //x to left
									{
										nLeft -= (m_ptDown.x - point.x);
									}
									else //x to right
									{
										if ((point.x - m_ptDown.x) - 10 > (nRight - nLeft))
										{
											nLeft = nRight - 10;
										}
										else
										{
											nLeft += (point.x - m_ptDown.x);
										}
									}

									if (m_ptDown.y > point.y)//y to up
									{
										nBottom -= (m_ptDown.y - point.y);
									}
									else //y to down
									{
										nBottom += (point.y - m_ptDown.y);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulerdrugcode")
							{
								CRulerDrugCode* pObj = sobj_cast<CRulerDrugCode>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();

									if (m_ptDown.x > point.x)  //x to left
									{
										nLeft -= (m_ptDown.x - point.x);
									}
									else //x to right
									{
										if ((point.x - m_ptDown.x) - 10 > (nRight - nLeft))
										{
											nLeft = nRight - 10;
										}
										else
										{
											nLeft += (point.x - m_ptDown.x);
										}
									}

									if (m_ptDown.y > point.y)//y to up
									{
										nBottom -= (m_ptDown.y - point.y);
									}
									else //y to down
									{
										nBottom += (point.y - m_ptDown.y);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulerqrcode")
							{
								CRulerQRCode* pObj = sobj_cast<CRulerQRCode>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();

									if (m_ptDown.x > point.x)  //x to left
									{
										nLeft -= (m_ptDown.x - point.x);
									}
									else //x to right
									{
										if ((point.x - m_ptDown.x) - 10 > (nRight - nLeft))
										{
											nLeft = nRight - 10;
										}
										else
										{
											nLeft += (point.x - m_ptDown.x);
										}
									}

									if (m_ptDown.y > point.y)//y to up
									{
										nBottom -= (m_ptDown.y - point.y);
									}
									else //y to down
									{
										nBottom += (point.y - m_ptDown.y);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulertext")
							{
								CRulerText* pObj = sobj_cast<CRulerText>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();

									if (m_ptDown.x > point.x)  //x to left
									{
										nLeft -= (m_ptDown.x - point.x);
									}
									else //x to right
									{
										if ((point.x - m_ptDown.x) - 10 > (nRight - nLeft))
										{
											nLeft = nRight - 10;
										}
										else
										{
											nLeft += (point.x - m_ptDown.x);
										}
									}

									if (m_ptDown.y > point.y)//y to up
									{
										nBottom -= (m_ptDown.y - point.y);
									}
									else //y to down
									{
										nBottom += (point.y - m_ptDown.y);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulerpicture")
							{
								CRulerPicture* pObj = sobj_cast<CRulerPicture>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();

									if (m_ptDown.x > point.x)  //x to left
									{
										nLeft -= (m_ptDown.x - point.x);
									}
									else //x to right
									{
										if ((point.x - m_ptDown.x) - 10 > (nRight - nLeft))
										{
											nLeft = nRight - 10;
										}
										else
										{
											nLeft += (point.x - m_ptDown.x);
										}
									}

									if (m_ptDown.y > point.y)//y to up
									{
										nBottom -= (m_ptDown.y - point.y);
									}
									else //y to down
									{
										nBottom += (point.y - m_ptDown.y);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulerpdf")
							{
								CRulerPDF* pObj = sobj_cast<CRulerPDF>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();

									if (m_ptDown.x > point.x)  //x to left
									{
										nLeft -= (m_ptDown.x - point.x);
									}
									else //x to right
									{
										if ((point.x - m_ptDown.x) - 10 > (nRight - nLeft))
										{
											nLeft = nRight - 10;
										}
										else
										{
											nLeft += (point.x - m_ptDown.x);
										}
									}

									if (m_ptDown.y > point.y)//y to up
									{
										nBottom -= (m_ptDown.y - point.y);
									}
									else //y to down
									{
										nBottom += (point.y - m_ptDown.y);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
						}
						break;
						case LeftCenter:
						{
							SStringW sstrClassName = m_vecSelectObjects[0]->GetObjectClass();
							if (sstrClassName == L"rulerrect")
							{
								CRulerRectangle* pObj = sobj_cast<CRulerRectangle>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();

									if (m_ptDown.x > point.x)  //x to left
									{
										nLeft -= (m_ptDown.x - point.x);
									}
									else //x to right
									{
										if ((point.x - m_ptDown.x) - 10 > (nRight - nLeft))
											nLeft = nRight - 10;
										else
											nLeft += (point.x - m_ptDown.x);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulerellipse")
							{
								CRulerEllipse* pObj = sobj_cast<CRulerEllipse>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();

									if (m_ptDown.x > point.x)  //x to left
									{
										nLeft -= (m_ptDown.x - point.x);
									}
									else //x to right
									{
										if ((point.x - m_ptDown.x) - 10 > (nRight - nLeft))
											nLeft = nRight - 10;
										else
											nLeft += (point.x - m_ptDown.x);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulerbarcode")
							{
								CRulerBarCode* pObj = sobj_cast<CRulerBarCode>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();

									if (m_ptDown.x > point.x)  //x to left
									{
										nLeft -= (m_ptDown.x - point.x);
									}
									else //x to right
									{
										if ((point.x - m_ptDown.x) - 10 > (nRight - nLeft))
											nLeft = nRight - 10;
										else
											nLeft += (point.x - m_ptDown.x);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulerdrugcode")
							{
								CRulerDrugCode* pObj = sobj_cast<CRulerDrugCode>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();

									if (m_ptDown.x > point.x)  //x to left
									{
										nLeft -= (m_ptDown.x - point.x);
									}
									else //x to right
									{
										if ((point.x - m_ptDown.x) - 10 > (nRight - nLeft))
											nLeft = nRight - 10;
										else
											nLeft += (point.x - m_ptDown.x);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulerqrcode")
							{
								CRulerQRCode* pObj = sobj_cast<CRulerQRCode>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();

									if (m_ptDown.x > point.x)  //x to left
									{
										nLeft -= (m_ptDown.x - point.x);
									}
									else //x to right
									{
										if ((point.x - m_ptDown.x) - 10 > (nRight - nLeft))
											nLeft = nRight - 10;
										else
											nLeft += (point.x - m_ptDown.x);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulertext")
							{
								CRulerText* pObj = sobj_cast<CRulerText>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();

									if (m_ptDown.x > point.x)  //x to left
									{
										nLeft -= (m_ptDown.x - point.x);
									}
									else //x to right
									{
										if ((point.x - m_ptDown.x) - 10 > (nRight - nLeft))
											nLeft = nRight - 10;
										else
											nLeft += (point.x - m_ptDown.x);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulerpicture")
							{
								CRulerPicture* pObj = sobj_cast<CRulerPicture>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();

									if (m_ptDown.x > point.x)  //x to left
									{
										nLeft -= (m_ptDown.x - point.x);
									}
									else //x to right
									{
										if ((point.x - m_ptDown.x) - 10 > (nRight - nLeft))
											nLeft = nRight - 10;
										else
											nLeft += (point.x - m_ptDown.x);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
							else if (sstrClassName == L"rulerpdf")
							{
								CRulerPDF* pObj = sobj_cast<CRulerPDF>(m_vecSelectObjects[0]);
								if (pObj)
								{
									int nLeft = pObj->GetLeft();
									int nTop = pObj->GetTop();
									int nRight = pObj->GetRight();
									int nBottom = pObj->GetBottom();

									if (m_ptDown.x > point.x)  //x to left
									{
										nLeft -= (m_ptDown.x - point.x);
									}
									else //x to right
									{
										if ((point.x - m_ptDown.x) - 10 > (nRight - nLeft))
											nLeft = nRight - 10;
										else
											nLeft += (point.x - m_ptDown.x);
									}

									SStringW sstrPos;
									sstrPos.Format(L"%d,%d,@%d,@%d", nLeft, nTop, nRight - nLeft, nBottom - nTop);
									pObj->SetAttribute(L"pos", sstrPos);
									pObj->CalcPos();
								}
							}
						}
						break;
						case SelectRect:
						{
							//
						}
						break;
						default:
							break;
						}
					}
				}
			}
		}
		else
		{
			if (m_vecSelectObjects.size() == 1)
			{
				int nChildCount = GetChildrenCount();
				for (int i = 0; i < nChildCount; i++)
				{
					SWindow* objChild = GetChild(i + 1);
					SStringW sstrClassName = objChild->GetObjectClass();
					if (sstrClassName == L"rulerrect")
					{
						CRulerRectangle* pRectangle = sobj_cast<CRulerRectangle>(objChild);
						if (pRectangle)
							ShowCursor(pRectangle->HitPos(point));
					}
	 				else if (sstrClassName == L"rulerellipse")
	 				{
	 					CRulerEllipse* pEllipse = sobj_cast<CRulerEllipse>(objChild);
	 					if (pEllipse)
							ShowCursor(pEllipse->HitPos(point));
	 				}
	 				else if (sstrClassName == L"rulerbarcode")
	 				{
	 					CRulerBarCode* pBarCode = sobj_cast<CRulerBarCode>(objChild);
	 					if (pBarCode)
							ShowCursor(pBarCode->HitPos(point));
	 				}
	 				else if (sstrClassName == L"rulerdrugcode")
	 				{
	 					CRulerDrugCode* pDrugCode = sobj_cast<CRulerDrugCode>(objChild);
	 					if (pDrugCode)
							ShowCursor(pDrugCode->HitPos(point));
	 				}
	 				else if (sstrClassName == L"rulerqrcode")
	 				{
	 					CRulerQRCode* pQRCode = sobj_cast<CRulerQRCode>(objChild);
	 					if (pQRCode)
							ShowCursor(pQRCode->HitPos(point));
	 				}
	 				else if (sstrClassName == L"rulertext")
	 				{
	 					CRulerText* pText = sobj_cast<CRulerText>(objChild);
	 					if (pText)
							ShowCursor(pText->HitPos(point));
	 				}
	 				else if (sstrClassName == L"rulerpicture")
	 				{
	 					CRulerPicture* pPicture = sobj_cast<CRulerPicture>(objChild);
	 					if (pPicture)
							ShowCursor(pPicture->HitPos(point));
	 				}
	 				else if (sstrClassName == L"rulerpdf")
	 				{
	 					CRulerPDF* pPDF = sobj_cast<CRulerPDF>(objChild);
	 					if (pPDF)
							ShowCursor(pPDF->HitPos(point));
	 				}
				}
			}
		}
		Invalidate();
	}

	void CRulerBox::AddRectangle(const SOUI::CRect& rcRectangle)
	{
		CRect rc(rcRectangle);
		rc.InflateRect(2, 2, 2, 2);
		//往rulerbox中插入一个rectangle控件
		CRulerRectangle* pRectangle = (CRulerRectangle*)SApplication::getSingleton().CreateWindowByName(L"rulerrect");
		SASSERT(pRectangle);

		SApplication::getSingleton().SetSwndDefAttr(pRectangle);
		this->InsertChild(pRectangle);
		pRectangle->SSendMessage(WM_CREATE);

		SStringT sstrRectPos;
		sstrRectPos.Format(_T("%d,%d,@%d,@%d"), rc.left, rc.top, rc.Width(), rc.Height());
		pRectangle->SetAttribute(L"pos", sstrRectPos);
		pRectangle->SetAttribute(L"msgTransparent", TRUE);

		int nLeft = rc.left;
		int nTop = rc.top;
		int nRight = nLeft + rc.Width();
		int nBottom = nTop + rc.Height();
		int nWidth = rc.Width();
		int nHeight = rc.Height();

		pRectangle->SetLeft(nLeft);
		pRectangle->SetTop(nTop);
		pRectangle->SetRight(nRight);
		pRectangle->SetBottom(nBottom);
		pRectangle->SetWidth(nWidth);
		pRectangle->SetHeight(nHeight);

		pRectangle->CalcPos();
		pRectangle->Invalidate();

		pRectangle->GetEventSet()->subscribeEvent(&CRulerBox::OnEventRectangleLButtonDown, this);
		pRectangle->GetEventSet()->subscribeEvent(&CRulerBox::OnEventRectangleLButtonUp, this);
		pRectangle->GetEventSet()->subscribeEvent(&CRulerBox::OnEventRectangleLMoving, this);
	}

	bool CRulerBox::OnEventRectangleLButtonDown(EventRectangleLButtonDown* pEvt)
	{
		CRulerRectangle* pRectangle = sobj_cast<CRulerRectangle>(pEvt->sender);
		if (pRectangle)
		{
			//
		}

		return true;
	}

	bool CRulerBox::OnEventRectangleLButtonUp(EventRectangleLButtonUp* pEvt)
	{
		CRulerRectangle* pRectangle = sobj_cast<CRulerRectangle>(pEvt->sender);
		if (pRectangle)
		{
			//
		}

		return true;
	}

	bool CRulerBox::OnEventRectangleLMoving(EventRectangleMoving* pEvt)
	{
		CRulerRectangle* pRectangle = sobj_cast<CRulerRectangle>(pEvt->sender);
		if (pRectangle)
		{
			//
		}

		return true;
	}


	void CRulerBox::AddEllipse(const SOUI::CRect& rcEllipse)
	{
		CRect rc(rcEllipse);
		rc.InflateRect(2, 2, 2, 2);
		CRulerEllipse* pEllipse = (CRulerEllipse*)SApplication::getSingleton().CreateWindowByName(L"rulerellipse");
		SASSERT(pEllipse);

		SApplication::getSingleton().SetSwndDefAttr(pEllipse);
		this->InsertChild(pEllipse);
		pEllipse->SSendMessage(WM_CREATE);

		SStringT sstrRectPos;
		sstrRectPos.Format(_T("%d,%d,@%d,@%d"), rc.left, rc.top, rc.Width(), rc.Height());
		pEllipse->SetAttribute(L"pos", sstrRectPos);
		pEllipse->SetAttribute(L"msgTransparent", TRUE);

		int nLeft = rc.left;
		int nTop = rc.top;
		int nRight = nLeft + rc.Width();
		int nBottom = nTop + rc.Height();
		int nWidth = rc.Width();
		int nHeight = rc.Height();

		pEllipse->SetLeft(nLeft);
		pEllipse->SetTop(nTop);
		pEllipse->SetRight(nRight);
		pEllipse->SetBottom(nBottom);
		pEllipse->SetWidth(nWidth);
		pEllipse->SetHeight(nHeight);

		pEllipse->CalcPos();
		pEllipse->Invalidate();
	}

	void CRulerBox::AddBarCode(const SOUI::CRect& rcBarCoed)
	{
		CRect rc(rcBarCoed);
		rc.InflateRect(2, 2, 2, 2);
		CRulerBarCode* pBarCode = (CRulerBarCode*)SApplication::getSingleton().CreateWindowByName(L"rulerbarcode");
		SASSERT(pBarCode);

		SApplication::getSingleton().SetSwndDefAttr(pBarCode);
		this->InsertChild(pBarCode);
		pBarCode->SSendMessage(WM_CREATE);

		SStringT sstrRectPos;
		sstrRectPos.Format(_T("%d,%d,@%d,@%d"), rc.left, rc.top, rc.Width(), rc.Height());
		pBarCode->SetAttribute(L"pos", sstrRectPos);
		pBarCode->SetAttribute(L"msgTransparent", TRUE);

		//设置条码内容

		int nLeft = rc.left;
		int nTop = rc.top;
		int nRight = nLeft + rc.Width();
		int nBottom = nTop + rc.Height();
		int nWidth = rc.Width();
		int nHeight = rc.Height();

		pBarCode->SetLeft(nLeft);
		pBarCode->SetTop(nTop);
		pBarCode->SetRight(nRight);
		pBarCode->SetBottom(nBottom);
		pBarCode->SetWidth(nWidth);
		pBarCode->SetHeight(nHeight);

		pBarCode->CalcPos();
		pBarCode->Invalidate();
	}

	void CRulerBox::AddDrugCode(const SOUI::CRect& rcDrugCode)
	{
		CRect rc(rcDrugCode);
		rc.InflateRect(2, 2, 2, 2);
		CRulerDrugCode* pDrugCode = (CRulerDrugCode*)SApplication::getSingleton().CreateWindowByName(L"rulerdrugcode");
		SASSERT(pDrugCode);

		SApplication::getSingleton().SetSwndDefAttr(pDrugCode);
		this->InsertChild(pDrugCode);
		pDrugCode->SSendMessage(WM_CREATE);

		SStringT sstrRectPos;
		sstrRectPos.Format(_T("%d,%d,@%d,@%d"), rc.left, rc.top, rc.Width(), rc.Height());
		pDrugCode->SetAttribute(L"pos", sstrRectPos);
		pDrugCode->SetAttribute(L"msgTransparent", TRUE);

		int nLeft = rc.left;
		int nTop = rc.top;
		int nRight = nLeft + rc.Width();
		int nBottom = nTop + rc.Height();
		int nWidth = rc.Width();
		int nHeight = rc.Height();

		pDrugCode->SetLeft(nLeft);
		pDrugCode->SetTop(nTop);
		pDrugCode->SetRight(nRight);
		pDrugCode->SetBottom(nBottom);
		pDrugCode->SetWidth(nWidth);
		pDrugCode->SetHeight(nHeight);

		pDrugCode->CalcPos();
		pDrugCode->Invalidate();
	}

	void CRulerBox::AddQRCode(const SOUI::CRect& rcQRCode)
	{
		CRect rc(rcQRCode);
		rc.InflateRect(2, 2, 2, 2);
		CRulerQRCode* pQRCode = (CRulerQRCode*)SApplication::getSingleton().CreateWindowByName(L"rulerqrcode");
		SASSERT(pQRCode);

		SApplication::getSingleton().SetSwndDefAttr(pQRCode);
		this->InsertChild(pQRCode);
		pQRCode->SSendMessage(WM_CREATE);

		SStringT sstrRectPos;
		sstrRectPos.Format(_T("%d,%d,@%d,@%d"), rc.left, rc.top, rc.Width(), rc.Height());
		pQRCode->SetAttribute(L"pos", sstrRectPos);
		pQRCode->SetAttribute(L"msgTransparent", TRUE);

		int nLeft = rc.left;
		int nTop = rc.top;
		int nRight = nLeft + rc.Width();
		int nBottom = nTop + rc.Height();
		int nWidth = rc.Width();
		int nHeight = rc.Height();

		pQRCode->SetLeft(nLeft);
		pQRCode->SetTop(nTop);
		pQRCode->SetRight(nRight);
		pQRCode->SetBottom(nBottom);
		pQRCode->SetWidth(nWidth);
		pQRCode->SetHeight(nHeight);

		pQRCode->CalcPos();
		pQRCode->Invalidate();
	}

	void CRulerBox::AddText(const SOUI::CRect& rcText)
	{
		CRect rc(rcText);
		rc.InflateRect(2, 2, 2, 2);
		CRulerText* pText = (CRulerText*)SApplication::getSingleton().CreateWindowByName(L"rulertext");
		SASSERT(pText);

		SApplication::getSingleton().SetSwndDefAttr(pText);
		this->InsertChild(pText);
		pText->SSendMessage(WM_CREATE);

		SStringT sstrRectPos;
		sstrRectPos.Format(_T("%d,%d,@%d,@%d"), rc.left, rc.top, rc.Width(), rc.Height());
		pText->SetAttribute(L"pos", sstrRectPos);
		pText->SetAttribute(L"msgTransparent", TRUE);

		int nLeft = rc.left;
		int nTop = rc.top;
		int nRight = nLeft + rc.Width();
		int nBottom = nTop + rc.Height();
		int nWidth = rc.Width();
		int nHeight = rc.Height();

		pText->SetLeft(nLeft);
		pText->SetTop(nTop);
		pText->SetRight(nRight);
		pText->SetBottom(nBottom);
		pText->SetWidth(nWidth);
		pText->SetHeight(nHeight);

		pText->CalcPos();
		pText->Invalidate();
	}

	void CRulerBox::AddPicture(const SOUI::CRect& rcPicture)
	{
		CRect rc(rcPicture);
		rc.InflateRect(2, 2, 2, 2);
		CRulerPicture* pPicture = (CRulerPicture*)SApplication::getSingleton().CreateWindowByName(L"rulerpicture");
		SASSERT(pPicture);

		SApplication::getSingleton().SetSwndDefAttr(pPicture);
		this->InsertChild(pPicture);
		pPicture->SSendMessage(WM_CREATE);

		SStringT sstrRectPos;
		sstrRectPos.Format(_T("%d,%d,@%d,@%d"), rc.left, rc.top, rc.Width(), rc.Height());
		pPicture->SetAttribute(L"pos", sstrRectPos);
		pPicture->SetAttribute(L"msgTransparent", TRUE);

		int nLeft = rc.left;
		int nTop = rc.top;
		int nRight = nLeft + rc.Width();
		int nBottom = nTop + rc.Height();
		int nWidth = rc.Width();
		int nHeight = rc.Height();

		pPicture->SetLeft(nLeft);
		pPicture->SetTop(nTop);
		pPicture->SetRight(nRight);
		pPicture->SetBottom(nBottom);
		pPicture->SetWidth(nWidth);
		pPicture->SetHeight(nHeight);

		pPicture->CalcPos();
		pPicture->Invalidate();
	}

	void CRulerBox::AddPDF(const SOUI::CRect& rcPDF)
	{
		//打开文件选择，导入一个pdf文件
		//将文件路径传入控件

		CRect rc(rcPDF);
		rc.InflateRect(2, 2, 2, 2);
		CRulerPDF* pPDF = (CRulerPDF*)SApplication::getSingleton().CreateWindowByName(L"rulerpdf");
		SASSERT(pPDF);

		SApplication::getSingleton().SetSwndDefAttr(pPDF);
		this->InsertChild(pPDF);
		pPDF->SSendMessage(WM_CREATE);

		SStringT sstrRectPos;
		sstrRectPos.Format(_T("%d,%d,@%d,@%d"), rc.left, rc.top, rc.Width(), rc.Height());
		pPDF->SetAttribute(L"pos", sstrRectPos);
		pPDF->SetAttribute(L"msgTransparent", TRUE);

		int nLeft = rc.left;
		int nTop = rc.top;
		int nRight = nLeft + rc.Width();
		int nBottom = nTop + rc.Height();
		int nWidth = rc.Width();
		int nHeight = rc.Height();

		pPDF->SetLeft(nLeft);
		pPDF->SetTop(nTop);
		pPDF->SetRight(nRight);
		pPDF->SetBottom(nBottom);
		pPDF->SetWidth(nWidth);
		pPDF->SetHeight(nHeight);

		SStringW sstrPDFPath = L"GDI+USE.pdf";
		pPDF->SetPDFPath(sstrPDFPath);

		pPDF->CalcPos();
		pPDF->Invalidate();
	}

	HBITMAP CRulerBox::CopyCurBitmap(int nx, int ny, int nWidth, int nHeight)
	{
		SOUI::CRect rcClient = GetClientRect();
		IRenderTarget* pRT = GetRenderTarget(rcClient);

		HDC hDC = pRT->GetDC();
		CJPDC dcCompatible;
		dcCompatible.CreateCompatibleDC(hDC);
		dcCompatible.SelectBitmap(*m_pBitmap);
		SOUI::CRect rect(0, 0, rcClient.Width(), rcClient.Height());
		BitBlt(hDC, 0, 0, rect.Width(), rect.Height(), dcCompatible, 0, 0, SRCCOPY);

		CJPDC hMemDC;
		HBITMAP   hBitmap, hOldBitmap;
		hMemDC = CreateCompatibleDC(hDC);

		hBitmap = CreateCompatibleBitmap(hDC, nWidth, nHeight);
		hOldBitmap = hMemDC.SelectBitmap(hBitmap);
		BitBlt(hMemDC, 0, 0, nWidth, nHeight, hDC, 0, 0, SRCCOPY);
		hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);

		return hBitmap;
	}

	void CRulerBox::ShowCursor(EcPosType ePos)
	{
		switch (ePos)
		{
		case EcPosType::TopLeft:
		case EcPosType::BottomRight:
			::SetCursor(m_hCursorTopLeft);
			break;
		case EcPosType::TopCenter:
		case EcPosType::BottomCenter:
			::SetCursor(m_hCursorTop);
			break;
		case EcPosType::TopRight:
		case EcPosType::BottomLeft:
			::SetCursor(m_hCursorTopRight);
			break;
		case EcPosType::LeftCenter:
		case EcPosType::RightCenter:
			::SetCursor(m_hCursorLeft);
			break;
		case EcPosType::SelectRect:
			::SetCursor(m_hCursorSelect);
			break;
		case EcPosType::Null:
		default:
			break;
		}
	}
}
