#include "pch.h"
#include "SAMtest.h"
#include "afxdialogex.h"
#include "CLaunchControlDlg.h"

IMPLEMENT_DYNAMIC(CLaunchControlDlg, CDialogEx)

CLaunchControlDlg::CLaunchControlDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LC_INFO_DLG, pParent)
{
}

CLaunchControlDlg::~CLaunchControlDlg()
{
}

void CLaunchControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	// 컨트롤 연결
	DDX_Control(pDX, IDC_LC_STATIC_ID2, m_staticID);
	DDX_Control(pDX, IDC_LC_STATIC_POSX, m_staticPosX);
	DDX_Control(pDX, IDC_LC_STATIC_POSY, m_staticPosY);
	DDX_Control(pDX, IDC_LC_STATIC_POSZ, m_staticPosZ);
}

BEGIN_MESSAGE_MAP(CLaunchControlDlg, CDialogEx)
	ON_STN_CLICKED(IDC_LC_STATIC_ID2, &CLaunchControlDlg::OnStnClickedLcStaticId2)
	ON_STN_CLICKED(IDC_LC_STATIC_POSX, &CLaunchControlDlg::OnStnClickedLcStaticPosx)
	ON_STN_CLICKED(IDC_LC_STATIC_POSY, &CLaunchControlDlg::OnStnClickedLcStaticPosy)
	ON_STN_CLICKED(IDC_LC_STATIC_POSZ, &CLaunchControlDlg::OnStnClickedLcStaticPosz)
END_MESSAGE_MAP()

// ✅ 외부에서 LC 상태 설정 함수
void CLaunchControlDlg::SetLCStatus(const LCStatus& status)
{
	m_lcStatus = status;
	UpdateUI();
}

// ✅ 내부 UI 업데이트 함수
void CLaunchControlDlg::UpdateUI()
{
	double posX = static_cast<double>(m_lcStatus.position.x) / 10000000.0;
	double posY = static_cast<double>(m_lcStatus.position.y) / 10000000.0;
	long posZ = static_cast<double>(m_lcStatus.position.z);
	CString idStr;
	idStr.Format(_T("%d"), m_lcStatus.id);
	m_staticID.SetWindowText(idStr);

	CString posXStr, posYStr,posZStr;
	posXStr.Format(_T("%.8f"), posX);
	posYStr.Format(_T("%.8f"), posY);
	posZStr.Format(_T("%ld"), posZ);
	m_staticPosX.SetWindowText(posXStr);
	m_staticPosY.SetWindowText(posYStr);
	m_staticPosZ.SetWindowText(posZStr);
}

// 클릭 핸들러 (필요 없으면 삭제 가능)
void CLaunchControlDlg::OnStnClickedLcStaticId2() {}
void CLaunchControlDlg::OnStnClickedLcStaticPosx() {}
void CLaunchControlDlg::OnStnClickedLcStaticPosy() {}


void CLaunchControlDlg::OnStnClickedLcStaticPosz()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
