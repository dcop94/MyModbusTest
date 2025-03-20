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
    // ���� ȭ�� �ʱ�ȭ �ڵ� �߰� (��: �޴�, ���� ��Ʈ�� ��)

    //�ΰ� �ε�
    HBITMAP hBmpLogo = (HBITMAP)::LoadImage(
        AfxGetInstanceHandle(),
        MAKEINTRESOURCE(IDB_LOGO),
        IMAGE_BITMAP,
        0, 0,
        LR_CREATEDIBSECTION
    );

    if (hBmpLogo == nullptr)
    {
        AfxMessageBox(_T("�ΰ� �̹����� �ε��� �� �����ϴ�."));
    }
    else
    {
        // ���ϴ� ũ��� �����ϸ� (��: 300x150)
        int nNewWidth = 445;
        int nNewHeight = 232;
        HBITMAP hBmpScaled = ScaleBitmap(hBmpLogo, nNewWidth, nNewHeight);
        // ���� ��Ʈ���� �� �̻� �ʿ� �����Ƿ� ����
        DeleteObject(hBmpLogo);

        // �����ϵ� ��Ʈ���� Picture Control�� ����
        m_staticLogo.ModifyStyle(0, SS_BITMAP);
        m_staticLogo.SetBitmap(hBmpScaled);

        // �ΰ� ��ȭ���� �߾ӿ� ��ġ
        CRect rcDlg, rcLogo;
        GetClientRect(&rcDlg);
        m_staticLogo.GetWindowRect(&rcLogo);
        ScreenToClient(&rcLogo);
        int x = (rcDlg.Width() - rcLogo.Width()) / 2;
        int y = (rcDlg.Height() - rcLogo.Height()) / 2;
        m_staticLogo.SetWindowPos(NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    }


    // â ��ġ(x,y) ũ�� (w,h)
    MoveWindow(100, 100, 1200, 600);
    return TRUE;
}

// ��Ʈ���� ������ ũ��� �����ϸ��ϴ� ���� �Լ�
HBITMAP CMAINDlg::ScaleBitmap(HBITMAP hBmpSrc, int nNewWidth, int nNewHeight)
{
    if (!hBmpSrc)
        return nullptr;

    BITMAP bm;
    GetObject(hBmpSrc, sizeof(bm), &bm);

    // �ҽ� DC ����
    CDC dcSrc;
    dcSrc.CreateCompatibleDC(NULL);
    HBITMAP hOldSrc = (HBITMAP)SelectObject(dcSrc.m_hDC, hBmpSrc);

    // ��� DC ����
    CDC dcMem;
    dcMem.CreateCompatibleDC(NULL);
    HBITMAP hBmpScaled = CreateCompatibleBitmap(dcSrc, nNewWidth, nNewHeight);
    HBITMAP hOldMem = (HBITMAP)SelectObject(dcMem.m_hDC, hBmpScaled);

    // StretchBlt�� �����ϸ�
    StretchBlt(dcMem.m_hDC, 0, 0, nNewWidth, nNewHeight,
        dcSrc.m_hDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

    // ���� DC ����
    SelectObject(dcSrc.m_hDC, hOldSrc);
    SelectObject(dcMem.m_hDC, hOldMem);

    return hBmpScaled;
}

BOOL CMAINDlg::OnEraseBkgnd(CDC* pDC)
{
    CRect rc;
    GetClientRect(&rc);

    //���� �г�
    CRect rcLeft(rc.left, rc.top, rc.left + 150, rc.bottom);
    pDC->FillSolidRect(rcLeft, RGB(0, 0, 0));

    //������
    CRect rcRight(rcLeft.right, rc.top, rc.right, rc.bottom);

    // ������ ���� ȸ��(RGB 240,240,240)���� ä��
    pDC->FillSolidRect(rcRight, RGB(255, 255, 255));
    return TRUE;  // TRUE ��ȯ�Ͽ� �⺻ ��� �׸��⸦ ���� ����
}

// ��ư Ŭ�� �ڵ鷯��
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