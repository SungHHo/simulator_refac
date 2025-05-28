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
            GMap.NET.GMaps.Instance.Mode = GMap.NET.AccessMode.ServerOnly;

            MapControl.Position = new PointLatLng(37.5665, 126.9780);
            MapControl.Zoom = 12;

            MapControl.MouseWheel += MapControl_MouseWheel;
            MapControl.OnPositionChanged += MapControl_OnPositionChanged;

            //AddMarker("발사대", new PointLatLng(37.5665, 126.9780));
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
                var marker = CreateLSMarker(ls);
                MapControl.Markers.Add(marker);
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
                var marker = CreateMFRMarker(mfr);
                MapControl.Markers.Add(marker);
            }
        }

        private GMapMarker CreateMissileMarker(Airsuface_map.Models.MockMissile missile)
        {
            var ellipse = new Ellipse
            {
                Width = 12,
                Height = 12,
                Stroke = Brushes.Blue,
                StrokeThickness = 2,
                Fill = Brushes.LightBlue
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
            var stackPanel = new StackPanel();
            stackPanel.Children.Add(ellipse);
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
            var ellipse = new Ellipse
            {
                Width = 12,
                Height = 12,
                Stroke = Brushes.Orange,
                StrokeThickness = 2,
                Fill = Brushes.Orange
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
            var stackPanel = new StackPanel();
            stackPanel.Children.Add(ellipse);
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
            var ellipse = new Ellipse
            {
                Width = 12,
                Height = 12,
                Stroke = Brushes.Green,
                StrokeThickness = 2,
                Fill = Brushes.LightGreen
            };
            var text = new TextBlock
            {
                Text = $"LC {lc.Id}",
                Foreground = Brushes.Black,
                Background = Brushes.White,
                FontWeight = FontWeights.Bold,
                FontSize = 11,
                Margin = new Thickness(0, 0, 0, 0),
                HorizontalAlignment = HorizontalAlignment.Center
            };
            var stackPanel = new StackPanel();
            stackPanel.Children.Add(ellipse);
            stackPanel.Children.Add(text);
            stackPanel.Tag = $"LC_{lc.Id}";

            var marker = new GMapMarker(new PointLatLng(lc.X, lc.Y))
            {
                Shape = stackPanel
            };
            return marker;
        }

        private GMapMarker CreateLSMarker(Airsuface_map.Models.LS ls)
        {
            var ellipse = new Ellipse
            {
                Width = 12,
                Height = 12,
                Stroke = Brushes.Green,
                StrokeThickness = 2,
                Fill = Brushes.LightGreen
            };
            var text = new TextBlock
            {
                Text = $"LS {ls.Id}",
                Foreground = Brushes.Black,
                Background = Brushes.White,
                FontWeight = FontWeights.Bold,
                FontSize = 11,
                Margin = new Thickness(0, 0, 0, 0),
                HorizontalAlignment = HorizontalAlignment.Center
            };
            var stackPanel = new StackPanel();
            stackPanel.Children.Add(ellipse);
            stackPanel.Children.Add(text);
            stackPanel.Tag = $"LS_{ls.Id}";

            var marker = new GMapMarker(new PointLatLng(ls.X, ls.Y))
            {
                Shape = stackPanel
            };
            return marker;
        }

        private GMapMarker CreateMFRMarker(Airsuface_map.Models.MFR mfr)
        {
            var ellipse = new Ellipse
            {
                Width = 12,
                Height = 12,
                Stroke = Brushes.Green,
                StrokeThickness = 2,
                Fill = Brushes.LightGreen
            };
            var text = new TextBlock
            {
                Text = $"MFR {mfr.Id}",
                Foreground = Brushes.Black,
                Background = Brushes.White,
                FontWeight = FontWeights.Bold,
                FontSize = 11,
                Margin = new Thickness(0, 0, 0, 0),
                HorizontalAlignment = HorizontalAlignment.Center
            };
            var stackPanel = new StackPanel();
            stackPanel.Children.Add(ellipse);
            stackPanel.Children.Add(text);
            stackPanel.Tag = $"MFR_{mfr.Id}";

            var marker = new GMapMarker(new PointLatLng(mfr.X, mfr.Y))
            {
                Shape = stackPanel
            };
            return marker;
        }

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