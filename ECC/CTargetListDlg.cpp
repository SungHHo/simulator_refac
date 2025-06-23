#include "pch.h"
#include "afxdialogex.h"
#include "CTargetListDlg.h"
#include "resource.h"
#include <algorithm>

IMPLEMENT_DYNAMIC(CTargetListDlg, CDialogEx)

CTargetListDlg::CTargetListDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TARGET_LIST_DLG, pParent)
{
}

CTargetListDlg::~CTargetListDlg()
{
}

void CTargetListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TARGET_LIST, m_listTarget);
}

BEGIN_MESSAGE_MAP(CTargetListDlg, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_TARGET_LIST, &CTargetListDlg::OnLvnItemchangedTargetList)
END_MESSAGE_MAP()

BOOL CTargetListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_listTarget.MoveWindow(10, 10, 950, 420);

	m_listTarget.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_listTarget.InsertColumn(0, _T("ID"), LVCFMT_CENTER, 70);
	m_listTarget.InsertColumn(1, _T("위도"), LVCFMT_CENTER, 120);
	m_listTarget.InsertColumn(2, _T("경도"), LVCFMT_CENTER, 120);
	m_listTarget.InsertColumn(3, _T("고도"), LVCFMT_CENTER, 70);
	m_listTarget.InsertColumn(4, _T("속도"), LVCFMT_CENTER, 100);
	m_listTarget.InsertColumn(5, _T("각도1"), LVCFMT_CENTER, 60);
	m_listTarget.InsertColumn(6, _T("각도2"), LVCFMT_CENTER, 60);
	m_listTarget.InsertColumn(7, _T("우선순위"), LVCFMT_CENTER, 80);
	m_listTarget.InsertColumn(8, _T("피격 여부"), LVCFMT_CENTER, 60);

	return TRUE;
}

void CTargetListDlg::SetTargetList(const std::vector<TargetStatus>& targets)
{
	bool needsUpdate = false;

	// ✅ 1. 크기 비교
	if (targets.size() != m_prevTargetList.size()) {
		needsUpdate = true;
	}
	else {
		// ✅ 2. 개별 항목 비교
		for (size_t i = 0; i < targets.size(); ++i)
		{
			if (targets[i].id != m_prevTargetList[i].id ||
				targets[i].priority != m_prevTargetList[i].priority ||
				targets[i].hit != m_prevTargetList[i].hit)
			{
				needsUpdate = true;
				break;
			}
		}
	}

	// ✅ 3. 변경이 없으면 리턴
	if (!needsUpdate)
		return;

	// ✅ 4. 변경됐으니 리스트 갱신
	m_prevTargetList = targets;
	m_targetList = targets;

	// 🔻 이하 기존 코드 (정렬 + 출력) 그대로 유지
	m_listTarget.SetRedraw(FALSE);
	m_listTarget.DeleteAllItems();

	std::sort(m_targetList.begin(), m_targetList.end(), [](const TargetStatus& a, const TargetStatus& b) {
		return a.priority > b.priority;
		});

	int highlightIndex = -1;

	for (size_t i = 0; i < m_targetList.size(); ++i)
	{
		const auto& t = m_targetList[i];

		CString idStr, xStr, yStr, hStr, sStr, aStr1, aStr2, pStr, hitStr;
		idStr.Format(_T("%d"), t.id);
		xStr.Format(_T("%.8f"), static_cast<double>(t.position.x) / 10000000.0);
		yStr.Format(_T("%.8f"), static_cast<double>(t.position.y) / 10000000.0);
		hStr.Format(_T("%lld"), t.position.z);
		sStr.Format(_T("%d"), t.speed);
		aStr1.Format(_T("%.1f"), t.angle1);
		aStr2.Format(_T("%.1f"), t.angle2);
		pStr.Format(_T("%d"), t.priority);
		hitStr = (t.hit != 0) ? _T("O") : _T("X");

		int row = static_cast<int>(i);
		m_listTarget.InsertItem(row, idStr);
		m_listTarget.SetItemText(row, 1, xStr);
		m_listTarget.SetItemText(row, 2, yStr);
		m_listTarget.SetItemText(row, 3, hStr);
		m_listTarget.SetItemText(row, 4, sStr);
		m_listTarget.SetItemText(row, 5, aStr1);
		m_listTarget.SetItemText(row, 6, aStr2);
		m_listTarget.SetItemText(row, 7, pStr);
		m_listTarget.SetItemText(row, 8, hitStr);

		if (highlightIndex == -1 && t.hit == 0)
			highlightIndex = row;
	}

	if (highlightIndex != -1)
	{
		m_listTarget.SetItemState(highlightIndex, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		m_listTarget.EnsureVisible(highlightIndex, FALSE);
	}

	m_listTarget.SetRedraw(TRUE);
	m_listTarget.Invalidate();
}


void CTargetListDlg::OnLvnItemchangedTargetList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// 선택 항목 변경 시 필요한 처리
	*pResult = 0;
}
