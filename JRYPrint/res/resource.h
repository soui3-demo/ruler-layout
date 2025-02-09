//stamp:0769f943ede78bc1
/*<------------------------------------------------------------------------------------------------->*/
/*该文件由uiresbuilder生成，请不要手动修改*/
/*<------------------------------------------------------------------------------------------------->*/
#pragma once
#include <res.mgr/snamedvalue.h>
#define ROBJ_IN_CPP \
namespace SOUI \
{\
    const _R R;\
    const _UIRES UIRES;\
}
namespace SOUI
{
	class _UIRES{
		public:
		class _UIDEF{
			public:
			_UIDEF(){
				XML_INIT = _T("UIDEF:XML_INIT");
			}
			const TCHAR * XML_INIT;
		}UIDEF;
		class _LAYOUT{
			public:
			_LAYOUT(){
				XML_MAINWND = _T("LAYOUT:XML_MAINWND");
			}
			const TCHAR * XML_MAINWND;
		}LAYOUT;
		class _values{
			public:
			_values(){
				string = _T("values:string");
				color = _T("values:color");
				skin = _T("values:skin");
			}
			const TCHAR * string;
			const TCHAR * color;
			const TCHAR * skin;
		}values;
		class _PNG{
			public:
			_PNG(){
				IDB_PNG_SHADOW = _T("PNG:IDB_PNG_SHADOW");
				IDB_PNG_BTNMIN = _T("PNG:IDB_PNG_BTNMIN");
				IDB_PNG_BTNMAX = _T("PNG:IDB_PNG_BTNMAX");
				IDB_PNG_BTNRESTORE = _T("PNG:IDB_PNG_BTNRESTORE");
				IDB_PNG_BTNCLOSE = _T("PNG:IDB_PNG_BTNCLOSE");
				IDB_PNG_BTNMENU = _T("PNG:IDB_PNG_BTNMENU");
				IDB_PNG_BTNSKIN = _T("PNG:IDB_PNG_BTNSKIN");
				IDB_PNG_TABCTRLBG = _T("PNG:IDB_PNG_TABCTRLBG");
			}
			const TCHAR * IDB_PNG_SHADOW;
			const TCHAR * IDB_PNG_BTNMIN;
			const TCHAR * IDB_PNG_BTNMAX;
			const TCHAR * IDB_PNG_BTNRESTORE;
			const TCHAR * IDB_PNG_BTNCLOSE;
			const TCHAR * IDB_PNG_BTNMENU;
			const TCHAR * IDB_PNG_BTNSKIN;
			const TCHAR * IDB_PNG_TABCTRLBG;
		}PNG;
		class _IMG{
			public:
			_IMG(){
			}
		}IMG;
		class _ICON{
			public:
			_ICON(){
				ICON_LOGO = _T("ICON:ICON_LOGO");
			}
			const TCHAR * ICON_LOGO;
		}ICON;
		class _translator{
			public:
			_translator(){
				lang_cn = _T("translator:lang_cn");
				lang_jp = _T("translator:lang_jp");
			}
			const TCHAR * lang_cn;
			const TCHAR * lang_jp;
		}translator;
	};
	const SNamedID::NAMEDVALUE namedXmlID[]={
		{L"_name_start",65535},
		{L"btn_addratio",65551},
		{L"btn_barcode",65545},
		{L"btn_close",65536},
		{L"btn_copy",65542},
		{L"btn_drugcode",65546},
		{L"btn_ellipse",65544},
		{L"btn_max",65537},
		{L"btn_min",65539},
		{L"btn_pdf",65550},
		{L"btn_picture",65549},
		{L"btn_qrcode",65547},
		{L"btn_rectangle",65543},
		{L"btn_restore",65538},
		{L"btn_subratio",65552},
		{L"btn_text",65548},
		{L"page_layout",65541},
		{L"page_print",65559},
		{L"ruler_left",65556},
		{L"ruler_top",65554},
		{L"rulerbox",65558},
		{L"scrollview",65557},
		{L"scrollview_leftruler",65555},
		{L"scrollview_topruler",65553},
		{L"tab_operate",65540}		};
	class _R{
	public:
		class _name{
		public:
		_name(){
			_name_start = namedXmlID[0].strName;
			btn_addratio = namedXmlID[1].strName;
			btn_barcode = namedXmlID[2].strName;
			btn_close = namedXmlID[3].strName;
			btn_copy = namedXmlID[4].strName;
			btn_drugcode = namedXmlID[5].strName;
			btn_ellipse = namedXmlID[6].strName;
			btn_max = namedXmlID[7].strName;
			btn_min = namedXmlID[8].strName;
			btn_pdf = namedXmlID[9].strName;
			btn_picture = namedXmlID[10].strName;
			btn_qrcode = namedXmlID[11].strName;
			btn_rectangle = namedXmlID[12].strName;
			btn_restore = namedXmlID[13].strName;
			btn_subratio = namedXmlID[14].strName;
			btn_text = namedXmlID[15].strName;
			page_layout = namedXmlID[16].strName;
			page_print = namedXmlID[17].strName;
			ruler_left = namedXmlID[18].strName;
			ruler_top = namedXmlID[19].strName;
			rulerbox = namedXmlID[20].strName;
			scrollview = namedXmlID[21].strName;
			scrollview_leftruler = namedXmlID[22].strName;
			scrollview_topruler = namedXmlID[23].strName;
			tab_operate = namedXmlID[24].strName;
		}
		 const wchar_t * _name_start;
		 const wchar_t * btn_addratio;
		 const wchar_t * btn_barcode;
		 const wchar_t * btn_close;
		 const wchar_t * btn_copy;
		 const wchar_t * btn_drugcode;
		 const wchar_t * btn_ellipse;
		 const wchar_t * btn_max;
		 const wchar_t * btn_min;
		 const wchar_t * btn_pdf;
		 const wchar_t * btn_picture;
		 const wchar_t * btn_qrcode;
		 const wchar_t * btn_rectangle;
		 const wchar_t * btn_restore;
		 const wchar_t * btn_subratio;
		 const wchar_t * btn_text;
		 const wchar_t * page_layout;
		 const wchar_t * page_print;
		 const wchar_t * ruler_left;
		 const wchar_t * ruler_top;
		 const wchar_t * rulerbox;
		 const wchar_t * scrollview;
		 const wchar_t * scrollview_leftruler;
		 const wchar_t * scrollview_topruler;
		 const wchar_t * tab_operate;
		}name;

