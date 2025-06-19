using System.Collections.Specialized;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Shapes;
using GMap.NET;
using GMap.NET.WindowsPresentation;
using Airsuface_map.ViewModels;
using System.Windows;
using System.Windows.Threading;
using System.Windows.Input;
using System.Linq;
using System.Windows.Media.Imaging;
using Airsuface_map.Models;
using GMap.NET.MapProviders;
using System.Reflection;

namespace Airsuface_map.Views
{
    public partial class gmapView : UserControl
    {
        private DispatcherTimer _updateTimer;

        private MissileMapViewModel? MissileViewModel => ((App.Current as Airsuface_map.App)?._mainViewModel?.MissileMapVM);
        private TargetMapViewModel? TargetViewModel => ((App.Current as Airsuface_map.App)?._mainViewModel?.TargetMapVM);
        private LCMapViewModel? LCViewModel => ((App.Current as Airsuface_map.App)?._mainViewModel?.LCMapVM);
        private LSMapViewModel? LSViewModel => ((App.Current as Airsuface_map.App)?._mainViewModel?.LSMapVM);
        private MFRMapViewModel? MFRViewModel => ((App.Current as Airsuface_map.App)?._mainViewModel?.MFRMapVM);

        public gmapView()
        {
            InitializeComponent();

            MapControl.MapProvider = GMap.NET.MapProviders.GoogleMapProvider.Instance;
            GMap.NET.GMaps.Instance.Mode = GMap.NET.AccessMode.ServerAndCache;

            MapControl.Position = new PointLatLng(37.5665, 126.9780);
            MapControl.Zoom = 12;

            MapControl.MouseWheel += MapControl_MouseWheel;
            MapControl.OnPositionChanged += MapControl_OnPositionChanged;

            //AddMarker("발사대", new PointLatLng(37.5665, 126.9780));
            //{
            //    var start = new PointLatLng(37.5665, 126.9780);
            //    double angleDegree = 45.0;
            //    double lengthMeters = 500.0;
            //    var (lineStart, lineEnd) = CreateLineByAngle(start, angleDegree, lengthMeters);

            //    // GMap.NET에서 선을 그리려면:
            //    var points = new List<PointLatLng> { lineStart, lineEnd };
            //    var polyline = new GMapRoute(points)
            //    {
            //        Shape = new System.Windows.Shapes.Path
            //        {
            //            Stroke = Brushes.Red,
            //            StrokeThickness = 2
            //        }
            //    };
            //    MapControl.Markers.Add(polyline);
            //}
            //AddMarker("목표지점", new PointLatLng(37.5676438, 127.159259));

            MapControl.CanDragMap = true;
            UpdateCoordinatesText(MapControl.Position);

            if (MissileViewModel != null)
            {
                MissileViewModel.Missiles.CollectionChanged += Missiles_CollectionChanged;
                UpdateMissileMarkers();
            }

            if (TargetViewModel != null)
            {
                TargetViewModel.Targets.CollectionChanged += Targets_CollectionChanged;
                UpdateTargetMarkers();
            }

            if (LCViewModel != null)
            {
                LCViewModel.LCS.CollectionChanged += LCs_CollectionChanged;
                UpdateLCMarkers();
            }

            if (LSViewModel != null)
            {
                LSViewModel.LSS.CollectionChanged += LCs_CollectionChanged;
                UpdateLSMarkers();
            }

            if (MFRViewModel != null)
            {
                MFRViewModel.MFRS.CollectionChanged += MFRs_CollectionChanged;
                UpdateMFRMarkers();
            }

            _updateTimer = new DispatcherTimer();
            _updateTimer.Interval = TimeSpan.FromMilliseconds(100);
            _updateTimer.Tick += (s, e) =>
            {
                UpdateMissileMarkers();
                UpdateTargetMarkers();
                UpdateLCMarkers();
                UpdateLSMarkers();
                UpdateMFRMarkers();
            };
            _updateTimer.Start();
        }

        private void Missiles_CollectionChanged(object? sender, NotifyCollectionChangedEventArgs e)
        {
            Dispatcher.Invoke(UpdateMissileMarkers);
        }

        private void Targets_CollectionChanged(object? sender, NotifyCollectionChangedEventArgs e)
        {
            Dispatcher.Invoke(UpdateTargetMarkers);
        }

        private void LCs_CollectionChanged(object? sender, NotifyCollectionChangedEventArgs e)
        {
            Dispatcher.Invoke(UpdateLCMarkers);
        }

        private void LSs_CollectionChanged(object? sender, NotifyCollectionChangedEventArgs e)
        {
            Dispatcher.Invoke(UpdateLSMarkers);
        }

