#pragma once
#include "afxdialogex.h"


// CLeftBottomDlg 대화 상자

class CLeftBottomDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLeftBottomDlg)

public:
	CLeftBottomDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CLeftBottomDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LEFT_BOTTOM_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeComboLsId();
	afx_msg void OnBnClickedBtnModeSwitch();
};
