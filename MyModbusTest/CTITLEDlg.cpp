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

// ��Ʈ���� ������ ũ��� �����ϸ��ϴ� ���� �Լ�
HBITMAP CTITLEDlg::ScaleBitmap(HBITMAP hBmpSrc, int nNewWidth, int nNewHeight)
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

BOOL CTITLEDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    const COLORREF color_text = RGB(255, 255, 255);

    // â ��ġ(x,y) ũ�� (w,h)
    MoveWindow(100, 100, 1200, 600);

    // ��ư����
    m_btnStart.EnableWindowsTheming(FALSE);

    // ��ư ��ġ����
    m_btnStart.SetWindowPos(NULL, 550, 500, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    // ��ư ������
    m_btnStart.SetFaceColor(RGB(0, 0, 0));
    m_btnStart.SetTextColor(color_text);

    // ��ư �׵θ�
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
    return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE ��ȯ
}

BOOL CTITLEDlg::OnEraseBkgnd(CDC* pDC)
{
    CRect rc;
    GetClientRect(&rc);
    // ������ ���� ȸ��(RGB 240,240,240)���� ä��
    pDC->FillSolidRect(rc, RGB(255, 255, 255));
    return TRUE;  // TRUE ��ȯ�Ͽ� �⺻ ��� �׸��⸦ ���� ����
}

void CTITLEDlg::OnBnClickedButtonStartTitle()
{
    // Ÿ��Ʋ ȭ�鿡 �ش��ϴ� ��Ʈ�ѵ��� ����ϴ�.
    GetDlgItem(IDC_STATIC_LOGO)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_BUTTON_TITLE_START)->ShowWindow(SW_HIDE);
}