		class _id{
		public:
		const static int _name_start	=	65535;
		const static int btn_addratio	=	65551;
		const static int btn_barcode	=	65545;
		const static int btn_close	=	65536;
		const static int btn_copy	=	65542;
		const static int btn_drugcode	=	65546;
		const static int btn_ellipse	=	65544;
		const static int btn_max	=	65537;
		const static int btn_min	=	65539;
		const static int btn_pdf	=	65550;
		const static int btn_picture	=	65549;
		const static int btn_qrcode	=	65547;
		const static int btn_rectangle	=	65543;
		const static int btn_restore	=	65538;
		const static int btn_subratio	=	65552;
		const static int btn_text	=	65548;
		const static int page_layout	=	65541;
		const static int page_print	=	65559;
		const static int ruler_left	=	65556;
		const static int ruler_top	=	65554;
		const static int rulerbox	=	65558;
		const static int scrollview	=	65557;
		const static int scrollview_leftruler	=	65555;
		const static int scrollview_topruler	=	65553;
		const static int tab_operate	=	65540;
		}id;

		class _string{
		public:
		const static int title	=	0;
		const static int ver	=	1;
		}string;

		class _color{
		public:
		const static int blue	=	0;
		const static int gray	=	1;
		const static int green	=	2;
		const static int red	=	3;
		const static int white	=	4;
		}color;

	};

#ifdef R_IN_CPP
	 extern const _R R;
	 extern const _UIRES UIRES;
#else
	 extern const __declspec(selectany) _R & R = _R();
	 extern const __declspec(selectany) _UIRES & UIRES = _UIRES();
#endif//R_IN_CPP
}
