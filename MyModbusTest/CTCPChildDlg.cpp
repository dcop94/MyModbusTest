#include "CTCPChildDlg.h"
#include "resource.h"
#include <string>

IMPLEMENT_DYNAMIC(CTCPChildDlg, CDialogEx)

CTCPChildDlg::CTCPChildDlg(CWnd* pPrarent) : CDialogEx(IDD_TCP_CHILD, pPrarent), m_nDisplayMode(0)
, m_strIP(_T(""))
, m_strPort(_T(""))
, m_strUnitId(_T(""))
, m_strFuncCode(_T(""))
, m_strStartAddr(_T(""))
, m_strQuantity(_T(""))
, m_nTxCount(0)
, m_bTimerStarted(FALSE)
{

}

CTCPChildDlg::~CTCPChildDlg()
{

}

void CTCPChildDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	// 에디트 박스
	DDX_Text(pDX, IDC_EDIT_IP, m_strIP);
	DDX_Text(pDX, IDC_EDIT_PORT, m_strPort);
	DDX_Text(pDX, IDC_EDIT_UNITID, m_strUnitId);
	DDX_Text(pDX, IDC_EDIT_FUNC, m_strFuncCode);
	DDX_Text(pDX, IDC_EDIT_ADDR, m_strStartAddr);
	DDX_Text(pDX, IDC_EDIT_QUANTITY, m_strQuantity);

	// 컨트롤 핸들
	DDX_Control(pDX, IDC_LIST_RESULT, m_listResult);
	DDX_Control(pDX, IDC_TEXT_STATIC_TITLE, m_stcTitle);
	DDX_Control(pDX, IDC_TEXT_STATIC_HEX, m_stcConvert);
	DDX_Control(pDX, IDC_BUTTON_START, m_btnStart);
	DDX_Control(pDX, IDC_BUTTON_STOP, m_btnStop);

	// 라디오 그룹
	DDX_Radio(pDX, IDC_RADIO_HEX, m_nDisplayMode);

}

