using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace TCPtoCAN_Bridge
{
    class CANImpl
    {
//        static uint BAUDRATE = 250000; // 250kbps
//        static uint BAUDRATE = 100000; // 100kbps
//        static uint BAUDRATE = 33333;
        static uint can_id;

        [DllImport("UC245A.dll")]
        static extern uint ucCAN_ListDevices();
        [DllImport("UC245A.dll")]
        static extern uint ucCAN_Open();
        [DllImport("UC245A.dll")]
        static extern uint ucCAN_Init(uint baudraute, byte extendmode);
        [DllImport("UC245A.dll")]
        static extern uint ucCAN_Reset();
        [DllImport("UC245A.dll")]
        static extern uint ucCAN_Send(uint id, byte dlc, [MarshalAs(UnmanagedType.LPArray)] byte[] p_data, byte opt);
        [DllImport("UC245A.dll")]
        static extern uint ucCAN_Recv(ref uint id, ref byte dlc, [MarshalAs(UnmanagedType.LPArray)] byte[] p_data, ref ulong time);
        [DllImport("UC245A.dll")]
        static extern uint ucCAN_Recv_Status();
        [DllImport("UC245A.dll")]
        static extern uint ucCAN_Close();

        public static void CAN_Init()
        {
            if (ucCAN_ListDevices() != 1)
            {
                Console.Write("デバイスがありません。");
                CAN_Close();
                System.Environment.Exit(1);
            }

            if (ucCAN_Open() != 0)
            {
                Console.Write("オープンできません。");
                CAN_Close();
                System.Environment.Exit(2);
            }

//            ucCAN_Init(BAUDRATE, 0);
            ucCAN_Init(TCPtoCANMain.getBaudraute(), 0);
            ucCAN_Reset();
        }

        public static void CAN_Send(uint id, byte length, byte[] data)
        {
            ucCAN_Send(id, length, data, 1);
            can_id = id;

            if (Debug.CAN_DEBUG)
            {
                Console.WriteLine("CAN_Send id = " + id);
                Debug.DebugPrint("CAN_Send data = ", length, data);
            }
        }

        public static uint CAN_Recv(byte rlength, byte[] recvBytes)
        {
            uint ret;
            uint rid = 0;
            ulong time_now = 0;

            if (Debug.CAN_DEBUG)
                Console.WriteLine("CAN_RecvStatus = " + ucCAN_Recv_Status());

            ret = ucCAN_Recv(ref rid, ref rlength, recvBytes, ref time_now);
            
            if (Debug.CAN_DEBUG)
            {
                Console.WriteLine("CAN_Recv rid = " + rid + " time_now = " + time_now);
                Debug.DebugPrint("CAN_Recv data = ", rlength, recvBytes);
                Console.WriteLine("CAN_Recv rlength = " + rlength + " ret = " + ret);
            }

//            if ((ret == 0) && (rid == can_id))
//                return 2; // 送信したデータを受信した時
                        
            return ret; // 0:データ受信  1:データなし
        }

        public static void CAN_Close()
        {
            ucCAN_Close();
        }


/*
        // ucCAN ダミー関数 start

//        static byte[] dummy_reply = { 0x47, 0x49, 0x4f, 0x50, 0x01, 0x02, 0x01, 0x01, 0x12, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0xff, 0x38, 0xff, 0x38, 0xff, 0x00, 0x00};
        static byte[] dummy_reply = { 0x47, 0x49, 0x4f, 0x50, 0x01, 0x02, 0x00, 0x01,  0x00, 0x00, 0x00, 0x12, 0x05, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x38, 0xff, 0x38, 0xff, 0x38, 0xff, 0x00, 0x00};

        static uint ucCAN_ListDevices()
        {
            return 1;
        }

        static uint ucCAN_Open()
        {
            return 0;
        }

        static uint ucCAN_Init(uint baudraute, byte extendmode)
        {
            return 0;
        }

        static uint ucCAN_Reset()
        {
            return 0;
        }

        static uint ucCAN_Send(uint id, byte dlc, [MarshalAs(UnmanagedType.LPArray)] byte[] p_data, byte opt)
        {
            if (Debug.DUMMY_CAN_DEBUG)
            {
                Debug.DebugPrint("ucCAN_Send data = ", dlc, p_data);
            }

            return 0;
        }

        static int index = 0;
        
        static uint ucCAN_Recv(ref uint id, ref byte dlc, [MarshalAs(UnmanagedType.LPArray)] byte[] p_data, ref ulong time)
        {
            
            id = 0x201;
            dlc = 8;
            time = 0;

            for (int i = 0; i < 8; i++)
                p_data[i] = dummy_reply[index + i];
            index += 8;

            if (index >= 32)
                index = 0;

            return 0;
        }

        static uint ucCAN_Recv_Status()
        {
            return 0;
        }

        static uint ucCAN_Close()
        {
            return 0;
        }

        // ucCAN ダミー関数 end
*/
    }
}
