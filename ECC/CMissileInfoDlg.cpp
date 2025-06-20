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
	return TRUE;
}

void CMissileInfoDlg::SetMissileStatus(const MissileStatus& status)
{
	m_missileStatus = status;

	// ✅ 현재 UTC 시간 (초 단위) 구하기
	using namespace std::chrono;
	auto now_utc = duration_cast<seconds>(system_clock::now().time_since_epoch()).count();

	// ✅ 남은 초 계산
	m_secondsRemaining = static_cast<int>(status.predicted_time - now_utc);
	if (m_secondsRemaining < 0) m_secondsRemaining = 0;

	UpdateUI();

	// ✅ 타이머 시작 (1초 주기)
	SetTimer(TIMER_ID_MISSILE, 1000, nullptr);
}

void CMissileInfoDlg::UpdateUI()
{
	CString str;
	double posX = static_cast<double>(m_missileStatus.position.x) / 10000000.0;
	double posY = static_cast<double>(m_missileStatus.position.y) / 10000000.0;

	str.Format(_T("%d"), m_missileStatus.id);
	m_staticID.SetWindowText(str);

	str.Format(_T("%.8f"), posX);
	m_staticPosX.SetWindowText(str);

	str.Format(_T("%.8f"), posY);
	m_staticPosY.SetWindowText(str);

	str.Format(_T("%lld"), m_missileStatus.position.z);
	m_staticPosZ.SetWindowText(str);

	str.Format(_T("%d"), m_missileStatus.speed);
	m_staticSpeed.SetWindowText(str);

	str.Format(_T("%.1f"), m_missileStatus.angle);
	m_staticAngle.SetWindowText(str);

	// ✅ 격추까지 남은 시간 출력
	/*if (m_secondsRemaining > 0)
	{
		str.Format(_T("%d초 남음"), m_secondsRemaining);
	}
	else
	{
		str = _T("격추 완료 또는 시간 초과");
	}*/
	str.Format(_T("%llu"), m_missileStatus.predicted_time);
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
