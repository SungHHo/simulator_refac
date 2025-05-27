#pragma once

#include "afxdialogex.h"
#include "RadarStatus.h"
#include "resource.h"
#include <vector>
#include "CMFRModeSelectDlg.h"

// ✅ forward declaration
class CSAMtestDlg;

// CLeftTopDlg 대화 상자
class CLeftTopDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLeftTopDlg)

private:
	CComboBox m_comboRadarID;
	RadarStatus m_currentRadarStatus;
	std::vector<RadarStatus> m_radarList;
	std::vector<TargetStatus> m_targetList;  // ✅ 표적 리스트 추가

	CSAMtestDlg* m_parent = nullptr;         // ✅ 메인 다이얼로그 참조

public:
	CLeftTopDlg(CWnd* pParent = nullptr);
	virtual ~CLeftTopDlg();
	void SetParentDlg(CSAMtestDlg* parent);
	enum { IDD = IDD_LEFT_TOP_DLG };

	

	// ✅ 레이더 및 표적 리스트 설정
	void SetRadarList(const std::vector<RadarStatus>& radarList);
	void SetTargetList(const std::vector<TargetStatus>& targetList);

	void UpdateRadarStatusFromSelection();
	void SetRadarUI(const RadarStatus& status);

	// ✅ 모드 변경 ACK 수신 후 UI 반영
	void OnRadarModeChangeSuccess(RadarStatus::Mode newMode);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedBtnModeSwitch();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnCbnSelchangeComboRadarId();
	afx_msg void OnStnClickedStaticMode();
	afx_msg void OnStnClickedStaticNetwork();
	afx_msg void OnStnClickedStaticAngle();
	afx_msg void OnStnClickedStaticPosition();
	afx_msg void OnStnClickedStaticMfrMode();
	afx_msg void OnStnClickedStaticMfrPosition2();
};
