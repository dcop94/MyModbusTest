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
	// 메인화면 전환 플래그
	BOOL m_bIsMainMode;

	// 타이틀 컨트롤
	CStatic m_staticTitleLogoRight;
	CMFCButton m_btnStart;

	// 메인 컨트롤
	CMFCButton m_btnTCP;
	CMFCButton m_btnRtuOver;
	CMFCButton m_btnRTU;
	CMFCButton m_btnASCII;
	CStatic m_staticMainLogoLeft;
	CStatic m_staticMainLogoRight;

	// 좌측 메뉴
	CFont m_fontMenu;
	CBrush m_brushBlack;

	//TCP 다이얼로그
	CTCPChildDlg* m_pTCPCHILD;

	HBITMAP ScaleBitmap(HBITMAP hBmpSrc, int nNewWidth, int nNewHeight);


public:
	// 버튼 클릭 핸들러
	afx_msg void OnBnClickedButtonStartTitle();
	afx_msg void OnBnClickedBtnTCP();
	afx_msg void OnBnClickedBtnRtuOver();
	afx_msg void OnBnClickedBtnRTU();
	afx_msg void OnBnClickedBtnASCII();
	
	// 화면전환
	void ShowTitleControls(BOOL bShow);
	void ShowMainControls(BOOL bShow);
	void ShowTcpControls(BOOL bShow);

	// 버튼색상초기화
	void ResetButtonColors();
};

