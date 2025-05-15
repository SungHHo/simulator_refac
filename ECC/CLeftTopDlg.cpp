// CLeftTopDlg.cpp: 구현 파일
//

#include "pch.h"
#include "SAMtest.h"
#include "afxdialogex.h"
#include "CLeftTopDlg.h"


// CLeftTopDlg 대화 상자

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
}


BEGIN_MESSAGE_MAP(CLeftTopDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_MODE_SWITCH, &CLeftTopDlg::OnBnClickedBtnModeSwitch)
    ON_CBN_SELCHANGE(IDC_COMBO_RADAR_ID, &CLeftTopDlg::OnCbnSelchangeComboRadarId)
    ON_STN_CLICKED(IDC_STATIC_MODE, &CLeftTopDlg::OnStnClickedStaticMode)
    ON_STN_CLICKED(IDC_STATIC_NETWORK, &CLeftTopDlg::OnStnClickedStaticNetwork)
    ON_STN_CLICKED(IDC_STATIC_ANGLE, &CLeftTopDlg::OnStnClickedStaticAngle)
    ON_STN_CLICKED(IDC_STATIC_POSITION, &CLeftTopDlg::OnStnClickedStaticPosition)
 

END_MESSAGE_MAP()


// CLeftTopDlg 메시지 처리기

BOOL CLeftTopDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();



    // 초기화 코드 추가 가능

    return TRUE;
}


void CLeftTopDlg::OnBnClickedBtnModeSwitch()
{
    // 모드 전환 로직 예시
    static bool isRotateMode = true;
    
    if (isRotateMode)
    {
        AfxMessageBox(_T("정지 모드로 전환됩니다."));
    }
    else
    {
        AfxMessageBox(_T("회전 모드로 전환됩니다."));
    }

    isRotateMode = !isRotateMode;
}


void CLeftTopDlg::OnCbnSelchangeCombo1()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CLeftTopDlg::OnCbnSelchangeComboRadarId()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CLeftTopDlg::OnStnClickedStaticMode()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CLeftTopDlg::OnStnClickedStaticNetwork()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CLeftTopDlg::OnStnClickedStaticAngle()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CLeftTopDlg::OnStnClickedStaticPosition()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}



