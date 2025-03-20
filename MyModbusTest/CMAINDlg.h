#pragma once
#include <afxdialogex.h>
#include <afxbutton.h>
#include "resource.h"

class CMAINDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMAINDlg)

public:
	CMAINDlg(CWnd* pParent = nullptr);
	virtual ~CMAINDlg();
	enum {IDD = IDD_MAIN_DLG};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	DECLARE_MESSAGE_MAP()

private:
	//��ư
	CMFCButton m_btnTCP;
	CMFCButton m_btnRtuOver;
	CMFCButton m_btnRTU;
	CMFCButton m_btnASCII;

	//�ΰ�
	CStatic m_staticLogo;

public:
	// ��ư Ŭ�� �ڵ鷯
	afx_msg void OnBnClickedBtnTCP();
	afx_msg void OnBnClickedBtnRtuOver();
	afx_msg void OnBnClickedBtnRTU();
	afx_msg void OnBnClickedBtnASCII();
	HBITMAP ScaleBitmap(HBITMAP hBmpSrc, int nNewWidth, int nNewHeight);
};

