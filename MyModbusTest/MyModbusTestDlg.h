#pragma once
#include "resource.h"

class CMyModbusTestDlg : public CDialogEx
{
public:
    CMyModbusTestDlg(CWnd* pParent = nullptr);  // 표준 생성자입니다.
    enum { IDD = IDD_MAIN_DLG };  // 수정: IDD_TITLE_DLG 대신 IDD_MAIN_DLG 사용

protected:
    virtual void DoDataExchange(CDataExchange* pDX);  // DDX/DDV 지원
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()

private:
    HICON m_hIcon;
};
