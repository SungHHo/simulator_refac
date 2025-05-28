#pragma once
#include "afxdialogex.h"
#include "MissileStatus.h"

// CMissileInfoDlg 대화 상자
class CMissileInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMissileInfoDlg)

public:
	CMissileInfoDlg(CWnd* pParent = nullptr);
	virtual ~CMissileInfoDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MISSILE_INFO_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()

public:
	// ✅ 외부에서 미사일 정보 설정
	void SetMissileStatus(const MissileStatus& status);

	// ✅ 내부적으로 UI 갱신
	void UpdateUI();

	// 컨트롤 핸들러들
	afx_msg void OnStnClickedStaticMissileId2();
	afx_msg void OnStnClickedStaticMissilePositionx();
	afx_msg void OnStnClickedStaticMissilePositiony();
	afx_msg void OnStnClickedStaticMissileHeight2();
	afx_msg void OnStnClickedStaticMissileSpeed2();
	afx_msg void OnStnClickedStaticMissileAngle2();
	afx_msg void OnStnClickedStaticMissileShootdowntime2();

private:
	MissileStatus m_missileStatus;

	// ✅ 미사일 UI 컨트롤들
	CStatic m_staticID;
	CStatic m_staticPosX;
	CStatic m_staticPosY;
	CStatic m_staticHeight;
	CStatic m_staticSpeed;
	CStatic m_staticAngle;
	CStatic m_staticShootTime;
};
