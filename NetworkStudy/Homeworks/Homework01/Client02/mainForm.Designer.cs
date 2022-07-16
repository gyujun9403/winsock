namespace csharp_test_client
{
    partial class mainForm
    {
        /// <summary>
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다. 
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마세요.
        /// </summary>
        private void InitializeComponent()
        {
            this.btnDisconnect = new System.Windows.Forms.Button();
            this.btnConnect = new System.Windows.Forms.Button();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.textBoxPort = new System.Windows.Forms.TextBox();
            this.label10 = new System.Windows.Forms.Label();
            this.checkBoxLocalHostIP = new System.Windows.Forms.CheckBox();
            this.textBoxIP = new System.Windows.Forms.TextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.button1 = new System.Windows.Forms.Button();
            this.labelStatus = new System.Windows.Forms.Label();
            this.listBoxLog = new System.Windows.Forms.ListBox();
            this.textBoxNum1 = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.textBoxOp1 = new System.Windows.Forms.TextBox();
            this.textBoxNum2 = new System.Windows.Forms.TextBox();
            this.textBoxNum3 = new System.Windows.Forms.TextBox();
            this.textBoxNum7 = new System.Windows.Forms.TextBox();
            this.textBoxNum5 = new System.Windows.Forms.TextBox();
            this.textBoxOp2 = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.textBoxNum4 = new System.Windows.Forms.TextBox();
            this.button3 = new System.Windows.Forms.Button();
            this.textBoxOp3 = new System.Windows.Forms.TextBox();
            this.textBoxNum6 = new System.Windows.Forms.TextBox();
            this.button4 = new System.Windows.Forms.Button();
            this.groupBox5.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnDisconnect
            // 
            this.btnDisconnect.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.btnDisconnect.Location = new System.Drawing.Point(421, 48);
            this.btnDisconnect.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.btnDisconnect.Name = "btnDisconnect";
            this.btnDisconnect.Size = new System.Drawing.Size(88, 32);
            this.btnDisconnect.TabIndex = 29;
            this.btnDisconnect.Text = "접속 끊기";
            this.btnDisconnect.UseVisualStyleBackColor = true;
            this.btnDisconnect.Click += new System.EventHandler(this.btnDisconnect_Click);
            // 
            // btnConnect
            // 
            this.btnConnect.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.btnConnect.Location = new System.Drawing.Point(420, 13);
            this.btnConnect.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.btnConnect.Name = "btnConnect";
            this.btnConnect.Size = new System.Drawing.Size(88, 32);
            this.btnConnect.TabIndex = 28;
            this.btnConnect.Text = "접속하기";
            this.btnConnect.UseVisualStyleBackColor = true;
            this.btnConnect.Click += new System.EventHandler(this.btnConnect_Click);
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.textBoxPort);
            this.groupBox5.Controls.Add(this.label10);
            this.groupBox5.Controls.Add(this.checkBoxLocalHostIP);
            this.groupBox5.Controls.Add(this.textBoxIP);
            this.groupBox5.Controls.Add(this.label9);
            this.groupBox5.Location = new System.Drawing.Point(12, 15);
            this.groupBox5.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Padding = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.groupBox5.Size = new System.Drawing.Size(403, 65);
            this.groupBox5.TabIndex = 27;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "Socket 더미 클라이언트 설정";
            // 
            // textBoxPort
            // 
            this.textBoxPort.Location = new System.Drawing.Point(225, 25);
            this.textBoxPort.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.textBoxPort.MaxLength = 6;
            this.textBoxPort.Name = "textBoxPort";
            this.textBoxPort.Size = new System.Drawing.Size(51, 23);
            this.textBoxPort.TabIndex = 18;
            this.textBoxPort.Text = "11021";
            this.textBoxPort.WordWrap = false;
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(163, 30);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(62, 15);
            this.label10.TabIndex = 17;
            this.label10.Text = "포트 번호:";
            // 
            // checkBoxLocalHostIP
            // 
            this.checkBoxLocalHostIP.AutoSize = true;
            this.checkBoxLocalHostIP.Checked = true;
            this.checkBoxLocalHostIP.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxLocalHostIP.Location = new System.Drawing.Point(285, 30);
            this.checkBoxLocalHostIP.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.checkBoxLocalHostIP.Name = "checkBoxLocalHostIP";
            this.checkBoxLocalHostIP.Size = new System.Drawing.Size(102, 19);
            this.checkBoxLocalHostIP.TabIndex = 15;
            this.checkBoxLocalHostIP.Text = "localhost 사용";
            this.checkBoxLocalHostIP.UseVisualStyleBackColor = true;
            // 
            // textBoxIP
            // 
            this.textBoxIP.Location = new System.Drawing.Point(68, 24);
            this.textBoxIP.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.textBoxIP.MaxLength = 6;
            this.textBoxIP.Name = "textBoxIP";
            this.textBoxIP.Size = new System.Drawing.Size(87, 23);
            this.textBoxIP.TabIndex = 11;
            this.textBoxIP.Text = "0.0.0.0";
            this.textBoxIP.WordWrap = false;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(6, 29);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(62, 15);
            this.label9.TabIndex = 10;
            this.label9.Text = "서버 주소:";
            // 
            // button1
            // 
            this.button1.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.button1.Location = new System.Drawing.Point(295, 83);
            this.button1.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(56, 32);
            this.button1.TabIndex = 39;
            this.button1.Text = "요청 1";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // labelStatus
            // 
            this.labelStatus.AutoSize = true;
            this.labelStatus.Location = new System.Drawing.Point(12, 348);
            this.labelStatus.Name = "labelStatus";
            this.labelStatus.Size = new System.Drawing.Size(112, 15);
            this.labelStatus.TabIndex = 40;
            this.labelStatus.Text = "서버 접속 상태: ???";
            // 
            // listBoxLog
            // 
            this.listBoxLog.FormattingEnabled = true;
            this.listBoxLog.HorizontalScrollbar = true;
            this.listBoxLog.ItemHeight = 15;
            this.listBoxLog.Location = new System.Drawing.Point(12, 162);
            this.listBoxLog.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.listBoxLog.Name = "listBoxLog";
            this.listBoxLog.Size = new System.Drawing.Size(490, 169);
            this.listBoxLog.TabIndex = 41;
            // 
            // textBoxNum1
            // 
            this.textBoxNum1.Location = new System.Drawing.Point(12, 88);
            this.textBoxNum1.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.textBoxNum1.MaxLength = 6;
            this.textBoxNum1.Name = "textBoxNum1";
            this.textBoxNum1.Size = new System.Drawing.Size(51, 23);
            this.textBoxNum1.TabIndex = 44;
            this.textBoxNum1.Text = "0";
            this.textBoxNum1.WordWrap = false;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(217, 92);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(15, 15);
            this.label3.TabIndex = 43;
            this.label3.Text = "=";
            // 
            // textBoxOp1
            // 
            this.textBoxOp1.Location = new System.Drawing.Point(66, 88);
            this.textBoxOp1.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.textBoxOp1.MaxLength = 6;
            this.textBoxOp1.Name = "textBoxOp1";
            this.textBoxOp1.Size = new System.Drawing.Size(18, 23);
            this.textBoxOp1.TabIndex = 49;
            this.textBoxOp1.Text = "+";
            this.textBoxOp1.WordWrap = false;
            // 
            // textBoxNum2
            // 
            this.textBoxNum2.Location = new System.Drawing.Point(87, 88);
            this.textBoxNum2.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.textBoxNum2.MaxLength = 6;
            this.textBoxNum2.Name = "textBoxNum2";
            this.textBoxNum2.Size = new System.Drawing.Size(51, 23);
            this.textBoxNum2.TabIndex = 50;
            this.textBoxNum2.Text = "0";
            this.textBoxNum2.WordWrap = false;
            // 
            // textBoxNum3
            // 
            this.textBoxNum3.Location = new System.Drawing.Point(238, 88);
            this.textBoxNum3.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.textBoxNum3.MaxLength = 6;
            this.textBoxNum3.Name = "textBoxNum3";
            this.textBoxNum3.Size = new System.Drawing.Size(51, 23);
            this.textBoxNum3.TabIndex = 51;
            this.textBoxNum3.Text = "0";
            this.textBoxNum3.WordWrap = false;
            // 
            // textBoxNum7
            // 
            this.textBoxNum7.Location = new System.Drawing.Point(239, 127);
            this.textBoxNum7.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.textBoxNum7.MaxLength = 6;
            this.textBoxNum7.Name = "textBoxNum7";
            this.textBoxNum7.Size = new System.Drawing.Size(51, 23);
            this.textBoxNum7.TabIndex = 57;
            this.textBoxNum7.Text = "0";
            this.textBoxNum7.WordWrap = false;
            // 
            // textBoxNum5
            // 
            this.textBoxNum5.Location = new System.Drawing.Point(87, 128);
            this.textBoxNum5.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.textBoxNum5.MaxLength = 6;
            this.textBoxNum5.Name = "textBoxNum5";
            this.textBoxNum5.Size = new System.Drawing.Size(51, 23);
            this.textBoxNum5.TabIndex = 56;
            this.textBoxNum5.Text = "0";
            this.textBoxNum5.WordWrap = false;
            // 
            // textBoxOp2
            // 
            this.textBoxOp2.Location = new System.Drawing.Point(66, 128);
            this.textBoxOp2.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.textBoxOp2.MaxLength = 6;
            this.textBoxOp2.Name = "textBoxOp2";
            this.textBoxOp2.Size = new System.Drawing.Size(18, 23);
            this.textBoxOp2.TabIndex = 55;
            this.textBoxOp2.Text = "+";
            this.textBoxOp2.WordWrap = false;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(218, 131);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(15, 15);
            this.label1.TabIndex = 53;
            this.label1.Text = "=";
            // 
            // textBoxNum4
            // 
            this.textBoxNum4.Location = new System.Drawing.Point(12, 128);
            this.textBoxNum4.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.textBoxNum4.MaxLength = 6;
            this.textBoxNum4.Name = "textBoxNum4";
            this.textBoxNum4.Size = new System.Drawing.Size(51, 23);
            this.textBoxNum4.TabIndex = 54;
            this.textBoxNum4.Text = "0";
            this.textBoxNum4.WordWrap = false;
            // 
            // button3
            // 
            this.button3.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.button3.Location = new System.Drawing.Point(296, 122);
            this.button3.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(56, 32);
            this.button3.TabIndex = 52;
            this.button3.Text = "요청 2";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click_1);
            // 
            // textBoxOp3
            // 
            this.textBoxOp3.Location = new System.Drawing.Point(141, 128);
            this.textBoxOp3.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.textBoxOp3.MaxLength = 6;
            this.textBoxOp3.Name = "textBoxOp3";
            this.textBoxOp3.Size = new System.Drawing.Size(18, 23);
            this.textBoxOp3.TabIndex = 58;
            this.textBoxOp3.Text = "+";
            this.textBoxOp3.WordWrap = false;
            // 
            // textBoxNum6
            // 
            this.textBoxNum6.Location = new System.Drawing.Point(161, 128);
            this.textBoxNum6.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.textBoxNum6.MaxLength = 6;
            this.textBoxNum6.Name = "textBoxNum6";
            this.textBoxNum6.Size = new System.Drawing.Size(51, 23);
            this.textBoxNum6.TabIndex = 59;
            this.textBoxNum6.Text = "0";
            this.textBoxNum6.WordWrap = false;
            // 
            // button4
            // 
            this.button4.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.button4.Location = new System.Drawing.Point(359, 121);
            this.button4.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(86, 32);
            this.button4.TabIndex = 60;
            this.button4.Text = "요청 1, 2";
            this.button4.UseVisualStyleBackColor = true;
            this.button4.Click += new System.EventHandler(this.button4_Click_1);
            // 
            // mainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(524, 366);
            this.Controls.Add(this.button4);
            this.Controls.Add(this.textBoxNum6);
            this.Controls.Add(this.textBoxOp3);
            this.Controls.Add(this.textBoxNum7);
            this.Controls.Add(this.textBoxNum5);
            this.Controls.Add(this.textBoxOp2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.textBoxNum4);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.textBoxNum3);
            this.Controls.Add(this.textBoxNum2);
            this.Controls.Add(this.textBoxOp1);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.textBoxNum1);
            this.Controls.Add(this.labelStatus);
            this.Controls.Add(this.listBoxLog);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.btnDisconnect);
            this.Controls.Add(this.btnConnect);
            this.Controls.Add(this.groupBox5);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.Name = "mainForm";
            this.Text = "네트워크 테스트 클라이언트";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.mainForm_FormClosing);
            this.Load += new System.EventHandler(this.mainForm_Load);
            this.groupBox5.ResumeLayout(false);
            this.groupBox5.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnDisconnect;
        private System.Windows.Forms.Button btnConnect;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.TextBox textBoxPort;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.CheckBox checkBoxLocalHostIP;
        private System.Windows.Forms.TextBox textBoxIP;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Label labelStatus;
        private System.Windows.Forms.ListBox listBoxLog;
        private System.Windows.Forms.TextBox textBoxNum1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox textBoxOp1;
        private System.Windows.Forms.TextBox textBoxNum2;
        private System.Windows.Forms.TextBox textBoxNum3;
        private System.Windows.Forms.TextBox textBoxNum7;
        private System.Windows.Forms.TextBox textBoxNum5;
        private System.Windows.Forms.TextBox textBoxOp2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox textBoxNum4;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.TextBox textBoxOp3;
        private System.Windows.Forms.TextBox textBoxNum6;
        private System.Windows.Forms.Button button4;
    }
}

