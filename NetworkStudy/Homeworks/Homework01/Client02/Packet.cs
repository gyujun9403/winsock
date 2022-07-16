using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace csharp_test_client
{
    struct PacketData
    {
        public Int16 DataSize;
        public Int16 PacketID;
        public SByte Type;
        public byte[] BodyData;
    }

    public class PacketDump
    {
        public static string Bytes(byte[] byteArr)
        {
            StringBuilder sb = new StringBuilder("[");
            for (int i = 0; i < byteArr.Length; ++i)
            {
                sb.Append(byteArr[i] + " ");
            }
            sb.Append("]");
            return sb.ToString();
        }
    }
    

    public class ErrorNtfPacket
    {
        public ERROR_CODE Error;

        public bool FromBytes(byte[] bodyData)
        {
            Error = (ERROR_CODE)BitConverter.ToInt16(bodyData, 0);
            return true;
        }
    }

    public class Calcu2ReqPacket
    {
        int N1;
        short Op1;  // +(0), -(1), *(2), %(3)
        int N2;       

        public void SetValue(int n1, short op, int n2)
        {
            N1 = n1;
            N2 = n2;
            Op1 = op;
        }

        public byte[] ToBytes()
        {
            List<byte> dataSource = new List<byte>();
            dataSource.AddRange(BitConverter.GetBytes(N1));
            dataSource.AddRange(BitConverter.GetBytes(Op1));
            dataSource.AddRange(BitConverter.GetBytes(N2));
            return dataSource.ToArray();
        }
    }

    public class Calcu3ReqPacket
    {
        int N1;
        short Op1;
        int N2; 
        short Op2;
        int N3;

        public void SetValue(int n1, short op1, int n2, short op2, int n3)
        {
            N1 = n1;
            N2 = n2;
            N3 = n3;
            Op1 = op1;
            Op2 = op2;
        }

        public byte[] ToBytes()
        {
            List<byte> dataSource = new List<byte>();
            dataSource.AddRange(BitConverter.GetBytes(N1));
            dataSource.AddRange(BitConverter.GetBytes(Op1));
            dataSource.AddRange(BitConverter.GetBytes(N2));
            dataSource.AddRange(BitConverter.GetBytes(Op2));
            dataSource.AddRange(BitConverter.GetBytes(N3));
            return dataSource.ToArray();
        }
    }

    public class Calcu2ResPacket
    {
        public Int32 Num;

        public bool FromBytes(byte[] bodyData)
        {
            Num = BitConverter.ToInt32(bodyData, 0);
            return true;
        }
    }


    
}
