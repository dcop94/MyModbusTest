#include "CMAINDlg.h"

IMPLEMENT_DYNAMIC(CMAINDlg, CDialogEx)

CMAINDlg::CMAINDlg(CWnd* pParent) : CDialogEx(IDD_MAIN_DLG, pParent), m_bIsMainMode(FALSE), m_pTCPCHILD(FALSE)
{
}

CMAINDlg::~CMAINDlg()
{
    // child 다이얼로그 정리
    if (m_pTCPCHILD)
    {
        m_pTCPCHILD->DestroyWindow();
        delete m_pTCPCHILD;
        m_pTCPCHILD = nullptr;
    }
}

void CMAINDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);

    //타이틀
    DDX_Control(pDX, IDC_STATIC_TITLE_LOGO_RIGHT, m_staticTitleLogoRight);
    DDX_Control(pDX, IDC_BUTTON_TITLE_START, m_btnStart);

    //메인
    DDX_Control(pDX, IDC_BTN_TCP, m_btnTCP);
    DDX_Control(pDX, IDC_BTN_RTUOVER, m_btnRtuOver);
    DDX_Control(pDX, IDC_BTN_RTU, m_btnRTU);
    DDX_Control(pDX, IDC_BTN_ASCII, m_btnASCII);
    DDX_Control(pDX, IDC_STATIC_MAIN_LOGO_LEFT, m_staticMainLogoLeft);
    DDX_Control(pDX, IDC_STATIC_MAIN_LOGO_RIGHT, m_staticMainLogoRight);
}

