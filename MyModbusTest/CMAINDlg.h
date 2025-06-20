#pragma once
#include <afxdialogex.h>
#include <afxbutton.h>
#include "resource.h"
#include "CTCPChildDlg.h"

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
	// ����ȭ�� ��ȯ �÷���
	BOOL m_bIsMainMode;

	// Ÿ��Ʋ ��Ʈ��
	CStatic m_staticTitleLogoRight;
	CMFCButton m_btnStart;

	// ���� ��Ʈ��
	CMFCButton m_btnTCP;
	CMFCButton m_btnRtuOver;
	CMFCButton m_btnRTU;
	CMFCButton m_btnASCII;
	CStatic m_staticMainLogoLeft;
	CStatic m_staticMainLogoRight;

	// ���� �޴�
	CFont m_fontMenu;
	CBrush m_brushBlack;

	//TCP ���̾�α�
	CTCPChildDlg* m_pTCPCHILD;

	HBITMAP ScaleBitmap(HBITMAP hBmpSrc, int nNewWidth, int nNewHeight);


public:
	// ��ư Ŭ�� �ڵ鷯
	afx_msg void OnBnClickedButtonStartTitle();
	afx_msg void OnBnClickedBtnTCP();
	afx_msg void OnBnClickedBtnRtuOver();
	afx_msg void OnBnClickedBtnRTU();
	afx_msg void OnBnClickedBtnASCII();
	
	// ȭ����ȯ
	void ShowTitleControls(BOOL bShow);
	void ShowMainControls(BOOL bShow);
	void ShowTcpControls(BOOL bShow);

	// ��ư�����ʱ�ȭ
	void ResetButtonColors();
};

