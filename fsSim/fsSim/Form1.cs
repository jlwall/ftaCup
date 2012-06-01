using System;
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
            public double center;
            public double error;
            public double errorRate;
            public double imem;
            public double dterm;

            public bool inCorner;
            public bool turnLeft;
            public bool turnRight;

            public double radiusTarget;

            public double c1, c2, c3, c4;
            
        }

        class carSensor
        {
            public byte[] sensorV;
            public double width;
            public double posX;
            public int leftOn;
            public int rightOn;
            public int senseWide;
        }

        class calibrationS
        {
            public double servGain;
            public double servGainIgain;
            public double servGainDTerm;
            public int errorTol;
            public double maxSpeed;
            public double lostSpeed;
            public double maxAccel;
        }

        class carStruct
        {
            public double x; //pos x in mm
            public double y; // pos y in mm
            public double dir; //radian Direction
            
            public double turnRad;
            public double vel; //mm/s
            public double velTarget;
            public double velRLratio;
            public double accel;

            public double servoDeg;

            public double setupTrack;
            public double setupWheelBase;
            public double servoArm;
            public double steeringNuckle;

            public double servoDegTarget;
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

        Boolean[,] trackGrid = new Boolean[10000, 6000];

        public void setupTrack()
        {
     
            for(int i=0;i<trackGrid.GetLength(0);i++)
                for(int j=0;j<trackGrid.GetLength(1);j++)
                    trackGrid[i,j]=false;


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
            trackInsetCurve(700, 1050, 500, 25, Math.PI, Math.PI / 2);

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
            Bitmap bm = new Bitmap(1000, 600);

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

                if (px < 0) px = 0;
                if (py < 0) py = 0;

                //    py = bm.Height - py;
                int spedc = (int)(c.speed / 20);
                if (spedc > 255) spedc = 255;
                if (spedc < 0) spedc = 0;
                bm.SetPixel(px, py, Color.FromArgb(255, 255 - spedc, spedc, 0));

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
            if (y > 5998) y = 5998;

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
            if (y > 5994) y = 5994;

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
                car.turnRad = Double.PositiveInfinity;
            }
            else
            {

                
                car.turnRad = car.setupTrack / 2 + (car.setupWheelBase / Math.Sin(Math.Sinh((car.servoArm * Math.Sin(car.servoDeg / 180 * Math.PI)) / car.steeringNuckle)));

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


            if (car.accel > 50)
            {
                car.ctrl.inCorner = true;
                car.ctrl.turnLeft = true;
                car.ctrl.turnRight = false;
            }
            else if (car.accel < -50)
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
            car.ctrl.c4 = car.ctrl.c3;
            car.ctrl.c3 = car.ctrl.c2;
            car.ctrl.c2 = car.ctrl.c1;
            car.ctrl.c1 = car.ctrl.center;
            car.ctrl.center = car.lightSense.leftOn + car.lightSense.senseWide / 2;

            car.ctrl.errorRate = ((car.ctrl.error * 4 / 5) + (64 - car.ctrl.center)) / 5;

            car.ctrl.error = 64 - car.ctrl.center;

            if ( (Math.Abs(car.ctrl.error) < car.cal.errorTol*2) & (Math.Abs(car.turnRad )> 5000))
                car.ctrl.imem = 0;
            else
            car.ctrl.imem += car.ctrl.errorRate;


            car.ctrl.dterm = (car.ctrl.dterm*4/4000 + (car.ctrl.c4 - car.ctrl.center))/5 * 1000;
         

//            Console.WriteLine(car.ctrl.center.ToString());
        }

        void updateControls()
        {
            //process Servo Lag
            double servoError = car.servoDegTarget - car.servoDeg;
            
            //max rate of servo
            if (servoError > 2.67)
                servoError = 2.67;

            if (servoError < -2.67)
                servoError = -2.67;
            car.servoDeg = car.servoDeg + servoError;
            if (car.servoDeg > 65) car.servoDeg = 65;
            if (car.servoDeg < -65) car.servoDeg = -65;

            //process Speed
            car.vel += (car.velTarget - car.vel) / 850;

          
                if (car.ctrl.error > car.cal.errorTol)
                {
                    car.ctrl.radiusTarget = ((5000-(double)car.ctrl.errorRate * car.cal.servGain) - car.ctrl.imem * car.cal.servGainIgain - car.ctrl.dterm * car.cal.servGainDTerm);
                    if (car.ctrl.radiusTarget < 300)
                        car.ctrl.radiusTarget = 300;
                }
                else if (car.ctrl.error < -car.cal.errorTol)
                {
                    car.ctrl.radiusTarget = -((5000 + (double)car.ctrl.errorRate * car.cal.servGain) + car.ctrl.imem * car.cal.servGainIgain + car.ctrl.dterm * car.cal.servGainDTerm);
                    if (car.ctrl.radiusTarget > -300)
                        car.ctrl.radiusTarget = -300;
                }
                else
                {
                    if(car.ctrl.error>0)
                    car.ctrl.radiusTarget = 5000;
                    else
                        car.ctrl.radiusTarget = -5000;
                }

                //20mSecSevoPWM Update
                int rem = 1;
                Math.DivRem(runStep, 20, out rem);

                if (rem == 0)
                {
                    double stn=0;
                if (Math.Abs(car.ctrl.radiusTarget) < 6000)
                {
                    if (car.ctrl.radiusTarget > 0)
                        stn = 76970 * Math.Pow(car.ctrl.radiusTarget, -1.239);
                    else
                       stn = -76970 * Math.Pow(-car.ctrl.radiusTarget, -1.239);

                }


                car.servoDegTarget = (stn*3 + car.servoDegTarget ) / 4;



                if (car.servoDegTarget > 70) 
                    car.servoDegTarget = 70;
                if (car.servoDegTarget < -70) 
                    car.servoDegTarget = -70;

            }


                if (Math.Abs(car.accel) < 400 & Math.Abs(car.ctrl.error) < 30)
                {
                    double maxV = Math.Sqrt(car.cal.maxAccel*9810 * Math.Abs(car.turnRad));
                    if (maxV < car.cal.maxSpeed)
                        car.velTarget = maxV;
                    else
                        car.velTarget = car.cal.maxSpeed;

                }
                else if (Math.Abs(car.ctrl.error) < 50)
                {
                    double maxV = Math.Sqrt(car.cal.maxAccel * 9810 * Math.Abs( car.turnRad));
                    if (maxV < car.cal.maxSpeed)
                        car.velTarget = maxV;
                    else
                        car.velTarget = car.cal.maxSpeed;
                }
                else
                    car.velTarget = car.cal.lostSpeed;


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

            car.x = 200;
            car.y = 1900;
            car.dir = Math.PI / 2 ;
            car.servoDeg = 0;
            car.turnRad = 500000;
            car.vel = 0;//200mm/s
            car.lightSense = new carSensor();
            car.lightSense.sensorV = new byte[128];
            car.velTarget = 800;
            car.velRLratio = 0.5;


            car.setupTrack = 190;
            car.setupWheelBase = 270;
            car.servoArm = 25;
            car.steeringNuckle = 20;



            car.lightSense.posX = 120;
            car.lightSense.width = 80;

            car.ctrl = new carControl();
            car.ctrl.center = 64;
            car.ctrl.imem = 0;
            car.cal = new calibrationS();
            runStep = 0;

            car.cal.servGain = 1.7;
            car.cal.servGainIgain = 2.0;
            car.cal.errorTol = 3;

            try { car.cal.servGain = Convert.ToDouble(txtGainServo.Text); }
            catch (Exception ee)
            {
            }

            try { car.cal.servGainIgain = Convert.ToDouble(txtGainServoDT.Text); }
            catch (Exception ee)
            {
            }

            try { car.cal.errorTol = Convert.ToInt32(txtErrorTol.Text); }
            catch (Exception ee)
            {
            }

            try { car.cal.servGainDTerm = Convert.ToDouble(txtGainServoDGain.Text); }
            catch (Exception ee)
            {
            }

            try { car.cal.maxSpeed = Convert.ToDouble(txtMavV.Text); }
            catch (Exception ee)
            {
            }

            try { car.cal.lostSpeed = Convert.ToDouble(txtLostV.Text); }
            catch (Exception ee)
            {
            }

            try { car.cal.maxAccel = Convert.ToDouble(txtMaxAcc.Text); }
            catch (Exception ee)
            {
            }

            if (errorSweep != 0)
                car.cal.errorTol = errorSweep;



            errorMean = 0;
            sr = new StreamWriter("C:/fsCup.e2csv");
            sr.WriteLine("time,posX,posY,speed,speedTarget,accel,turnRad,servoDeg,servoDegTarget,dir,senseL,senseR,senseW,ctError,ctErrorRate,ctcenter,dterm,radiusTarget,bTurn,bL,bR,iterm");
            sr.WriteLine("sec,mm,mm,mm/s,mm/s,g,mm,deg,deg,rad,x,x,mm,x,x,x,x,mm,b,b,b,x");
            double lapTime = 0;
            for (int i = 0; i < 45000; i++)
            {
                renderCarStep(1000);
                int rem;
                Math.DivRem(runStep, 4, out rem);
                if (rem == 0)
                    updateSensor();

                updateControls();
                sr.Write(((float)i / 1000).ToString() + ",");
                sr.Write(car.x.ToString("F3") + ",");
                sr.Write(car.y.ToString("F3") + ",");
                sr.Write(car.vel.ToString("F3") + ",");
                sr.Write(car.velTarget.ToString("F3") + ",");
                sr.Write((car.accel/1000/9.81).ToString("F3") + ",");
                sr.Write(car.turnRad.ToString("F3") + ",");
                sr.Write(car.servoDeg.ToString("F3") + ",");
                sr.Write(car.servoDegTarget.ToString("F3") + ",");
                sr.Write(car.dir.ToString("F3") + ",");
                sr.Write(car.lightSense.leftOn.ToString() + ",");
                sr.Write(car.lightSense.rightOn.ToString() + ",");
                sr.Write(car.lightSense.senseWide.ToString() + ",");
                sr.Write(car.ctrl.error.ToString() + ",");
                sr.Write(car.ctrl.errorRate.ToString() + ",");
                sr.Write(car.ctrl.center.ToString() + ",");
                sr.Write(car.ctrl.dterm.ToString() + ",");
                sr.Write(car.ctrl.radiusTarget.ToString() + ",");
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
                
                sr.WriteLine(car.ctrl.imem.ToString() + ",");

                lapTime = i / 1000f;
                carPos cc = new carPos();
                cc.x = car.x;
                cc.y = car.y;
                cc.speed = car.vel;

                cpHist.Add(cc);

                errorMean += Math.Abs(car.ctrl.error);

                if (car.lightSense.senseWide > 100)
                {
                    errorMean = errorMean / (double)runStep;
                    break;
                }
                runStep++;
            }
            sr.Close();
            label1.Text = "Lap :" + lapTime.ToString("F3");
            Console.WriteLine("Done : er=" + errorSweep + " time =" + lapTime.ToString("F3") + " error:"+errorMean.ToString("F1")); ;
            renderTrack();

        }

        private void button3_Click(object sender, EventArgs e)
        {
           // for (int i = 1; i < 20; i++)
            {

                simulate(0);
            }
        }

        private void label6_Click(object sender, EventArgs e)
        {

        }



    }
}
