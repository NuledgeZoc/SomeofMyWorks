#include"smg.h"
#include"temp.h"
//MCU用
u8 code smgduan[16]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};//0-F
u8 smgdispTEMP[8]={0x39,0x63,0};//前两位数码管显示摄氏度
u8 code DU=0x63;
u8 code dp=0x80;
unsigned int alarm=45;
void GETtempDISP()
{
	int temp;
	temp=(int)(tempS*1000);
	smgdispTEMP[7]=0x00;
  smgdispTEMP[6]=smgduan[temp/10000];
  smgdispTEMP[5]=smgduan[temp%10000/1000];
  smgdispTEMP[4]=smgduan[temp%10000%1000/100];
  smgdispTEMP[3]=smgduan[temp%10000%1000%100/10];
  smgdispTEMP[2]=smgduan[temp%10000%1000%100%10];
    
	smgdispTEMP[5]+=0x80;

}
void smgdisp(u8 mode)//mode : 1显示当前读取的温度 2显示设定报警温度 3静默模式
{
	u8 i,tempalarm;
   tempalarm=alarm;
	for(i=0;i<8;i++)
	{
		switch(i)	 //位选，选择点亮的数码管，
		{
		   case(0):
				LSA=0;LSB=0;LSC=0; break;//显示第0位
			case(1):
				LSA=1;LSB=0;LSC=0; break;//显示第1位
			case(2):
				LSA=0;LSB=1;LSC=0; break;//显示第2位
			case(3):	
				LSA=1;LSB=1;LSC=0; break;//显示第3位
			case(4):
				LSA=0;LSB=0;LSC=1; break;//显示第4位
			case(5):
				LSA=1;LSB=0;LSC=1; break;//显示第5位
			case(6):
				LSA=0;LSB=1;LSC=1; break;//显示第6位
			case(7):
				LSA=1;LSB=1;LSC=1; break;//显示第7位	
		}
		switch(mode)
		{
			case 1:
				P0=smgdispTEMP[i];break;//发送段码
			case 2:
      {
          if(i<2) 
          {
            P0=smgdispTEMP[i];
            break;
          }
         else
         {
            P0=smgduan[tempalarm % 10];
            tempalarm/=10;
            if(tempalarm==0) return;
            break;
         }
      }
			case 3:
			{
        P0=dp;
			  break;
      }
		}
		DelayYus(20); 
		P0=0x00;//消隐
	}
}