BEGIN_MESSAGE_MAP(CTCPChildDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_START, &CTCPChildDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CTCPChildDlg::OnBnClickedButtonStop)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL CTCPChildDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//
	// 1) 브러시 / 폰트 준비
	//

	m_brushWhite.CreateSolidBrush(RGB(255, 255, 255)); // 배경 흰색
	m_brushEditBg.CreateSolidBrush(RGB(255, 255, 255));  // 에디트
	m_brushRadioBg.CreateSolidBrush(RGB(0, 0, 0)); // "변환값 선택" 라벨

	m_fontTitle.CreatePointFont(200, _T("Segoe UI"), 0); // "모드버스 TCP 설정"
	m_stcTitle.SetFont(&m_fontTitle);
	m_stcTitle.ModifyStyle(SS_LEFT, SS_CENTER);

	m_fontLabel.CreatePointFont(120, _T("Segoe UI")); // 필드 레이블
	m_fontEdit.CreatePointFont(120, _T("Segoe UI")); // 에디트

	m_fontRadioLabel.CreatePointFont(110, _T("Segoe UI Bold")); // "변환값 선택"
	m_stcConvert.SetFont(&m_fontRadioLabel);

	// 시작 정지 버튼
	// 12pt Bold, 흰 텍스트 / 검정 배경 / 반경 4px
	m_fontBtn.CreatePointFont(120, _T("Segoe UI Bold"));
	auto SetupWorkBtn = [&](CMFCButton& b) {
		b.EnableWindowsTheming(FALSE);
		b.m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
		b.SetFont(&m_fontBtn);
		b.SetFaceColor(RGB(0, 0, 0));
		b.SetTextColor(RGB(255, 255, 255));
		// 크기 80×30
		CRect r; b.GetWindowRect(&r); ScreenToClient(&r);
		b.SetWindowPos(NULL, r.left, r.top, 80, 30, SWP_NOZORDER);
		};
	SetupWorkBtn(m_btnStart);
	SetupWorkBtn(m_btnStop);

	// 리스트 헤더폰트
	m_fontList.CreatePointFont(110, _T("Segoe UI"));
	// 헤더(컬럼) 스타일
	if (auto pHdr = m_listResult.GetHeaderCtrl()) {
		pHdr->SetFont(&m_fontList);         // 11pt
		// 배경색은 Custom Draw 로 처리해야 하나, 기본 그리드만으로도 깔끔합니다.
	}

	// 리스트 기본 스타일
	m_listResult.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listResult.SetBkColor(RGB(255, 255, 255));
	m_listResult.SetTextColor(RGB(0x33, 0x33, 0x33));

	//
	// 2) 주요 컨트롤들의 위치 크기 계산
	// ─── 1) 리스트 컨트롤 영역, 중간 X 계산 ──────────────────────
	CRect rcList;
	m_listResult.GetWindowRect(&rcList);
	ScreenToClient(&rcList);
	int midX = (rcList.left + rcList.right) / 2;
	int topY = rcList.top - (3 * (24 + 15)   // 3행 필드 높이+간격
		+ 24 + 15        // 라디오 높이+간격
		+ 20 + 15);      // TX/ID 높이+간격
	// 필요시 topY 값은 약간 조정하세요.

	const int labelW = 100;
	const int editW = 250;
	const int fieldH = 24;
	const int hGap = 5;
	const int vGap = 15;
	const int leftIndent = rcList.left + 20;
	const int rightIndent = midX + 20;

	// ─── 2) IP/SlaveID/Address 3행 배치 ────────────────────────────
	UINT leftLbl[] = { IDC_TEXT_STATIC,       IDC_TEXT_STATIC2,     IDC_TEXT_STATIC3 };
	UINT leftEdt[] = { IDC_EDIT_IP,         IDC_EDIT_UNITID,       IDC_EDIT_ADDR };
	UINT rghtLbl[] = { IDC_TEXT_STATIC4,     IDC_TEXT_STATIC5,       IDC_TEXT_STATIC6 };
	UINT rghtEdt[] = { IDC_EDIT_PORT,       IDC_EDIT_FUNC,         IDC_EDIT_QUANTITY };

	// 세로 위치 기준 Y 계산
	int startY = rcList.top
		- (3 * (fieldH + vGap)      // 3행
			+ (fieldH + vGap)         // 라디오
			+ (fieldH + vGap));       // TX 영역
	for (int i = 0;i < 3;i++) {
		int y = startY + i * (fieldH + vGap);
		// 왼쪽
		GetDlgItem(leftLbl[i])->SetWindowPos(nullptr, leftIndent - 60, y, labelW, fieldH, SWP_NOZORDER);
		GetDlgItem(leftEdt [i] )->SetWindowPos(nullptr, (leftIndent - 60) + labelW + hGap, y, editW, fieldH, SWP_NOZORDER);
		// 오른쪽
		GetDlgItem(rghtLbl[i])->SetWindowPos(nullptr, rightIndent, y, labelW , fieldH + 100, SWP_NOZORDER);
		GetDlgItem(rghtEdt[i])->SetWindowPos(nullptr, rightIndent + labelW + hGap, y, editW, fieldH, SWP_NOZORDER);
	}

	// “변환값 선택” + 라디오 3개
	int radioY = startY + 3 * (fieldH + vGap);
	GetDlgItem(IDC_TEXT_STATIC_HEX)
		->SetWindowPos(nullptr, leftIndent, radioY, 80, fieldH, SWP_NOZORDER);
	GetDlgItem(IDC_RADIO_HEX)->SetWindowPos(nullptr, leftIndent + 110, radioY + 4, 50, 16, SWP_NOZORDER);
	GetDlgItem(IDC_RADIO_INT16)->SetWindowPos(nullptr, leftIndent + 170, radioY + 4, 60, 16, SWP_NOZORDER);
	GetDlgItem(IDC_RADIO_INT32)->SetWindowPos(nullptr, leftIndent + 240, radioY + 4, 60, 16, SWP_NOZORDER);


	// 시작/정지 버튼
	GetDlgItem(IDC_BUTTON_START)
		->SetWindowPos(nullptr, rightIndent + 190, radioY, 80, fieldH, SWP_NOZORDER);
	GetDlgItem(IDC_BUTTON_STOP)
		->SetWindowPos(nullptr, rightIndent + 275, radioY, 80, fieldH, SWP_NOZORDER);

	// “TX = … / ID = … / FC = …” 영역: 리스트 끝까지 펼치기
	int infoY = radioY + fieldH + vGap;
	int txWidth = rcList.right - leftIndent - 20;
	GetDlgItem(IDC_STATIC_TXINFO)
		->SetWindowPos(nullptr, leftIndent, infoY, txWidth, fieldH, SWP_NOZORDER);

	// 타이틀
	CWnd* pTitle = GetDlgItem(IDC_TEXT_STATIC_TITLE);
	CRect rcTitle;
	pTitle->GetWindowRect(&rcTitle);
	ScreenToClient(&rcTitle);

	int titleW = rcTitle.Width();

	int titleY = rcTitle.top - 20;

	pTitle->SetWindowPos(NULL, midX - titleW / 2, titleY, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

	//GetDlgItem(IDC_TEXT_STATIC5)->SetWindowText(_T("Function Code"));

	

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	
	

	// 레이블 일괄 설정
	UINT lbls[] = {
		IDC_TEXT_STATIC, IDC_TEXT_STATIC2,
		IDC_TEXT_STATIC3, IDC_TEXT_STATIC4,
		IDC_TEXT_STATIC5, IDC_TEXT_STATIC6
	};
	for (auto id : lbls) {
		CStatic* pS = (CStatic*)GetDlgItem(id);
		pS->SetFont(&m_fontLabel);
		// 오른쪽 정렬
		pS->ModifyStyle(SS_LEFT, SS_RIGHT);
	}

	// 에디트 일괄 설정
	UINT edits[] = {
		IDC_EDIT_IP, IDC_EDIT_PORT,
		IDC_EDIT_UNITID, IDC_EDIT_FUNC,
		IDC_EDIT_ADDR, IDC_EDIT_QUANTITY
	};
	for (auto id : edits) {
		CEdit* pE = (CEdit*)GetDlgItem(id);
		pE->SetFont(&m_fontEdit);
		// 테두리 1px 회색은 OnCtlColor() 에서 처리
	}



	

	// tx/id/fc 정보영역
	// 11pt, 회색(#333333)
	m_fontInfo.CreatePointFont(110, _T("Segoe UI"));
	CStatic* pInfo = (CStatic*)GetDlgItem(IDC_STATIC_TXINFO);
	pInfo->SetFont(&m_fontInfo);

	// 1) 리스트 컨트롤 폭 줄이기
	CRect rcListResize;
	m_listResult.GetWindowRect(&rcList);
	ScreenToClient(&rcList);
	int reduceBy = 50;  // 줄일 픽셀 수
	rcList.right = rcList.left + (rcList.Width() - reduceBy);
	m_listResult.MoveWindow(&rcList);

	// 2) 다이얼로그 전체 폭도 동일하게 줄이기
	CRect rcDlg;
	GetWindowRect(&rcDlg);
	int newWidth = rcDlg.Width() - reduceBy;
	SetWindowPos(nullptr,
		0, 0,
		newWidth,
		rcDlg.Height(),
		SWP_NOMOVE | SWP_NOZORDER);


	TRACE(_T("CTCPChildDlg OnInitDialog 호출됨\n"));

	return TRUE;
}

