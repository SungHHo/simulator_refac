#pragma once
#include "afxdialogex.h"


// CLeftTopDlg 대화 상자

class CLeftTopDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLeftTopDlg)

public:
	CLeftTopDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CLeftTopDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LEFT_TOP_DLG };
#endif
	COLORREF m_commStatusColor = RGB(0, 255, 0);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnModeSwitch();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeComboRadarId();
	afx_msg void OnStnClickedStaticMode();
	afx_msg void OnStnClickedStaticNetwork();
	afx_msg void OnStnClickedStaticAngle();
	afx_msg void OnStnClickedStaticPosition();

};
