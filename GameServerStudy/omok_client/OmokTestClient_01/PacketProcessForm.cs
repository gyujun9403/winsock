using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace csharp_test_client
{
    public partial class mainForm
    {
        Dictionary<PACKET_ID, Action<byte[]>> PacketFuncDic = new Dictionary<PACKET_ID, Action<byte[]>>();

        void SetPacketHandler()
        {
            //PacketFuncDic.Add(PACKET_ID.PACKET_ID_ERROR_NTF, PacketProcess_ErrorNotify);
            PacketFuncDic.Add(PACKET_ID.LOGIN_RES, PacketProcess_LoginResponse);
            PacketFuncDic.Add(PACKET_ID.ROOM_ENTER_RES, PacketProcess_RoomEnterResponse);
            PacketFuncDic.Add(PACKET_ID.ROOM_USER_LIST_NTF, PacketProcess_RoomUserListNotify);
            PacketFuncDic.Add(PACKET_ID.ROOM_NEW_USER_NTF, PacketProcess_RoomNewUserNotify);
            PacketFuncDic.Add(PACKET_ID.ROOM_LEAVE_RES, PacketProcess_RoomLeaveResponse);
            PacketFuncDic.Add(PACKET_ID.ROOM_LEAVE_USER_NTF, PacketProcess_RoomLeaveUserNotify);
            PacketFuncDic.Add(PACKET_ID.ROOM_CHAT_RES, PacketProcess_RoomChatResponse);            
            PacketFuncDic.Add(PACKET_ID.ROOM_CHAT_NOTIFY, PacketProcess_RoomChatNotify);
            PacketFuncDic.Add(PACKET_ID.OMOK_PLACE_STONE_RES, PacketProcess_PlaceStoneResponse);
            PacketFuncDic.Add(PACKET_ID.OMOK_PLACE_STONE_NTF, PacketProcess_PlaceStoneNotify);
            PacketFuncDic.Add(PACKET_ID.OMOK_RESULT_NTF, PacketProcess_GameResultNotify);
            PacketFuncDic.Add(PACKET_ID.OMOK_READY_RES, PacketProcess_ReadyResponse);
            PacketFuncDic.Add(PACKET_ID.OMOK_GAME_START_NTF, PacketProcess_GameStartNotify);
            PacketFuncDic.Add(PACKET_ID.OMOK_TURN_NTF, PacketProcess_GameTurnNotify);
            //PacketFuncDic.Add(PACKET_ID.PACKET_ID_ROOM_RELAY_NTF, PacketProcess_RoomRelayNotify);
        }

        void PacketProcess(PacketData packet)
        {
            var packetType = (PACKET_ID)packet.PacketID;
            //DevLog.Write("Packet Error:  PacketID:{packet.PacketID.ToString()},  Error: {(ERROR_CODE)packet.Result}");
            //DevLog.Write("RawPacket: " + packet.PacketID.ToString() + ", " + PacketDump.Bytes(packet.BodyData));

            if (PacketFuncDic.ContainsKey(packetType))
            {
                PacketFuncDic[packetType](packet.BodyData);
            }
            else
            {
                DevLog.Write("Unknown Packet Id: " + packet.PacketID.ToString());
            }         
        }

        void PacketProcess_ErrorNotify(byte[] bodyData)
        {
            var notifyPkt = new ErrorNtfPacket();
            notifyPkt.FromBytes(bodyData);

            DevLog.Write($"에러 통보 받음:  {notifyPkt.Error}");
        }


        void PacketProcess_LoginResponse(byte[] bodyData)
        {
            var responsePkt = new LoginResPacket();
            responsePkt.FromBytes(bodyData);

            DevLog.Write($"로그인 결과:  {(ERROR_CODE)responsePkt.Result}");
        }


        void PacketProcess_RoomEnterResponse(byte[] bodyData)
        {
            var responsePkt = new RoomEnterResPacket();
            responsePkt.FromBytes(bodyData);

            DevLog.Write($"방 입장 결과:  {(ERROR_CODE)responsePkt.Result}");
        }

        void PacketProcess_RoomUserListNotify(byte[] bodyData)
        {
            var notifyPkt = new RoomUserListNtfPacket();
            notifyPkt.FromBytes(bodyData);

            for (int i = 0; i < notifyPkt.UserCount; ++i)
            {
                AddRoomUserList(notifyPkt.UserUniqueIdList[i], notifyPkt.UserIDList[i]);
            }

            DevLog.Write($"방의 기존 유저 리스트 받음");
        }

        void PacketProcess_RoomNewUserNotify(byte[] bodyData)
        {
            var notifyPkt = new RoomNewUserNtfPacket();
            notifyPkt.FromBytes(bodyData);

            AddRoomUserList(notifyPkt.UserUniqueId, notifyPkt.UserID);
            
            DevLog.Write($"방에 새로 들어온 유저 받음");
        }


        void PacketProcess_RoomLeaveResponse(byte[] bodyData)
        {
            var responsePkt = new RoomLeaveResPacket();
            responsePkt.FromBytes(bodyData);

            DevLog.Write($"방 나가기 결과:  {(ERROR_CODE)responsePkt.Result}");

            RoomUserListClear();
        }
        void RoomUserListClear()
        {
            listBoxRoomUserList.Items.Clear();
            listBoxRoomChatMsg.Items.Clear();
        }

        void PacketProcess_RoomLeaveUserNotify(byte[] bodyData)
        {
            var notifyPkt = new RoomLeaveUserNtfPacket();
            notifyPkt.FromBytes(bodyData);

            RemoveRoomUserList(notifyPkt.UserUniqueId);

            DevLog.Write($"방에서 나간 유저 받음");

            RoomUserDelete(notifyPkt.UserUniqueId, notifyPkt.UserID);
        }

        void RoomUserDelete(Int64 userUniqueId, string userID)
        {
            var msg = $"{userUniqueId}: {userID}";
            listBoxRoomUserList.Items.Remove(msg);
        }


        void PacketProcess_RoomChatResponse(byte[] bodyData)
        {
            var responsePkt = new RoomChatResPacket();
            responsePkt.FromBytes(bodyData);

            var errorCode = (ERROR_CODE)responsePkt.Result;
            var msg = $"방 채팅 요청 결과:  {(ERROR_CODE)responsePkt.Result}";
            if (errorCode == ERROR_CODE.ERROR_NONE)
            {
                DevLog.Write(msg, LOG_LEVEL.ERROR);
            }
            else
            {
                AddRoomChatMessageList(0, msg);
            }
        }


        void PacketProcess_RoomChatNotify(byte[] bodyData)
        {
            var responsePkt = new RoomChatNtfPacket();
            responsePkt.FromBytes(bodyData);

            AddRoomChatMessageList(responsePkt.UserUniqueId, responsePkt.Message);
        }

        void AddRoomChatMessageList(Int64 userUniqueId, string msgssage)
        {
            var msg = $"{userUniqueId}:  {msgssage}";

            if (listBoxRoomChatMsg.Items.Count > 512)
            {
                listBoxRoomChatMsg.Items.Clear();
            }

            listBoxRoomChatMsg.Items.Add(msg);
            listBoxRoomChatMsg.SelectedIndex = listBoxRoomChatMsg.Items.Count - 1;
        }


        void PacketProcess_RoomRelayNotify(byte[] bodyData)
        {
            var notifyPkt = new RoomRelayNtfPacket();
            notifyPkt.FromBytes(bodyData);

            var stringData = Encoding.UTF8.GetString(notifyPkt.RelayData);
            DevLog.Write($"방에서 릴레이 받음. {notifyPkt.UserUniqueId} - {stringData}");
        }

        // 오목 과제
        //void PacketProcess_OmokPlaceStoneRequest() -> 받아서 처리하는 내용들을 여기에 적어야 하므로, Res나 Ntf를 적는게 맞다.
        void PacketProcess_PlaceStoneResponse(byte[] BodyData)
        {
            var responsePkt = new PlacingStoneResPacket();

            responsePkt.FromBytes(BodyData);

            var errorCode = (ERROR_CODE)responsePkt.Result;
            var msg = $"오목 돌 놓기 결과:  {(ERROR_CODE)responsePkt.Result}";
            if (errorCode == ERROR_CODE.ERROR_NONE)
            {
                DevLog.Write(msg, LOG_LEVEL.ERROR);
            }
            else
            {
                
            }
        }

        void PacketProcess_PlaceStoneNotify(byte[] BodyData)
        {
            var notifyPkt = new PlacingStoneNtfPakcet();

            notifyPkt.FromBytes(BodyData);
            placeStoneAt(notifyPkt.X, notifyPkt.Y, notifyPkt.color);
        }

        void PacketProcess_GameResultResponse(byte[] BodyData)
        { 
        }

        void PacketProcess_GameResultNotify(byte[] BodyData)
        {
            string msg;
            GameResultNtfPacket notifyPkt = new GameResultNtfPacket();

            notifyPkt.FromBytes(BodyData);
            if (notifyPkt.isWin == 1)
            {
                msg = "YOU WIN!!!"; 
            }
            else
            {
                msg = "you are loser ^^&";
            }
            setIsReady(false);
            btnReady.Enabled = true;
            btnReady.Text = "READY";
            btnReady.BackColor = System.Drawing.Color.White;
            DevLog.Write(msg, LOG_LEVEL.ERROR);
            DrawBoard(); //이렇게 막 가지고 와도 되나?
        }

        void PacketProcess_ReadyResponse(byte[] BodyData)
        {
            ReadyResponsePacket resPkt = new ReadyResponsePacket();

            resPkt.FromBytes(BodyData);
            setIsReady(resPkt.isReady);
            if (resPkt.isReady == true)
            {
                btnReady.Text = "WAITING...";
            }
            else
            {
                btnReady.Text = "READY";
            }
        }

        // OMOK_GAME_START_NTF = 87,
        //OMOK_GAME_END_NTF = 88,
        ///OMOK_TURN_NTF = 89,
        void PacketProcess_GameStartNotify(byte[] BodyData)
        {
            btnReady.Enabled = false;
            btnReady.Text = "In Play";
        }

        void PacketProcess_GameTurnNotify(byte[] BodyData)
        {
            GameTurnNtfPacket ntfPkt = new GameTurnNtfPacket();

            ntfPkt.FromBytes(BodyData);
            //ShowWhosTurn(ntfPkt.UserUniqueId, ntfPkt.UserID);'
            if (ntfPkt.isMyturn == true)
            {
                btnReady.BackColor = System.Drawing.Color.Yellow;
                btnReady.Text = "Your Turn";
            }
            else
            {
                btnReady.BackColor = System.Drawing.Color.White;
                btnReady.Text = "In Play";
            }
        }

        //void ShowWhosTurn(Int64 userUniqueId, string userID)
        //{
        //    //var msg = $"{userUniqueId}: {userID}";
        //    ////listBoxRoomUserList.Items.Remove(msg);
        //    //var idx = listBoxRoomUserList.Items.IndexOf(msg);
        //    //listBoxRoomUserList.Items[idx].
        //    if ()

        //}
    }
}
