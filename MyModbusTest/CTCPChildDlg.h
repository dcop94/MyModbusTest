#pragma once
#include <afxdialogex.h>
#include "resource.h"
#include <afxbutton.h>

extern "C"
{
	#include <modbus.h>
}

class CTCPChildDlg :public CDialogEx
{
	DECLARE_DYNAMIC(CTCPChildDlg)

public:
	CTCPChildDlg(CWnd* pParent = nullptr);
	virtual ~CTCPChildDlg();

	enum {IDD = IDD_TCP_CHILD};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()

private:
	// 멤버변수 
	CString m_strIP;
	CString m_strPort;
	CString m_strUnitId;
	CString m_strFuncCode;
	CString m_strStartAddr;
	CString m_strQuantity;

	// 제목 변수
	CStatic m_stcTitle;
	CFont m_fontTitle;

	// 필드레이블 및 에디트박스
	CFont m_fontLabel, m_fontEdit;
	CBrush m_brushEditBg;

	// 변환값 선택 라디오그룹
	CStatic m_stcConvert;
	CFont m_fontRadioLabel;
	CBrush m_brushRadioBg;

	// 시작 정지 버튼
	CMFCButton m_btnStart, m_btnStop;
	CFont m_fontBtn;

	// tx/id/fc 정보영역
	CFont m_fontInfo;

	//list font
	CFont m_fontList;

	// 결과 표시 List Control
	CListCtrl m_listResult;

	CBrush m_brushWhite;

	// TX COUNT
	int m_nTxCount;
	BOOL m_bTimerStarted;

	// 모드버스 통신 함수
	void DoModbusRead();

	void UpdateTxInfo();

	int m_nDisplayMode;

public:
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
};

