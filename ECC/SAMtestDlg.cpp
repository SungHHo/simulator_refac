
// SAMtestDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "SAMtest.h"
#include "SAMtestDlg.h"
#include "afxdialogex.h"
#include "SendMessageTypes.h"
#include "Serializer.h"
#include "ReceiveMessageTypes.h"
#include "Deserializer.h"
#include "PacketParser.h"
#include <iostream>
#include <variant>

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

	m_leftTop.SetParentDlg(this); // CLeftTopDlg에게 부모 다이얼로그를 전달
	m_leftBottom.SetParentDlg(this); // CLeftBottomDlg에게 부모 다이얼로그를 전달

	////임시 레이더 더미 시작
	//std::vector<RadarStatus> dummyRadarList;

	//for (int i = 1; i <= 3; ++i) {
	//	RadarStatus radar{};
	//	radar.id = i;                         // ✅ 임시 레이더 ID
	//	radar.mode = RadarStatus::ROTATE;    // ✅ 임시 모드: 회전
	//	radar.angle = 123.4 + i;             // 임의의 탐지 각
	//	radar.position = { 1000 * i, 2000 * i };  // 임의의 좌표

	//	dummyRadarList.push_back(radar);
	//}

	//m_leftTop.SetRadarList(dummyRadarList);
	//
	//// ✅ 더미 표적 리스트 설정
	//std::vector<TargetStatus> dummyTargets;
	//for (int i = 0; i < 5; ++i) {
	//	TargetStatus target{};
	//	target.id = 200 + i;
	//	target.position = { 10000 + i * 500, 15000 + i * 500 };
	//	target.angle = 45.0 + i * 5;
	//	target.speed = 300.0 + i * 10;
	//	dummyTargets.push_back(target);
	//}
	//m_leftTop.SetTargetList(dummyTargets);
	//m_leftBottom.SetTargetList(dummyTargets);
	////임시 레이더 더미 끝

	//// ✅ 더미 LSStatus 생성
	//std::vector<LSStatus> dummyLSList;
	//LSStatus dummyLS;
	//dummyLS.id = 1;
	//dummyLS.mode = 0;  // STOP
	//dummyLS.angle = 75.5;
	//dummyLS.position = { 1000 , 2000 };
	//dummyLSList.push_back(dummyLS);

	//// ✅ 발사대 UI에 전달
	//m_leftBottom.SetLSList(dummyLSList);



	m_tcp = std::make_unique<ECC_TCP>();
	if (!m_tcp->connect("127.0.0.1", 9000)) {
		AfxMessageBox(_T("서버 연결 실패"));
		return FALSE;
	}

	m_tcp->registerReceiver(this);
	m_tcp->startReceiving();

	// 2. 주기적 상태 요청 타이머 시작
	SetTimer(TIMER_ID_REQUEST, 100, nullptr); // 100ms 주기

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
	if (len <= 0 || packet == nullptr) return;

	try {
		ParsedPacket parsed = PacketParser::Parse(packet, static_cast<size_t>(len));

		std::visit([&](auto&& msg) {
			using T = std::decay_t<decltype(msg)>;

			if constexpr (std::is_same_v<T, ParsedStatusResponse>) {
				std::cout << "[수신] 상태 패킷: Radar=" << msg.radarList.size()
					<< ", LS=" << msg.lsList.size()
					<< ", LC=" << msg.lcList.size()
					<< ", Target=" << msg.targetList.size()
					<< ", Missile=" << msg.missileList.size() << "\n";

				// ✅ 구조체 크기 출력 (최초 1회)
				static bool printed = false;
				if (!printed) {
					std::cout << "[DEBUG] sizeof(RadarStatus)   = " << sizeof(RadarStatus) << "\n";
					std::cout << "[DEBUG] sizeof(LCStatus)      = " << sizeof(LCStatus) << "\n";
					std::cout << "[DEBUG] sizeof(LSStatus)      = " << sizeof(LSStatus) << "\n";
					std::cout << "[DEBUG] sizeof(TargetStatus)  = " << sizeof(TargetStatus) << "\n";
					std::cout << "[DEBUG] sizeof(MissileStatus) = " << sizeof(MissileStatus) << "\n";
					printed = true;
				}

				for (const auto& r : msg.radarList) {
					std::cout << "  [Radar] ID=" << static_cast<int>(r.id)
						<< ", Pos=(" << r.position.x << "," << r.position.y << ")"
						<< ", Mode=" << (int)r.mode
						<< ", Angle=" << r.angle << "\n";
				}
				for (const auto& lc : msg.lcList) {
					std::cout << "  [LC] ID=" << static_cast<int>(lc.id)
						<< ", Pos=(" << lc.position.x << "," << lc.position.y << ")\n";
				}
				for (const auto& ls : msg.lsList) {
					std::cout << "  [LS] ID=" << static_cast<int>(ls.id)
						<< ", Pos=(" << ls.position.x << "," << ls.position.y << ")"
						<< ", Mode=" << (int)ls.mode
						<< ", Angle=" << ls.angle << "\n";
				}
				for (const auto& m : msg.missileList) {
					std::cout << "  [Missile] ID=" << (int)m.id
						<< ", Pos=(" << m.position.x << "," << m.position.y << ")"
						<< ", Height=" << m.height
						<< ", Speed=" << m.speed
						<< ", Angle=" << m.angle
						<< ", PredictedTime=" << m.predicted_time
						<< ", Hit=" << static_cast<int>(m.hit) << "\n";
				}
				for (const auto& t : msg.targetList) {
					std::cout << "  [Target] ID=" << (int)t.id
						<< ", Pos=(" << t.position.x << "," << t.position.y << ")"
						<< ", Height=" << t.height
						<< ", Speed=" << t.speed
						<< ", Angle=" << t.angle
						<< ", FirstDetected=" << t.first_detect_time
						<< ", Priority=" << t.priority
						<< ", Hit=" << static_cast<int>(t.hit) << "\n";
				}

				// UI 반영
				m_leftTop.SetRadarList(msg.radarList);
				m_leftTop.SetTargetList(msg.targetList);
				m_leftBottom.SetLSList(msg.lsList);
				m_leftBottom.SetTargetList(msg.targetList);
				if (!msg.lcList.empty())
					m_rightPane.SetLCStatus(msg.lcList[0]);
			}
			else {
				std::cout << "[수신] ACK or 기타 패킷 수신됨\n";
			}
			}, parsed);
	}
	catch (const std::exception& ex) {
		std::cerr << "[에러] 패킷 파싱 실패: " << ex.what() << "\n";
	}
}









