
// SAMtestDlg.h: 헤더 파일
//
#pragma once
#include "CLeftTopDlg.h"
#include "CLeftBottomDlg.h"
#include "CRightPaneDlg.h"
#include <afxsock.h>
#include "TcpClient.h"
#include "IReceiver.h"
#include "CommonMessage.h"
#include "CTestReceiverDlg.h"

// CSAMtestDlg 대화 상자
class CSAMtestDlg : public CDialogEx, public IReceiver
{
private:
	CLeftTopDlg m_leftTop;
	CLeftBottomDlg m_leftBottom;
	CRightPaneDlg m_rightPane;
	CStatic m_mapArea; // 지도
	CTcpClient m_tcp;
	CTestReceiverDlg* m_pTestReceiverDlg = nullptr;
public:
	CSAMtestDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	virtual void receive(int len, const char* packet) override;
	void HandleParsedMessage(const CommonMessage& msg);
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SAMTEST_DIALOG };
#endif
	enum { TIMER_ID_REQUEST = 1 };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()
};