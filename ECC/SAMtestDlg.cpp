
// SAMtestDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "SAMtest.h"
#include "SAMtestDlg.h"
#include "afxdialogex.h"
#include "MessageDeserializer.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSAMtestDlg 대화 상자



CSAMtestDlg::CSAMtestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SAMTEST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSAMtestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSAMtestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CSAMtestDlg 메시지 처리기

BOOL CSAMtestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetWindowPos(NULL, 0, 0, 1500, 900, SWP_NOMOVE | SWP_NOZORDER);
	// 클라이언트 영역 크기 구하기
	CRect rect;
	GetClientRect(&rect);
	int width = rect.Width();
	int height = rect.Height();

	int leftWidth = 400;
	int rightWidth = 400;
	int centerWidth = width - leftWidth - rightWidth;
	int halfHeight = height / 2;

	// 왼쪽 위 다이얼로그
	m_leftTop.Create(IDD_LEFT_TOP_DLG, this);
	m_leftTop.MoveWindow(0, 0, leftWidth, halfHeight);
	m_leftTop.ShowWindow(SW_SHOW);

	// 왼쪽 아래 다이얼로그
	m_leftBottom.Create(IDD_LEFT_BOTTOM_DLG, this);
	m_leftBottom.MoveWindow(0, halfHeight, leftWidth, height - halfHeight);
	m_leftBottom.ShowWindow(SW_SHOW);
	

	// 중앙 지도 영역
	m_mapArea.Create(_T(""), WS_CHILD | WS_VISIBLE | SS_BLACKFRAME,
		CRect(leftWidth, 0, leftWidth + centerWidth, height), this, 1001);

	// 오른쪽 패널
	m_rightPane.Create(IDD_RIGHT_PANE_DLG, this);
	m_rightPane.MoveWindow(leftWidth + centerWidth, 0, rightWidth, height);
	m_rightPane.ShowWindow(SW_SHOW);

	// ✅ 테스트 다이얼로그 생성 및 실행 (모델리스)
	m_pTestReceiverDlg = new CTestReceiverDlg;
	m_pTestReceiverDlg->Create(IDD_TEST_RECEIVER_DLG, this);  // Create 먼저
	m_pTestReceiverDlg->ShowWindow(SW_SHOW);                  // 그런 다음 보여주기

	m_tcp.SetReceiver(m_pTestReceiverDlg);
	m_tcp.Connect(_T("127.0.0.1"), 9000);
	m_tcp.StartReceiving();
	SetTimer(TIMER_ID_REQUEST, 1000, nullptr);

	return TRUE;
}

void CSAMtestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CSAMtestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CSAMtestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSAMtestDlg::receive(int len, const char* packet)
{
	//CString msg(packet, len);
	//SetDlgItemText(IDC_STATIC_DATA, msg);
	CommonMessage msg = DeserializeCommonMessage((uint8_t*)packet, len);
	HandleParsedMessage(msg);
}

void CSAMtestDlg::HandleParsedMessage(const CommonMessage& msg)
{
//	if (msg.messageType != MessageType::RESPONSE)
//		return;
//
//	for (const auto& item : msg.payload)
//	{
//		std::visit([this](auto&& typedMsg) {
//			using T = std::decay_t<decltype(typedMsg)>;
//
//			if constexpr (std::is_same_v<T, LS::Message>)
//				m_gui->UpdateLS(typedMsg);
//			else if constexpr (std::is_same_v<T, MFR::Message>)
//				m_gui->UpdateMFR(typedMsg);
//			else if constexpr (std::is_same_v<T, LC::Message>)
//				m_gui->UpdateLC(typedMsg);
//			else if constexpr (std::is_same_v<T, Target::Message>)
//				m_gui->UpdateTarget(typedMsg);
//			else if constexpr (std::is_same_v<T, Missile::Message>)
//				m_gui->UpdateMissile(typedMsg);
//			}, item);
//	}
}

void CSAMtestDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_ID_REQUEST)
	{
		m_tcp.SendData(_T("GET"));
		OutputDebugString(L"[TIMER] 'GET' 요청 전송\n");
	}

	CDialogEx::OnTimer(nIDEvent);
}