        private void MFRs_CollectionChanged(object? sender, NotifyCollectionChangedEventArgs e)
        {
            Dispatcher.Invoke(UpdateMFRMarkers);
        }

        private void UpdateMissileMarkers()
        {
            var missileMarkers = MapControl.Markers
                .Where(m => m.Shape is FrameworkElement fe && fe.Tag is string tag && tag.StartsWith("Missile_"))
                .ToList();
            foreach (var marker in missileMarkers)
                MapControl.Markers.Remove(marker);

            var vm = MissileViewModel;
            if (vm == null) return;
            foreach (var missile in vm.Missiles)
            {
                var marker = CreateMissileMarker(missile);
                MapControl.Markers.Add(marker);
            }
        }

        private void UpdateTargetMarkers()
        {
            var targetMarkers = MapControl.Markers
                .Where(m => m.Shape is FrameworkElement fe && fe.Tag is string tag && tag.StartsWith("Target_"))
                .ToList();
            foreach (var marker in targetMarkers)
                MapControl.Markers.Remove(marker);

            var vm = TargetViewModel;
            if (vm == null) return;
            foreach (var target in vm.Targets)
            {
                var marker = CreateTargetMarker(target);
                MapControl.Markers.Add(marker);
            }
        }

        private void UpdateLCMarkers()
        {
            var lcMarkers = MapControl.Markers
                .Where(m => m.Shape is FrameworkElement fe && fe.Tag is string tag && tag.StartsWith("LC_"))
                .ToList();
            foreach (var marker in lcMarkers)
                MapControl.Markers.Remove(marker);

            var vm = LCViewModel;
            if (vm == null) return;
            foreach (var lc in vm.LCS)
            {
                var marker = CreateLCMarker(lc);
                MapControl.Markers.Add(marker);
            }
        }

        private void UpdateLSMarkers()
        {
            var lsMarkers = MapControl.Markers
                .Where(m => m.Shape is FrameworkElement fe && fe.Tag is string tag && tag.StartsWith("LS_"))
                .ToList();
            foreach (var marker in lsMarkers)
                MapControl.Markers.Remove(marker);

            var vm = LSViewModel;
            if (vm == null) return;
            foreach (var ls in vm.LSS)
            {
                var (marker, circle) = CreateLSMarker(ls, 100.0);
                MapControl.Markers.Add(marker);
                MapControl.Markers.Add(circle);
            }
        }

        private void UpdateMFRMarkers()
        {
            var mfrMarkers = MapControl.Markers
                .Where(m => m.Shape is FrameworkElement fe && fe.Tag is string tag && tag.StartsWith("MFR_"))
                .ToList();
            foreach (var marker in mfrMarkers)
                MapControl.Markers.Remove(marker);

            var vm = MFRViewModel;
            if (vm == null) return;
            foreach (var mfr in vm.MFRS)
            {
                var (marker, circle) = CreateMFRMarker(mfr, 100.0);
                MapControl.Markers.Add(marker);
                MapControl.Markers.Add(circle);
            }
        }

        private GMapMarker CreateMissileMarker(Airsuface_map.Models.MockMissile missile)
        {
            var image = new Image
            {
                Width = 20,
                Height = 20,
                Source = new BitmapImage(new Uri(System.IO.Path.GetFullPath("Friend-Air.png"))),
                Stretch = Stretch.Uniform
            };

            var text = new TextBlock
            {
                Text = $"미사일 {missile.Id}",
                Foreground = Brushes.Black,
                Background = Brushes.White,
                FontWeight = FontWeights.Bold,
                FontSize = 11,
                Margin = new Thickness(0, 0, 0, 0),
                HorizontalAlignment = HorizontalAlignment.Center
            };

            // 각도(도)를 라디안으로 변환
            double angleRad = missile.Angle * Math.PI / 180.0;
            double lineLength = 500; // px 단위

            // 선의 끝점 계산 (StackPanel 기준)
            double x2 = Math.Sin(angleRad) * lineLength;
            double y2 = -Math.Cos(angleRad) * lineLength;

            var line = new Line
            {
                X1 = 10, // 이미지 중앙(20/2)
                Y1 = 20, // 이미지 하단
                X2 = 10 + x2,
                Y2 = 20 + y2,
                Stroke = Brushes.Red,
                StrokeThickness = 2
            };

            var canvas = new Canvas
            {
                Width = Math.Max(20, Math.Abs(x2) + 20),
                Height = Math.Max(20, Math.Abs(y2) + 20)
            };
            Canvas.SetLeft(image, 0);
            Canvas.SetTop(image, 0);
            canvas.Children.Add(image);
            canvas.Children.Add(line);

            var stackPanel = new StackPanel();
            stackPanel.Children.Add(canvas);
            stackPanel.Children.Add(text);
            stackPanel.Tag = $"Missile_{missile.Id}";

            var marker = new GMapMarker(new PointLatLng(missile.X, missile.Y))
            {
                Shape = stackPanel
            };
            return marker;
        }

