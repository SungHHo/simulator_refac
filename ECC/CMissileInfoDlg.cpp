// CMissileInfoDlg.cpp: 구현 파일
//

#include "pch.h"
#include "SAMtest.h"
#include "afxdialogex.h"
#include "CMissileInfoDlg.h"


// CMissileInfoDlg 대화 상자

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
}


BEGIN_MESSAGE_MAP(CMissileInfoDlg, CDialogEx)
END_MESSAGE_MAP()


// CMissileInfoDlg 메시지 처리기
