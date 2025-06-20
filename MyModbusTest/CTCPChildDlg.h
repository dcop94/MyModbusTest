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
	// ������� 
	CString m_strIP;
	CString m_strPort;
	CString m_strUnitId;
	CString m_strFuncCode;
	CString m_strStartAddr;
	CString m_strQuantity;

	// ���� ����
	CStatic m_stcTitle;
	CFont m_fontTitle;

	// �ʵ巹�̺� �� ����Ʈ�ڽ�
	CFont m_fontLabel, m_fontEdit;
	CBrush m_brushEditBg;

	// ��ȯ�� ���� �����׷�
	CStatic m_stcConvert;
	CFont m_fontRadioLabel;
	CBrush m_brushRadioBg;

	// ���� ���� ��ư
	CMFCButton m_btnStart, m_btnStop;
	CFont m_fontBtn;

	// tx/id/fc ��������
	CFont m_fontInfo;

	//list font
	CFont m_fontList;

	// ��� ǥ�� List Control
	CListCtrl m_listResult;

	CBrush m_brushWhite;

	// TX COUNT
	int m_nTxCount;
	BOOL m_bTimerStarted;

	// ������ ��� �Լ�
	void DoModbusRead();

	void UpdateTxInfo();

	int m_nDisplayMode;

public:
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
};

