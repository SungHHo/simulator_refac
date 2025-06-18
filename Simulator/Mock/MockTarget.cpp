#include "MockTarget.h"

#include <chrono>
#include <thread>
#include <cmath>
#include <iostream>
#include <cstring>

constexpr double DEGREE_TO_INT = 1e8;			   // 위도/경도를 정수로 저장할 때 사용하는 스케일
constexpr double METERS_PER_DEGREE_LAT = 111320.0; // 위도 1도당 거리 (m)

MockTarget::MockTarget(const TargetInfo &target_info, std::shared_ptr<MFRSendUDPManager> mfr_send_manager)
	: target_info_(target_info),
	  mfr_send_manager_(mfr_send_manager),
	  last_time_(std::chrono::steady_clock::now()),
	  total_elapsed_(0.0),
	  accumulated_distance_(0.0)
{
}

MockTarget::~MockTarget()
{
}

void MockTarget::updatePos()
{
	// static 변수 제거하고 멤버 변수 사용
	auto now = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed = now - last_time_;
	last_time_ = now;

	double elapsed_seconds = elapsed.count();
	total_elapsed_ += elapsed_seconds;

	// 시속 → m/s 변환
	double speed_mps = target_info_.speed * 0.27778;

	// 이동 거리 계산
	double distance = speed_mps * elapsed_seconds;
	accumulated_distance_ += distance;

	// 현재 위경도 (정수 → 실수)
	double lat = static_cast<double>(target_info_.x) / DEGREE_TO_INT;
	double lon = static_cast<double>(target_info_.y) / DEGREE_TO_INT;

	// 위도, 경도 보정값 계산
	double meters_per_deg_lon = METERS_PER_DEGREE_LAT * std::cos(lat * M_PI / 180.0);

	double delta_lat = std::cos(target_info_.angle * M_PI / 180.0) * distance / METERS_PER_DEGREE_LAT;
	double delta_lon = std::sin(target_info_.angle * M_PI / 180.0) * distance / meters_per_deg_lon;

	// 정수로 환산하여 위치 갱신
	target_info_.x += static_cast<long long>(delta_lat * DEGREE_TO_INT);
	target_info_.y += static_cast<long long>(delta_lon * DEGREE_TO_INT);

	// 4초마다 이동 거리 및 위치 출력
	// if (total_elapsed >= 4.0)
	// {
	// 	std::cout << "[4 sec update] Target moved " << accumulated_distance << " meters.\n";
	// 	std::cout << " → Current lat: " << static_cast<double>(target_info_.x) / DEGREE_TO_INT
	// 			  << ", lon: " << static_cast<double>(target_info_.y) / DEGREE_TO_INT << "\n\n";

	// 	total_elapsed = 0.0;
	// 	accumulated_distance = 0.0;
	// }

	// 현재시간 출력
	auto now_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	// std::cout << "[Current time, MockTarget]: " << std::ctime(&now_time);

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
}

void MockTarget::sendData()
{
	char buffer[1024];
	std::memcpy(buffer, &target_info_, sizeof(target_info_));
	mfr_send_manager_->sendData(buffer, sizeof(target_info_));
}

bool MockTarget::downTargetStatus(const MissileInfo &missileInfo)
{
	// 현재 타깃의 위경도
	// 현재 위경도 (정수 → 실수)
	double lat = static_cast<double>(target_info_.x) / DEGREE_TO_INT;
	double lon = static_cast<double>(target_info_.y) / DEGREE_TO_INT;

	// 미사일의 위경도
	double missile_lat = static_cast<double>(missileInfo.x) / DEGREE_TO_INT;
	double missile_lon = static_cast<double>(missileInfo.y) / DEGREE_TO_INT;

	// 두 위경도간의 m거리 계산
	double meters_per_deg_lon = METERS_PER_DEGREE_LAT * std::cos(lat * M_PI / 180.0);
	double dy = (missile_lat - lat) * METERS_PER_DEGREE_LAT;
	double dx = (missile_lon - lon) * meters_per_deg_lon;
	double dist_m = std::sqrt(dx * dx + dy * dy);
	// std::cout << "미사일과 타깃 간 거리 계산 결과 → dx: " << dx << "m, dy: " << dy << "m, 총거리: " << dist_m << "m\n";
	// 미사일과 타깃 간 거리가 100m 이하인 경우
	if (dist_m <= 100.0) // 100m 기준
	{
		std::cout << "Target hit by missile!" << std::endl;
		target_info_.is_hit = true; // 타깃 명중 처리
		return true;
	}
	// std::cout << "Target not hit by missile." << std::endl;

	// const int missile_range = 100; // m 기준

	// // 위도/경도 간 거리 차이(m) 계산
	// double lat1 = static_cast<double>(missileInfo.x) / DEGREE_TO_INT;
	// double lon1 = static_cast<double>(missileInfo.y) / DEGREE_TO_INT;
	// double lat2 = static_cast<double>(target_info_.x) / DEGREE_TO_INT;
	// double lon2 = static_cast<double>(target_info_.y) / DEGREE_TO_INT;

	// const double meters_per_deg_lat = 111320.0;
	// const double meters_per_deg_lon = 111320.0 * std::cos(lat1 * M_PI / 180.0);

	// double dy = (lat2 - lat1) * meters_per_deg_lat;
	// double dx = (lon2 - lat2) * meters_per_deg_lon;
	// double dist_m = std::sqrt(dx * dx + dy * dy);
	// std::cout << "미사일 표적간 거리 계산 결과 → dx: " << dx << "m, dy: " << dy << "m, 총거리: " << dist_m << "m\n";

	return false;
}
