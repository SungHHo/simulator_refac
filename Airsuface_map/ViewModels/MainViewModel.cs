using System;
using System.Collections.Specialized;
using System.Diagnostics;
using System.Globalization;
using System.IO;
using Airsuface_map.Models;

namespace Airsuface_map.ViewModels
{
    public class MainViewModel
    {
        private static double ConvertLongToDouble(long value)
        {
            // 8자리 소수부 분리
            long intPart = value / 100_000_000L;
            long fracPart = Math.Abs(value % 100_000_000L);
            double result = intPart + fracPart / 100_000_000.0;
            // 음수 처리
            if (value < 0 && intPart == 0)
                result *= -1;
            return result;
        }

        private static long ReadCustomInt64(BinaryReader reader)
        {
            // 8바이트 읽기
            byte[] originalBytes = reader.ReadBytes(8);
            if (originalBytes.Length < 8)
                throw new EndOfStreamException("Int64를 읽기에 충분한 바이트가 없습니다.");

            //Debug.WriteLine($"원본 바이트 배열: {BitConverter.ToString(originalBytes)}");

            // 2바이트씩 자리를 바꾸기 위해 2바이트 단위로 교환
            byte[] swappedBytes = new byte[8];
            swappedBytes[0] = originalBytes[7];
            swappedBytes[1] = originalBytes[6];
            swappedBytes[2] = originalBytes[5];
            swappedBytes[3] = originalBytes[4];
            swappedBytes[4] = originalBytes[3];
            swappedBytes[5] = originalBytes[2];
            swappedBytes[6] = originalBytes[1];
            swappedBytes[7] = originalBytes[0];

            //Debug.WriteLine($"오리지널 바이트 배열: {BitConverter.ToString(originalBytes)}");
            //Debug.WriteLine($"2바이트씩 자리 변경 후 바이트 배열: {BitConverter.ToString(swappedBytes)}");

            // 앞쪽의 0x00 바이트 제거
            int startIndex = 0;
            while (startIndex < swappedBytes.Length && swappedBytes[startIndex] == 0x00)
            {
                startIndex++;
            }

            // 모든 바이트가 0x00이면 0 반환
            if (startIndex == swappedBytes.Length)
            {
                return 0;
            }

            // 유의미한 바이트 배열 추출
            int significantLength = swappedBytes.Length - startIndex;
            byte[] significantBytes = new byte[significantLength];
            Array.Copy(swappedBytes, startIndex, significantBytes, 0, significantLength);

            // 바이트 배열을 뒤집기 (Big Endian에서 Little Endian으로)
            Array.Reverse(significantBytes);

            // 8바이트로 패딩 (필요한 경우)
            byte[] paddedBytes = new byte[8];
            Array.Copy(significantBytes, 0, paddedBytes, 0, significantBytes.Length);

            // unsigned long으로 변환
            ulong unsignedResult = BitConverter.ToUInt64(paddedBytes, 0);

            // 필요한 경우 long으로 캐스팅
            long result = (long)unsignedResult;

            return result;
        }

        public MissileMapViewModel MissileMapVM { get; } = new();
        public TargetMapViewModel TargetMapVM { get; } = new();
        public LCMapViewModel LCMapVM { get; } = new();
        public LSMapViewModel LSMapVM { get; } = new();
        public MFRMapViewModel MFRMapVM { get; } = new();

        // C++ 서버에서 온 바이너리 데이터 파싱 예시
        public void OnDataReceived(byte[] rawData)
        {
            if (rawData == null || rawData.Length == 0) return;

            using var ms = new MemoryStream(rawData);
            using var reader = new BinaryReader(ms);

            // 1. Header 파싱
            var commandType = reader.ReadByte(); // CommandType (1 byte)
            var numRadar = reader.ReadByte();    // 1
            var numLC = reader.ReadByte();       // 1
            var numLS = reader.ReadByte();       // 1
            var numTarget = reader.ReadByte();   // 최대 100
            var numMissile = reader.ReadByte();  // 1

            // 2. RadarStatus 파싱
            var mfr = new MFR {
                Id = reader.ReadInt32(),                 // uint8_t
                X = ConvertLongToDouble(ReadCustomInt64(reader)),
                Y = ConvertLongToDouble(ReadCustomInt64(reader)),
                Mode = reader.ReadByte(),               // uint8_t
                Angle = reader.ReadDouble()            // double
            };

            // 3. LCStatus 파싱
            var lc = new LC {
                Id = reader.ReadInt32(),                   // uint8_t
                X = ConvertLongToDouble(ReadCustomInt64(reader)),                 // long long
                Y = ConvertLongToDouble(ReadCustomInt64(reader))                 // long long
            };

            // 4. LSStatus 파싱
            var ls = new LS {
                Id = reader.ReadInt32(),                    // uint8_t
                X = ConvertLongToDouble(ReadCustomInt64(reader)),                 // long long
                Y= ConvertLongToDouble(ReadCustomInt64(reader)),                // long long
                Mode = reader.ReadByte(),                  // uint8_t
                Angle = reader.ReadDouble()               // double
            };

            // 5. TargetStatus 파싱 (numTarget 개)
            var targets = new List<MockTarget>();
            for (int i = 0; i < numTarget; i++)
            {
                var tId = reader.ReadInt32();                 // uint8_t
                var tPosX = ReadCustomInt64(reader);              // long long
                var tPosY = ReadCustomInt64(reader);              // long long
                var tHeight = ReadCustomInt64(reader);            // long long
                var tSpeed = reader.ReadInt64();             // long long
                var tAngle = reader.ReadDouble();            // double
                var tFirstDetectTime = reader.ReadInt64();   // long long
                var tPriority = reader.ReadInt32();          // int
                var tHit = reader.ReadByte();                // uint8_t

                targets.Add(new MockTarget
                {
                    Id = tId,
                    X = ConvertLongToDouble(tPosX),
                    Y = ConvertLongToDouble(tPosY),
                    Z = tHeight,
                    Angle = tAngle,
                    Speed = (int)tSpeed
                });
            }

            // 6. MissileStatus 파싱
            var missileId = reader.ReadInt32();               // uint8_t
            var missilePosX = reader.ReadInt64();            // long long
            var missilePosY = reader.ReadInt64();            // long long
            var missileHeight = reader.ReadInt64();          // long long
            var missileSpeed = reader.ReadInt64();           // long long
            var missileAngle = reader.ReadDouble();          // double
            var missilePredictedTime = reader.ReadInt64();   // long long
            var missileHit = reader.ReadByte();              // uint8_t

            var missile = new MockMissile
            {
                Id = missileId,
                X = ConvertLongToDouble(missilePosX),
                Y = ConvertLongToDouble(missilePosY),
                Z = missileHeight,
                Angle = missileAngle,
                Speed = (int)missileSpeed
            };

            // ViewModel에 데이터 전달
            MissileMapVM.UpdateMissiles(new[] { missile });
            TargetMapVM.UpdateTargets(targets); // 주석 해제 필요
            LCMapVM.UpdateLC(lc);
            LSMapVM.UpdateLS(ls);
            MFRMapVM.UpdateMFR(mfr);
        }
    }
}