void CSAMtestDlg::sendStatusRequest() {
	StatusRequest msg{};
	auto data = SerializeStatusRequest(msg);
	std::cout << "[클라이언트] STATUS_REQUEST 전송 (0x01)\n";
	m_tcp->send(reinterpret_cast<const char*>(data.data()), static_cast<int>(data.size()));
}

void CSAMtestDlg::sendRadarModeChange(uint8_t radar_id, uint8_t mode, uint8_t target_id) {
	RadarModeChange msg{ CommandType::RADAR_MODE_CHANGE, radar_id, mode, target_id };
	auto data = SerializeRadarModeChange(msg);
	m_tcp->send(reinterpret_cast<const char*>(data.data()), static_cast<int>(data.size()));
}

void CSAMtestDlg::sendLSModeChange(uint8_t ls_id, uint8_t mode) {
	LSModeChange msg{ CommandType::LS_MODE_CHANGE, ls_id, mode };
	auto data = SerializeLSModeChange(msg);
	m_tcp->send(reinterpret_cast<const char*>(data.data()), static_cast<int>(data.size()));
}

void CSAMtestDlg::sendMissileLaunch(uint8_t ls_id, uint8_t target_id) {
	MissileLaunch msg{ CommandType::MISSILE_LAUNCH, ls_id, target_id };
	auto data = SerializeMissileLaunch(msg);
	m_tcp->send(reinterpret_cast<const char*>(data.data()), static_cast<int>(data.size()));
}
void CSAMtestDlg::sendLSMove(uint8_t ls_id, Pos2D pos) {
	LSMove msg{ CommandType::LS_MOVE, ls_id, pos };
	auto data = SerializeLSMove(msg);
	m_tcp->send(reinterpret_cast<const char*>(data.data()), static_cast<int>(data.size()));
}


void CSAMtestDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_ID_REQUEST) {
		sendStatusRequest();  // 주기적 상태 요청
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CSAMtestDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	KillTimer(TIMER_ID_REQUEST);  // 타이머 해제
	if (m_tcp) {
		m_tcp->stop();  // 수신 스레드 종료
	}
}
