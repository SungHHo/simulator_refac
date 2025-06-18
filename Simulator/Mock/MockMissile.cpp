#include <cmath>
#include <thread>
#include <iostream>
#include <cstring>
#include <memory>
#include <chrono>

#include "MockMissile.h"

constexpr double DEGREE_TO_INT = 1e8; // 실수 → 정수 저장시 스케일
constexpr double METERS_PER_DEGREE_LAT = 111320.0;

MockMissile::MockMissile(const MissileInfo &missile_info,
						 std::shared_ptr<MFRSendUDPManager> mfr_send_manager,
						 std::shared_ptr<MockTargetManager> mock_target_manager)
	: missile_info_(missile_info), mfr_send_manager_(mfr_send_manager), mock_target_manager_(mock_target_manager) {}

void MockMissile::updatePosMissile()
{
	auto last_time = std::chrono::steady_clock::now();
	double accumulated_distance = 0.0;
	double total_elapsed = 0.0;

	while (true)
	{
		auto now = std::chrono::steady_clock::now();
		std::chrono::duration<double> elapsed = now - last_time;
		last_time = now;

		double elapsed_seconds = elapsed.count();
		total_elapsed += elapsed_seconds;

		double speed_mps = missile_info_.speed * 0.27778;
		double distance = speed_mps * elapsed_seconds;
		accumulated_distance += distance;

		// 현재 위도, 경도 (실수 변환)
		double lat = static_cast<double>(missile_info_.x) / DEGREE_TO_INT;
		double lon = static_cast<double>(missile_info_.y) / DEGREE_TO_INT;

		// 위경도 1도당 m 계산
		double meters_per_deg_lon = METERS_PER_DEGREE_LAT * std::cos(lat * M_PI / 180.0);

		// 위도/경도 증가량 계산 (도 단위)
		double delta_lat = std::cos(missile_info_.angle * M_PI / 180.0) * distance / METERS_PER_DEGREE_LAT;
		double delta_lon = std::sin(missile_info_.angle * M_PI / 180.0) * distance / meters_per_deg_lon;

		// 정수형으로 환산해서 반영
		missile_info_.x += static_cast<long long>(delta_lat * DEGREE_TO_INT);
		missile_info_.y += static_cast<long long>(delta_lon * DEGREE_TO_INT);

		// 4초마다 거리 출력
		if (total_elapsed >= 4.0)
		{
			std::cout << "[4 sec update] Missile moved " << accumulated_distance << " meters.\n";
			std::cout << " → Current lat: " << static_cast<double>(missile_info_.x) / DEGREE_TO_INT
					  << ", lon: " << static_cast<double>(missile_info_.y) / DEGREE_TO_INT << "\n\n";
			total_elapsed = 0.0;
			accumulated_distance = 0.0;
		}

		// 명중 여부 판단
		// if (mock_target_manager_->downTargetStatus(missile_info_) > 0)
		// {
		// 	std::cout << "Missile hit target!" << std::endl;
		// 	missile_info_.is_hit = true;
		// }
		// else
		// {
		// 	auto now_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		// 	std::cout << "[Current time, MockMissile]: " << std::ctime(&now_time);
		// 	std::cout << "Missile is still in flight.\n";
		// }

		sendData();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void MockMissile::sendData()
{
	char buffer[1024];
	std::memcpy(buffer, &missile_info_, sizeof(missile_info_));
	mfr_send_manager_->sendData(buffer, sizeof(missile_info_));
}
