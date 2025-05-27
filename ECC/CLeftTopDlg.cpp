#include "pch.h"
#include "CLeftTopDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include "CMFRModeSelectDlg.h"
#include "SAMtestDlg.h"  // 부모 다이얼로그 포인터 호출용
#include <iostream>

IMPLEMENT_DYNAMIC(CLeftTopDlg, CDialogEx)

CLeftTopDlg::CLeftTopDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LEFT_TOP_DLG, pParent)
{
}

CLeftTopDlg::~CLeftTopDlg()
{
}

void CLeftTopDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_RADAR_ID, m_comboRadarID);
}

BEGIN_MESSAGE_MAP(CLeftTopDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_MODE_SWITCH, &CLeftTopDlg::OnBnClickedBtnModeSwitch)
	ON_CBN_SELCHANGE(IDC_COMBO_RADAR_ID, &CLeftTopDlg::OnCbnSelchangeComboRadarId)
	ON_STN_CLICKED(IDC_STATIC_MODE, &CLeftTopDlg::OnStnClickedStaticMode)
	ON_STN_CLICKED(IDC_STATIC_NETWORK, &CLeftTopDlg::OnStnClickedStaticNetwork)
	ON_STN_CLICKED(IDC_STATIC_ANGLE, &CLeftTopDlg::OnStnClickedStaticAngle)
	ON_STN_CLICKED(IDC_STATIC_POSITION, &CLeftTopDlg::OnStnClickedStaticPosition)
	ON_STN_CLICKED(IDC_STATIC_MFR_MODE, &CLeftTopDlg::OnStnClickedStaticMfrMode)
	ON_STN_CLICKED(IDC_STATIC_MFR_POSITION2, &CLeftTopDlg::OnStnClickedStaticMfrPosition2)
END_MESSAGE_MAP()

BOOL CLeftTopDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	return TRUE;
}

// ✅ 부모 다이얼로그 등록
void CLeftTopDlg::SetParentDlg(CSAMtestDlg* parent)
{
	m_parent = parent;
}

// ✅ 외부에서 레이더 리스트 설정
void CLeftTopDlg::SetRadarList(const std::vector<RadarStatus>& radarList)
{
	std::cout << "[UI] SetRadarList 호출됨, 개수: " << radarList.size() << "\n";
	m_radarList = radarList;
	m_comboRadarID.ResetContent();

	for (const auto& radar : m_radarList) { // 시험 끝나면 const 다시 넣기
		CString str;
		str.Format(_T("%d"), radar.id);
		m_comboRadarID.AddString(str);
	}

	if (!m_radarList.empty()) {
		m_comboRadarID.SetCurSel(0);
		UpdateRadarStatusFromSelection();
	}
}

// ✅ 표적 리스트 설정
void CLeftTopDlg::SetTargetList(const std::vector<TargetStatus>& targetList)
{
	m_targetList = targetList;
}

// ✅ 콤보박스 선택 변경 시
void CLeftTopDlg::OnCbnSelchangeComboRadarId()
{
	UpdateRadarStatusFromSelection();
}

// ✅ 선택한 레이더 상태 UI로 출력
void CLeftTopDlg::UpdateRadarStatusFromSelection()
{
	int sel = m_comboRadarID.GetCurSel();
	if (sel == CB_ERR || sel >= static_cast<int>(m_radarList.size())) return;

	CString strID;
	m_comboRadarID.GetLBText(sel, strID);
	int radarID = _ttoi(strID);

	for (const auto& radar : m_radarList) {
		if (radar.id == radarID) {
			m_currentRadarStatus = radar;
			SetRadarUI(radar);
			break;
		}
	}
}

// ✅ 레이더 상태 출력
void CLeftTopDlg::SetRadarUI(const RadarStatus& status)
{
	double posX = static_cast<double>(status.position.x) / 100000000.0;
	double posY = static_cast<double>(status.position.y) / 100000000.0;
	CString modeStr = (status.mode == RadarStatus::ROTATE) ? _T("회전") : _T("정지");
	CString angleStr;
	angleStr.Format(_T("%.1f"), status.angle);

	CString posXStr, posYStr;
	posXStr.Format(_T("%.8f"), posX);
	posYStr.Format(_T("%.8f"), posY);

	GetDlgItem(IDC_STATIC_MFR_MODE)->SetWindowText(modeStr);
	GetDlgItem(IDC_STATIC_MFR_ANGLE)->SetWindowText(angleStr);
	GetDlgItem(IDC_STATIC_MFR_POSITION)->SetWindowText(posXStr);
	GetDlgItem(IDC_STATIC_MFR_POSITION2)->SetWindowText(posYStr);
	GetDlgItem(IDC_STATIC_MFR_NETWORK)->SetWindowText(_T("연결됨"));
}

// ✅ 모드 전환 버튼 클릭 시
void CLeftTopDlg::OnBnClickedBtnModeSwitch()
{
	uint8_t radarId = static_cast<uint8_t>(m_currentRadarStatus.id);

	if (m_currentRadarStatus.mode == RadarStatus::ROTATE)
	{
		CMFRModeSelectDlg dlg;
		dlg.SetTargetList(m_targetList);
		dlg.SetContext(m_parent, static_cast<uint8_t>(m_currentRadarStatus.id));  // ✅ 서버 접근을 위한 context 전달
		if (dlg.DoModal() == IDOK)
		{
			int selectedTargetId = dlg.GetSelectedTargetID();
			
		}
	}
	else if (m_currentRadarStatus.mode == RadarStatus::STOP)
	{
		if (m_parent)
		{
			m_parent->sendRadarModeChange(radarId, RadarStatus::ROTATE, 0);
		}
	}
}

// ✅ ACK 수신 후 모드 갱신
void CLeftTopDlg::OnRadarModeChangeSuccess(RadarStatus::Mode newMode)
{
	m_currentRadarStatus.mode = newMode;
	SetRadarUI(m_currentRadarStatus); // UI 업데이트
}

// 나머지 클릭 핸들러 (필요 시 확장 가능)
void CLeftTopDlg::OnStnClickedStaticMode() {}
void CLeftTopDlg::OnStnClickedStaticNetwork() {}
void CLeftTopDlg::OnStnClickedStaticAngle() {}
void CLeftTopDlg::OnStnClickedStaticPosition() {}
void CLeftTopDlg::OnStnClickedStaticMfrMode() {}
void CLeftTopDlg::OnStnClickedStaticMfrPosition2(){}
