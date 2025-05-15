#pragma once
#include "afxdialogex.h"


// CMissileInfoDlg 대화 상자

class CMissileInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMissileInfoDlg)

public:
	CMissileInfoDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CMissileInfoDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MISSILE_INFO_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