        private GMapMarker CreateTargetMarker(Airsuface_map.Models.MockTarget target)
        {
            var image = new Image
            {
                Width = 20,
                Height = 20,
                Source = new BitmapImage(new Uri(System.IO.Path.GetFullPath("Air-Hostile.png"))),
                Stretch = Stretch.Uniform
            };

            var text = new TextBlock
            {
                Text = $"타겟 {target.Id}",
                Foreground = Brushes.Black,
                Background = Brushes.White,
                FontWeight = FontWeights.Bold,
                FontSize = 11,
                Margin = new Thickness(0, 0, 0, 0),
                HorizontalAlignment = HorizontalAlignment.Center
            };

            //// 각도(도)를 라디안으로 변환
            //double angleRad = target.Angle * Math.PI / 180.0;
            //double lineLength = 500; // px 단위

            //// 선의 끝점 계산 (StackPanel 기준)
            //double x2 = Math.Sin(angleRad) * lineLength;
            //double y2 = -Math.Cos(angleRad) * lineLength;

            //var line = new Line
            //{
            //    X1 = 10, // 이미지 중앙(20/2)
            //    Y1 = 20, // 이미지 하단
            //    X2 = 10 + x2,
            //    Y2 = 20 + y2,
            //    Stroke = Brushes.Green,
            //    StrokeThickness = 2
            //};

            //var canvas = new Canvas
            //{
            //    Width = Math.Max(20, Math.Abs(x2) + 20),
            //    Height = Math.Max(20, Math.Abs(y2) + 20)
            //};
            //Canvas.SetLeft(image, 0);
            //Canvas.SetTop(image, 0);
            //canvas.Children.Add(image);
            //canvas.Children.Add(line);

            var stackPanel = new StackPanel();
            stackPanel.Children.Add(image);
            stackPanel.Children.Add(text);
            stackPanel.Tag = $"Target_{target.Id}";

            var marker = new GMapMarker(new PointLatLng(target.X, target.Y))
            {
                Shape = stackPanel
            };
            return marker;
        }

        private GMapMarker CreateLCMarker(Airsuface_map.Models.LC lc)
        {
            double ellipseSize = 80;
            double imageSize = 20;

            //// 원(반경 표시)
            //var ellipse = new Ellipse
            //{
            //    Width = ellipseSize,
            //    Height = ellipseSize,
            //    Stroke = Brushes.Blue,
            //    StrokeThickness = 3,
            //    Fill = Brushes.Transparent,
            //    Opacity = 0.4
            //};

            // 마커 이미지
            var image = new Image
            {
                Width = imageSize,
                Height = imageSize,
                Source = new BitmapImage(new Uri(System.IO.Path.GetFullPath("Friend-Units.png"))),
                Stretch = Stretch.Uniform
            };

            // 텍스트 (배경을 투명으로)
            var text = new TextBlock
            {
                Text = $"LC {lc.Id}",
                Foreground = Brushes.Black,
                Background = Brushes.Transparent, // 반드시 투명!
                FontWeight = FontWeights.Bold,
                FontSize = 11,
                Margin = new Thickness(0, 2, 0, 0),
                HorizontalAlignment = HorizontalAlignment.Center,
                TextAlignment = TextAlignment.Center
            };

            // Overlay용 Canvas (모든 요소를 겹치게 배치)
            var overlay = new Canvas
            {
                Width = ellipseSize,
                Height = ellipseSize
            };

            //// 원을 가장 위에 (ZIndex 높게)
            //Canvas.SetLeft(ellipse, 0);
            //Canvas.SetTop(ellipse, 0);
            //Panel.SetZIndex(ellipse, 100);
            //overlay.Children.Add(ellipse);

            // 이미지를 중앙에
            Canvas.SetLeft(image, (ellipseSize - imageSize) / 2);
            Canvas.SetTop(image, (ellipseSize - imageSize) / 2);
            Panel.SetZIndex(image, 10);
            overlay.Children.Add(image);

            // 텍스트를 이미지 바로 아래에
            Canvas.SetLeft(text, (ellipseSize - imageSize) / 2);
            Canvas.SetTop(text, (ellipseSize + imageSize) / 2 + 2); // 이미지 하단 + 약간의 간격
            Panel.SetZIndex(text, 10);
            overlay.Children.Add(text);

            // 중심이 실제 좌표에 오도록 설정
            overlay.RenderTransformOrigin = new Point(0.5, 0.5);
            overlay.RenderTransform = new TranslateTransform(-ellipseSize / 2, -ellipseSize / 2);

            overlay.Tag = $"LC_{lc.Id}";

            var marker = new GMapMarker(new PointLatLng(lc.X, lc.Y))
            {
                Shape = overlay
            };

            return marker;
        }