BEGIN_MESSAGE_MAP(CMAINDlg, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON_TITLE_START, &CMAINDlg::OnBnClickedButtonStartTitle)
    ON_BN_CLICKED(IDC_BTN_TCP, &CMAINDlg::OnBnClickedBtnTCP)
    ON_BN_CLICKED(IDC_BTN_RTUOVER, &CMAINDlg::OnBnClickedBtnRtuOver)
    ON_BN_CLICKED(IDC_BTN_RTU, &CMAINDlg::OnBnClickedBtnRTU)
    ON_BN_CLICKED(IDC_BTN_ASCII, &CMAINDlg::OnBnClickedBtnASCII)
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL CMAINDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // 좌측메뉴 설정
    m_brushBlack.CreateSolidBrush(RGB(0, 0, 0));
    m_fontMenu.CreatePointFont(120, _T("Segoe UI Bold"));

    // — 3) 버튼 스타일·폰트·색 지정
    auto SetupMenuBtn = [&](CMFCButton& btn, COLORREF back, COLORREF text, BOOL bold) {
        btn.EnableWindowsTheming(FALSE);
        btn.m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
        btn.SetFont(&m_fontMenu);
        btn.SetFaceColor(back);
        btn.SetTextColor(text);
        };

    // 기본(비선택) 색
    SetupMenuBtn(m_btnTCP, RGB(0xF0, 0xF0, 0xF0), RGB(0x33, 0x33, 0x33), FALSE);
    SetupMenuBtn(m_btnRtuOver, RGB(0xF0, 0xF0, 0xF0), RGB(0x33, 0x33, 0x33), FALSE);
    SetupMenuBtn(m_btnRTU, RGB(0xF0, 0xF0, 0xF0), RGB(0x33, 0x33, 0x33), FALSE);
    SetupMenuBtn(m_btnASCII, RGB(0xF0, 0xF0, 0xF0), RGB(0x33, 0x33, 0x33), FALSE);

    // IDC_BTN_TCP 컨트롤 핸들 체크
    CWnd* pbtnTCP = GetDlgItem(IDC_BTN_TCP);
    if (pbtnTCP == nullptr)
    {
        TRACE(_T("오류: IDC_BTN_TCP 컨트롤을 찾을 수 없습니다.\n"));
    }
    else
    {
        TRACE(_T("IDC_BTN_TCP 컨트롤 핸들: 0x%p\n"), pbtnTCP->GetSafeHwnd());
    }
    // 공통 다이얼로그 창 위치(x,y) 크기 (w,h)
    MoveWindow(100, 100, 1200, 600);

    // 타이틀모드
    m_bIsMainMode = FALSE;
    ShowTitleControls(TRUE);
    ShowMainControls(FALSE);

    {
        // 타이틀로고 (오른쪽)
        HBITMAP hBmp = (HBITMAP)::LoadImage(
            AfxGetInstanceHandle(),
            MAKEINTRESOURCE(IDB_LOGO),
            IMAGE_BITMAP,
            0, 0,
            LR_CREATEDIBSECTION
        );
        if (hBmp)
        {
            HBITMAP hScaled = ScaleBitmap(hBmp, 445, 232);
            DeleteObject(hBmp);

            m_staticTitleLogoRight.ModifyStyle(0, SS_BITMAP);
            m_staticTitleLogoRight.SetBitmap(hScaled);

            CRect rcDlg, rcLogo;
            GetClientRect(&rcDlg);
            m_staticTitleLogoRight.GetWindowRect(&rcLogo);
            ScreenToClient(&rcLogo);
            int x = (rcDlg.Width() - rcLogo.Width()) / 2;
            int y = (rcDlg.Height() - rcLogo.Height()) / 2;
            m_staticTitleLogoRight.SetWindowPos(NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
        }
    }

    {
        // 타이틀 버튼
        m_btnStart.EnableWindowsTheming(FALSE);
        m_btnStart.SetFaceColor(RGB(0, 0, 0));
        m_btnStart.SetTextColor(RGB(255, 255, 255));
        m_btnStart.m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
        m_btnStart.m_bTransparent = false;

        // 버튼 위치 (x=550, y=500) 등
        m_btnStart.SetWindowPos(NULL, 550, 500, 100, 40, SWP_NOZORDER);
    }
    
    // ---------------
    // 메인화면
    // ---------------

    {
        HBITMAP hBmp = (HBITMAP)::LoadImage(
            AfxGetInstanceHandle(),
            MAKEINTRESOURCE(IDB_LOGO),
            IMAGE_BITMAP,
            0, 0,
            LR_CREATEDIBSECTION
        );
        if (hBmp)
        {
            // 예: 100×50으로 축소
            HBITMAP hScaled = ScaleBitmap(hBmp, 223, 116);
            DeleteObject(hBmp);

            m_staticMainLogoLeft.ModifyStyle(0, SS_BITMAP);
            m_staticMainLogoLeft.SetBitmap(hScaled);

            // 위치 (왼쪽 상단, 예: x=10, y=10)
            m_staticMainLogoLeft.SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);


        }
    }

    {
        HBITMAP hBmp = (HBITMAP)::LoadImage(
            AfxGetInstanceHandle(),
            MAKEINTRESOURCE(IDB_LOGO),
            IMAGE_BITMAP,
            0, 0,
            LR_CREATEDIBSECTION
        );
        if (hBmp)
        {
            // 예: 300×150
            HBITMAP hScaled = ScaleBitmap(hBmp, 445, 232);
            DeleteObject(hBmp);

            m_staticMainLogoRight.ModifyStyle(0, SS_BITMAP);
            m_staticMainLogoRight.SetBitmap(hScaled);

            // 중앙 배치
            CRect rcDlg, rcLogo;
            GetClientRect(&rcDlg);
            m_staticMainLogoRight.GetWindowRect(&rcLogo);
            ScreenToClient(&rcLogo);
            int x = ((rcDlg.Width()+223) - rcLogo.Width()) / 2;
            int y = (rcDlg.Height() - rcLogo.Height()) / 2;
            m_staticMainLogoRight.SetWindowPos(NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

        }
    }

    {
        // 메인버튼
        // 메인 버튼들의 위치와 크기, 색상 조절
    // 예: 버튼 크기를 120x40, 색상 배경을 파란색 등
        m_btnTCP.EnableWindowsTheming(FALSE);
        m_btnTCP.SetWindowPos(NULL, 10, 150, 213, 40, SWP_NOZORDER);
        m_btnTCP.EnableWindowsTheming(FALSE);
        m_btnTCP.SetFaceColor(RGB(255, 255, 255));
        m_btnTCP.SetTextColor(RGB(0, 0, 0));

        // 버튼 텍스트 색상은 기본값 그대로 사용
        m_btnRtuOver.EnableWindowsTheming(FALSE);
        m_btnRtuOver.SetWindowPos(NULL, 10, 220, 213, 40, SWP_NOZORDER);
        m_btnRtuOver.EnableWindowsTheming(FALSE);
        m_btnRtuOver.SetFaceColor(RGB(255, 255, 255));
        m_btnRtuOver.SetTextColor(RGB(0, 0, 0));

        m_btnRTU.EnableWindowsTheming(FALSE);
        m_btnRTU.SetWindowPos(NULL, 10, 290, 213, 40, SWP_NOZORDER);
        m_btnRTU.EnableWindowsTheming(FALSE);
        m_btnRTU.SetFaceColor(RGB(255, 255, 255));
        m_btnRTU.SetTextColor(RGB(0, 0, 0));

        m_btnASCII.EnableWindowsTheming(FALSE);
        m_btnASCII.SetWindowPos(NULL, 10, 360, 213, 40, SWP_NOZORDER);
        m_btnASCII.EnableWindowsTheming(FALSE);
        m_btnASCII.SetFaceColor(RGB(255, 255, 255));
        m_btnASCII.SetTextColor(RGB(0, 0, 0));
    }


    return TRUE;
}

// 비트맵을 지정된 크기로 스케일링하는 헬퍼 함수
HBITMAP CMAINDlg::ScaleBitmap(HBITMAP hBmpSrc, int nNewWidth, int nNewHeight)
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

