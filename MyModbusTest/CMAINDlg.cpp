#include "CMAINDlg.h"

IMPLEMENT_DYNAMIC(CMAINDlg, CDialogEx)

CMAINDlg::CMAINDlg(CWnd* pParent) : CDialogEx(IDD_MAIN_DLG, pParent)
{
}

CMAINDlg::~CMAINDlg()
{

}

void CMAINDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_BTN_TCP, m_btnTCP);
    DDX_Control(pDX, IDC_BTN_RTUOVER, m_btnRtuOver);
    DDX_Control(pDX, IDC_BTN_RTU, m_btnRTU);
    DDX_Control(pDX, IDC_BTN_ASCII, m_btnASCII);

    DDX_Control(pDX, IDC_STATIC_LOGO, m_staticLogo);
}

BEGIN_MESSAGE_MAP(CMAINDlg, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_TCP, &CMAINDlg::OnBnClickedBtnTCP)
    ON_BN_CLICKED(IDC_BTN_RTUOVER, &CMAINDlg::OnBnClickedBtnRtuOver)
    ON_BN_CLICKED(IDC_BTN_RTU, &CMAINDlg::OnBnClickedBtnRTU)
    ON_BN_CLICKED(IDC_BTN_ASCII, &CMAINDlg::OnBnClickedBtnASCII)
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL CMAINDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    // 메인 화면 초기화 코드 추가 (예: 메뉴, 설정 컨트롤 등)

    //로고 로드
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


    // 창 위치(x,y) 크기 (w,h)
    MoveWindow(100, 100, 1200, 600);
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

    //왼쪽 패널
    CRect rcLeft(rc.left, rc.top, rc.left + 150, rc.bottom);
    pDC->FillSolidRect(rcLeft, RGB(0, 0, 0));

    //나머지
    CRect rcRight(rcLeft.right, rc.top, rc.right, rc.bottom);

    // 배경색을 연한 회색(RGB 240,240,240)으로 채움
    pDC->FillSolidRect(rcRight, RGB(255, 255, 255));
    return TRUE;  // TRUE 반환하여 기본 배경 그리기를 하지 않음
}

// 버튼 클릭 핸들러들
void CMAINDlg::OnBnClickedBtnTCP()
{
    AfxMessageBox(_T("TCP/IP clicked"));
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