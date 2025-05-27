// CLeftBottomDlg.cpp: 구현 파일

#include "pch.h"
#include "SAMtest.h"
#include "afxdialogex.h"
#include "CLeftBottomDlg.h"
#include "CLSMoveDlg.h"
#include "CLSLaunchDlg.h"
#include "SAMtestDlg.h"  // 부모 다이얼로그 포인터 호출용
IMPLEMENT_DYNAMIC(CLeftBottomDlg, CDialogEx)

CLeftBottomDlg::CLeftBottomDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LEFT_BOTTOM_DLG, pParent)
{
}

CLeftBottomDlg::~CLeftBottomDlg()
{
}

void CLeftBottomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_LS_ID, m_comboLSID);  // ✅ 콤보박스 컨트롤 연결
}

BEGIN_MESSAGE_MAP(CLeftBottomDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_LS_ID, &CLeftBottomDlg::OnCbnSelchangeComboLsId)
	ON_BN_CLICKED(IDC_BTN_MODE_SWITCH, &CLeftBottomDlg::OnBnClickedBtnModeSwitch)
	ON_BN_CLICKED(IDC_BUTTON_MOVE, &CLeftBottomDlg::OnBnClickedButtonMove)
	ON_BN_CLICKED(IDC_BUTTON_LAUNCH, &CLeftBottomDlg::OnBnClickedButtonLaunch)
END_MESSAGE_MAP()

BOOL CLeftBottomDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 콤보박스는 SetLSList 후 설정됨
	return TRUE;
}
void CLeftBottomDlg::SetParentDlg(CSAMtestDlg* parent)
{
	m_parent = parent;
}
// ✅ 외부에서 발사대 리스트를 전달받아 콤보박스에 출력
void CLeftBottomDlg::SetLSList(const std::vector<LSStatus>& list)
{
	m_lsList = list;
	m_comboLSID.ResetContent();

	for (const auto& ls : m_lsList) {
		CString idStr;
		idStr.Format(_T("%d"), ls.id);
		m_comboLSID.AddString(idStr);
	}

	if (!m_lsList.empty()) {
		m_comboLSID.SetCurSel(0);
		UpdateLSStatusFromSelection();
	}
}

void CLeftBottomDlg::SetTargetList(const std::vector<TargetStatus>& targetList)
{
	m_targetList = targetList;
	// 콤보박스나 내부 UI에 바로 반영하려면 여기에 추가 작성
}

// ✅ 콤보박스 선택 변경 시 현재 발사대 정보 갱신
void CLeftBottomDlg::UpdateLSStatusFromSelection()
{
	int sel = m_comboLSID.GetCurSel();
	if (sel == CB_ERR || sel >= static_cast<int>(m_lsList.size())) return;

	CString strID;
	m_comboLSID.GetLBText(sel, strID);
	int id = _ttoi(strID);

	for (const auto& ls : m_lsList) {
		if (ls.id == id) {
			m_currentLSStatus = ls;
			SetLSUI(m_currentLSStatus);  // 선택된 발사대 정보로 화면 갱신
			UpdateLSModeUI();  // 버튼 상태 반영
			break;
		}
	}
}
void CLeftBottomDlg::OnLSModeChangeAck(uint8_t lsId, uint8_t newMode)
{
	// ID 일치 시 업데이트
	if (m_currentLSStatus.id == lsId) {
		m_currentLSStatus.mode = newMode;
		SetLSUI(m_currentLSStatus);
		UpdateLSModeUI();  // 버튼 상태도 갱신
	}
}

void CLeftBottomDlg::UpdateLSModeUI()
{
	auto mode = m_currentLSStatus.mode;

	// 모드 전환 버튼
	bool canSwitch = (mode == 0 || mode == 2);  // STOP or WAR
	GetDlgItem(IDC_BTN_MODE_SWITCH)->EnableWindow(canSwitch);

	// 발사 버튼: 교전 모드에서만 활성
	GetDlgItem(IDC_BUTTON_LAUNCH)->EnableWindow(mode == 2);

	// 이동 버튼: 정지 모드에서만 활성
	GetDlgItem(IDC_BUTTON_MOVE)->EnableWindow(mode == 0);
}

