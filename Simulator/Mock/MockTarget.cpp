#include "MockTarget.h"

#include <chrono>
#include <thread>
#include <cmath>

MockTarget::MockTarget(const TargetInfo &target_info, std::shared_ptr<MFRSendUDPManager> mfr_send_manager) : target_info_(target_info), mfr_send_manager_(mfr_send_manager)
{
	// Constructor implementation
}

MockTarget::~MockTarget()
{
	// Destructor implementation
}

void MockTarget::updatePos()
{
	// 위치 업데이트 로직 (예: 속도와 각도를 기반으로 위치 계산)
	target_info_.x += std::cos(target_info_.angle * M_PI / 180.0) * target_info_.speed * 1;
	target_info_.y += std::sin(target_info_.angle * M_PI / 180.0) * target_info_.speed * 1;
	// target_info_.z += target_info_.speed * 0.01; // 고도 증가 예제

	// 만약 미사일이 탐지되면 10초뒤의 나의 위치 출력
	// if (mock_missile_manager_->getFlightStatus() && !first_flight_)
	// {
	// 	first_flight_ = true;
	// 	int ex_x = std::cos(target_info_.angle * M_PI / 180.0) * target_info_.speed * 100;
	// 	int ex_y = std::sin(target_info_.angle * M_PI / 180.0) * target_info_.speed * 100;
	// 	std::cout << "[Target position after 10 seconds]: "
	// 			  << "x: " << target_info_.x + ex_x
	// 			  << ", y: " << target_info_.y + ex_y
	// 			  << std::endl;
	// }

	// 데이터 전송
	sendData();

	std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void MockTarget::sendData()
{
	// 타겟 데이터를 전송
	char buffer[1024];
	// std::cout << "target size : " << sizeof(target_info_) << std::endl;
	std::memcpy(buffer, &target_info_, sizeof(target_info_));
	mfr_send_manager_->sendData(buffer, sizeof(target_info_));
	// std::cout << "Target data sent." << std::endl;
}

bool MockTarget::downTargetStatus(const MissileInfo &missileInfo)
{
	const int missile_range = 100;

	double dx = static_cast<double>(missileInfo.x - target_info_.x);
	double dy = static_cast<double>(missileInfo.y - target_info_.y);

	double distance = std::sqrt(dx * dx + dy * dy);

	if (distance <= missile_range)
	{
		target_info_.is_hit = true;
		return true;
	}

	return false;
}