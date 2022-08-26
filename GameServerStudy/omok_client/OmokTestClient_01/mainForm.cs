using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace csharp_test_client
{
    public partial class mainForm : Form
    {
        ClientSimpleTcp Network = new ClientSimpleTcp();

        bool IsNetworkThreadRunning = false;
        bool IsBackGroundProcessRunning = false;

        System.Threading.Thread NetworkReadThread = null;
        System.Threading.Thread NetworkSendThread = null;

        PacketBufferManager PacketBuffer = new PacketBufferManager();
        Queue<PacketData> RecvPacketQueue = new Queue<PacketData>();
        Queue<byte[]> SendPacketQueue = new Queue<byte[]>();

        Timer dispatcherUITimer;

        // 오목 게임판
        Graphics g;
        Pen pen;
        Brush wBrush, bBrush;
        int margin = 40;
        int 눈Size = 30; // gridSize
        int 돌Size = 28; // stoneSize
        int 화점Size = 10; // flowerSize
        Int16[,] board = new Int16[19, 19];
        bool isReady = false;

        public void setIsReady(bool ready)
        {
            isReady = ready;
        }

        public bool getIsready()
        {
            return isReady;
        }

        public mainForm()
        {
            InitializeComponent();

            // 오목 게임판
            pen = new Pen(Color.Black);
            bBrush = new SolidBrush(Color.Black);
            wBrush = new SolidBrush(Color.White);
        }

        private void mainForm_Load(object sender, EventArgs e)
        {
            PacketBuffer.Init((8096 * 10), PacketDef.PACKET_HEADER_SIZE, 1024);

            IsNetworkThreadRunning = true;
            NetworkReadThread = new System.Threading.Thread(this.NetworkReadProcess);
            NetworkReadThread.Start();
            NetworkSendThread = new System.Threading.Thread(this.NetworkSendProcess);
            NetworkSendThread.Start();

            IsBackGroundProcessRunning = true;            
            dispatcherUITimer = new Timer();
            dispatcherUITimer.Tick += new EventHandler(BackGroundProcess);
            dispatcherUITimer.Interval = 100;
            dispatcherUITimer.Start();

            btnDisconnect.Enabled = false;

            SetPacketHandler();
            DevLog.Write("프로그램 시작 !!!", LOG_LEVEL.INFO);
        }

        private void mainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            IsNetworkThreadRunning = false;
            IsBackGroundProcessRunning = false;

            Network.Close();
        }

        private void btnConnect_Click(object sender, EventArgs e)
        {
            string address = textBoxIP.Text;

            if (checkBoxLocalHostIP.Checked)
            {
                address = "127.0.0.1";
            }

            int port = Convert.ToInt32(textBoxPort.Text);

            if (Network.Connect(address, port))
            {
                labelStatus.Text = string.Format("{0}. 서버에 접속 중", DateTime.Now);
                btnConnect.Enabled = false;
                btnDisconnect.Enabled = true;

                DevLog.Write($"서버에 접속 중", LOG_LEVEL.INFO);
            }
            else
            {
                labelStatus.Text = string.Format("{0}. 서버에 접속 실패", DateTime.Now);
            }
        }

        private void btnDisconnect_Click(object sender, EventArgs e)
        {
            SetDisconnectd();
            Network.Close();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(textSendText.Text))
            {
                MessageBox.Show("보낼 텍스트를 입력하세요");
                return;
            }
                        
            List<byte> dataSource = new List<byte>();
            dataSource.AddRange(Encoding.UTF8.GetBytes(textSendText.Text));

            SendPacketQueue.Enqueue(dataSource.ToArray());
        }



        void NetworkReadProcess()
        {
            const Int16 PacketHeaderSize = PacketDef.PACKET_HEADER_SIZE;

            while (IsNetworkThreadRunning)
            {
                if (Network.IsConnected() == false)
                {
                    System.Threading.Thread.Sleep(1);
                    continue;
                }

                var recvData = Network.Receive();

                if (recvData != null)
                {
                    PacketBuffer.Write(recvData.Item2, 0, recvData.Item1);

                    while (true)
                    {
                        var data = PacketBuffer.Read();
                        if (data.Count < 1)
                        {
                            break;
                        }

                        var packet = new PacketData();
                        packet.DataSize = (short)(data.Count - PacketHeaderSize);
                        packet.PacketID = BitConverter.ToInt16(data.Array, data.Offset + 2);
                        packet.Type = (SByte)data.Array[(data.Offset + 4)];
                        packet.BodyData = new byte[packet.DataSize];
                        Buffer.BlockCopy(data.Array, (data.Offset + PacketHeaderSize), packet.BodyData, 0, (data.Count - PacketHeaderSize));
                        lock (((System.Collections.ICollection)RecvPacketQueue).SyncRoot)
                        {
                            RecvPacketQueue.Enqueue(packet);
                        }
                    }
                    //DevLog.Write($"받은 데이터: {recvData.Item2}", LOG_LEVEL.INFO);
                }
                else
                {
                    Network.Close();
                    SetDisconnectd();
                    DevLog.Write("서버와 접속 종료 !!!", LOG_LEVEL.INFO);
                }
            }
        }

        void NetworkSendProcess()
        {
            while (IsNetworkThreadRunning)
            {
                System.Threading.Thread.Sleep(1);

                if (Network.IsConnected() == false)
                {
                    continue;
                }

                lock (((System.Collections.ICollection)SendPacketQueue).SyncRoot)
                {
                    if (SendPacketQueue.Count > 0)
                    {
                        var packet = SendPacketQueue.Dequeue();
                        Network.Send(packet);
                    }
                }
            }
        }


        void BackGroundProcess(object sender, EventArgs e)
        {
            ProcessLog();

            try
            {
                var packet = new PacketData();

                lock (((System.Collections.ICollection)RecvPacketQueue).SyncRoot)
                {
                    if (RecvPacketQueue.Count() > 0)
                    {
                        packet = RecvPacketQueue.Dequeue();
                    }
                }

                if (packet.PacketID != 0)
                {
                    PacketProcess(packet);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(string.Format("ReadPacketQueueProcess. error:{0}", ex.Message));
            }
        }

        private void ProcessLog()
        {
            // 너무 이 작업만 할 수 없으므로 일정 작업 이상을 하면 일단 패스한다.
            int logWorkCount = 0;

            while (IsBackGroundProcessRunning)
            {
                System.Threading.Thread.Sleep(1);

                string msg;

                if (DevLog.GetLog(out msg))
                {
                    ++logWorkCount;

                    if (listBoxLog.Items.Count > 512)
                    {
                        listBoxLog.Items.Clear();
                    }

                    listBoxLog.Items.Add(msg);
                    listBoxLog.SelectedIndex = listBoxLog.Items.Count - 1;
                }
                else
                {
                    break;
                }

                if (logWorkCount > 8)
                {
                    break;
                }
            }
        }


        public void SetDisconnectd()
        {
            if (btnConnect.Enabled == false)
            {
                btnConnect.Invoke(new MethodInvoker(delegate { btnConnect.Enabled = true; }));
                btnDisconnect.Invoke(new MethodInvoker(delegate { btnDisconnect.Enabled = false; }));
            }
            SendPacketQueue.Clear();
            listBoxRoomChatMsg.Invoke(new MethodInvoker(delegate { listBoxRoomChatMsg.Items.Clear(); }));
            listBoxRoomUserList.Invoke(new MethodInvoker(delegate { listBoxRoomUserList.Items.Clear(); }));
            labelStatus.Invoke(new MethodInvoker(delegate { labelStatus.Text = "서버 접속이 끊어짐"; }));
            //labelStatus.Text = "서버 접속이 끊어짐";
        }

        public void PostSendPacket(PACKET_ID packetID, byte[] bodyData)
        {
            if (Network.IsConnected() == false)
            {
                DevLog.Write("서버 연결이 되어 있지 않습니다", LOG_LEVEL.ERROR);
                return;
            }

            Int16 bodyDataSize = 0;
            if (bodyData != null)
            {
                bodyDataSize = (Int16)bodyData.Length;
            }
            var packetSize = bodyDataSize + PacketDef.PACKET_HEADER_SIZE;

            List<byte> dataSource = new List<byte>();
            dataSource.AddRange(BitConverter.GetBytes((Int16)packetSize));
            dataSource.AddRange(BitConverter.GetBytes((Int16)packetID));
            dataSource.AddRange(new byte[] { (byte)0 });
            
            if (bodyData != null)
            {
                dataSource.AddRange(bodyData);
            }
           
            SendPacketQueue.Enqueue(dataSource.ToArray());
        }

        void AddRoomUserList(Int64 userUniqueId, string userID)
        {
            var msg = $"{userUniqueId}: {userID}";
            listBoxRoomUserList.Items.Add(msg);
        }

        void RemoveRoomUserList(Int64 userUniqueId)
        {
            object removeItem = null;

            foreach( var user in listBoxRoomUserList.Items)
            {
                var items = user.ToString().Split(":");
                if( items[0].ToInt64() == userUniqueId)
                {
                    removeItem = user;
                    return;
                }
            }

            if (removeItem != null)
            {
                listBoxRoomUserList.Items.Remove(removeItem);
            }
        }

        public void BoardClear()
        {
            board.ClearAll();
        }

        // 로그인 요청
        private void button2_Click(object sender, EventArgs e)
        {
            var loginReq = new LoginReqPacket();
            loginReq.SetValue(textBoxUserID.Text, "");
                    
            PostSendPacket(PACKET_ID.LOGIN_REQ, loginReq.ToBytes());
            //DevLog.Write($"로그인 요청:  {textBoxUserID.Text}, {textBoxUserPW.Text}");
            DevLog.Write($"로그인 요청:  {textBoxUserID.Text}");
        }

        private void btn_RoomEnter_Click(object sender, EventArgs e)
        {
            var requestPkt = new RoomEnterReqPacket();
            requestPkt.SetValue(textBoxRoomNumber.Text.ToInt32());

            PostSendPacket(PACKET_ID.ROOM_ENTER_REQ, requestPkt.ToBytes());
            DevLog.Write($"방 입장 요청:  {textBoxRoomNumber.Text} 번");
        }

        private void btn_RoomLeave_Click(object sender, EventArgs e)
        {
            PostSendPacket(PACKET_ID.ROOM_LEAVE_REQ,  null);
            DevLog.Write($"방 입장 요청:  {textBoxRoomNumber.Text} 번");
        }

        private void btnRoomChat_Click(object sender, EventArgs e)
        {
            if(textBoxRoomSendMsg.Text.IsEmpty())
            {
                MessageBox.Show("채팅 메시지를 입력하세요");
                return;
            }

            var requestPkt = new RoomChatReqPacket();
            requestPkt.SetValue(textBoxRoomSendMsg.Text);

            PostSendPacket(PACKET_ID.ROOM_CHAT_REQ, requestPkt.ToBytes());
            DevLog.Write($"방 채팅 요청");
        }

        private void groupBox5_Enter(object sender, EventArgs e)
        {

        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {
            DrawBoard();
            drawAllStones();
        }

        private void DrawBoard()
        {
            // panel1에 Graphics 객체 생성
            g = panel1.CreateGraphics();
            SolidBrush blueBrush = new SolidBrush(Color.Orange);

            // Create rectangle for region.
            Rectangle fillRect = new Rectangle(0, 0, 2 * margin + 18 * 눈Size, 2 * margin + 18 * 눈Size);

            // Create region for fill.
            Region fillRegion = new Region(fillRect);

            // Fill region to screen.
            g.FillRegion(blueBrush, fillRegion);
            // 세로선 19개
            for (int i = 0; i < 19; i++)
            {
                g.DrawLine(pen, new Point(margin + i * 눈Size, margin),
                  new Point(margin + i * 눈Size, margin + 18 * 눈Size));
            }

            // 가로선 19개
            for (int i = 0; i < 19; i++)
            {
                g.DrawLine(pen, new Point(margin, margin + i * 눈Size),
                  new Point(margin + 18 * 눈Size, margin + i * 눈Size));
            }

            // 화점그리기
            for (int x = 3; x <= 15; x += 6)
                for (int y = 3; y <= 15; y += 6)
                {
                    g.FillEllipse(bBrush,
                      margin + 눈Size * x - 화점Size / 2,
                      margin + 눈Size * y - 화점Size / 2,
                      화점Size, 화점Size);
                }
        }

        private void drawAllStones()
        {
            for (Int16 x = 0; x < 19; x++)
            {
                for (Int16 y = 0; y < 19; y++)
                {
                    if (board[x, y] == 0)
                    {
                        continue ;
                    }
                    Rectangle r = new Rectangle(
                      margin + 눈Size * x - 돌Size / 2,
                      margin + 눈Size * y - 돌Size / 2,
                      돌Size, 돌Size);
                    if (board[x, y] == 1)
                    {
                        g.FillEllipse(wBrush, r);
                    }
                    else if (board[x, y] == 2)
                    {
                        g.FillEllipse(bBrush, r);
                    }
                }
            }
        }

        private void placeStoneAt(Int32 x, Int32 y, bool color)
        {
            Rectangle r = new Rectangle(
              margin + 눈Size * x - 돌Size / 2,
              margin + 눈Size * y - 돌Size / 2,
              돌Size, 돌Size);
            if (color == true)
            {
                board[x, y] = 1;
            }
            else
            {
                board[x, y] = 2;
            }
            if (color == false)
            {
                g.FillEllipse(bBrush, r);
                //flag = true;
                //바둑판[x, y] = STONE.black;
            }
            else
            {
                g.FillEllipse(wBrush, r);
                //flag = false;
                //바둑판[x, y] = STONE.white;
            }
        }

        private void panel1_MouseDown(object sender, MouseEventArgs e)
        {
            // e.X는 픽셀단위, x는 바둑판 좌표
            Int32 x = (e.X - margin + 눈Size / 2) / 눈Size;
            Int32 y = (e.Y - margin + 눈Size / 2) / 눈Size;

            // 해당 좌표를 클릭한다 -> 서버에 그 좌표를 보낸다 -> 서버가 그 좌표에 찍어도 된다고 하면 찍는다.
            // 클라단에서 특정 위치가 되는지 안되는지에 대한 정보를 가지고 있을 필요는 없다.
            // 서버가 보낼 패킷의 정보에는 좌표, 색의 위치를 Ntf로 보낸다.
            // 그렇다면, panel1_MouseDown에서는 좌표 정보를 서버로 보내는 함수가 되고, 그리는 함수는 따로 패킷처리 하는 함수 만들어야 한다.
            //
            // 서버 쪽에 좌표를 보내는 코드.

            var requestPkt = new PlacingStoneReqPacket();
            requestPkt.SetValue(x, y);

            PostSendPacket(PACKET_ID.OMOK_PLACE_STONE_REQ, requestPkt.ToBytes());
            DevLog.Write($"돌 놓기 요청");
        }

        private void textBoxRelay_TextChanged(object sender, EventArgs e)
        {

        }

        private void checkBoxLocalHostIP_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void label3_Click(object sender, EventArgs e)
        {

        }

        private void textBoxRoomNumber_TextChanged(object sender, EventArgs e)
        {

        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void Room_Enter(object sender, EventArgs e)
        {

        }

        private void textBoxUserID_TextChanged(object sender, EventArgs e)
        {

        }

        private void btnReady_Click_1(object sender, EventArgs e)
        {
            var requestPkt = new ReadyRequestPacket();

            if (isReady == false)
            {
                requestPkt.isReady = true;
                PostSendPacket(PACKET_ID.OMOK_READY_REQ, requestPkt.ToBytes());
                DevLog.Write($"READY 요청");
            }
            else
            {
                requestPkt.isReady = false;
                PostSendPacket(PACKET_ID.OMOK_READY_REQ, requestPkt.ToBytes());
                DevLog.Write($"READY 해제 요청");
            }
        }
    }
}
