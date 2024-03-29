﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace fsSim
{
    public partial class Form1 : Form
    {

        class carControl
        {
         
            public Int16 errorRate;
            public float iTerm;
            public float dterm;          

            public float radiusTarget;
            public Int16 servoPWMTarget;
            public Int16 servoDegTarget;
            public UInt16 velTarget;
            public UInt16 velBiasTarget;

            public byte centerTarget;

            public byte center;
            public Int16 error;
            public byte c1, c2, c3, c4;

            public bool inCorner;
            public bool turnLeft;
            public bool turnRight;

            public byte ctrlMode;
            public UInt16 straightLearn;
        }

        class carSensor
        {
            public byte[] sensorV;
            public float width;
            public float posX;
            public int leftOn;
            public int rightOn;
            public int senseWide;
            public byte valid;
        }

        class calibrationS
        {
            public float servGain;
            public float servGainIgain;
            public float servGainDTerm;
            public float setupTrack;
            public float setupWheelBase;
            public float servoArm;
            public float steeringNuckle;
            public float maxAccel;

            public Int16 errorTol;
            public UInt16 maxSpeed;
            public UInt16 lostSpeed;
          

        
        }

        class carStruct
        {
            public double x; //pos x in mm
            public double y; // pos y in mm
            public double dir; //radian Direction
            
            public float turnRad;
            public float vel; //mm/s

            public float velRLratio;
            public float accel;

            public float servoDeg;

            
            public carSensor lightSense;
            public carControl ctrl;
            public calibrationS cal;
        }

        carStruct car = new carStruct();


        public Form1()
        {
            InitializeComponent();

            car.x = 200;
            car.y = 300;
            car.dir = Math.PI / 2 - 0.1;
            car.servoDeg = 0;
            car.turnRad = 500000;
            car.vel = 400;//200mm/s
            car.lightSense = new carSensor();
            car.lightSense.sensorV = new byte[128];

            car.lightSense.posX = 220;
            car.lightSense.width = 70;

            car.ctrl = new carControl();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            setupTrack();

            renderTrack();

        }

        Boolean[,] trackGrid = new Boolean[10000, 9000];

        public void setupTrack()
        {
     
            for(int i=0;i<trackGrid.GetLength(0);i++)
                for(int j=0;j<trackGrid.GetLength(1);j++)
                    trackGrid[i,j]=false;

            //Base Track
            /*
            trackInsertLine(200, 1000, true, false, false, 2000);
            trackInsertLine(50, 2000, false, true, false, 300); //sf Line
            trackInsetCurve(700, 3000, 500, 25, -Math.PI*12/8, Math.PI/2 );
            trackInsertLine(700, 3500, false, true, false, 1000);
            trackInsetCurve(1700, 3000, 500, 25, -Math.PI*2 , Math.PI /2);
            trackInsertLine(2200, 3000, true, false, true, 1250);
            trackInsetCurve(2700, 1750, 500, 25, Math.PI, Math.PI / 2);
            trackInsertLine(2700, 1250, false, true, false, 3750);
            trackInsetCurve(6450, 850, 400, 25, -Math.PI*5/2, Math.PI );            
            trackInsertLine(6450, 450, false, true, true, 3000);
            trackInsetCurve(3450, 3450, 3000, 25, -Math.PI * 14 / 24, Math.PI/12);
            trackInsetCurve(2280, -2415, 3000, 25, Math.PI * 11 / 24, Math.PI / 12);
            trackInsertLine(1900, 550, false, true, true, 1250);
            trackInsetCurve(700, 1050, 500, 25, Math.PI, Math.PI / 2);*
             */

            trackInsertLine(500, 1000, true, false, false, 1064); //SF

            //sf line

            trackInsertLine(400, 1500, false, true, false, 50); //SF
            trackInsertLine(600, 1500, false, true, true, 50); //SF

            trackInsetCurve(500 + 532, 500 + 532, 532, 26, Math.PI, Math.PI / 2); // Turn 1
            trackInsertLine(1000, 500, false, true, false, 1064*6); // Straight 1;
            trackInsetCurve(1000 + 1064 * 6, 500 + 532, 532, 26, -Math.PI / 2, Math.PI / 2); // Turn 2
            trackInsertLine(1000 + 1064 * 6 + 532, 1000, true, false, false, 1064); //straight 2
            //hill 1
            trackInsertLine(1000 + 1064 * 6 + 532, 1000 + 1064 , true, false, false, 1064*2); //straight 2
            //hill2
            trackInsertLine(1000 + 1064 * 6 + 532, 1000 + 1064 * 3, true, false, false, 1064*2); //straight 2
            trackInsetCurve(1000 + 1064 * 6, 1000 + 1064 * 5, 532, 26, 0 , Math.PI); // HP 3/4
            //swivels, straight 3
            trackInsertLine(1000 + 1064 * 6 - 532, 1000 + 1064 * 5, true, false, true, 1064*4); //straight 3

            trackInsetCurve(1000 + 1064 * 6 - 532*2, 1000 + 1064, 532, 26, -Math.PI/2, Math.PI/2); // Turn 5

            trackInsertLine(1000 + 1064 * 6 - 532*2, 1000 + 1064-532 , false, true, true, 1064 * 3); //straight 4

            trackInsetCurve(1000 + 1064 * 3 - 532 * 2, 1000 + 1064, 532, 26, -Math.PI , Math.PI / 2); // Turn 6

            
            trackInsertLine(1000 + 1064 * 3 - 532 * 3, 1000 + 1064, true, false, false, 1064); //straight 5

            trackInsetCurve(1000 + 1064 * 3 - 532 * 2, 1000 + 1064*2, 532, 26, Math.PI/2, Math.PI / 2); // Turn 7

            //cross 1
            trackInsertLine(1000 + 1064 * 3 - 532 * 2, 1000 + 1064*2 + 532, false, true, false, 1064); //straight 5

            //loop 1
            trackInsetCurve(1000 + 1064 * 4 - 532 * 2, 1000 + 1064 * 2, 532, 26, Math.PI, 3*Math.PI / 2); // Turn 7

            //cross 2
            trackInsertLine(1000 + 1064 * 4 - 532 * 3, 1000 + 1064 * 2, true, false, false, 1064 * 3); //straight 5

            //loop 2
            trackInsetCurve(1000 + 1064 * 4 - 532 * 2, 1000 + 1064 * 5, 532, 26, -Math.PI/2, 3 * Math.PI / 2); // Turn 7

            //cross 2
            trackInsertLine(1000 + 1064 * 4 - 532 * 2, 1000 + 1064 * 5 - 532, false, true, true, 1064*2); //straight 5

            trackInsetCurve(1032 + 1064 * 2 - 532 * 2, 1000 + 1064 * 5, 532, 26, Math.PI, Math.PI / 2); // Turn 5

            trackInsetCurve(1032 + 1064 * 2 - 532 * 4 , 1000 + 1064 * 5 , 532, 26, 0, Math.PI); // Turn 5

            //cross 2
            trackInsertLine(500 , 1000 + 1064 * 5 , true, false, true, 1064 * 4); //straight 5

         
        }

        class carPos
        {
            public double x;
            public double y;
            public double speed;
        }

        List<carPos> cpHist = new List<carPos>();

        public void trackInsertLine(int x, int y, bool vecX, bool vecY, bool rev, int len)
        {
            if (rev == false)
            {
                for (int i = 0; i < len; i++)
                {
                    trackGrid[x, y] = true;
                    if (vecX)
                    {
                        for (int j = -12; j <= 12; j++)
                        {
                            if(((x + j)>=0) & ((x + j)<trackGrid.GetLength(0)))
                                if (((y + i) >= 0) & ((y + i) < trackGrid.GetLength(1)))
                                    trackGrid[x + j, y + i] = true;
                        }
                    }
                    else if (vecY)
                    {
                        for (int j = -12; j <= 12; j++)
                        {
                            if (((x + i) >= 0) & ((x + i) < trackGrid.GetLength(0)))
                                if (((y + j) >= 0) & ((y + j) < trackGrid.GetLength(1)))
                                    trackGrid[x + i, y + j] = true;
                        }
                    }
                }
            }
            else
            {
                for (int i = 0; i > -len; i--)
                {
                    trackGrid[x, y] = true;
                    if (vecX)
                    {
                        for (int j = -12; j <= 12; j++)
                        {
                            if (((x + j) >= 0) & ((x + j) < trackGrid.GetLength(0)))
                                if (((y + i) >= 0) & ((y + i) < trackGrid.GetLength(1)))
                            trackGrid[x + j, y + i] = true;
                        }
                    }
                    else if (vecY)
                    {
                        for (int j = -12; j <= 12; j++)
                        {
                            if (((x + i) >= 0) & ((x + i) < trackGrid.GetLength(0)))
                                if (((y + j) >= 0) & ((y + j) < trackGrid.GetLength(1)))
                            trackGrid[x + i, y + j] = true;
                        }
                    }
                }
            }

        }
        public void trackInsetCurve(int x, int y, int rad, int width, double radS, double radE)
        {

            for (double ww = rad - width / 2; ww < rad + width / 2; ww += 0.05)
            {

                for (double r = radS; r < radS + radE; r += 0.0005)
                {

                    double px = x + (double)ww * Math.Cos(r);
                    double py = y + (double)ww * Math.Sin(r);

                    if(py<0)py=0;
                    if(px<0)px=0;

                    trackGrid[(int)px, (int)py] = true;

                }
            }

        }

        private void renderTrack()
        {
            Bitmap bm = new Bitmap(1000, 900);

            for (int i = 0; i < bm.Width; i++)
                for (int j = 0; j < bm.Height; j++)
                {
                    int pd = get_avgPixCM((i  ) * 10, (j) * 10);
                    bm.SetPixel(i, j, Color.FromArgb(255, pd, pd, pd));

                }


            foreach (carPos c in cpHist)
            {
                int px = (int)c.x / 10;
                int py = (int)c.y / 10;
                if (py >= bm.Height) py = bm.Height - 1;

                if (px >= bm.Width) px = bm.Width - 1;

                if (px < 0) px = 1;
                if (py < 0) py = 1;

                //    py = bm.Height - py;
                int spedc = (int)(c.speed / 20);
                if (spedc > 255) spedc = 255;
                if (spedc < 0) spedc = 0;
                bm.SetPixel(px, py, Color.FromArgb(255,spedc, spedc, 0));

                bm.SetPixel(px + 1, py + 1, Color.Green);
                bm.SetPixel(px + 1, py - 1, Color.Green);
                bm.SetPixel(px - 1, py + 1, Color.Green);
                bm.SetPixel(px - 1, py - 1, Color.Green);

            }




            pbTrack.Image = bm;
        }
        private void button2_Click(object sender, EventArgs e)
        {
            renderTrack();
        }


        private byte get_avgPixdM(double x, double y)
        {
            byte ret = 0;

            if (double.IsNaN(x))
                return 0;
            if (double.IsNaN(y))
                return 0;

            if (x < 0.1) x = 0.1;
            if (y < 0.1) y = 0.1;

            if (x > 9998) x = 9998;
            if (y > 8998) y = 8998;

            int x0 = (int)Math.Round(x,0);
            int x1 = x0 + 1; 

            int y0 = (int)Math.Round(y, 0);
            int y1 = y0 + 1;


            int avgX = Convert.ToInt32(trackGrid[x0, y0]) + Convert.ToInt32(trackGrid[x1, y0]) + Convert.ToInt32(trackGrid[x0, y1]) + Convert.ToInt32(trackGrid[x1, y1]);



           
            ret = (byte)(avgX * 63);

          //  if(ret>0)
          //      Console.WriteLine("X:" + x0.ToString() + " Y:"+y0.ToString() + " ="+ ret.ToString());
            return ret;

        }

        private int get_avgPixCM(int x, int y)
        {
            int ret =0;

            if (x < 5) x = 5;
            if (y < 5) y = 5;

            if (x > 9994) x = 9994;
            if (y > 8994) y = 8994;

            for(int i=x-5;i<5+x;i++)
                for (int j = y-5; j < 5+y; j++)
                {
                    if(trackGrid[i,j])
                        ret++;
                }

            if (ret > 0)
            {

            }

            ret = 255 - ret * 255 / 100;
            return ret;

        }


        public void renderCarStep(int mSecStep)
        {

            if ((car.servoDeg < 1) & (car.servoDeg > -1))
            {
                car.turnRad = float.PositiveInfinity;
            }
            else
        {


                
                car.turnRad =  (car.cal.setupWheelBase / (float)Math.Sin(Math.Sinh((car.cal.servoArm * Math.Sin(car.servoDeg / 180 * Math.PI)) / car.cal.steeringNuckle)));

                if (car.servoDeg > 0)
                    car.turnRad = car.turnRad- 307.4336015f + car.cal.setupTrack / 2;
                else
                    car.turnRad = car.turnRad + 307.4336015f - car.cal.setupTrack / 2;
            //    if (car.servoDeg < 0)
              //      car.turnRad = -car.turnRad;

                if ((car.turnRad > 0) & (car.turnRad < 100))
                    car.turnRad = 100;
                if ((car.turnRad < 0) & (car.turnRad > -100))
                    car.turnRad = -100;
            }

            if(Double.IsPositiveInfinity(car.turnRad))
                car.turnRad = 10000000;
            else if (Double.IsNegativeInfinity (car.turnRad))
                car.turnRad = -10000000;

            double posNewX = car.x - ( car.turnRad*Math.Sin(car.dir)) + (car.turnRad*Math.Sin(car.vel/car.turnRad/mSecStep + car.dir));// car.vel / (float)uSecStep * Math.Cos(car.dir + car.turnRad);
            double posNewY = car.y + (car.turnRad * Math.Cos(car.dir)) - (car.turnRad * Math.Cos(car.vel / car.turnRad / mSecStep + car.dir));

            car.x = posNewX;
            car.y = posNewY;
            car.dir = car.dir + car.vel / car.turnRad / mSecStep;
            car.accel = car.vel * car.vel / car.turnRad;


            if (car.accel > 2100)
            {
                car.ctrl.inCorner = true;
                car.ctrl.turnLeft = true;
                car.ctrl.turnRight = false;
            }
            else if (car.accel < -2100)
            {
                car.ctrl.inCorner = true;
                car.ctrl.turnLeft = false;
                car.ctrl.turnRight = true;
            }
            else
            {
                car.ctrl.inCorner = false;
            }

        }


        void updateSensor()
        {
            double spX = car.x + car.lightSense.posX * Math.Cos(car.dir);
            double spY = car.y + car.lightSense.posX * Math.Sin(car.dir);

            double spXstart = spX + car.lightSense.posX / 2 * Math.Cos(car.dir + Math.PI / 2);
            double spYstart = spY + car.lightSense.posX / 2 * Math.Sin(car.dir + Math.PI / 2);

            double spXend = spX - car.lightSense.posX / 2 * Math.Cos(car.dir + Math.PI / 2);
            double spYend = spY - car.lightSense.posX / 2 * Math.Sin(car.dir + Math.PI / 2);

            bool foundLeft = false;
            bool foundRight = false;

            for (int i = 0; i < 128; i++)
            {
                double px = spXstart + i * (spXend - spXstart) / 128;
                double py = spYstart + i * (spYend - spYstart) / 128;

                car.lightSense.sensorV[i] = get_avgPixdM(px, py);

         
                if (!foundLeft & car.lightSense.sensorV[i] > 25)
                {

                    foundLeft = true;
                    car.lightSense.leftOn = i;
                   
                }
                else if (foundLeft & !foundRight)
                {
                    if (car.lightSense.sensorV[i] < 15)
                    {
                        foundRight = true;
                        car.lightSense.rightOn = i;
                        car.lightSense.senseWide = car.lightSense.rightOn - car.lightSense.leftOn;

                    }
                    else if (i == 127)
                    {
                        car.lightSense.rightOn = 127;
                        car.lightSense.senseWide = car.lightSense.rightOn - car.lightSense.leftOn;
                    }
                }             
            }


            if ((car.lightSense.senseWide > 12) && (car.lightSense.senseWide < 40))
            {
                car.lightSense.valid = 1;
               // Random r = new Random();

             // r.NextDouble();

               // car.lightSense.leftOn += (int)(((float)rr - 0.5f) * 15.0f);

              //  rr = r.NextDouble();

                //car.lightSense.rightOn += (int)(((float)rr - 0.5f) * 15.0f);
                car.lightSense.senseWide = car.lightSense.rightOn - car.lightSense.leftOn;

            }
            else
                car.lightSense.valid = 0;

//            Console.WriteLine(car.ctrl.center.ToString());
        }

        void updateControls()
        {
            //process Servo Lag
            float servoError = car.ctrl.servoDegTarget - car.servoDeg;
            
            //max rate of servo
            if (servoError > 2.67)
                servoError = 2.67f;

            if (servoError < -2.67)
                servoError = -2.67f;
            car.servoDeg = car.servoDeg + servoError;
            if (car.servoDeg > 65) car.servoDeg = 65;
            if (car.servoDeg < -65) car.servoDeg = -65;

            //process Speed
            car.vel += (car.ctrl.velTarget - car.vel) / 150;


            if (car.ctrl.servoPWMTarget > 20)
                car.ctrl.centerTarget = (byte)((car.ctrl.centerTarget + (byte)(64 + ((car.ctrl.servoPWMTarget) / 20))) / 2);
            else if (car.ctrl.servoPWMTarget <-20)
                car.ctrl.centerTarget = (byte)((car.ctrl.centerTarget + (byte)(64 + (( car.ctrl.servoPWMTarget) / 20))) / 2);
            else
                car.ctrl.centerTarget = (byte)((car.ctrl.centerTarget + 64) / 2);

          
               

                //20mSecSevoPWM Update
                int rem = 1;
                Math.DivRem(runStep, 10, out rem);

                if (rem == 0 & car.lightSense.valid==1)
                {
                    car.ctrl.c4 = car.ctrl.c3;
                    car.ctrl.c3 = car.ctrl.c2;
                    car.ctrl.c2 = car.ctrl.c1;
                    car.ctrl.c1 = car.ctrl.center;
                    car.ctrl.center = (byte)((car.lightSense.leftOn + car.lightSense.rightOn) / 2);

                    car.ctrl.error = (Int16)(car.ctrl.centerTarget - car.ctrl.center);

                    if ((Math.Abs(car.ctrl.error) > car.cal.errorTol / 2))
                        car.ctrl.iTerm += car.ctrl.error * car.cal.servGainIgain;

                    if (car.ctrl.iTerm > 300) car.ctrl.iTerm = 300;
                    else if (car.ctrl.iTerm < -300) car.ctrl.iTerm = -300;

                    car.ctrl.dterm = (car.ctrl.dterm * 85 + (car.ctrl.c4 - car.ctrl.center) * car.cal.servGainDTerm * 15) / 100.0f;//(car.ctrl.dterm*4/4000 + (car.ctrl.c4 - car.ctrl.center))/5 * 1000;
      

                    car.ctrl.servoPWMTarget = (short)(car.ctrl.iTerm + car.ctrl.dterm + car.ctrl.error * car.cal.servGain);

                    if (car.ctrl.servoPWMTarget > 300)
                        car.ctrl.servoPWMTarget = 300;
                    if (car.ctrl.servoPWMTarget < -300)
                        car.ctrl.servoPWMTarget = -300;


                    if (Math.Abs(car.ctrl.servoPWMTarget) < 30)
                    {
                        if (car.ctrl.straightLearn < 100)
                            car.ctrl.straightLearn += 1;
                        else
                            car.ctrl.straightLearn = 100;
                    }
                    else
                        
                    {
                        if (car.ctrl.straightLearn > 10)
                            car.ctrl.straightLearn -= 10;
                        else
                            car.ctrl.straightLearn = 0;
                    }


                car.ctrl.servoDegTarget = (Int16)((float)car.ctrl.servoPWMTarget / 695.5033146f * 90.0f);


                if (car.ctrl.servoPWMTarget > 40)
                    car.ctrl.velBiasTarget = (UInt16)(493.18 - car.ctrl.servoPWMTarget * 0.1264 - car.ctrl.servoPWMTarget * car.ctrl.servoPWMTarget * 0.0007);
                else if (car.ctrl.servoPWMTarget < -40)
                    car.ctrl.velBiasTarget = (UInt16)(506.82 - car.ctrl.servoPWMTarget * 0.1264 + car.ctrl.servoPWMTarget * car.ctrl.servoPWMTarget * 0.0007);
                else
                    car.ctrl.velBiasTarget = 500;


                if (car.ctrl.servoDegTarget > 70) 
                    car.ctrl.servoDegTarget = 70;
                if (car.ctrl.servoDegTarget < -70) 
                    car.ctrl.servoDegTarget = -70;


        
            }


                if (Math.Abs(car.accel) < 400 & Math.Abs(car.ctrl.error) < 30)
                {
                    float maxV = (float)Math.Sqrt(car.cal.maxAccel*9810 * Math.Abs(car.turnRad));
                    if (maxV < car.cal.maxSpeed)
                        car.ctrl.velTarget = (UInt16)maxV;
                    else
                        car.ctrl.velTarget = (UInt16)(car.cal.maxSpeed + (car.ctrl.straightLearn * 4));

                }
                else if (Math.Abs(car.ctrl.error) < 50)
                {
                    float maxV = (float)Math.Sqrt(car.cal.maxAccel * 9810 * Math.Abs( car.turnRad));
                    if (maxV < car.cal.maxSpeed)
                        car.ctrl.velTarget = (UInt16)maxV;
                    else
                        car.ctrl.velTarget = (UInt16)car.cal.maxSpeed;
                }
                else
                    car.ctrl.velTarget = (UInt16)car.cal.lostSpeed;


        }



        private void Form1_Load(object sender, EventArgs e)
        {

        }

        StreamWriter sr;

        int runStep;
        double errorMean;

        private void simulate(int errorSweep)
        {
            cpHist = new List<carPos>();

            car.x = 450;
            car.y = 1500;
            car.dir = -Math.PI / 3;
            car.servoDeg = 0;
            car.turnRad = 500000;
            car.vel = 0;//200mm/s
            car.lightSense = new carSensor();
            car.lightSense.sensorV = new byte[128];
            car.ctrl.velTarget = 800;
            car.velRLratio = 0.5f;

            car.cal = new calibrationS();

            car.cal.setupTrack = 140;
            car.cal.setupWheelBase = 210;
            car.cal.servoArm = 13;
            car.cal.steeringNuckle = 22;



            car.lightSense.posX = 200;
            car.lightSense.width = 150;

            car.ctrl = new carControl();
            car.ctrl.center = 64;
            car.ctrl.centerTarget = 64;
            car.ctrl.iTerm = 0;
            runStep = 0;

            car.cal.servGain = 1.7f;
            car.cal.servGainIgain = 2.0f;
            car.cal.errorTol = 3;

            car.ctrl.straightLearn = 0;

            try { car.cal.servGain = Convert.ToSingle(txtGainServo.Text); }
            catch (Exception ee)
            {
            }

            

            try { car.cal.servGainIgain = Convert.ToSingle(txtGainServoDT.Text); }
            catch (Exception ee)
            {
            }

            try { car.cal.errorTol = (byte)Convert.ToInt32(txtErrorTol.Text); }
            catch (Exception ee)
            {
            }

            try { car.cal.servGainDTerm = Convert.ToSingle(txtGainServoDGain.Text); }
            catch (Exception ee)
            {
            }

           // car.cal.servGainDTerm = Convert.ToSingle(txtGainServoDGain.Text) + (10.0f - errorSweep) / 10.0f;

            try { car.cal.maxSpeed = Convert.ToUInt16(txtMavV.Text); }
            catch (Exception ee)
            {
            }

            try { car.cal.lostSpeed = Convert.ToUInt16(txtLostV.Text); }
            catch (Exception ee)
            {
            }

            try { car.cal.maxAccel = Convert.ToSingle(txtMaxAcc.Text); }
            catch (Exception ee)
            {
            }

            if (errorSweep != 0)
                car.cal.errorTol = (byte)errorSweep;



            errorMean = 0;
            sr = new StreamWriter("C:/fsCup.e2csv");
            sr.WriteLine("time,posX,posY,speed,speedTarget,accel,turnRad,servoDeg,servoDegTarget,servoPwmTarget,dir,senseL,senseR,senseW,ctError,ctErrorRate,ctcenter,dterm,radiusTarget,biasVelTarget,bTurn,bL,bR,iterm,centerTarget");
            sr.WriteLine("sec,mm,mm,mm/s,mm/s,g,mm,deg,deg,rad,x,x,x,mm,x,x,x,x,mm,rat,b,b,b,x,x");
            double lapTime = 0;
            for (int i = 0; i < 45000; i++)
            {
                renderCarStep(1000);
                int rem;
                
                    updateSensor();

                Math.DivRem(runStep, 5, out rem);
                if (rem == 0)
                    updateControls();

                sr.Write(((float)i / 1000).ToString() + ",");
                sr.Write(car.x.ToString("F3") + ",");
                sr.Write(car.y.ToString("F3") + ",");
                sr.Write(car.vel.ToString("F3") + ",");
                sr.Write(car.ctrl.velTarget.ToString("F3") + ",");
                sr.Write((car.accel/1000/9.81).ToString("F3") + ",");
                sr.Write(car.turnRad.ToString("F3") + ",");
                sr.Write(car.servoDeg.ToString("F3") + ",");
                sr.Write(car.ctrl.servoDegTarget.ToString("F3") + ",");
                sr.Write(car.ctrl.servoPWMTarget.ToString() + ",");
                sr.Write(car.dir.ToString("F3") + ",");
                sr.Write(car.lightSense.leftOn.ToString() + ",");
                sr.Write(car.lightSense.rightOn.ToString() + ",");
                sr.Write(car.lightSense.senseWide.ToString() + ",");
                sr.Write(car.ctrl.error.ToString() + ",");
                sr.Write(car.ctrl.errorRate.ToString() + ",");
                sr.Write(car.ctrl.center.ToString() + ",");
                sr.Write(car.ctrl.dterm.ToString() + ",");
                sr.Write(car.ctrl.radiusTarget.ToString() + ",");
                sr.Write(car.ctrl.velBiasTarget.ToString() + ",");
                if(car.ctrl.inCorner)
                    sr.Write("1,");
                else
                    sr.Write("0,");

                if (car.ctrl.turnLeft)
                    sr.Write("1,");
                else
                    sr.Write("0,");

                if (car.ctrl.turnRight)
                    sr.Write("1,");
                else
                    sr.Write("0,");
                sr.Write(car.ctrl.iTerm.ToString() + ",");
                sr.WriteLine(car.ctrl.centerTarget.ToString() + ",");

                lapTime = i / 1000f;
                carPos cc = new carPos();
                cc.x = car.x;
                cc.y = car.y;
                cc.speed = car.vel;
                
                Math.DivRem(runStep, 10, out rem);
                if (rem == 0)
                    cpHist.Add(cc);

                errorMean += Math.Abs(car.ctrl.error);

                if (car.lightSense.senseWide > 28 && car.lightSense.senseWide < 40)
                {
                    errorMean = errorMean / (double)runStep;
                    break;
                }
                runStep++;
            }
            errorMean = errorMean / (double)runStep;
            sr.Close();
            label1.Text = "Lap :" + lapTime.ToString("F3");
            Console.WriteLine("Done : er=" + errorSweep + " time =" + lapTime.ToString("F3") + " error:"+errorMean.ToString("F1")); ;
            renderTrack();

        }

        private void button3_Click(object sender, EventArgs e)
        {
       //     for (int i = 0; i < 20; i++)
            {

                simulate(0);
            }
        }

        private void label6_Click(object sender, EventArgs e)
        {

        }


        byte[] spRxData = new byte[512];
        int spEnd = 0;
        int spRead = 0;
        private void spCar_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            int rB = spCar.BytesToRead;

            if ((rB + spEnd) > spRxData.Length)
            {
                int rol = - spEnd + spRxData.Length;

                spCar.Read(spRxData, spEnd, rol);
                spEnd += rol;

                if (spEnd >= 512)
                    spEnd = 0;

                spCar.Read(spRxData, spEnd, rB-rol);
                spEnd += rB-rol;
                if (spEnd >= 512)
                    spEnd = 0;
            }
            else
            {
                spCar.Read(spRxData, spEnd, rB);
                spEnd += rB;

                if(spEnd>=512)
                    spEnd = 0;
            }

            parseUartData();

        }


        private void parseUartData()
        {
            //packet = 0x01|0x02|DLC|data|0x55
            int spEnde = spEnd;

            if (spEnd < spRead)
                spEnde += spRxData.Length;

            for (int i = spRead; ((i < spRead + spRxData.Length) && (i < spEnde)); i++)
            {   
                if (spRxData[(i& 0x1FF)] == 0x01)
                {
                    if (spRxData[((i+1)& 0x1FF)] == 0x02)
                    {
                        int dlc = (int)spRxData[((i+2)& 0x1FF)];
                        byte[] packet = new byte[dlc];
                        for (int r = 0; r < dlc; r++)
                        {
                            packet[r] = spRxData[((i + 3 + r) & 0x1FF)];

                        }
                        if(spRxData[((i + 3 + dlc) & 0x1FF)]==0x55)
                        {
                            spRead += 4 + dlc;
                            if (spRead >= spRxData.Length)
                                spRead = spRead -spRxData.Length;
                            processCommand(packet);
                        }

                    }
                }


            }



        }

        private void processCommand(byte[] data)
        {
            if (data.Length < 1)
                return;
            switch (data[0])
            {
                case 0x50:
                    car.lightSense.leftOn = data[1];
                    car.lightSense.rightOn = data[2];
                    car.lightSense.width = data[3];
                    car.ctrl.center = data[4];

                    car.ctrl.velTarget = (UInt16)((UInt16)data[6] + 256 * (UInt16)data[5]);

                    car.ctrl.servoDegTarget = (Int16)((UInt16)data[8] + 256 * (UInt16)data[7]);
                    car.ctrl.servoDegTarget = (Int16)((UInt16)data[12] + 256 * (UInt16)data[11]);
                    car.ctrl.ctrlMode = data[13];
                    car.lightSense.valid = data[14];

                  

                    break;

            }
        }

        private void button4_Click(object sender, EventArgs e)
        {
            spCar.PortName = "COM5";
            spCar.Open();
            timerUpdate.Start();
            /*
            spRxData[0] = 0x00;
            spRxData[1] = 0x01;
            spRxData[2] = 0x02;
            spRxData[3] = 0x04;
            spRxData[4] = 0x0a;
            spRxData[5] = 0x0b;
            spRxData[6] = 0x0c;
            spRxData[7] = 0x0d;
            spRxData[8] = 0x55;

            spRead = 0;
            spEnd = 8;
            parseUartData();*/
        }

        private void timerUpdate_Tick(object sender, EventArgs e)
        {
            txtSleftOn.Text = car.lightSense.leftOn.ToString();
            txtSrightOn.Text = car.lightSense.rightOn.ToString();
            txtSWidth.Text = car.lightSense.width.ToString();
            txtSCenter.Text = car.ctrl.center.ToString() + " : " + car.lightSense.valid.ToString();
            txtMode.Text = car.ctrl.ctrlMode.ToString();

            txtCtargetVel.Text = car.ctrl.velTarget.ToString();
            txtCtargetSer.Text = car.ctrl.servoDegTarget.ToString();
            textBox2.Text = car.ctrl.error.ToString();
        }

        private void button5_Click(object sender, EventArgs e)
        {
            if (spCar.IsOpen)
                spCar.Write("0");
        }

        private void button6_Click(object sender, EventArgs e)
        {
            if (spCar.IsOpen)
                spCar.Write("1");
        }

        private void button7_Click(object sender, EventArgs e)
        {
            if (spCar.IsOpen)
                spCar.Write("2");
        }

        private void button12_Click(object sender, EventArgs e)
        {

            if (spCar.IsOpen)
                spCar.Write("x");
        }

        private void button8_Click(object sender, EventArgs e)
        {

            if (spCar.IsOpen)
                spCar.Write("t");
        }

        private void button9_Click(object sender, EventArgs e)
        {

            if (spCar.IsOpen)
                spCar.Write("g");
        }

        private void button11_Click(object sender, EventArgs e)
        {

            if (spCar.IsOpen)
                spCar.Write("h");
        }

        private void button10_Click(object sender, EventArgs e)
        {

            if (spCar.IsOpen)
                spCar.Write("f");
        }

        private void txtGainServo_TextChanged(object sender, EventArgs e)
        {

        }



    }
}
