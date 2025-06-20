#include "pch.h"
#include "SAMtest.h"
#include "CTargetInfoDlg.h"
#include "afxdialogex.h"
#include <algorithm>
#include <chrono>      // ✅ chrono 사용
#include <ctime>

IMPLEMENT_DYNAMIC(CTargetInfoDlg, CDialogEx)

CTargetInfoDlg::CTargetInfoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TARGET_INFO_DLG, pParent)
{
}

CTargetInfoDlg::~CTargetInfoDlg()
{
}

void CTargetInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_TARGET_ID, m_comboTargetID);
	DDX_Control(pDX, IDC_STATIC_TARGET_ID2, m_staticID);
	DDX_Control(pDX, IDC_STATIC_TARGET_POSITIONX, m_staticPosX);
	DDX_Control(pDX, IDC_STATIC_TARGET_POSITIONY, m_staticPosY);
	DDX_Control(pDX, IDC_STATIC_TARGET_HEIGHT2, m_staticPosZ);
	DDX_Control(pDX, IDC_STATIC_TARGET_SPEED2, m_staticSpeed);
	DDX_Control(pDX, IDC_STATIC_TARGET_ANGLE2, m_staticAngle1);
	DDX_Control(pDX, IDC_STATIC_TARGET_ANGLE3, m_staticAngle2);
	DDX_Control(pDX, IDC_STATIC_TARGET_PRIORITY2, m_staticPriority);
	DDX_Control(pDX, IDC_STATIC_TARGET_DETECT2, m_staticDetect);
	DDX_Control(pDX, IDC_STATIC_TARGET_HIT2, m_staticHit);
}

BEGIN_MESSAGE_MAP(CTargetInfoDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_TARGET_ID, &CTargetInfoDlg::OnCbnSelchangeComboTargetId)
	ON_STN_CLICKED(IDC_STATIC_TARGET_ID2, &CTargetInfoDlg::OnStnClickedStaticTargetId2)
	ON_STN_CLICKED(IDC_STATIC_TARGET_HEIGHT2, &CTargetInfoDlg::OnStnClickedStaticTargetHeight2)
	ON_STN_CLICKED(IDC_STATIC_TARGET_SPEED2, &CTargetInfoDlg::OnStnClickedStaticTargetSpeed2)
	ON_STN_CLICKED(IDC_STATIC_TARGET_ANGLE2, &CTargetInfoDlg::OnStnClickedStaticTargetAngle2)
	ON_STN_CLICKED(IDC_STATIC_TARGET_PRIORITY2, &CTargetInfoDlg::OnStnClickedStaticTargetPriority2)
	ON_STN_CLICKED(IDC_STATIC_TARGET_DETECT2, &CTargetInfoDlg::OnStnClickedStaticTargetDetect2)
	ON_STN_CLICKED(IDC_STATIC_TARGET_HIT2, &CTargetInfoDlg::OnStnClickedStaticTargetHit2)
	ON_STN_CLICKED(IDC_STATIC_TARGET_ANGLE3, &CTargetInfoDlg::OnStnClickedStaticTargetAngle3)
END_MESSAGE_MAP()

BOOL CTargetInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	return TRUE;
}

