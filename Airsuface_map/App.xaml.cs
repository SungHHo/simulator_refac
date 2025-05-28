using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using Airsuface_map.Services;
using Airsuface_map.ViewModels;

namespace Airsuface_map
{
    public partial class App : Application
    {
        private TcpListener? _server;
        private CancellationTokenSource? _cts;
        public MainViewModel? _mainViewModel { get; private set; } = null!;

        protected override void OnStartup(StartupEventArgs e)
        {
            base.OnStartup(e);

            _mainViewModel = new MainViewModel();

            // 서버 소켓 시작 (0.0.0.0:9001)
            _cts = new CancellationTokenSource();
            _server = new TcpListener(IPAddress.Any, 9001);
            _server.Start();
            Task.Run(() => AcceptClientsAsync(_cts.Token));
        }

        private async Task AcceptClientsAsync(CancellationToken token)
        {
            try
            {
                while (!token.IsCancellationRequested)
                {
                    var client = await _server!.AcceptTcpClientAsync(token);
                    _ = Task.Run(() => HandleClientAsync(client, token), token);
                }
            }
            catch (OperationCanceledException)
            {
                // 정상 종료
            }
        }

        private async Task HandleClientAsync(TcpClient client, CancellationToken token)
        {
            using (client)
            using (var stream = client.GetStream())
            {
                var buffer = new byte[4096];
                while (!token.IsCancellationRequested)
                {
                    int bytesRead = 0;
                    try
                    {
                        bytesRead = await stream.ReadAsync(buffer, 0, buffer.Length, token);
                    }
                    catch
                    {
                        break;
                    }
                    if (bytesRead == 0) break; // 연결 종료

                    var data = new byte[bytesRead];
                    Array.Copy(buffer, data, bytesRead);

                    // UI 스레드에서 ViewModel에 데이터 전달
                    Application.Current.Dispatcher.Invoke(() =>
                    {
                        _mainViewModel?.OnDataReceived(data);
                    });
                }
            }
        }

        protected override void OnExit(ExitEventArgs e)
        {
            _cts?.Cancel();
            _server?.Stop();
            base.OnExit(e);
        }
    }
}