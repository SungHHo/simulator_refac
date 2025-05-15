// CLeftBottomDlg.cpp: 구현 파일
//

#include "pch.h"
#include "SAMtest.h"
#include "afxdialogex.h"
#include "CLeftBottomDlg.h"


// CLeftBottomDlg 대화 상자

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
}


BEGIN_MESSAGE_MAP(CLeftBottomDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_LS_ID, &CLeftBottomDlg::OnCbnSelchangeComboLsId)
	ON_BN_CLICKED(IDC_BTN_MODE_SWITCH, &CLeftBottomDlg::OnBnClickedBtnModeSwitch)
END_MESSAGE_MAP()


// CLeftBottomDlg 메시지 처리기


void CLeftBottomDlg::OnCbnSelchangeComboLsId()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CLeftBottomDlg::OnBnClickedBtnModeSwitch()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