void CLeftBottomDlg::SetLSUI(const LSStatus& status)
{
	double posX = static_cast<double>(status.position.x) / 100000000.0;
	double posY = static_cast<double>(status.position.y) / 100000000.0;
	CString modeStr;
	switch (status.mode) {
	case LSStatus::STOP: modeStr = _T("정지"); break;
	case LSStatus::WAR:  modeStr = _T("교전"); break;
	case LSStatus::MOVE: modeStr = _T("이동"); break;
	default:             modeStr = _T("알수없음"); break;
	}

	GetDlgItem(IDC_STATIC_LS_MODE)->SetWindowText(modeStr);

	CString angleStr;
	angleStr.Format(_T("%.1f"), status.angle);
	GetDlgItem(IDC_STATIC_LS_ANGLE)->SetWindowText(angleStr);

	CString xStr, yStr;
	xStr.Format(_T("%.8f"), posX);
	yStr.Format(_T("%.8f"), posY);
	GetDlgItem(IDC_STATIC_LS_POSITION)->SetWindowText(xStr);
	GetDlgItem(IDC_STATIC_LS_POSITION2)->SetWindowText(yStr);

	// 버튼 활성화 조건
	GetDlgItem(IDC_BUTTON_LAUNCH)->EnableWindow(status.mode == LSStatus::WAR);
	GetDlgItem(IDC_BUTTON_MOVE)->EnableWindow(status.mode == LSStatus::STOP);
	GetDlgItem(IDC_BTN_MODE_SWITCH)->EnableWindow(status.mode != LSStatus::MOVE);
}


void CLeftBottomDlg::OnLSModeChangeSuccess(uint8_t lsId, uint8_t newMode)
{
	if (m_currentLSStatus.id != lsId) return;

	m_currentLSStatus.mode = newMode;

	SetLSUI(m_currentLSStatus);  // UI 업데이트 함수 호출
}


// ✅ 콤보박스 변경 이벤트 핸들러
void CLeftBottomDlg::OnCbnSelchangeComboLsId()
{
	UpdateLSStatusFromSelection();
}

// ✅ 모드 전환 버튼
void CLeftBottomDlg::OnBnClickedBtnModeSwitch()
{
	uint8_t lsId = static_cast<uint8_t>(m_currentLSStatus.id);

	if (m_currentLSStatus.mode == LSStatus::STOP)
	{
		// STOP → WAR 전환
		
		if (m_parent)
			m_parent->sendLSModeChange(lsId, LSStatus::WAR);
	}
	else if (m_currentLSStatus.mode == LSStatus::WAR)
	{
		// WAR → STOP 전환
		
		if (m_parent)
			m_parent->sendLSModeChange(lsId, LSStatus::STOP);
	}
}



// ✅ 위치 이동 다이얼로그
void CLeftBottomDlg::OnBnClickedButtonMove()
{
	CLSMoveDlg dlg;
	dlg.SetLauncherID(m_currentLSStatus.id);
	dlg.SetParent(m_parent);
	if (dlg.DoModal() == IDOK)
	{
		AfxMessageBox(_T("모드 선택 완료!"));
	}
}

// ✅ 발사 다이얼로그
void CLeftBottomDlg::OnBnClickedButtonLaunch()
{
	CLSLaunchDlg dlg;
	dlg.SetTargetList(m_targetList);
	dlg.SetLauncherID(m_currentLSStatus.id);  // ✅ 발사대 ID 전달
	dlg.SetParent(m_parent);                  // ✅ CSAMtestDlg 포인터 전달
	if (dlg.DoModal() == IDOK)
	{
		AfxMessageBox(_T("모드 선택 완료!"));
	}
}
