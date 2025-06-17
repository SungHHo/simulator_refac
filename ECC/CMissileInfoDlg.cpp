#include "pch.h"
#include "SAMtest.h"
#include "afxdialogex.h"
#include "CMissileInfoDlg.h"

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

// ✅ 외부에서 미사일 정보 전달
void CMissileInfoDlg::SetMissileStatus(const MissileStatus& status)
{
	m_missileStatus = status;
	UpdateUI();
}

// ✅ UI에 값 업데이트
void CMissileInfoDlg::UpdateUI()
{
	CString str;
	double posX = static_cast<double>(m_missileStatus.position.x) / 100000000.0;
	double posY = static_cast<double>(m_missileStatus.position.y) / 100000000.0;
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

	str.Format(_T("%lld"), m_missileStatus.predicted_time);
	m_staticShootTime.SetWindowText(str);
}

// 클릭 핸들러 (필요 없으면 비워둬도 됨)
void CMissileInfoDlg::OnStnClickedStaticMissileId2() {}
void CMissileInfoDlg::OnStnClickedStaticMissilePositionx() {}
void CMissileInfoDlg::OnStnClickedStaticMissilePositiony() {}
void CMissileInfoDlg::OnStnClickedStaticMissileHeight2() {}
void CMissileInfoDlg::OnStnClickedStaticMissileSpeed2() {}
void CMissileInfoDlg::OnStnClickedStaticMissileAngle2() {}
void CMissileInfoDlg::OnStnClickedStaticMissileShootdowntime2() {}
