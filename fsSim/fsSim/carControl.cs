using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;




namespace fsSim
{


    public class carControl
    {
        byte camLostCenter = 0;
        byte camLostLeft = 1;
        byte camLeft = 2;
        byte camCenter = 3;
        byte camRight = 4;
        byte camLostRight = 5;
        Int16 constServoMax = 350;
        Int16 constBiasCenter = 500;

        static CAR car;
         CAR_CAL cal;

         class CAR_ADJUST
        {
	public float adjPgain;
	public float adjIgain;
	public float adjDgain;
	public float adjSpeedgain;
	public Int16[] biasVelTable; //40 is neutral
	
        };

 class CAR_CAL 
{
	public float servGain;
	public float servGainIgain;
	public float servGainDTerm;
	public float setupTrack;
	public float setupWheelBase;
	public float servoArm;
	public float steeringNuckle;
	public float maxAccel;
 	
	public float servGainIgainLimit;

	public Int16 errorTol;
	public UInt16 maxSpeed;
	public UInt16 lostSpeed;
	
	public byte  senseWidthMin;
	public byte  senseWidthMax;
	public byte  senseWidthMinDer;
	public byte  senseWidthMaxDer;
	
	public byte  runTime;
	public UInt16  sensorMinDynRange;
	public sbyte  sensorMaxError;
	public byte apexModError;
	public UInt16 maxLearn;
	
};

 struct LIGHT_SENSOR
{
	public UInt16[]  array;
	public UInt16[]  arrayTeach;
	public UInt16  threshold;
	public UInt16  valMax;
	public UInt16  valMin;
	public byte	cornLeft;
	public byte	cornRight;
	public byte 	width;
	public byte  center;
	public byte  c1;
	public byte  c2;
	public byte  c3;
	public byte  c4;
	public byte  valid;
	public byte teachDone;
	public UInt16  valMaxTemp;
	public UInt16  valMinTemp;
	public byte 	errorCounter;
		
}; //16 bytes to log

 struct CAR_CTRL {
public float iTerm;
public float dterm;  
public Int16 targetServoPos;
public UInt16 targetVelocity;
public Int16 biasVelocity;
public Int16 servoDegTarget;
public UInt16 velTarget;
public Int16 error;
public UInt16 straightLearn;
public sbyte autoTimer;
public byte manualMode;
public byte  controlCenter;
     /// <summary>
     /// These are new
     /// </summary>
public byte apexExitDown;
public byte apexEntryIn;
public byte apexEntry;
public byte breakLearn;
} ; //24  bytes to log

 class CAR {
  public CAR_CTRL ctrl;
  public LIGHT_SENSOR sensor;
  public CAR_ADJUST adjust;

 public float pGain;
 public float iGain;
 public float dGain;
 public float speedGain;
 public byte TeachSenseWidthMin;
 public byte TeachSenseWidthMax;
 public UInt16 TeachSensorMinDynRange;
 //public UInt16 logPacketIndex;
};
        

        void setupCarControl()
        {
            //car = new CAR();
           // cal = new CAR_CAL();
            
        }
        public void setPIDLoop()
        {
            //set the steering position, iTerm

            car.ctrl.iTerm += (float)car.ctrl.error * car.iGain;

            //Limit the iTerm
            if (car.ctrl.iTerm > cal.servGainIgainLimit)
                car.ctrl.iTerm = cal.servGainIgainLimit;
            else if (car.ctrl.iTerm < -cal.servGainIgainLimit)
                car.ctrl.iTerm = -cal.servGainIgainLimit;


            if (car.ctrl.apexExitDown < 180)
                car.ctrl.dterm = (car.sensor.c2 - car.sensor.center) * car.dGain;
            else
                car.ctrl.dterm = 0;


            //set the position, P, and I term only here
            car.ctrl.targetServoPos = (Int16)((float)car.ctrl.error * car.pGain + car.ctrl.iTerm + car.ctrl.dterm);

            //limit servo position
            if (car.ctrl.targetServoPos < -constServoMax)
                car.ctrl.targetServoPos = (short)-constServoMax;
            else if (car.ctrl.targetServoPos > constServoMax)
                car.ctrl.targetServoPos = constServoMax;

        }


