#include <cmath>

constexpr double WGS84_A = 6378137.0;              // 장반경
constexpr double WGS84_E2 = 6.69437999014e-3;      // 이심률 제곱

struct Vec3 {
    double x, y, z;  // m 단위
};

/// 위도/경도/고도 → ECEF 좌표계 (지구 중심 기준)
inline Vec3 latLonAltToECEF(double lat_deg, double lon_deg, double alt_m) {
    double lat = lat_deg * M_PI / 180.0;
    double lon = lon_deg * M_PI / 180.0;
    double N = WGS84_A / std::sqrt(1.0 - WGS84_E2 * std::sin(lat) * std::sin(lat));

    double x = (N + alt_m) * std::cos(lat) * std::cos(lon);
    double y = (N + alt_m) * std::cos(lat) * std::sin(lon);
    double z = (N * (1.0 - WGS84_E2) + alt_m) * std::sin(lat);
    return {x, y, z};
}

/// ECEF → ENU 변환 (기준점 기준)
inline Vec3 ecefToENU(const Vec3& ecef, const Vec3& ref_ecef, double ref_lat_deg, double ref_lon_deg) {
    double lat = ref_lat_deg * M_PI / 180.0;
    double lon = ref_lon_deg * M_PI / 180.0;

    double dx = ecef.x - ref_ecef.x;
    double dy = ecef.y - ref_ecef.y;
    double dz = ecef.z - ref_ecef.z;

    double sin_lat = std::sin(lat);
    double cos_lat = std::cos(lat);
    double sin_lon = std::sin(lon);
    double cos_lon = std::cos(lon);

    double east  = -sin_lon * dx + cos_lon * dy;
    double north = -cos_lon * sin_lat * dx - sin_lon * sin_lat * dy + cos_lat * dz;
    double up    =  cos_lon * cos_lat * dx + sin_lon * cos_lat * dy + sin_lat * dz;

    return {east, north, up};  // 단위: meter
}
