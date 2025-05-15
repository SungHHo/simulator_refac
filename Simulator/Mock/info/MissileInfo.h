#pragma once
// LaunchData 구조체 정의
struct MissileInfo
{
	unsigned int id; // 미사일 ID
	float x;		 // 위도
	float y;		 // 경도
	float z;		 // 고도
	float angle;	 // 발사각
	int speed;		 // 속도
};