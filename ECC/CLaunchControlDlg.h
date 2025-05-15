#pragma once
#include "afxdialogex.h"


// CLaunchControlDlg 대화 상자

class CLaunchControlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLaunchControlDlg)

public:
	CLaunchControlDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CLaunchControlDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LC_INFO_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