void CTargetInfoDlg::SetTargetList(const std::vector<TargetStatus>& targets)
{
	// 🔴 타겟이 하나도 없는 경우
	if (targets.empty()) {
		m_comboTargetID.ResetContent();
		m_targetList.clear();

		// 모든 Static 텍스트를 "N/A"로 초기화
		m_staticID.SetWindowText(_T("N/A"));
		m_staticPosX.SetWindowText(_T("N/A"));
		m_staticPosY.SetWindowText(_T("N/A"));
		m_staticPosZ.SetWindowText(_T("N/A"));
		m_staticSpeed.SetWindowText(_T("N/A"));
		m_staticAngle1.SetWindowText(_T("N/A"));
		m_staticAngle2.SetWindowText(_T("N/A"));
		m_staticPriority.SetWindowText(_T("N/A"));
		m_staticDetect.SetWindowText(_T("N/A"));
		m_staticHit.SetWindowText(_T("N/A"));
		return;
	}

	// 🔁 기존대로 우선순위 기준 정렬
	CString selectedIDStr;
	int selIndex = m_comboTargetID.GetCurSel();
	if (selIndex != CB_ERR)
		m_comboTargetID.GetLBText(selIndex, selectedIDStr);

	std::vector<TargetStatus> sortedTargets = targets;
	std::sort(sortedTargets.begin(), sortedTargets.end(), [](const TargetStatus& a, const TargetStatus& b) {
		return a.priority > b.priority;
		});

	// 🧠 변경 여부 판단
	bool needUpdate = (sortedTargets.size() != m_targetList.size());
	if (!needUpdate) {
		for (size_t i = 0; i < sortedTargets.size(); ++i) {
			if (sortedTargets[i].id != m_targetList[i].id ||
				sortedTargets[i].priority != m_targetList[i].priority) {
				needUpdate = true;
				break;
			}
		}
	}

	if (needUpdate) {
		m_comboTargetID.ResetContent();
		int restoreIndex = 0;

		for (size_t i = 0; i < sortedTargets.size(); ++i) {
			CString idStr;
			idStr.Format(_T("%d"), sortedTargets[i].id);
			m_comboTargetID.AddString(idStr);
			if (idStr == selectedIDStr)
				restoreIndex = static_cast<int>(i);
		}

		m_comboTargetID.SetCurSel(restoreIndex);
		m_targetList = sortedTargets;
	}
	else {
		m_targetList = sortedTargets;
	}

	UpdateUIFromSelection();  // 현재 선택된 타겟 기준으로 UI 갱신
}


void CTargetInfoDlg::UpdateUIFromSelection()
{
	int sel = m_comboTargetID.GetCurSel();
	if (sel == CB_ERR || sel >= m_targetList.size()) return;

	const TargetStatus& t = m_targetList[sel];
	CString str;

	str.Format(_T("%d"), t.id);
	m_staticID.SetWindowText(str);

	str.Format(_T("%.8f"), static_cast<double>(t.position.x) / 10000000.0);
	m_staticPosX.SetWindowText(str);

	str.Format(_T("%.8f"), static_cast<double>(t.position.y) / 10000000.0);
	m_staticPosY.SetWindowText(str);

	str.Format(_T("%lld"), t.position.z);
	m_staticPosZ.SetWindowText(str);

	str.Format(_T("%d"), t.speed);
	m_staticSpeed.SetWindowText(str);

	str.Format(_T("%.1f"), t.angle1);
	m_staticAngle1.SetWindowText(str);

	str.Format(_T("%.1f"), t.angle2);
	m_staticAngle2.SetWindowText(str);

	str.Format(_T("%d"), t.priority);
	m_staticPriority.SetWindowText(str);

	// ✅ UTC → KST 시간 변환
	//str = FormatUtcToKST(t.first_detect_time);
	str.Format(_T("%lld"), t.first_detect_time);  // 시간 출력
	m_staticDetect.SetWindowText(str);

	str.Format(_T("%d"), static_cast<int>(t.hit));
	m_staticHit.SetWindowText(str);
}

// ✅ UTC → KST 변환 함수 (C++17 chrono 기반)
//CString FormatUtcToKST(unsigned long long utc_seconds)
//{
//	using namespace std::chrono;
//
//	system_clock::time_point utc_time = system_clock::time_point(seconds(utc_seconds));
//	system_clock::time_point kst_time = utc_time + hours(9);
//
//	time_t tt = system_clock::to_time_t(kst_time);
//	struct tm tm_kst;
//	localtime_s(&tm_kst, &tt);
//
//	CString result;
//	result.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"),
//		tm_kst.tm_year + 1900,
//		tm_kst.tm_mon + 1,
//		tm_kst.tm_mday,
//		tm_kst.tm_hour,
//		tm_kst.tm_min,
//		tm_kst.tm_sec);
//	return result;
//}

// 클릭 핸들러 (비워둠 또는 로그 출력용)
void CTargetInfoDlg::OnCbnSelchangeComboTargetId() { UpdateUIFromSelection(); }
void CTargetInfoDlg::OnStnClickedStaticTargetId2() {}
void CTargetInfoDlg::OnStnClickedStaticTargetHeight2() {}
void CTargetInfoDlg::OnStnClickedStaticTargetSpeed2() {}
void CTargetInfoDlg::OnStnClickedStaticTargetAngle2() {}
void CTargetInfoDlg::OnStnClickedStaticTargetPriority2() {}
void CTargetInfoDlg::OnStnClickedStaticTargetDetect2() {}
void CTargetInfoDlg::OnStnClickedStaticTargetHit2() {}
void CTargetInfoDlg::OnStnClickedStaticTargetAngle3() {}