HBRUSH CTCPChildDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	int id = pWnd->GetDlgCtrlID();

	// 1) 에디트 컨트롤 흰배경, 어두운글자
	if (nCtlColor == CTLCOLOR_EDIT) {
		pDC->SetBkColor(RGB(255, 255, 255));
		pDC->SetTextColor(RGB(0x33, 0x33, 0x33));
		return m_brushEditBg;
	}

	// 2) 필드 레이블들 (IP, Port, SlaveID, Function, Address, Quantity)
	if (nCtlColor == CTLCOLOR_STATIC &&
		(id == IDC_TEXT_STATIC || id == IDC_TEXT_STATIC2 ||
			id == IDC_TEXT_STATIC3 || id == IDC_TEXT_STATIC4 ||
			id == IDC_TEXT_STATIC5 || id == IDC_TEXT_STATIC6))
	{
		pDC->SetBkMode(OPAQUE);
		pDC->SetBkColor(RGB(255, 255, 255));
		pDC->SetTextColor(RGB(0x33, 0x33, 0x33));
		return m_brushWhite;
	}

	// 3) 라디오 버튼(HEX/INT16/INT32)의 텍스트
	//    → CTLCOLOR_STATIC 로도 잡히는 걸 감안해서 여기에 추가
	if (nCtlColor == CTLCOLOR_STATIC &&
		(id == IDC_RADIO_HEX ||
			id == IDC_RADIO_INT16 ||
			id == IDC_RADIO_INT32))
	{
		pDC->SetBkMode(OPAQUE);
		pDC->SetBkColor(RGB(255, 255, 255));
		pDC->SetTextColor(RGB(0x33, 0x33, 0x33));
		return m_brushWhite;
	}

	// 4) “변환값 선택” 블랙 레이블
	if (nCtlColor == CTLCOLOR_STATIC && id == IDC_TEXT_STATIC_HEX) {
		pDC->SetBkMode(OPAQUE);
		pDC->SetBkColor(RGB(0, 0, 0));
		pDC->SetTextColor(RGB(255, 255, 255));
		return m_brushRadioBg;
	}

	// 5) 타이틀 “모드버스 TCP 설정”
	if (nCtlColor == CTLCOLOR_STATIC && id == IDC_TEXT_STATIC_TITLE) {
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0x33, 0x33, 0x33));
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}

	//tx~~
	if (nCtlColor == CTLCOLOR_STATIC && id == IDC_STATIC_TXINFO) {
		// OPAQUE 모드로 배경을 채우고
		pDC->SetBkMode(OPAQUE);
		pDC->SetBkColor(RGB(255, 255, 255));
		// 텍스트 색은 진한 회색
		pDC->SetTextColor(RGB(0x33, 0x33, 0x33));
		// 흰 브러시를 리턴해서 배경이 지워지도록
		return m_brushWhite;
	}


	return hbr;
}

