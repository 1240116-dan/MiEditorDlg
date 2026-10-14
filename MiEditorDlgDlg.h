#pragma once

// IDs de los controles (se definen aquí; si ya existen en Resource.h, se respetan)
#ifndef IDC_PIC
#define IDC_PIC             5001
#endif
#ifndef IDC_BTN_OPEN
#define IDC_BTN_OPEN        5002
#endif
#ifndef IDC_BTN_SAVE
#define IDC_BTN_SAVE        5003
#endif
#ifndef IDC_BTN_GRAY
#define IDC_BTN_GRAY        5004 
#endif
#ifndef IDC_BTN_CANNY
#define IDC_BTN_CANNY       5005
#endif
#ifndef IDC_BTN_FLIP
#define IDC_BTN_FLIP        5006
#endif
#ifndef IDC_BTN_RESET
#define IDC_BTN_RESET       5007
#endif
#ifndef IDC_SLIDER_BRIGHT
#define IDC_SLIDER_BRIGHT   5008
#endif
#ifndef IDC_SLIDER_CONTRAST
#define IDC_SLIDER_CONTRAST 5009
#endif
#ifndef IDC_SLIDER_BLUR
#define IDC_SLIDER_BLUR     5010
#endif

class CMiEditorDlgDlg : public CDialogEx
{
public:
	CMiEditorDlgDlg(CWnd* pParent = nullptr);

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MIEDITORDLG_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK() {}   // evita que Enter cierre la aplicación

	HICON m_hIcon;

	// Controles creados por código
	CStatic     m_pic;
	CButton     m_btnOpen, m_btnSave, m_btnGray, m_btnCanny, m_btnFlip, m_btnReset;
	CSliderCtrl m_sliderBright, m_sliderContrast, m_sliderBlur;
	CStatic     m_lblBright, m_lblContrast, m_lblBlur;

	// Datos de OpenCV
	cv::Mat m_original;   // imagen cargada, nunca se modifica
	cv::Mat m_current;    // imagen con los cambios aplicados
	bool m_gray = false;
	bool m_edges = false;
	bool m_flip = false;

	void ApplyPipeline();
	void ShowImage(const cv::Mat& src);

	// Mensajes
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedBtnOpen();
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnGray();
	afx_msg void OnBnClickedBtnCanny();
	afx_msg void OnBnClickedBtnFlip();
	afx_msg void OnBnClickedBtnReset();
	DECLARE_MESSAGE_MAP()
};