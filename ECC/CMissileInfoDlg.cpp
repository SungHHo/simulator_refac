#include "pch.h"
#include "SAMtest.h"
#include "CMissileInfoDlg.h"
#include "afxdialogex.h"
#include <chrono>

IMPLEMENT_DYNAMIC(CMissileInfoDlg, CDialogEx)

CMissileInfoDlg::CMissileInfoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MISSILE_INFO_DLG, pParent)
{
}

CMissileInfoDlg::~CMissileInfoDlg()
{
}

void CMissileInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	// DDX 컨트롤 바인딩
	DDX_Control(pDX, IDC_STATIC_MISSILE_ID2, m_staticID);
	DDX_Control(pDX, IDC_STATIC_MISSILE_POSITIONX, m_staticPosX);
	DDX_Control(pDX, IDC_STATIC_MISSILE_POSITIONY, m_staticPosY);
	DDX_Control(pDX, IDC_STATIC_MISSILE_HEIGHT2, m_staticPosZ);
	DDX_Control(pDX, IDC_STATIC_MISSILE_SPEED2, m_staticSpeed);
	DDX_Control(pDX, IDC_STATIC_MISSILE_ANGLE2, m_staticAngle);
	DDX_Control(pDX, IDC_STATIC_MISSILE_SHOOTDOWNTIME2, m_staticShootTime);
	DDX_Control(pDX, IDC_LIST_MISSILE_INFO, m_listMissileInfo);
}

BEGIN_MESSAGE_MAP(CMissileInfoDlg, CDialogEx)
	ON_STN_CLICKED(IDC_STATIC_MISSILE_ID2, &CMissileInfoDlg::OnStnClickedStaticMissileId2)
	ON_STN_CLICKED(IDC_STATIC_MISSILE_POSITIONX, &CMissileInfoDlg::OnStnClickedStaticMissilePositionx)
	ON_STN_CLICKED(IDC_STATIC_MISSILE_POSITIONY, &CMissileInfoDlg::OnStnClickedStaticMissilePositiony)
	ON_STN_CLICKED(IDC_STATIC_MISSILE_HEIGHT2, &CMissileInfoDlg::OnStnClickedStaticMissileHeight2)
	ON_STN_CLICKED(IDC_STATIC_MISSILE_SPEED2, &CMissileInfoDlg::OnStnClickedStaticMissileSpeed2)
	ON_STN_CLICKED(IDC_STATIC_MISSILE_ANGLE2, &CMissileInfoDlg::OnStnClickedStaticMissileAngle2)
	ON_STN_CLICKED(IDC_STATIC_MISSILE_SHOOTDOWNTIME2, &CMissileInfoDlg::OnStnClickedStaticMissileShootdowntime2)
END_MESSAGE_MAP()

BOOL CMissileInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_listMissileInfo.MoveWindow(10, 10, 650, 320);

	m_listMissileInfo.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listMissileInfo.InsertColumn(0, _T("ID"), LVCFMT_LEFT, 50);
	m_listMissileInfo.InsertColumn(1, _T("위도"), LVCFMT_LEFT, 100);
	m_listMissileInfo.InsertColumn(2, _T("경도"), LVCFMT_LEFT, 100);
	m_listMissileInfo.InsertColumn(3, _T("고도"), LVCFMT_LEFT, 50);
	m_listMissileInfo.InsertColumn(4, _T("속도"), LVCFMT_LEFT, 50);
	m_listMissileInfo.InsertColumn(5, _T("각도"), LVCFMT_LEFT, 50);
	m_listMissileInfo.InsertColumn(6, _T("격추"), LVCFMT_LEFT, 50);

	return TRUE;
}

void CMissileInfoDlg::SetMissileStatus(const MissileStatus& status)
{
	// 남은 시간 계산
	using namespace std::chrono;
	auto now_ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	m_secondsRemaining = static_cast<int>((status.predicted_time - now_ms) / 1000);
	if (m_secondsRemaining < 0) m_secondsRemaining = 0;

	m_missileStatus = status;

	// 유효하지 않은 ID는 무시
	if (status.id == 0)
	{
		return;
	}		

	CString str;
	int rowIndex = m_listMissileInfo.GetItemCount();

	// ID
	str.Format(_T("%d"), status.id);
	m_listMissileInfo.InsertItem(rowIndex, str);

	// 위도(x)
	str.Format(_T("%.8f"), static_cast<double>(status.position.x) / coordsScale);
	m_listMissileInfo.SetItemText(rowIndex, 1, str);

	// 경도(y)
	str.Format(_T("%.8f"), static_cast<double>(status.position.y) / coordsScale);
	m_listMissileInfo.SetItemText(rowIndex, 2, str);

	// 고도(z)
	str.Format(_T("%.2f"), static_cast<double>(status.position.z) / coordsScale);
	m_listMissileInfo.SetItemText(rowIndex, 3, str);

	// 각도
	str.Format(_T("%.1f"), static_cast<double>(status.angle));
	m_listMissileInfo.SetItemText(rowIndex, 5, str);

	// 연결 상태
	m_listMissileInfo.SetItemText(rowIndex, 6, status.hit ? _T("격추 성공") : _T("미격추"));

	UpdateUI();

	// 타이머 시작
	SetTimer(TIMER_ID_MISSILE, 1000, nullptr);
}


void CMissileInfoDlg::UpdateUI()
{
	CString str;
	// 격추까지 남은 시간 출력
	if (m_secondsRemaining > 0)
	{
		int roundedSeconds = (m_secondsRemaining / 3) * 3;
		str.Format(_T("약%d초 남음"), roundedSeconds + 3);
	}
	else
	{
		str = _T("격추 완료 또는 시간 초과");
	}
	m_staticShootTime.SetWindowText(str);
}

void CMissileInfoDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_ID_MISSILE)
	{
		if (m_secondsRemaining > 0)
		{
			--m_secondsRemaining;
			UpdateUI();
		}
		else
		{
			KillTimer(TIMER_ID_MISSILE);  // 타이머 정지
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CMissileInfoDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	KillTimer(TIMER_ID_MISSILE);
}

// 클릭 핸들러 (비워둬도 됨)
void CMissileInfoDlg::OnStnClickedStaticMissileId2() {}
void CMissileInfoDlg::OnStnClickedStaticMissilePositionx() {}
void CMissileInfoDlg::OnStnClickedStaticMissilePositiony() {}
void CMissileInfoDlg::OnStnClickedStaticMissileHeight2() {}
void CMissileInfoDlg::OnStnClickedStaticMissileSpeed2() {}
void CMissileInfoDlg::OnStnClickedStaticMissileAngle2() {}
void CMissileInfoDlg::OnStnClickedStaticMissileShootdowntime2() {}
