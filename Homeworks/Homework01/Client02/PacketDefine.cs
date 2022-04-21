using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace csharp_test_client
{
    class PacketDef
    {
        public const Int16 PACKET_HEADER_SIZE = 5;
    }

    public enum PACKET_ID : ushort
    {
        CALCU_2_REQ = 21,
        CALCU_3_REQ = 22,

        CALCU_RES = 31,
    }


    public enum ERROR_CODE : Int16
    {
        ERROR_NONE = 0,
    }
}
