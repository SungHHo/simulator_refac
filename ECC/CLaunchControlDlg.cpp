// CLaunchControlDlg.cpp: 구현 파일
//

#include "pch.h"
#include "SAMtest.h"
#include "afxdialogex.h"
#include "CLaunchControlDlg.h"


// CLaunchControlDlg 대화 상자

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
}


BEGIN_MESSAGE_MAP(CLaunchControlDlg, CDialogEx)
END_MESSAGE_MAP()


// CLaunchControlDlg 메시지 처리기