        void setStraightLearn()
        {
            //straightLearning
            if ((car.sensor.valid == camCenter) && (car.ctrl.targetServoPos > -50) && car.ctrl.targetServoPos < 50) //if steering is somewhat straight
            {
                if (car.ctrl.straightLearn < cal.maxLearn)
                    car.ctrl.straightLearn += 1;
                else
                    car.ctrl.straightLearn = cal.maxLearn;
            }
            else
            {
                if (car.ctrl.straightLearn > 15) // can learn down to null in 0.11 seconds
                    car.ctrl.straightLearn -= 15;
                else
                    car.ctrl.straightLearn = 0;

                if (car.ctrl.straightLearn > 100)
                    car.ctrl.breakLearn = 5;
            }
        }

        
        void setApexTargeting()
        {
	        //apex set
	        if((car.ctrl.targetServoPos>200) && car.ctrl.apexExitDown == 0)
		        {
                    car.ctrl.apexEntryIn = 100;
                    car.ctrl.apexExitDown = 100;
                    car.ctrl.apexEntry = 1;
		        }
            else if ((car.ctrl.targetServoPos < -200) && car.ctrl.apexExitDown == 0)
		        {
                    car.ctrl.apexEntryIn = 100;
                    car.ctrl.apexExitDown = 100;
                    car.ctrl.apexEntry = 0;
	        }

	        //set the control center Apex seeking portion
            if (car.ctrl.apexEntryIn > 0)
		        {
                    if (car.ctrl.apexEntry == 1)
                        car.ctrl.controlCenter = (byte)((Int16)64 + (Int16)((100 - car.ctrl.apexEntryIn) * (Int16)cal.apexModError / 128));
		        else
                        car.ctrl.controlCenter = (byte)((Int16)64 - (Int16)((100 - car.ctrl.apexEntryIn) * (Int16)cal.apexModError / 128));
                    car.ctrl.apexEntryIn--;
	        }
            else if (car.ctrl.apexExitDown > 0)
		        {
                    if (car.ctrl.apexEntry == 1)
                    car.ctrl.controlCenter = (byte)((Int16)64 + (Int16)(car.ctrl.apexExitDown * (Int16)cal.apexModError / 128));
		        else
                    car.ctrl.controlCenter = (byte)((Int16)64 - (Int16)(car.ctrl.apexExitDown * (Int16)cal.apexModError / 128));
                    car.ctrl.apexExitDown--;
		        }
	        else
		        car.ctrl.controlCenter = (byte)(64);
        }

        void setSpeedTargets()
        {
            if (car.sensor.valid >= 1) //sensor has a valid read
            {
                //set the target open loop velocity
                car.ctrl.targetVelocity = (UInt16)((UInt16)car.speedGain + (UInt16)car.ctrl.straightLearn * 3);
                car.ctrl.biasVelocity = lookupBiasVel(car.ctrl.targetServoPos, car.ctrl.straightLearn);
            }
            else
            {
                car.ctrl.targetVelocity = cal.lostSpeed;
                car.ctrl.biasVelocity = lookupBiasVel(car.ctrl.targetServoPos, car.ctrl.straightLearn);
            }
        }

        void setupBiasTable()
        {
            sbyte i = 41;
            float res;
            float rad;


            while (i <= 81)
            {
                rad = (float)(i - 40) * 10;
                res = -0.0016f * rad * rad - 0.1564f * rad + 493.18f;
                car.adjust.biasVelTable[i] = Convert.ToInt16(res);
                i++;
            }


            i = 39;
            while (i >= 0)
            {
                rad = -(float)(i - 40) * 10;
                res = 1000 - (-0.0016f * rad * rad - 0.1564f * rad + 493.18f);
                car.adjust.biasVelTable[i] = (Int16)res;
                i--;
            }

            car.adjust.biasVelTable[40] = 500;
        }

        Int16 lookupBiasVel(Int16 pwmTarget, UInt16 speedlearn)
        {
            Int16 ret = 0;
            sbyte index = (sbyte)((sbyte)(pwmTarget / 10) + 40);

            if (index < 0) index = 0;
            if (index > 80) index = 80;
            ret = car.adjust.biasVelTable[(byte)index];
            if (speedlearn > 40)
            {
                if (index < 40)
                    ret = Convert.ToInt16(ret * 4 / 3);

                else if (index > 40)
                    ret = Convert.ToInt16(ret * 2 / 3);
            }

            return ret;
        }


        void task_5msec()
        {
            UInt32 motLeft;
            UInt32 motRight;

            //Smoothen the velocity command target from where the App set it
            car.ctrl.velTarget = Convert.ToUInt16(((UInt32)car.ctrl.velTarget * 3 + car.ctrl.targetVelocity) >> 2);

            if (car.ctrl.velTarget > 1400) //limit the applied velocity Target
                car.ctrl.velTarget = 1400;


            //Apply velocities to left and right based off bias
            if (car.ctrl.biasVelocity > 500)
            {
                motLeft = Convert.ToUInt32((car.ctrl.velTarget * 500) / constBiasCenter);
                motRight = Convert.ToUInt32((Convert.ToUInt32(car.ctrl.velTarget) * (constBiasCenter * 2 - Convert.ToUInt32(car.ctrl.biasVelocity))) / constBiasCenter);
            }
            else
            {
                motLeft = Convert.ToUInt32(((UInt32)car.ctrl.velTarget * (UInt32)car.ctrl.biasVelocity) / constBiasCenter);
                motRight = Convert.ToUInt32(((UInt32)car.ctrl.velTarget * 500) / constBiasCenter);
            }

            if (car.ctrl.breakLearn > 0)
            {
                if (car.ctrl.targetServoPos > 100)
                    motRight = 0;
                else
                    motLeft = 0;
                car.ctrl.breakLearn--;
            }


           // vfnSet_Duty_Opwm(6, motLeft);
            //vfnSet_Duty_Opwm(7, motRight);
            //vfnSet_Servo(car.ctrl.targetServoPos);

           // taskUpdateCameraStart();

            //taskCTR_5msec = 0;
        }

    }
}