        private (GMapMarker marker, GMapPolygon circle) CreateLSMarker(Airsuface_map.Models.LS ls, double radiusKm = 2.0)
        {
            // 마커 이미지
            double imageSize = 20;

            var image = new Image
            {
                Width = imageSize,
                Height = imageSize,
                Source = new BitmapImage(new Uri(System.IO.Path.GetFullPath("Friend-Units.png"))),
                Stretch = Stretch.Uniform
            };

            // 텍스트 (배경을 투명으로)
            var text = new TextBlock
            {
                Text = $"LS {ls.Id}",
                Foreground = Brushes.Black,
                Background = Brushes.Transparent, // 반드시 투명으로!
                FontWeight = FontWeights.Bold,
                FontSize = 11,
                Margin = new Thickness(0, 2, 0, 0),
                HorizontalAlignment = HorizontalAlignment.Center,
                TextAlignment = TextAlignment.Center
            };

            // Canvas로 이미지와 텍스트를 배치
            var canvas = new Canvas
            {
                Width = imageSize,
                Height = imageSize + 18 // 텍스트 높이 고려
            };
            // 이미지 중앙 배치
            Canvas.SetLeft(image, 0);
            Canvas.SetTop(image, 0);
            canvas.Children.Add(image);

            // 텍스트를 이미지 바로 아래에 배치
            Canvas.SetLeft(text, 0);
            Canvas.SetTop(text, imageSize);
            canvas.Children.Add(text);

            // 중심이 실제 좌표에 오도록 Transform 적용 (이미지 중앙이 중심)
            canvas.RenderTransformOrigin = new Point(0.5, 0.0);
            canvas.RenderTransform = new TranslateTransform(-imageSize / 2, 0);

            canvas.Tag = $"LS_{ls.Id}";

            var marker = new GMapMarker(new PointLatLng(ls.X, ls.Y))
            {
                Shape = canvas
            };

            // 실제 반경 원(GMapPolygon)
            var center = new PointLatLng(ls.X, ls.Y);
            var circle = CreateCircle(center, radiusKm, 72, Brushes.Green);
            circle.Tag = $"MFR_CIRCLE_{ls.Id}";

            return (marker, circle);
        }


        private (GMapMarker marker, GMapPolygon circle) CreateMFRMarker(Airsuface_map.Models.MFR mfr, double radiusKm = 2.0)
        {
            // 마커 이미지
            double imageSize = 20;

            var image = new Image
            {
                Width = imageSize,
                Height = imageSize,
                Source = new BitmapImage(new Uri(System.IO.Path.GetFullPath("Friend-Units.png"))),
                Stretch = Stretch.Uniform
            };

            var text = new TextBlock
            {
                Text = $"MFR {mfr.Id}",
                Foreground = Brushes.Black,
                Background = Brushes.White,
                FontWeight = FontWeights.Bold,
                FontSize = 11,
                Margin = new Thickness(0, 2, 0, 0),
                HorizontalAlignment = HorizontalAlignment.Center,
                TextAlignment = TextAlignment.Center
            };

            // Canvas로 이미지와 텍스트를 배치
            var canvas = new Canvas
            {
                Width = imageSize,
                Height = imageSize + 18 // 텍스트 높이 고려
            };
            // 이미지 중앙 배치
            Canvas.SetLeft(image, 0);
            Canvas.SetTop(image, 0);
            canvas.Children.Add(image);

            // 텍스트를 이미지 바로 아래에 배치
            Canvas.SetLeft(text, 0);
            Canvas.SetTop(text, imageSize);
            canvas.Children.Add(text);

            // 중심이 실제 좌표에 오도록 Transform 적용 (이미지 중앙이 중심)
            canvas.RenderTransformOrigin = new Point(0.5, 0.0);
            canvas.RenderTransform = new TranslateTransform(-imageSize / 2, 0);

            canvas.Tag = $"MFR_{mfr.Id}";

            var marker = new GMapMarker(new PointLatLng(mfr.X, mfr.Y))
            {
                Shape = canvas
            };

            // 실제 반경 원(GMapPolygon)
            var center = new PointLatLng(mfr.X, mfr.Y);
            var circle = CreateCircle(center, radiusKm, 72, Brushes.Red);
            circle.Tag = $"MFR_CIRCLE_{mfr.Id}";

            return (marker, circle);
        }