BOOL CMAINDlg::OnEraseBkgnd(CDC* pDC)
{
    CRect rc;
    GetClientRect(&rc);

    if (m_bIsMainMode)
    {
        //왼쪽 패널
        CRect rcLeft(rc.left, rc.top, rc.left + 223, rc.bottom);
        pDC->FillSolidRect(rcLeft, RGB(0, 0, 0));

        //나머지
        CRect rcRight(rcLeft.right, rc.top, rc.right, rc.bottom);
        pDC->FillSolidRect(rcRight, RGB(255, 255, 255));
    }
    else
    {
        pDC->FillSolidRect(rc, RGB(255, 255, 255));
    }

    
    return TRUE;  // TRUE 반환하여 기본 배경 그리기를 하지 않음
}

void CMAINDlg::OnBnClickedButtonStartTitle()
{
    m_bIsMainMode = TRUE;
    ShowTitleControls(FALSE);
    ShowMainControls(TRUE);
    Invalidate();
}

// 버튼 클릭 핸들러들
void CMAINDlg::OnBnClickedBtnTCP()
{
    m_staticMainLogoRight.ShowWindow(SW_HIDE);

    // 버튼색상
    ResetButtonColors();
    m_btnTCP.SetFaceColor(RGB(0xFE, 0xD1, 0xB4));
    m_btnTCP.SetTextColor(RGB(0, 0, 0));

    ShowTcpControls(TRUE);
}
void CMAINDlg::OnBnClickedBtnRtuOver()
{
    AfxMessageBox(_T("RTU Over TCP clicked"));
}
void CMAINDlg::OnBnClickedBtnRTU()
{
    AfxMessageBox(_T("RTU clicked"));
}
void CMAINDlg::OnBnClickedBtnASCII()
{
    AfxMessageBox(_T("ASCII clicked"));
}

// 화면 전환용 함수
void CMAINDlg::ShowTitleControls(BOOL bShow)
{
    int nCmd = bShow ? SW_SHOW : SW_HIDE;
    // 오른쪽로고
    m_staticTitleLogoRight.ShowWindow(nCmd);
    GetDlgItem(IDC_STATIC_TITLE_LOGO_RIGHT)->ShowWindow(nCmd);
    m_btnStart.ShowWindow(nCmd);
}

void CMAINDlg::ShowMainControls(BOOL bShow)
{
    int nCmd = bShow ? SW_SHOW : SW_HIDE;
    // 왼쪽로고
    m_staticMainLogoLeft.ShowWindow(nCmd);

    // 오른쪽로고
    m_staticMainLogoRight.ShowWindow(nCmd);

    // 4버튼
    GetDlgItem(IDC_BTN_TCP)->ShowWindow(nCmd);
    GetDlgItem(IDC_BTN_RTUOVER)->ShowWindow(nCmd);
    GetDlgItem(IDC_BTN_RTU)->ShowWindow(nCmd);
    GetDlgItem(IDC_BTN_ASCII)->ShowWindow(nCmd);
    GetDlgItem(IDC_STATIC_MAIN_LOGO_LEFT)->ShowWindow(nCmd);
    GetDlgItem(IDC_STATIC_MAIN_LOGO_RIGHT)->ShowWindow(nCmd);
    

}

void CMAINDlg::ShowTcpControls(BOOL bShow)
{
    if (bShow)
    {
        if (!m_pTCPCHILD)
        {
            m_pTCPCHILD = new CTCPChildDlg;
            if (!m_pTCPCHILD->Create(IDD_TCP_CHILD, this))
            {
                AfxMessageBox(_T("TCP Child Dialog 생성실패"));
                delete m_pTCPCHILD;
                m_pTCPCHILD = nullptr;
                return;
            }
            

            // 오른쪽 영역
            CRect rc; GetClientRect(&rc);
            rc.left += 223;
            m_pTCPCHILD->SetWindowPos(NULL, rc.left, rc.top, rc.Width()  , rc.Height(), SWP_SHOWWINDOW);

        }
        else
        {
            m_pTCPCHILD->ShowWindow(SW_SHOW);
        }
    }
    else
    {
        if (m_pTCPCHILD)
        {
            m_pTCPCHILD->ShowWindow(SW_HIDE);
        }
    }
}

// 버튼 색상 초기화
void CMAINDlg::ResetButtonColors()
{
    // 모든 메뉴 버튼을 기본 색상(흰색 배경, 검정 텍스트)으로
    m_btnTCP.SetFaceColor(RGB(255, 255, 255));
    m_btnTCP.SetTextColor(RGB(0, 0, 0));

    m_btnRtuOver.SetFaceColor(RGB(255, 255, 255));
    m_btnRtuOver.SetTextColor(RGB(0, 0, 0));

    m_btnRTU.SetFaceColor(RGB(255, 255, 255));
    m_btnRTU.SetTextColor(RGB(0, 0, 0));

    m_btnASCII.SetFaceColor(RGB(255, 255, 255));
    m_btnASCII.SetTextColor(RGB(0, 0, 0));
}