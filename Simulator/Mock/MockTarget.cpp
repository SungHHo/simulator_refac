#include "MockTarget.h"

#include <chrono>
#include <thread>
#include <cmath>
#include <iostream>
#include <cstring>

constexpr double DEGREE_TO_INT = 1e8;                     // 위도/경도를 정수로 저장할 때 사용하는 스케일
constexpr double METERS_PER_DEGREE_LAT = 111320.0;        // 위도 1도당 거리 (m)

MockTarget::MockTarget(const TargetInfo &target_info, std::shared_ptr<MFRSendUDPManager> mfr_send_manager)
	: target_info_(target_info), mfr_send_manager_(mfr_send_manager)
{
}

MockTarget::~MockTarget()
{
}
void MockTarget::updatePos()
{
	static auto last_time = std::chrono::steady_clock::now(); // 루프 시작 시간 저장
	static double total_elapsed = 0.0;
	static double accumulated_distance = 0.0;

	// 경과 시간 측정
	auto now = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed = now - last_time;
	last_time = now;

	double elapsed_seconds = elapsed.count();
	total_elapsed += elapsed_seconds;

	// 시속 → m/s 변환
	double speed_mps = target_info_.speed * 0.27778;

	// 이동 거리 계산
	double distance = speed_mps * elapsed_seconds;
	accumulated_distance += distance;

	// 현재 위경도 (정수 → 실수)
	double lat = static_cast<double>(target_info_.x) / DEGREE_TO_INT;
	double lon = static_cast<double>(target_info_.y) / DEGREE_TO_INT;
	double alt_start = static_cast<double>(target_info_.z);

	// 위도, 경도 보정값 계산
	double meters_per_deg_lon = METERS_PER_DEGREE_LAT * std::cos(lat * M_PI / 180.0);

	double delta_lat = std::cos(target_info_.angle * M_PI / 180.0) * distance / METERS_PER_DEGREE_LAT;
	double delta_lon = std::sin(target_info_.angle * M_PI / 180.0) * distance / meters_per_deg_lon;
	double alt_change = std::tan(target_info_.angle2 * M_PI / 180.0) * distance;
	
	// 정수로 환산하여 위치 갱신
	target_info_.x += static_cast<long long>(delta_lat * DEGREE_TO_INT);
	target_info_.y += static_cast<long long>(delta_lon * DEGREE_TO_INT);
	alt_start += alt_change;
	target_info_.z = static_cast<long long>(alt_start);
	// 4초마다 이동 거리 및 위치 출력
	if (total_elapsed >= 4.0)
	{
		std::cout << "[4 sec update] Target moved " << accumulated_distance << " meters.\n";
		std::cout << " → Current lat: " << static_cast<double>(target_info_.x) / DEGREE_TO_INT
				  << ", lon: " << static_cast<double>(target_info_.y) / DEGREE_TO_INT
				  << ", alt: " << static_cast<double>(target_info_.z) << "\n\n";

		total_elapsed = 0.0;
		accumulated_distance = 0.0;
	}

	// 현재시간 출력
	auto now_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	//std::cout << "[Current time, MockTarget]: " << std::ctime(&now_time);

	// 예측 위치 (10초 후)
	double pred_distance = speed_mps * 10.0;
	double pred_delta_lat = std::cos(target_info_.angle * M_PI / 180.0) * pred_distance / METERS_PER_DEGREE_LAT;
	double pred_delta_lon = std::sin(target_info_.angle * M_PI / 180.0) * pred_distance / meters_per_deg_lon;

	// std::cout << "[Target position now]: "
	// 		  << "lat: " << static_cast<double>(target_info_.x) / DEGREE_TO_INT
	// 		  << ", lon: " << static_cast<double>(target_info_.y) / DEGREE_TO_INT
	// 		  << ", speed: " << target_info_.speed << " km/h\n";

	// std::cout << "[Target position after 10 seconds]: "
	// 		  << "lat: " << static_cast<double>(target_info_.x) / DEGREE_TO_INT + pred_delta_lat
	// 		  << ", lon: " << static_cast<double>(target_info_.y) / DEGREE_TO_INT + pred_delta_lon
	// 		  << std::endl;

	// 데이터 전송
	sendData();

	std::this_thread::sleep_for(std::chrono::milliseconds(100));
}


void MockTarget::sendData()
{
	char buffer[1024];
	std::memcpy(buffer, &target_info_, sizeof(target_info_));
	mfr_send_manager_->sendData(buffer, sizeof(target_info_));
}

bool MockTarget::downTargetStatus(const MissileInfo &missileInfo)
{
	const int missile_range = 100; // m 기준

	// 위도/경도 간 거리 차이(m) 계산
	double lat1 = static_cast<double>(missileInfo.x) / DEGREE_TO_INT;
	double lon1 = static_cast<double>(missileInfo.y) / DEGREE_TO_INT;
	double lat2 = static_cast<double>(target_info_.x) / DEGREE_TO_INT;
	double lon2 = static_cast<double>(target_info_.y) / DEGREE_TO_INT;

	double avg_lat = (lat1 + lat2) / 2.0;
	double meters_per_deg_lon = METERS_PER_DEGREE_LAT * std::cos(avg_lat * M_PI / 180.0);

	double dx = (lon1 - lon2) * meters_per_deg_lon;
	double dy = (lat1 - lat2) * METERS_PER_DEGREE_LAT;

	double distance = std::sqrt(dx * dx + dy * dy);

	if (distance <= missile_range)
	{
		target_info_.is_hit = true;
		return true;
	}
	return false;
}
