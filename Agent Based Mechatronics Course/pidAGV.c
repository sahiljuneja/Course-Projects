#pragma config(Sensor, S1,     rightS,         sensorLightActive)
#pragma config(Sensor, S2,     midS,           sensorLightActive)
#pragma config(Sensor, S3,     leftS,          sensorLightActive)
#pragma config(Motor,  motorA,          rightM,        tmotorNXT, openLoop, encoder)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorC,          leftM,         tmotorNXT, openLoop, encoder)

task main()
{	int Pm = 80;
	int powerLeft = 0;
	int powerRight = 0;
	int error = 0;
	int Kp = 12;
	int Kd = 0;
	int Ki = 0.1;
	int P = 0;
	int I = 0;
	int D = 0;
	int prevError = 0;
	int corr = 0;
	int target;
	int bl,wh,bl1,bl2,wh1,wh2;
	int i1 = 0,i2 = 0,i3 = 0,i4 = 0,i5 = 0;

	wait1Msec(2000);
	bl1 = SensorValue[rightS];
	bl2 = SensorValue[leftS];
	bl = (bl1 + bl2)/2;
	PlaySound(soundBlip);
	wait1Msec(2000);
	wh1 = SensorValue[rightS];
	wh2 = SensorValue[leftS];
	wh = (wh1 + wh2)/2;
	PlaySound(soundBlip);
	wait1Msec(2000);
	target = ((bl + wh)/2);


	while (true)
	{

		if ((SensorValue[leftS]  >= (bl - 4)) && (SensorValue[midS] >= (bl - 4)) && (SensorValue[rightS] >= (bl - 4)))//111
		{
			motor[rightM] = Pm;
			motor[leftM] = Pm;
			i1 = i1 + 1;
		}
		if ((SensorValue[leftS] <= (wh + 4)) && (SensorValue[midS] >= (bl - 4)) && (SensorValue[rightS] <= (wh + 4)))//010
		{
			motor[rightM] = Pm;
			motor[leftM] = Pm;
			i2 = i2 + 1;
		}
		if ((SensorValue[leftS] <= (wh + 4)) && (SensorValue[midS] <= (wh + 4)) && (SensorValue[rightS] <= (wh + 4)))//000
		{
			motor[rightM] = 0;
			motor[leftM] = 0;
			i3 = i3 + 1;
		}
		if ((SensorValue[leftS] >= (bl - 4)) && (SensorValue[midS] >= (bl - 4)) && (SensorValue[rightS] <= (wh + 4)))//Line curving left (110)
			{
				error = bl - target;
				P = error*Kp;
				I = (I + error)*Ki;
				D = (error - prevError)*Kd;
				corr = P + I + D;
				powerLeft = Pm - corr;
				powerRight = Pm + corr;
				motor[rightM] = powerRight;
				motor[leftM] = -powerLeft;
				prevError = error;
				i4 = i4 + 1;
				wait1Msec(5);
			}
		if ((SensorValue[leftS] <= (wh + 4)) && (SensorValue[midS] >= (bl - 4)) && (SensorValue[rightS] >= (bl - 4)))//Line curving right (011)
			{
				error = wh - target;
				P = error*Kp;
				I = (I + error)*Ki;
				D = (error - prevError)*Kd;
				corr = P + I + D;
				powerLeft = Pm - corr;
				powerRight = Pm + corr;
				motor[rightM] = -powerRight;
				motor[leftM] = powerLeft;
				prevError = error;
				i5 = i5 + 1;
				wait1Msec(5);
			}
		nxtDisplayTextLine(1, "i1: %d", i1);
	nxtDisplayTextLine(2, "i2: %d", i2);
	nxtDisplayTextLine(3, "i3: %d", i3);
	nxtDisplayTextLine(4, "i4: %d", i4);
	nxtDisplayTextLine(5, "i5: %d", i5);

}
}