void CTCPChildDlg::OnBnClickedButtonStart()
{
	// 한번만 수동으로 읽기
	UpdateData(TRUE);

	if (m_strIP.IsEmpty() || m_strPort.IsEmpty() || m_strFuncCode.IsEmpty() || m_strUnitId.IsEmpty() || m_strStartAddr.IsEmpty() || m_strQuantity.IsEmpty())
	{
		MessageBox(_T("모두 입력해주세요"), _T("입력 오류"), MB_ICONERROR);
		return;
	}

	DoModbusRead();

	m_nTxCount++;
	UpdateTxInfo();

	if (!m_bTimerStarted)
	{
		SetTimer(1, 1000, NULL);
		m_bTimerStarted = TRUE;
	}
}

void CTCPChildDlg::OnBnClickedButtonStop()
{
	// 정지
	if (m_bTimerStarted)
	{
		KillTimer(1);
		m_bTimerStarted = FALSE;
	}

	// TX 카운트 및 표시값 초기화
	m_nTxCount = 0;
	UpdateTxInfo();

	// 리스트초기화
	m_listResult.DeleteAllItems();
	int colCount = m_listResult.GetHeaderCtrl()->GetItemCount();
	for (int i = colCount - 1; i >= 0; i--)
	{
		m_listResult.DeleteColumn(i);
	}
}

BOOL CTCPChildDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rc;
	GetClientRect(&rc);
	pDC->FillSolidRect(&rc, RGB(255, 255, 255));
	return TRUE;
}

void CTCPChildDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		UpdateData(TRUE);
		DoModbusRead();
		m_nTxCount++;
		UpdateTxInfo();
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CTCPChildDlg::DoModbusRead()
{
	UpdateData(TRUE);

	int port = _ttoi(m_strPort);
	int unitId = _ttoi(m_strUnitId);
	int funcCode = _ttoi(m_strFuncCode);
	int startAddr = _ttoi(m_strStartAddr);
	int quantity = _ttoi(m_strQuantity);

	// IP 문자열 ASC
	CT2A asciiIP(m_strIP);
	std::string ip = asciiIP;

	// 모드버스 TCP 연결생성
	modbus_t* ctx = modbus_new_tcp(ip.c_str(), port);
	if (!ctx)
	{
		MessageBox(_T("모드버스 초기화 실패"), _T("오류"), MB_ICONERROR);
		return;
	}

	if (modbus_connect(ctx) == -1)
	{
		modbus_free(ctx);
		return;
	}

	// 유닛 ID설정
	modbus_set_slave(ctx, unitId);

	// 요청처리
	uint16_t response[256] = { 0 };
	int rc = -1;

	//Function Code 따른 요청처리
	if (funcCode == 3)
	{
		rc = modbus_read_registers(ctx, startAddr, quantity, response);
	}
	else if (funcCode == 4)
	{
		rc = modbus_read_input_registers(ctx, startAddr, quantity, response);
	}
	else
	{
		MessageBox(_T("지원되지 않는 펑션 코드입니다"), _T("오류"), MB_ICONERROR);
		modbus_close(ctx);
		modbus_free(ctx);
		return;
	}

	modbus_close(ctx);
	modbus_free(ctx);

	if (rc <= 0)
	{
		m_listResult.DeleteAllItems();
		return;

	}

	quantity = rc;

	// 리스트 컨트롤 초기화
	m_listResult.DeleteAllItems();

	// 기존 열 삭제
	int colCount = m_listResult.GetHeaderCtrl()->GetItemCount();

	for (int c = colCount - 1; c >= 0; c--)
	{
		m_listResult.DeleteColumn(c);
	}

	int sets = (quantity + 9) / 10;

	// 열생성
	for (int s = 0; s < sets; s++)
	{
		CString colAlias;
		colAlias.Format(_T("Alias[%d~%d]"), s * 10, s * 10 + 9);

		CString colAddr;
		colAddr.Format(_T("Address[%d~%d]"), s * 10, s * 10 + 9);

		CString colVal;
		colVal.Format(_T("Value[%d~%d]"), s * 10, s * 10 + 9);

		int baseCol = s * 3;
		m_listResult.InsertColumn(baseCol, colAlias, LVCFMT_LEFT, 120);
		m_listResult.InsertColumn(baseCol + 1, colAddr, LVCFMT_LEFT, 120);
		m_listResult.InsertColumn(baseCol + 2, colVal, LVCFMT_LEFT, 120);
	}

	// 행 최대 10개
	int rows = (quantity < 10) ? quantity : 10;

	for (int i = 0; i < rows; i++)
	{
		int nItem = m_listResult.InsertItem(i, _T(""));

		for (int s = 0; s < sets; s++)
		{
			int regIndex = s * 10 + i;
			if (regIndex >= quantity)
			{
				continue;
			}

			// Alias
			CString strAlias;
			strAlias.Format(_T("%d"), regIndex);
			int colAlias = s * 3;
			m_listResult.SetItemText(nItem, colAlias, strAlias);

			// Address
			CString strAddr;
			strAddr.Format(_T("%05d"), startAddr + regIndex);
			int colAddr = s * 3 + 1;
			m_listResult.SetItemText(nItem, colAddr, strAddr);

			// Value
			CString strVal;
			switch (m_nDisplayMode)
			{
			case 0:
				strVal.Format(_T("0x%04X"), response[regIndex]);
				break;

			case 1:
			{
				int16_t val16 = static_cast<int16_t>(response[regIndex]);
				strVal.Format(_T("%d"), val16);
			}
			break;

			case 2:
			{
				if (regIndex + 1 < quantity)
				{
					uint32_t high = response[regIndex];
					uint32_t low = response[regIndex + 1];
					int32_t val32 = static_cast<int32_t>((high << 16) | (low & 0xFFFF));
					strVal.Format(_T("%d"), val32);
				}
				else
				{
					strVal = _T("-");
				}
			}

			break;
			}

			int colVal = s * 3 + 2;
			m_listResult.SetItemText(nItem, colVal, strVal);
		}
	}
}

void CTCPChildDlg::UpdateTxInfo()
{
	CString strInfo;

	int unitId = _ttoi(m_strUnitId);
	int funcCode = _ttoi(m_strFuncCode);
	strInfo.Format(_T("TX = %d / ID = %d / FC = %d"), m_nTxCount, unitId, funcCode);
	SetDlgItemText(IDC_STATIC_TXINFO, strInfo);
}