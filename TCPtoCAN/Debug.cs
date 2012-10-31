using System;
using System.Collections.Generic;
using System.Text;

namespace TCPtoCAN_Bridge
{
    class Debug
    {
        static public bool MAIN_DEBUG = false;
        static public bool CAN_DEBUG = false;
        static public bool DUMMY_CAN_DEBUG = false;
        static public bool TCP_DEBUG = false;
        static public bool TIME_DEBUG = false;

        static public void DebugPrint(string msg, int length, byte[] data)
        {
            Console.Write(msg);
            for (int i = 0; i < length; i++)
                Console.Write("{0:X2} ", data[i]);
            Console.Write("\n");
        }
    }
}
