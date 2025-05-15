// CTargetInfoDlg.cpp: 구현 파일
//

#include "pch.h"
#include "SAMtest.h"
#include "afxdialogex.h"
#include "CTargetInfoDlg.h"


// CTargetInfoDlg 대화 상자

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
}


BEGIN_MESSAGE_MAP(CTargetInfoDlg, CDialogEx)
END_MESSAGE_MAP()


// CTargetInfoDlg 메시지 처리기
