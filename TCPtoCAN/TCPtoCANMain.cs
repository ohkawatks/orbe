using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using System.IO;

namespace TCPtoCAN_Bridge
{
    public class TCPtoCANMain
    {
        static uint BUF_SIZE = 4096;
        static int GIOP_HDR = 12;
        static uint send_canid = 0x101;

        static public bool dualThread = true;

        static Thread cantotcp;
        static Thread tcptocan;

        static public bool CanToTcp_execute = true;
        static public bool TcpToCan_execute = true;
        static public bool Single_Thread = true;

        byte[] tcdata = new byte[BUF_SIZE];
        byte[] tmp_buf = new byte[8];
        int current_index = 0, recvSize = 0;
        int length = 0;
        byte[] ctdata = new byte[BUF_SIZE];
        byte[] tmp_buf_CanToTcp = new byte[8];
        int current_index_CanToTcp = 0, giop_length = 0;
        byte rlength = 0;
        uint ret;

        static public uint baudraute;

        Stopwatch sw_tcptocan = new Stopwatch();
        Stopwatch sw_cantotcp = new Stopwatch();


        static void Main(string[] args)
        {
            if (args.Length == 1)
            {
                setBaudraute(args[0]);
            }
            else if ((args.Length == 2) && (args[1].Equals("-d")))
            {
                setBaudraute(args[0]);
                Debug.CAN_DEBUG = true;
            }
            else
            {
                Console.WriteLine("Usage: TCPtoCAN_Bridge baudraute [-d]");
                return;
            }
            
            new TCPtoCANMain().run();
        }

        static public void setBaudraute(String br)
        {
            baudraute = uint.Parse(br);
            Console.WriteLine("Baudraute = " + baudraute + "kbps");
        }

        static public uint getBaudraute()
        {
            return (baudraute * 1000);
        }
        
        public TCPtoCANMain()
        {
            CANImpl.CAN_Init();
            TCPImpl.TCP_Init();

        }

        public void run()
        {
            if (dualThread)
            {
                tcptocan = new Thread(new ThreadStart(TcpToCan));
                cantotcp = new Thread(new ThreadStart(CanToTcp));

                tcptocan.Start();
                cantotcp.Start();

                tcptocan.Join();
                cantotcp.Join();

                Close();
            }
            else
            {
                while (Single_Thread)
                {
                    stepTcpToCan();
                    Thread.Sleep(1);
                    stepCanToTcp();
                }

                Close();
            }
        }

        private static void Close()
        {
            CANImpl.CAN_Close();
            TCPImpl.TCP_Close();
        }

        private void TcpToCan() // TcpToCan Thread
        {
            if (Debug.MAIN_DEBUG)
            {
                Console.WriteLine("TcpToCan start");
            }


            while (TcpToCan_execute)
            {
                stepTcpToCan();
            }

            if (Debug.MAIN_DEBUG)
            {
                Console.WriteLine("TcpToCan stop");
            }
        }

        private void stepTcpToCan()
        {
            recvSize = TCPImpl.TCP_Recv(tcdata);



            if (recvSize == -1)
            {
                return;
            }
            else if (recvSize == 0)
            {
                TcpToCan_execute = false;
                CanToTcp_execute = false;
                Single_Thread = false;
                return;
            }
            else
            {
                FileStream outf = new FileStream("can.data", FileMode.Create);
                outf.Write(tcdata, 0, recvSize);
                outf.Close();

                if (Debug.TIME_DEBUG)
                    sw_tcptocan.Start();

                while ((recvSize - current_index) > 0)
                {
                    if ((length = recvSize - current_index) > 8)
                        length = 8;

                    Array.Copy(tcdata, current_index, tmp_buf, 0, length);
                    current_index += length;

                    //Console.WriteLine("CAN_Send 1 " + (DateTime.Now.Ticks));// / 10000));
                    CANImpl.CAN_Send(send_canid, (byte)length, tmp_buf);
                    //Console.WriteLine("CAN_Send 2 " + (DateTime.Now.Ticks));// / 10000));
                }
                current_index = 0;

                if (Debug.TIME_DEBUG)
                    sw_tcptocan.Stop();
            }

            if (Debug.TIME_DEBUG)
            {
                Console.WriteLine("TCPtoCAN = " + sw_tcptocan.ElapsedMilliseconds + "msec");
                sw_tcptocan.Reset();
            }
        }

        private void CanToTcp() // CanToTcp Thread
        {
            if (Debug.MAIN_DEBUG)
            {
                Console.WriteLine("CanToTcp start");
            }

            while (CanToTcp_execute)
            {
                stepCanToTcp();
            }

            if (Debug.MAIN_DEBUG)
            {
                Console.WriteLine("CanToTcp stop");
            }
        }

        void stepCanToTcp()
        {
            while (true)
            {
                //Console.WriteLine("CanToTcp_execute");
                ret = CANImpl.CAN_Recv(rlength, tmp_buf_CanToTcp);

                if (ret == 2)
                {
                    // Echo back
                    return;
                }
                else if (ret == 0)
                {
                    //Console.WriteLine("CAN_Recv " + (DateTime.Now.Ticks));// / 10000));
                    rlength = 8;
                    if (Debug.MAIN_DEBUG)
                    {
                        Console.WriteLine("rlength = " + rlength);
                    }

                    Array.Copy(tmp_buf_CanToTcp, 0, ctdata, current_index_CanToTcp, rlength);
                    current_index_CanToTcp += rlength;

                    if ((giop_length == 0) && (current_index_CanToTcp >= GIOP_HDR))
                    {
                        giop_length = getLength(ctdata);
                        if (Debug.TIME_DEBUG)
                            sw_cantotcp.Start();

                        if (Debug.MAIN_DEBUG)
                            Console.WriteLine("giop_length = " + giop_length);
                    }

                    if ((giop_length + GIOP_HDR) > current_index_CanToTcp)
                    {
                        continue;
                    }

                    break;
                }
                else
                {
                    if (dualThread)
                        Thread.Sleep(1);
                    return;
                }
            }

            try
            {
                TCPImpl.TCP_Send(ctdata, (giop_length + GIOP_HDR));
                giop_length = 0;
                current_index_CanToTcp = 0;

                if (Debug.TIME_DEBUG)
                {
                    sw_cantotcp.Stop();
                    Console.WriteLine("CANtoTCP = " + sw_cantotcp.ElapsedMilliseconds + "msec");
                    sw_cantotcp.Reset();
                }
            }
            catch // TCP‚Ìfin‚æ‚è‚àŒã‚ÉSend‚µ‚æ‚¤‚Æ‚µ‚½Žž
            {
                Single_Thread = false;
                return;
            }
        }

        static int getLength(byte[] buf) // Get GIOP message length
        {
            int length = 0;
            byte[] work = new byte[4];
            byte tmp;
            byte endian = buf[6];

            Array.Copy(buf, 8, work, 0, 4);

            if (endian == 1)
            {
                tmp = work[0];
                work[0] = work[3];
                work[3] = tmp;
                tmp = work[1];
                work[1] = work[2];
                work[2] = tmp;
            }

            length += (work[0] << 24);
            length += (work[1] << 16);
            length += (work[2] << 8);
            length += work[3];

            return length;
        }
    }
}
