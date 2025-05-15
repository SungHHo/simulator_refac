// CRightPaneDlg.cpp: 구현 파일
//


#include "pch.h"
#include "SAMtest.h"
#include "afxdialogex.h"
#include "CRightPaneDlg.h"


// CRightPaneDlg 대화 상자

IMPLEMENT_DYNAMIC(CRightPaneDlg, CDialogEx)

CRightPaneDlg::CRightPaneDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RIGHT_PANE_DLG, pParent)
{

}

CRightPaneDlg::~CRightPaneDlg()
{
}

void CRightPaneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRightPaneDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_LAUNCH_CTRL, &CRightPaneDlg::OnBnClickedCheckLaunchCtrl)
	ON_BN_CLICKED(IDC_CHECK_MISSILE, &CRightPaneDlg::OnBnClickedCheckMissile)
	ON_BN_CLICKED(IDC_CHECK_TARGET, &CRightPaneDlg::OnBnClickedCheckTarget)
END_MESSAGE_MAP()

BOOL CRightPaneDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect client;
	GetClientRect(&client);

	const int totalHeight = client.Height();  // == 900
	const int width = client.Width();

	const int checkBoxHeight = 70;
	const int launchHeight = 150;
	const int targetHeight = 290;
	const int missileHeight = 300;

	int y = checkBoxHeight;

	// 발사통제기
	m_launchDlg.Create(IDD_LC_INFO_DLG, this);
	m_launchDlg.MoveWindow(0, y, width, launchHeight);
	y += launchHeight;

	// 표적
	m_targetDlg.Create(IDD_TARGET_INFO_DLG, this);
	m_targetDlg.MoveWindow(0, y, width, targetHeight);
	y += targetHeight;

	// 유도탄
	m_missileDlg.Create(IDD_MISSILE_INFO_DLG, this);
	m_missileDlg.MoveWindow(0, y, width, missileHeight);

	m_launchDlg.ShowWindow(SW_SHOW);
	m_targetDlg.ShowWindow(SW_SHOW);
	m_missileDlg.ShowWindow(SW_SHOW);

	((CButton*)GetDlgItem(IDC_CHECK_LAUNCH_CTRL))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_CHECK_MISSILE))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_CHECK_TARGET))->SetCheck(TRUE);

	return TRUE;
}




void CRightPaneDlg::OnBnClickedCheckLaunchCtrl()
{
	BOOL checked = ((CButton*)GetDlgItem(IDC_CHECK_LAUNCH_CTRL))->GetCheck();
	m_launchDlg.ShowWindow(checked ? SW_SHOW : SW_HIDE);
}

void CRightPaneDlg::OnBnClickedCheckMissile()
{
	BOOL checked = ((CButton*)GetDlgItem(IDC_CHECK_MISSILE))->GetCheck();
	m_missileDlg.ShowWindow(checked ? SW_SHOW : SW_HIDE);
}

void CRightPaneDlg::OnBnClickedCheckTarget()
{
	BOOL checked = ((CButton*)GetDlgItem(IDC_CHECK_TARGET))->GetCheck();
	m_targetDlg.ShowWindow(checked ? SW_SHOW : SW_HIDE);
}
