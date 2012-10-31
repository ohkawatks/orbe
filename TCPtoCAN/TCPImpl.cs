using System;
using System.Collections.Generic;
using System.Text;
using System.Net;
using System.Net.Sockets;

namespace TCPtoCAN_Bridge
{
    class TCPImpl
    {
        static string HOST = "127.0.0.1";
//        static string HOST = "192.168.1.11";
        static int PORT = 5656;

        static NetworkStream ns;
        static TcpClient tcp;
        static TcpListener listener;

        public static void TCP_Init()
        {
            string host = HOST;
            int port = PORT;

            IPAddress ipAddr = Dns.GetHostEntry(host).AddressList[0];
            listener = new TcpListener(ipAddr, port);
            listener.Start();

            tcp = listener.AcceptTcpClient();
            ns = tcp.GetStream();
        }

        public static void TCP_Send(byte[] sendBytes, int length)
        {
            ns.Write(sendBytes, 0, length);

            if (Debug.TCP_DEBUG)
            {
                Debug.DebugPrint("TCP_Send data = ", length, sendBytes);
            }
        }

        public static int TCP_Recv(byte[] recvBytes)
        {

            int recvSize = 0;

            try
            {
                if (TCPtoCANMain.dualThread || ns.DataAvailable)
                    recvSize = ns.Read(recvBytes, 0, recvBytes.Length);
                else
                    return -1;
            }
            catch // TCPのresetが来た時
            {
                Console.WriteLine("クライアントが切断しました。");

                TCPtoCANMain.TcpToCan_execute = false;
                TCPtoCANMain.CanToTcp_execute = false;
            }

            if (Debug.TCP_DEBUG)
            {
                Debug.DebugPrint("TCP_Recv data = ", recvSize, recvBytes);
                Console.WriteLine("recvSize = " + recvSize);
            }

            return recvSize;
        }

        public static void TCP_Close()
        {
            tcp.Close();
            listener.Stop();
        }
    }
}