        // 위도/경도 기준 원의 점 리스트 생성 (단위: meter)
        // test
        public static (PointLatLng start, PointLatLng end) CreateLineByAngle(PointLatLng start, double angleDegree, double lengthMeters)
        {
            double earthRadius = 6378137.0; // WGS84 기준 지구 반지름(m)
            double angleRad = angleDegree * Math.PI / 180.0;

            // 위도, 경도를 라디안으로 변환
            double latRad = start.Lat * Math.PI / 180.0;
            double lngRad = start.Lng * Math.PI / 180.0;

            // 거리만큼 이동한 후의 위도/경도 계산
            double dLat = (lengthMeters * Math.Cos(angleRad)) / earthRadius;
            double dLng = (lengthMeters * Math.Sin(angleRad)) / (earthRadius * Math.Cos(latRad));

            double endLat = start.Lat + dLat * 180.0 / Math.PI;
            double endLng = start.Lng + dLng * 180.0 / Math.PI;

            return (start, new PointLatLng(endLat, endLng));
        }

        public static PointLatLng GetEndPointByAngle(PointLatLng start, double angleDegree, double lengthMeters)
        {
            double earthRadius = 6378137.0; // WGS84 기준 지구 반지름(m)
            double angleRad = angleDegree * Math.PI / 180.0;

            // 위도, 경도를 라디안으로 변환
            double latRad = start.Lat * Math.PI / 180.0;

            // 거리만큼 이동한 후의 위도/경도 계산
            double dLat = (lengthMeters * Math.Cos(angleRad)) / earthRadius;
            double dLng = (lengthMeters * Math.Sin(angleRad)) / (earthRadius * Math.Cos(latRad));

            double endLat = start.Lat + dLat * 180.0 / Math.PI;
            double endLng = start.Lng + dLng * 180.0 / Math.PI;

            return new PointLatLng(endLat, endLng);
        }
        // test

        private void MapControl_MouseWheel(object sender, MouseWheelEventArgs e)
        {
            if (e.Delta > 0)
                MapControl.Zoom += 1;
            else if (e.Delta < 0)
                MapControl.Zoom -= 1;
            e.Handled = true;
        }

        private void MapControl_OnPositionChanged(PointLatLng point)
        {
            UpdateCoordinatesText(point);
        }

        private void UpdateCoordinatesText(PointLatLng point)
        {
            CoordinatesTextBlock.Text = $"위도: {point.Lat}, 경도: {point.Lng}";
        }

        private GMapPolygon CreateCircle(PointLatLng center, double radiusKm, int segments, Brush stroke, Brush fill = null)
        {
            var points = new List<PointLatLng>();
            double seg = 360.0 / segments;
            for (int i = 0; i < segments; i++)
            {
                double theta = Math.PI * i * seg / 180.0;
                // 위도 1도 ≈ 111.32km, 경도 1도 ≈ 111.32km * cos(위도)
                double dLat = (radiusKm / 111.32) * Math.Sin(theta);
                double dLng = (radiusKm / (111.32 * Math.Cos(center.Lat * Math.PI / 180))) * Math.Cos(theta);
                points.Add(new PointLatLng(center.Lat + dLat, center.Lng + dLng));
            }
            var polygon = new GMapPolygon(points);

            // WPF Path 스타일 적용
            polygon.Shape = new System.Windows.Shapes.Path
            {
                Stroke = stroke,
                StrokeThickness = 3,
                Fill = fill ?? Brushes.Transparent,
                Opacity = 0.4
            };

            return polygon;
        }

        private void AddMarker(string name, PointLatLng point)
        {
            StackPanel stackPanel = new StackPanel();
            stackPanel.Children.Add(new Ellipse
            {
                Width = 10,
                Height = 10,
                Stroke = Brushes.Red,
                StrokeThickness = 2,
                Fill = Brushes.Red
            });
            stackPanel.Children.Add(new TextBlock
            {
                Text = name,
                Foreground = Brushes.Black,
                Background = Brushes.White,
                FontWeight = FontWeights.Bold,
                FontSize = 12,
                Margin = new Thickness(0, 0, 10, 10),
                HorizontalAlignment = HorizontalAlignment.Center
            });

            GMapMarker marker = new GMapMarker(point)
            {
                Shape = stackPanel
            };
            MapControl.Markers.Add(marker);
        }
    }
}