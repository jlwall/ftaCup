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
            this.pbTrack.Size = new System.Drawing.Size(1000, 600);
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
            this.txtGainServo.Text = "400";
            // 
            // txtGainServoDT
            // 
            this.txtGainServoDT.Location = new System.Drawing.Point(1089, 166);
            this.txtGainServoDT.Name = "txtGainServoDT";
            this.txtGainServoDT.Size = new System.Drawing.Size(100, 20);
            this.txtGainServoDT.TabIndex = 6;
            this.txtGainServoDT.Text = "4";
            // 
            // txtErrorTol
            // 
            this.txtErrorTol.Location = new System.Drawing.Point(1089, 218);
            this.txtErrorTol.Name = "txtErrorTol";
            this.txtErrorTol.Size = new System.Drawing.Size(100, 20);
            this.txtErrorTol.TabIndex = 7;
            this.txtErrorTol.Text = "1";
            // 
            // txtGainServoDGain
            // 
            this.txtGainServoDGain.Location = new System.Drawing.Point(1089, 192);
            this.txtGainServoDGain.Name = "txtGainServoDGain";
            this.txtGainServoDGain.Size = new System.Drawing.Size(100, 20);
            this.txtGainServoDGain.TabIndex = 8;
            this.txtGainServoDGain.Text = "5";
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
            this.spCar.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.spCar_DataReceived);
            // 
            // button4
            // 
            this.button4.Location = new System.Drawing.Point(1032, 590);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(105, 22);
            this.button4.TabIndex = 19;
            this.button4.Text = "SerialPrep";
            this.button4.UseVisualStyleBackColor = true;
            this.button4.Click += new System.EventHandler(this.button4_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1246, 769);
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
    }
}

