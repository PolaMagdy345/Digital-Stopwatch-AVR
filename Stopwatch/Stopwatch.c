#include "MemMap.h"
#include "StdTypes.h"
#include "Utils.h"
#include "DIO_Interface.h"
#include "Stopwatch.h"
#include <util/delay.h>



#define SEGMENT_PORT PORTA

char Segment_display(unsigned char num){	char d0,d1;	char SegmentArr[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
	d0=num%10;
	d1=num/10;
	PORTC=0x40;
	SEGMENT_PORT=(SegmentArr[d0]<<1);
	_delay_ms(1);
	PORTC=0x80;
	SEGMENT_PORT=(SegmentArr[d1]<<1);
	_delay_ms(1);
	return 1;
}

void Segment_display_BCD(unsigned char num){
	SEGMENT_PORT=((num/10)<<4)|(num%10);
}
int main(void)
{
	 DDRD=0;
	 DDRC=0xff;
	 DDRA=0xff;
	 PORTD=0x1C;
	 unsigned char Start_Pause=0,Mode=0,B_flag=0,x,n=30;
	 x=n;
	 while (1)
	 {
		if(READ_BIT(PIND,4)==0)
		{
			Mode^=1;
			while(Segment_display(x) && READ_BIT(PIND,4)==0);

		}
		if(READ_BIT(PIND,2)==0 && Mode==0)
		{
			x=n;
			B_flag=0;
			while(Segment_display(x) && READ_BIT(PIND,2)==0);
		}
		if(READ_BIT(PIND,3)==0 && Mode==0)
		{
			Start_Pause^=1;
			while(Segment_display(x) && READ_BIT(PIND,3)==0);
		}
		if(Start_Pause==0 && Mode==0)
		{
			Segment_display(x);
		}
		if(Mode==0)
		{
			if(x && Start_Pause==1)
			{
				for(int i=0;i<=250 && Start_Pause==1;i++)
				{
					if(READ_BIT(PIND,4)==0)
					{
						Mode^=1;
						while(Segment_display(x) && READ_BIT(PIND,4)==0);

					}
					if(READ_BIT(PIND,2)==0)
					{
						x=n;
						B_flag=0;
						while(Segment_display(x) && READ_BIT(PIND,2)==0);
					}
					if(READ_BIT(PIND,3)==0)
					{
						Start_Pause^=1;
						while(Segment_display(x) && READ_BIT(PIND,3)==0);
					}

					Segment_display(x);
				}
				x--;
			}
			if(x==0 && B_flag==0)
			{
				for(int i=0;i<75;i++)
				{
					SET_BIT(PORTC,5);
					_delay_ms(2);
					Segment_display(x);
				}
				CLR_BIT(PORTC,5);
				_delay_ms(30);
				for(int i=0;i<75;i++)
				{
					SET_BIT(PORTC,5);
					_delay_ms(2);
					Segment_display(x);
				}
				CLR_BIT(PORTC,5);
				B_flag=1;
			}
			Segment_display(x);
		}
		else
		{
				x=n;
				Start_Pause=0;
				Segment_display(x);
			if(READ_BIT(PIND,2)==0 && Mode==1)
			{
				n+=10;
				if(n>=100)
				{
					n-=100;
				}
					x=n;
				for(int i=0;i<=100;i++)
				{
					Segment_display(x);
				}
			}
			if(READ_BIT(PIND,3)==0 && Mode==1)
			{
				n++;
				if((n/10)>=10)
				n-=10;
				x=n;
				for(int i=0;i<=100;i++)
				{
					Segment_display(x);
				}
			}
		}
	}	
}
