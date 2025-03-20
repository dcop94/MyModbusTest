#include "CTITLEDlg.h"
#include <afxbutton.h>

IMPLEMENT_DYNAMIC(CTITLEDlg, CDialogEx)

CTITLEDlg::CTITLEDlg(CWnd* pParent) : CDialogEx(IDD_TITLE_DLG, pParent)
{

}
CTITLEDlg::~CTITLEDlg()
{

}

void CTITLEDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_LOGO, m_staticLogo);
	DDX_Control(pDX, IDC_BUTTON_TITLE_START, m_btnStart);
}

BEGIN_MESSAGE_MAP(CTITLEDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_TITLE_START, &CTITLEDlg::OnBnClickedButtonStartTitle)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// 비트맵을 지정된 크기로 스케일링하는 헬퍼 함수
HBITMAP CTITLEDlg::ScaleBitmap(HBITMAP hBmpSrc, int nNewWidth, int nNewHeight)
{
    if (!hBmpSrc)
        return nullptr;

    BITMAP bm;
    GetObject(hBmpSrc, sizeof(bm), &bm);

    // 소스 DC 생성
    CDC dcSrc;
    dcSrc.CreateCompatibleDC(NULL);
    HBITMAP hOldSrc = (HBITMAP)SelectObject(dcSrc.m_hDC, hBmpSrc);

    // 대상 DC 생성
    CDC dcMem;
    dcMem.CreateCompatibleDC(NULL);
    HBITMAP hBmpScaled = CreateCompatibleBitmap(dcSrc, nNewWidth, nNewHeight);
    HBITMAP hOldMem = (HBITMAP)SelectObject(dcMem.m_hDC, hBmpScaled);

    // StretchBlt로 스케일링
    StretchBlt(dcMem.m_hDC, 0, 0, nNewWidth, nNewHeight,
        dcSrc.m_hDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

    // 원래 DC 복원
    SelectObject(dcSrc.m_hDC, hOldSrc);
    SelectObject(dcMem.m_hDC, hOldMem);

    return hBmpScaled;
}

BOOL CTITLEDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    const COLORREF color_text = RGB(255, 255, 255);

    // 창 위치(x,y) 크기 (w,h)
    MoveWindow(100, 100, 1200, 600);

    // 버튼설정
    m_btnStart.EnableWindowsTheming(FALSE);

    // 버튼 위치조정
    m_btnStart.SetWindowPos(NULL, 550, 500, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    // 버튼 색설정
    m_btnStart.SetFaceColor(RGB(0, 0, 0));
    m_btnStart.SetTextColor(color_text);

    // 버튼 테두리
    m_btnStart.m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
    m_btnStart.m_bTransparent = false;

    HBITMAP hBmpLogo = (HBITMAP)::LoadImage(
        AfxGetInstanceHandle(),
        MAKEINTRESOURCE(IDB_LOGO),
        IMAGE_BITMAP,
        0, 0,
        LR_CREATEDIBSECTION
    );

    if (hBmpLogo == nullptr)
    {
        AfxMessageBox(_T("로고 이미지를 로드할 수 없습니다."));
    }
    else
    {
        // 원하는 크기로 스케일링 (예: 300x150)
        int nNewWidth = 445;
        int nNewHeight = 232;
        HBITMAP hBmpScaled = ScaleBitmap(hBmpLogo, nNewWidth, nNewHeight);
        // 원본 비트맵은 더 이상 필요 없으므로 삭제
        DeleteObject(hBmpLogo);

        // 스케일된 비트맵을 Picture Control에 설정
        m_staticLogo.ModifyStyle(0, SS_BITMAP);
        m_staticLogo.SetBitmap(hBmpScaled);

        // 로고를 대화상자 중앙에 배치
        CRect rcDlg, rcLogo;
        GetClientRect(&rcDlg);
        m_staticLogo.GetWindowRect(&rcLogo);
        ScreenToClient(&rcLogo);
        int x = (rcDlg.Width() - rcLogo.Width()) / 2;
        int y = (rcDlg.Height() - rcLogo.Height()) / 2;
        m_staticLogo.SetWindowPos(NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    }
    return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE 반환
}

BOOL CTITLEDlg::OnEraseBkgnd(CDC* pDC)
{
    CRect rc;
    GetClientRect(&rc);
    // 배경색을 연한 회색(RGB 240,240,240)으로 채움
    pDC->FillSolidRect(rc, RGB(255, 255, 255));
    return TRUE;  // TRUE 반환하여 기본 배경 그리기를 하지 않음
}

void CTITLEDlg::OnBnClickedButtonStartTitle()
{
    // 타이틀 화면에 해당하는 컨트롤들을 숨깁니다.
    GetDlgItem(IDC_STATIC_LOGO)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_BUTTON_TITLE_START)->ShowWindow(SW_HIDE);
}
