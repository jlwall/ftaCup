namespace fsSim
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.button1 = new System.Windows.Forms.Button();
            this.pbTrack = new System.Windows.Forms.PictureBox();
            this.button2 = new System.Windows.Forms.Button();
            this.button3 = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.txtGainServo = new System.Windows.Forms.TextBox();
            this.txtGainServoDT = new System.Windows.Forms.TextBox();
            this.txtErrorTol = new System.Windows.Forms.TextBox();
            this.txtGainServoDGain = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.txtMaxAcc = new System.Windows.Forms.TextBox();
            this.txtLostV = new System.Windows.Forms.TextBox();
            this.txtMavV = new System.Windows.Forms.TextBox();
            this.spCar = new System.IO.Ports.SerialPort(this.components);
            this.button4 = new System.Windows.Forms.Button();
            this.colorDialog1 = new System.Windows.Forms.ColorDialog();
            this.txtSleftOn = new System.Windows.Forms.TextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.txtSrightOn = new System.Windows.Forms.TextBox();
            this.label10 = new System.Windows.Forms.Label();
            this.txtSWidth = new System.Windows.Forms.TextBox();
            this.label11 = new System.Windows.Forms.Label();
            this.txtSCenter = new System.Windows.Forms.TextBox();
            this.label12 = new System.Windows.Forms.Label();
            this.timerUpdate = new System.Windows.Forms.Timer(this.components);
            this.txtMode = new System.Windows.Forms.TextBox();
            this.label13 = new System.Windows.Forms.Label();
            this.button5 = new System.Windows.Forms.Button();
            this.button6 = new System.Windows.Forms.Button();
            this.button7 = new System.Windows.Forms.Button();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.label14 = new System.Windows.Forms.Label();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.label15 = new System.Windows.Forms.Label();
            this.txtCtargetSer = new System.Windows.Forms.TextBox();
            this.label16 = new System.Windows.Forms.Label();
            this.txtCtargetVel = new System.Windows.Forms.TextBox();
            this.label17 = new System.Windows.Forms.Label();
            this.button8 = new System.Windows.Forms.Button();
            this.button9 = new System.Windows.Forms.Button();
            this.button10 = new System.Windows.Forms.Button();
            this.button11 = new System.Windows.Forms.Button();
            this.button12 = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.pbTrack)).BeginInit();
            this.SuspendLayout();
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(1018, 12);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 0;
            this.button1.Text = "SetupTrack";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // pbTrack
            // 
            this.pbTrack.Location = new System.Drawing.Point(12, 12);
            this.pbTrack.Name = "pbTrack";
            this.pbTrack.Size = new System.Drawing.Size(1000, 900);
            this.pbTrack.TabIndex = 1;
            this.pbTrack.TabStop = false;
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(1018, 41);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(105, 22);
            this.button2.TabIndex = 2;
            this.button2.Text = "Render";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(1018, 69);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(75, 23);
            this.button3.TabIndex = 3;
            this.button3.Text = "TimeStep";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(1018, 119);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(54, 13);
            this.label1.TabIndex = 4;
            this.label1.Text = "Lap Time:";
            // 
            // txtGainServo
            // 
            this.txtGainServo.Location = new System.Drawing.Point(1089, 140);
            this.txtGainServo.Name = "txtGainServo";
            this.txtGainServo.Size = new System.Drawing.Size(100, 20);
            this.txtGainServo.TabIndex = 5;
            this.txtGainServo.Text = "8";
            this.txtGainServo.TextChanged += new System.EventHandler(this.txtGainServo_TextChanged);
            // 
            // txtGainServoDT
            // 
            this.txtGainServoDT.Location = new System.Drawing.Point(1089, 166);
            this.txtGainServoDT.Name = "txtGainServoDT";
            this.txtGainServoDT.Size = new System.Drawing.Size(100, 20);
            this.txtGainServoDT.TabIndex = 6;
            this.txtGainServoDT.Text = ".2";
            // 
            // txtErrorTol
            // 
            this.txtErrorTol.Location = new System.Drawing.Point(1089, 218);
            this.txtErrorTol.Name = "txtErrorTol";
            this.txtErrorTol.Size = new System.Drawing.Size(100, 20);
            this.txtErrorTol.TabIndex = 7;
            this.txtErrorTol.Text = "5";
            // 
            // txtGainServoDGain
            // 
            this.txtGainServoDGain.Location = new System.Drawing.Point(1089, 192);
            this.txtGainServoDGain.Name = "txtGainServoDGain";
            this.txtGainServoDGain.Size = new System.Drawing.Size(100, 20);
            this.txtGainServoDGain.TabIndex = 8;
            this.txtGainServoDGain.Text = "4";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(1029, 143);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(39, 13);
            this.label2.TabIndex = 9;
            this.label2.Text = "P Gain";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(1029, 169);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(35, 13);
            this.label3.TabIndex = 10;
            this.label3.Text = "I Gain";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(1029, 195);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(40, 13);
            this.label4.TabIndex = 11;
            this.label4.Text = "D Gain";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(1029, 221);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(52, 13);
            this.label5.TabIndex = 12;
            this.label5.Text = "Max Error";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(1031, 247);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(37, 13);
            this.label6.TabIndex = 13;
            this.label6.Text = "Max V";
            this.label6.Click += new System.EventHandler(this.label6_Click);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(1032, 273);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(37, 13);
            this.label7.TabIndex = 14;
            this.label7.Text = "Lost V";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(1032, 299);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(49, 13);
            this.label8.TabIndex = 15;
            this.label8.Text = "Max Acc";
            // 
            // txtMaxAcc
            // 
            this.txtMaxAcc.Location = new System.Drawing.Point(1089, 296);
            this.txtMaxAcc.Name = "txtMaxAcc";
            this.txtMaxAcc.Size = new System.Drawing.Size(100, 20);
            this.txtMaxAcc.TabIndex = 16;
            this.txtMaxAcc.Text = ".5";
            // 
            // txtLostV
            // 
            this.txtLostV.Location = new System.Drawing.Point(1089, 270);
            this.txtLostV.Name = "txtLostV";
            this.txtLostV.Size = new System.Drawing.Size(100, 20);
            this.txtLostV.TabIndex = 17;
            this.txtLostV.Text = "600";
            // 
            // txtMavV
            // 
            this.txtMavV.Location = new System.Drawing.Point(1089, 240);
            this.txtMavV.Name = "txtMavV";
            this.txtMavV.Size = new System.Drawing.Size(100, 20);
            this.txtMavV.TabIndex = 18;
            this.txtMavV.Text = "1650";
            // 
            // spCar
            // 
            this.spCar.BaudRate = 115200;
            this.spCar.PortName = "COM8";
            this.spCar.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.spCar_DataReceived);
            // 
            // button4
            // 
            this.button4.Location = new System.Drawing.Point(1036, 442);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(105, 22);
            this.button4.TabIndex = 19;
            this.button4.Text = "SerialPrep";
            this.button4.UseVisualStyleBackColor = true;
            this.button4.Click += new System.EventHandler(this.button4_Click);
            // 
            // txtSleftOn
            // 
            this.txtSleftOn.Location = new System.Drawing.Point(1089, 626);
            this.txtSleftOn.Name = "txtSleftOn";
            this.txtSleftOn.Size = new System.Drawing.Size(100, 20);
            this.txtSleftOn.TabIndex = 21;
            this.txtSleftOn.Text = ".5";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(1032, 629);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(52, 13);
            this.label9.TabIndex = 20;
            this.label9.Text = "S Left On";
            // 
            // txtSrightOn
            // 
            this.txtSrightOn.Location = new System.Drawing.Point(1089, 652);
            this.txtSrightOn.Name = "txtSrightOn";
            this.txtSrightOn.Size = new System.Drawing.Size(100, 20);
            this.txtSrightOn.TabIndex = 23;
            this.txtSrightOn.Text = ".5";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(1032, 655);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(59, 13);
            this.label10.TabIndex = 22;
            this.label10.Text = "S Right On";
            // 
            // txtSWidth
            // 
            this.txtSWidth.Location = new System.Drawing.Point(1089, 678);
            this.txtSWidth.Name = "txtSWidth";
            this.txtSWidth.Size = new System.Drawing.Size(100, 20);
            this.txtSWidth.TabIndex = 25;
            this.txtSWidth.Text = ".5";
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(1032, 681);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(45, 13);
            this.label11.TabIndex = 24;
            this.label11.Text = "S Width";
            // 
            // txtSCenter
            // 
            this.txtSCenter.Location = new System.Drawing.Point(1089, 704);
            this.txtSCenter.Name = "txtSCenter";
            this.txtSCenter.Size = new System.Drawing.Size(100, 20);
            this.txtSCenter.TabIndex = 27;
            this.txtSCenter.Text = ".5";
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(1032, 707);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(48, 13);
            this.label12.TabIndex = 26;
            this.label12.Text = "S Center";
            // 
            // timerUpdate
            // 
            this.timerUpdate.Tick += new System.EventHandler(this.timerUpdate_Tick);
            // 
            // txtMode
            // 
            this.txtMode.Location = new System.Drawing.Point(1103, 730);
            this.txtMode.Name = "txtMode";
            this.txtMode.Size = new System.Drawing.Size(54, 20);
            this.txtMode.TabIndex = 29;
            this.txtMode.Text = "0";
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(1033, 733);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(51, 13);
            this.label13.TabIndex = 28;
            this.label13.Text = "CT Mode";
            // 
            // button5
            // 
            this.button5.Location = new System.Drawing.Point(1163, 730);
            this.button5.Name = "button5";
            this.button5.Size = new System.Drawing.Size(105, 22);
            this.button5.TabIndex = 30;
            this.button5.Text = "Mode Off";
            this.button5.UseVisualStyleBackColor = true;
            this.button5.Click += new System.EventHandler(this.button5_Click);
            // 
            // button6
            // 
            this.button6.Location = new System.Drawing.Point(1163, 756);
            this.button6.Name = "button6";
            this.button6.Size = new System.Drawing.Size(105, 22);
            this.button6.TabIndex = 31;
            this.button6.Text = "Mode Manual";
            this.button6.UseVisualStyleBackColor = true;
            this.button6.Click += new System.EventHandler(this.button6_Click);
            // 
            // button7
            // 
            this.button7.Location = new System.Drawing.Point(1163, 782);
            this.button7.Name = "button7";
            this.button7.Size = new System.Drawing.Size(105, 22);
            this.button7.TabIndex = 32;
            this.button7.Text = "Mode Auto";
            this.button7.UseVisualStyleBackColor = true;
            this.button7.Click += new System.EventHandler(this.button7_Click);
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(1103, 548);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(100, 20);
            this.textBox1.TabIndex = 40;
            this.textBox1.Text = ".5";
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(1046, 551);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(48, 13);
            this.label14.TabIndex = 39;
            this.label14.Text = "S Center";
            // 
            // textBox2
            // 
            this.textBox2.Location = new System.Drawing.Point(1103, 522);
            this.textBox2.Name = "textBox2";
            this.textBox2.Size = new System.Drawing.Size(100, 20);
            this.textBox2.TabIndex = 38;
            this.textBox2.Text = ".5";
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(1046, 525);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(45, 13);
            this.label15.TabIndex = 37;
            this.label15.Text = "S Width";
            // 
            // txtCtargetSer
            // 
            this.txtCtargetSer.Location = new System.Drawing.Point(1121, 496);
            this.txtCtargetSer.Name = "txtCtargetSer";
            this.txtCtargetSer.Size = new System.Drawing.Size(100, 20);
            this.txtCtargetSer.TabIndex = 36;
            this.txtCtargetSer.Text = ".5";
            // 
            // label16
            // 
            this.label16.AutoSize = true;
            this.label16.Location = new System.Drawing.Point(1046, 499);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(69, 13);
            this.label16.TabIndex = 35;
            this.label16.Text = "Target Servo";
            // 
            // txtCtargetVel
            // 
            this.txtCtargetVel.Location = new System.Drawing.Point(1121, 470);
            this.txtCtargetVel.Name = "txtCtargetVel";
            this.txtCtargetVel.Size = new System.Drawing.Size(100, 20);
            this.txtCtargetVel.TabIndex = 34;
            this.txtCtargetVel.Text = ".5";
            // 
            // label17
            // 
            this.label17.AutoSize = true;
            this.label17.Location = new System.Drawing.Point(1046, 473);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(56, 13);
            this.label17.TabIndex = 33;
            this.label17.Text = "Target Vel";
            // 
            // button8
            // 
            this.button8.Location = new System.Drawing.Point(1180, 836);
            this.button8.Name = "button8";
            this.button8.Size = new System.Drawing.Size(41, 37);
            this.button8.TabIndex = 41;
            this.button8.Text = "v+";
            this.button8.UseVisualStyleBackColor = true;
            this.button8.Click += new System.EventHandler(this.button8_Click);
            // 
            // button9
            // 
            this.button9.Location = new System.Drawing.Point(1180, 876);
            this.button9.Name = "button9";
            this.button9.Size = new System.Drawing.Size(41, 37);
            this.button9.TabIndex = 42;
            this.button9.Text = "v-";
            this.button9.UseVisualStyleBackColor = true;
            this.button9.Click += new System.EventHandler(this.button9_Click);
            // 
            // button10
            // 
            this.button10.Location = new System.Drawing.Point(1133, 857);
            this.button10.Name = "button10";
            this.button10.Size = new System.Drawing.Size(41, 37);
            this.button10.TabIndex = 43;
            this.button10.Text = "s-";
            this.button10.UseVisualStyleBackColor = true;
            this.button10.Click += new System.EventHandler(this.button10_Click);
            // 
            // button11
            // 
            this.button11.Location = new System.Drawing.Point(1227, 857);
            this.button11.Name = "button11";
            this.button11.Size = new System.Drawing.Size(41, 37);
            this.button11.TabIndex = 44;
            this.button11.Text = "s+";
            this.button11.UseVisualStyleBackColor = true;
            this.button11.Click += new System.EventHandler(this.button11_Click);
            // 
            // button12
            // 
            this.button12.Location = new System.Drawing.Point(1133, 810);
            this.button12.Name = "button12";
            this.button12.Size = new System.Drawing.Size(135, 25);
            this.button12.TabIndex = 45;
            this.button12.Text = "Exit";
            this.button12.UseVisualStyleBackColor = true;
            this.button12.Click += new System.EventHandler(this.button12_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1280, 924);
            this.Controls.Add(this.button12);
            this.Controls.Add(this.button11);
            this.Controls.Add(this.button10);
            this.Controls.Add(this.button9);
            this.Controls.Add(this.button8);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.label14);
            this.Controls.Add(this.textBox2);
            this.Controls.Add(this.label15);
            this.Controls.Add(this.txtCtargetSer);
            this.Controls.Add(this.label16);
            this.Controls.Add(this.txtCtargetVel);
            this.Controls.Add(this.label17);
            this.Controls.Add(this.button7);
            this.Controls.Add(this.button6);
            this.Controls.Add(this.button5);
            this.Controls.Add(this.txtMode);
            this.Controls.Add(this.label13);
            this.Controls.Add(this.txtSCenter);
            this.Controls.Add(this.label12);
            this.Controls.Add(this.txtSWidth);
            this.Controls.Add(this.label11);
            this.Controls.Add(this.txtSrightOn);
            this.Controls.Add(this.label10);
            this.Controls.Add(this.txtSleftOn);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.button4);
            this.Controls.Add(this.txtMavV);
            this.Controls.Add(this.txtLostV);
            this.Controls.Add(this.txtMaxAcc);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.txtGainServoDGain);
            this.Controls.Add(this.txtErrorTol);
            this.Controls.Add(this.txtGainServoDT);
            this.Controls.Add(this.txtGainServo);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.pbTrack);
            this.Controls.Add(this.button1);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.pbTrack)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.PictureBox pbTrack;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox txtGainServo;
        private System.Windows.Forms.TextBox txtGainServoDT;
        private System.Windows.Forms.TextBox txtErrorTol;
        private System.Windows.Forms.TextBox txtGainServoDGain;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.TextBox txtMaxAcc;
        private System.Windows.Forms.TextBox txtLostV;
        private System.Windows.Forms.TextBox txtMavV;
        private System.IO.Ports.SerialPort spCar;
        private System.Windows.Forms.Button button4;
        private System.Windows.Forms.ColorDialog colorDialog1;
        private System.Windows.Forms.TextBox txtSleftOn;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.TextBox txtSrightOn;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.TextBox txtSWidth;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.TextBox txtSCenter;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Timer timerUpdate;
        private System.Windows.Forms.TextBox txtMode;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.Button button5;
        private System.Windows.Forms.Button button6;
        private System.Windows.Forms.Button button7;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.TextBox textBox2;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.TextBox txtCtargetSer;
        private System.Windows.Forms.Label label16;
        private System.Windows.Forms.TextBox txtCtargetVel;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.Button button8;
        private System.Windows.Forms.Button button9;
        private System.Windows.Forms.Button button10;
        private System.Windows.Forms.Button button11;
        private System.Windows.Forms.Button button12;
    }
}

