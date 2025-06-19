using System.Collections.ObjectModel;
using System.ComponentModel;
using Airsuface_map.Models;
using System.Linq;

namespace Airsuface_map.ViewModels
{
    public class MissileMapViewModel : INotifyPropertyChanged
    {
        public ObservableCollection<MockMissile> Missiles { get; } = new();

        // 최초 미사일 정보 리스트 (Id 기준)
        public ObservableCollection<MockMissile> FirstMissileInfos { get; } = new();

        public event PropertyChangedEventHandler? PropertyChanged;

        // 미사일 데이터 갱신 메서드 (Id 기준으로 업데이트)
        public void UpdateMissiles(IEnumerable<MockMissile> newMissiles)
        {
            foreach (var newMissile in newMissiles)
            {
                var existing = Missiles.FirstOrDefault(m => m.Id == newMissile.Id);
                if (existing != null)
                {
                    // 기존 미사일 정보 갱신
                    existing.X = newMissile.X;
                    existing.Y = newMissile.Y;
                    existing.Z = newMissile.Z;
                    existing.Angle = newMissile.Angle;
                    existing.Speed = newMissile.Speed;
                }
                else
                {
                    // 새 미사일 추가
                    Missiles.Add(newMissile);

                    // 최초 정보 저장 (깊은 복사)
                    FirstMissileInfos.Add(new MockMissile
                    {
                        Id = newMissile.Id,
                        X = newMissile.X,
                        Y = newMissile.Y,
                        Z = newMissile.Z,
                        Angle = newMissile.Angle,
                        Speed = newMissile.Speed
                    });
                }
            }

            // 더 이상 존재하지 않는 미사일 제거
            var toRemove = Missiles.Where(m => !newMissiles.Any(nm => nm.Id == m.Id)).ToList();
            foreach (var m in toRemove)
            {
                Missiles.Remove(m);

                // 최초 정보도 같이 제거
                var firstInfo = FirstMissileInfos.FirstOrDefault(f => f.Id == m.Id);
                if (firstInfo != null)
                    FirstMissileInfos.Remove(firstInfo);
            }
        }
    }
}