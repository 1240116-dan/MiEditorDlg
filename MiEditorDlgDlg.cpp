#include "pch.h"
#include "framework.h"
#include "MiEditorDlg.h"
#include "MiEditorDlgDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ---------------- Cuadro "Acerca de" ----------------
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX) {}
void CAboutDlg::DoDataExchange(CDataExchange* pDX) { CDialogEx::DoDataExchange(pDX); }
BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// ---------------- Diálogo principal ----------------
CMiEditorDlgDlg::CMiEditorDlgDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MIEDITORDLG_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMiEditorDlgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	// Los controles se crean por código en OnInitDialog
}

BEGIN_MESSAGE_MAP(CMiEditorDlgDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BTN_OPEN, &CMiEditorDlgDlg::OnBnClickedBtnOpen)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CMiEditorDlgDlg::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_GRAY, &CMiEditorDlgDlg::OnBnClickedBtnGray)
	ON_BN_CLICKED(IDC_BTN_CANNY, &CMiEditorDlgDlg::OnBnClickedBtnCanny)
	ON_BN_CLICKED(IDC_BTN_FLIP, &CMiEditorDlgDlg::OnBnClickedBtnFlip)
	ON_BN_CLICKED(IDC_BTN_RESET, &CMiEditorDlgDlg::OnBnClickedBtnReset)
END_MESSAGE_MAP()

BOOL CMiEditorDlgDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Elemento "Acerca de..." en el menú del sistema
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		CString strAboutMenu;
		BOOL bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	// ---------- Limpiar el diálogo vacío (texto TODO, botón Aceptar) ----------
	for (CWnd* p = GetWindow(GW_CHILD); p != nullptr; )
	{
		CWnd* next = p->GetNextWindow();
		int id = p->GetDlgCtrlID();
		if (id == IDOK)          p->ShowWindow(SW_HIDE);
		else if (id == -1)       p->DestroyWindow();   // texto "TODO"
		p = next;
	}

	// ---------- Tamaño de la ventana ----------
	CRect rcWin(0, 0, 900, 500);
	AdjustWindowRectEx(&rcWin, GetStyle(), FALSE, GetExStyle());
	SetWindowPos(nullptr, 0, 0, rcWin.Width(), rcWin.Height(),
		SWP_NOMOVE | SWP_NOZORDER);
	CenterWindow();

	CFont* pFont = GetFont();
	const DWORD wsBtn = WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON;
	const DWORD wsSld = WS_CHILD | WS_VISIBLE | WS_TABSTOP | TBS_HORZ | TBS_AUTOTICKS;
	const DWORD wsLbl = WS_CHILD | WS_VISIBLE | SS_LEFT;

	// ---------- Cuadro de imagen (640 x 480) ----------
	m_pic.Create(_T(""), WS_CHILD | WS_VISIBLE | WS_BORDER | SS_BITMAP | SS_CENTERIMAGE,
		CRect(10, 10, 650, 490), this, IDC_PIC);

	// ---------- Botones ----------
	struct BtnInfo { CButton* b; UINT id; const TCHAR* text; };
	BtnInfo btns[] = {
		{ &m_btnOpen,  IDC_BTN_OPEN,  _T("Abrir")     },
		{ &m_btnSave,  IDC_BTN_SAVE,  _T("Guardar")   },
		{ &m_btnGray,  IDC_BTN_GRAY,  _T("Grises")    },
		{ &m_btnCanny, IDC_BTN_CANNY, _T("Bordes")    },
		{ &m_btnFlip,  IDC_BTN_FLIP,  _T("Voltear")   },
		{ &m_btnReset, IDC_BTN_RESET, _T("Restaurar") },
	};
	for (int i = 0; i < 6; i++)
	{
		int y = 10 + i * 38;
		btns[i].b->Create(btns[i].text, wsBtn, CRect(670, y, 890, y + 32), this, btns[i].id);
		btns[i].b->SetFont(pFont);
	}

	// ---------- Sliders con sus etiquetas ----------
	m_lblBright.Create(_T("Brillo"), wsLbl, CRect(670, 250, 890, 268), this);
	m_sliderBright.Create(wsSld, CRect(670, 268, 890, 298), this, IDC_SLIDER_BRIGHT);

	m_lblContrast.Create(_T("Contraste"), wsLbl, CRect(670, 310, 890, 328), this);
	m_sliderContrast.Create(wsSld, CRect(670, 328, 890, 358), this, IDC_SLIDER_CONTRAST);

	m_lblBlur.Create(_T("Desenfoque"), wsLbl, CRect(670, 370, 890, 388), this);
	m_sliderBlur.Create(wsSld, CRect(670, 388, 890, 418), this, IDC_SLIDER_BLUR);

	m_lblBright.SetFont(pFont);
	m_lblContrast.SetFont(pFont);
	m_lblBlur.SetFont(pFont);

	m_sliderBright.SetRange(-100, 100);  m_sliderBright.SetPos(0);
	m_sliderContrast.SetRange(1, 30);    m_sliderContrast.SetPos(10);
	m_sliderBlur.SetRange(0, 20);        m_sliderBlur.SetPos(0);

	// ---------- Botón Cancelar convertido en "Salir" ----------
	CWnd* pCancel = GetDlgItem(IDCANCEL);
	if (pCancel)
	{
		pCancel->SetWindowText(_T("Salir"));
		pCancel->MoveWindow(670, 450, 220, 32);
		pCancel->BringWindowToTop();
	}

	// Versión de OpenCV en la barra de título (evidencia de que la librería funciona)
	SetWindowText(CString(_T("Editor de imagenes - OpenCV ")) + CString(CV_VERSION));

	return TRUE;
}

void CMiEditorDlgDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void CMiEditorDlgDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CMiEditorDlgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// =====================================================
//  INTEGRACIÓN MFC <-> OpenCV
// =====================================================

// Convierte una cv::Mat en HBITMAP y la muestra dentro de IDC_PIC
void CMiEditorDlgDlg::ShowImage(const cv::Mat& src)
{
	if (src.empty()) return;

	CRect rc;
	m_pic.GetClientRect(&rc);
	if (rc.Width() <= 0 || rc.Height() <= 0) return;

	// Ajustar al tamaño del control manteniendo la proporción
	double scale = (std::min)((double)rc.Width() / src.cols,
		(double)rc.Height() / src.rows);
	cv::Mat resized, bgr;
	cv::resize(src, resized, cv::Size(), scale, scale, cv::INTER_AREA);

	// Siempre 3 canales (por si la imagen es gris o de bordes)
	if (resized.channels() == 1)
		cv::cvtColor(resized, bgr, cv::COLOR_GRAY2BGR);
	else
		bgr = resized;

	// Cada fila de un DIB debe alinearse a 4 bytes
	int w = bgr.cols, h = bgr.rows;
	int stride = ((w * 3 + 3) / 4) * 4;
	std::vector<uchar> buf((size_t)stride * h, 0);
	for (int y = 0; y < h; y++)
		memcpy(&buf[(size_t)y * stride], bgr.ptr(y), (size_t)w * 3);

	BITMAPINFO bmi = {};
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = w;
	bmi.bmiHeader.biHeight = -h;   // negativo = de arriba hacia abajo
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 24;
	bmi.bmiHeader.biCompression = BI_RGB;

	CClientDC dc(this);
	HBITMAP hbm = CreateDIBitmap(dc.GetSafeHdc(), &bmi.bmiHeader,
		CBM_INIT, buf.data(), &bmi, DIB_RGB_COLORS);

	// Asignar al control y liberar el mapa de bits anterior
	HBITMAP old = (HBITMAP)m_pic.SendMessage(STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbm);
	if (old) DeleteObject(old);
	m_pic.Invalidate(TRUE);
}

// Recalcula todo desde la imagen original según el estado de la interfaz
void CMiEditorDlgDlg::ApplyPipeline()
{
	if (m_original.empty()) return;

	cv::Mat m = m_original.clone();

	// Brillo y contraste: salida = alpha * entrada + beta
	double alpha = m_sliderContrast.GetPos() / 10.0;   // 0.1 a 3.0
	int    beta = m_sliderBright.GetPos();            // -100 a 100
	m.convertTo(m, -1, alpha, beta);

	// Desenfoque gaussiano (el kernel debe ser impar)
	int k = m_sliderBlur.GetPos();
	if (k > 0)
		cv::GaussianBlur(m, m, cv::Size(2 * k + 1, 2 * k + 1), 0);

	if (m_gray)
		cv::cvtColor(m, m, cv::COLOR_BGR2GRAY);

	if (m_edges)
	{
		if (m.channels() == 3) cv::cvtColor(m, m, cv::COLOR_BGR2GRAY);
		cv::Canny(m, m, 80, 160);
	}

	if (m_flip)
		cv::flip(m, m, 1);   // espejo horizontal

	m_current = m;
	ShowImage(m_current);
}

// ---------------- Eventos de la interfaz ----------------
void CMiEditorDlgDlg::OnBnClickedBtnOpen()
{
	CFileDialog dlg(TRUE, nullptr, nullptr, OFN_FILEMUSTEXIST,
		_T("Imagenes|*.png;*.jpg;*.jpeg;*.bmp||"));
	if (dlg.DoModal() != IDOK) return;

	CT2A path(dlg.GetPathName());
	m_original = cv::imread(std::string(path));
	if (m_original.empty())
	{
		AfxMessageBox(_T("No se pudo cargar la imagen."));
		return;
	}
	m_gray = m_edges = m_flip = false;
	ApplyPipeline();
}

void CMiEditorDlgDlg::OnBnClickedBtnSave()
{
	if (m_current.empty()) return;

	CFileDialog dlg(FALSE, _T("png"), _T("resultado"),
		OFN_OVERWRITEPROMPT, _T("PNG|*.png|JPG|*.jpg||"));
	if (dlg.DoModal() != IDOK) return;

	CT2A path(dlg.GetPathName());
	if (cv::imwrite(std::string(path), m_current))
		AfxMessageBox(_T("Imagen guardada."));
	else
		AfxMessageBox(_T("No se pudo guardar la imagen."));
}

void CMiEditorDlgDlg::OnBnClickedBtnGray() { m_gray = !m_gray;  ApplyPipeline(); }
void CMiEditorDlgDlg::OnBnClickedBtnCanny() { m_edges = !m_edges; ApplyPipeline(); }
void CMiEditorDlgDlg::OnBnClickedBtnFlip() { m_flip = !m_flip;  ApplyPipeline(); }

void CMiEditorDlgDlg::OnBnClickedBtnReset()
{
	m_sliderBright.SetPos(0);
	m_sliderContrast.SetPos(10);
	m_sliderBlur.SetPos(0);
	m_gray = m_edges = m_flip = false;
	ApplyPipeline();
}

void CMiEditorDlgDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
	ApplyPipeline();
}