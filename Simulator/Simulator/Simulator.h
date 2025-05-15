#include <thread>

#include "LSRecvUDPManager.h"
#include "MFRSendUDPManager.h"
#include "MockTargetManager.h"
#include "MockMissileManager.h"

class Simulator
{
private:
	LSRecvUDPManager *ls_recv_manager_ = nullptr;
	MFRSendUDPManager *mfr_send_manager_ = nullptr;

	MockTargetManager *mock_target_manager_ = nullptr;
	MockMissileManager *mock_missile_manager_ = nullptr;

	std::thread recv_thread_;
	std::thread flight_target_thread_;

public:
	Simulator();
	~Simulator();

	bool init();
	void start();
};