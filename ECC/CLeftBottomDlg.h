#pragma once
#include "afxdialogex.h"
#include "LSStatus.h"
#include <vector>
#include "resource.h"
#include "TargetStatus.h" 
// CLeftBottomDlg 대화 상자
class CSAMtestDlg;

class CLeftBottomDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLeftBottomDlg)

private:
	CComboBox m_comboLSID;                    // 콤보박스 컨트롤
	std::vector<LSStatus> m_lsList;           // ✅ 발사대 목록 저장
	LSStatus m_currentLSStatus{};             // ✅ 현재 선택된 발사대 정보
	CSAMtestDlg* m_parent = nullptr;
	std::vector<TargetStatus> m_targetList;
public:
	CLeftBottomDlg(CWnd* pParent = nullptr);
	virtual ~CLeftBottomDlg();
	void SetParentDlg(CSAMtestDlg* parent);
	enum { IDD = IDD_LEFT_BOTTOM_DLG };

	void SetLSList(const std::vector<LSStatus>& list);     // ✅ 외부에서 발사대 리스트 설정
	void UpdateLSStatusFromSelection();                    // ✅ 콤보박스 선택 시 정보 갱신
	void SetLSUI(const LSStatus& status);					// UI 출력 함수
	void UpdateLSModeUI();                              // 버튼 UI 상태 설정
	void OnLSModeChangeAck(uint8_t lsId, uint8_t mode); // ACK 핸들러
	void SetTargetList(const std::vector<TargetStatus>& targetList);
	void OnLSModeChangeSuccess(uint8_t lsId, uint8_t newMode);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnCbnSelchangeComboLsId();
	afx_msg void OnBnClickedBtnModeSwitch();
	afx_msg void OnBnClickedButtonMove();
	afx_msg void OnBnClickedButtonLaunch();
};
