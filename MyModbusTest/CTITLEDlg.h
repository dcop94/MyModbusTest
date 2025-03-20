#pragma once
#include <afxdialogex.h>
#include <afxbutton.h>
#include "resource.h"

class CTITLEDlg :public CDialogEx
{
	DECLARE_DYNAMIC(CTITLEDlg)

public:
	CTITLEDlg(CWnd* pParent = nullptr);
	virtual ~CTITLEDlg();
	enum { IDD = IDD_TITLE_DLG};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	DECLARE_MESSAGE_MAP()

private:
	CStatic m_staticLogo;
	CMFCButton m_btnStart;


public:
	afx_msg void OnBnClickedButtonStartTitle();
	HBITMAP ScaleBitmap(HBITMAP hBmpSrc, int nNewWidth, int nNewHeight);
};

