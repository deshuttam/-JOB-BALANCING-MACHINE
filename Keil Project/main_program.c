#include<stdio.h>
#include<lpc214x.h>
#include "controller.h"

unsigned char cmd1[5] = {0x28, 0x0e, 0x06, 0x01, 0x80};
static int rpm = 1000, max_rpm = 1500, min_rpm = 700, max_temp = 300, min_temp = 10;
static float temp = 0.0;
char* ch;

unsigned char message1[] = "Current RPM:";
unsigned char message2[] = "Temp: ";
unsigned char message3[] = "1.RPM";
unsigned char message4[] = "2.Temperature";
unsigned char message5[] = "Max: ";
unsigned char message6[] = "Min: ";
unsigned char warn3[] = "Error: limits";


int main()
{
	unsigned int i,flag=100,flag1=2,mark = 0,count=0;
	unsigned int  switchStatus1,switchStatus2,switchStatus3,switchStatus4;
	
		
  EXTMODE	=0x01;                						//edge i.e falling egge trigger and active low
 	EXTPOLAR= 0X00;
 	VICVectAddr0 = (unsigned long)Ext_ISR;    //Assign the EINT0 ISR function 
 	VICVectCntl0 = 0x20 | 14;      						//Assign the VIC channel EINT0 to interrupt priority 0
 	VICIntEnable |= 0x00004000;     
	
	gpio_init();
	
	delay(3200);
	lcd_cmd(0x30);
	lcd_cmd(0x20);
	
	for(i=0; i<5; i++)
	disassemble_cmd(cmd1[i]);
	delay(3200);
	
	for(i=0; message1[i] != '\0'; i++)
	disassemble_data(message1[i]);
	count = timer_init();
	count = (count*59.73) + 243;
	disassemble_cmd(0x8C);
	ch = itoa(count, 10);
	for(;*ch!='\0'; ch++)
	disassemble_data1(*ch);
	
	disassemble_cmd(0xc0);
	
	for(i=0;message2[i]!='\0';i++)
	disassemble_data(message2[i]);
	display_num(temp);
	flag = 0;
	
	while(1)
	{
		switchStatus1 = (IOPIN1>>16) & 0x01;
		switchStatus2 = (IOPIN1>>17) & 0x01;
		switchStatus3 = (IOPIN1>>18) & 0x01;
		switchStatus4 = (IOPIN1>>19) & 0x01;
				
		if(flag == 11 && switchStatus3 == 0)
		{
			flag = 0;
			disassemble_cmd(0x01);
			disassemble_cmd(0x80);
			for(i=0; message1[i] != '\0'; i++)
				disassemble_data(message1[i]);
			
			disassemble_cmd(0xc0);
			
			for(i=0;message2[i]!='\0';i++)
				disassemble_data(message2[i]);
			continue;
		}
		
		if(switchStatus1 == 0xFF)
		{
			delay(10);
			 if(flag == 0)
			 {
				disassemble_cmd(0x01);
			
				disassemble_cmd(0x80);
				for(i=0; message3[i] != '\0'; i++)
				disassemble_data(message3[i]);
	
				disassemble_cmd(0xc0);
				for(i=0;message4[i]!='\0';i++)
				disassemble_data(message4[i]);
				flag = 1;
				delay(5000);
			 }
			 
			else if((flag == 1) && (flag1 == 0))
			 {
				disassemble_cmd(0x01);
			
				disassemble_cmd(0x80);
				for(i=0; message5[i] != '\0'; i++)
				disassemble_data(message5[i]);
				 if(max_rpm%1000 == max_rpm)
				{
					disassemble_data('0');
					display_num(max_rpm);
				}
				else
					display_num(max_rpm);
	
				disassemble_cmd(0xc0);
				for(i=0;message6[i]!='\0';i++)
				disassemble_data(message6[i]);
				 if(min_rpm%1000 == min_rpm)
				{
					disassemble_data('0');
					display_num(min_rpm);
				}
				else
					display_num(min_rpm);
		
				flag = 2;
				flag1 = 2;
				delay(50000);
			 }
			 
			 else if((flag == 1) && (flag1 == 1))
			 {
				disassemble_cmd(0x01);
			
				disassemble_cmd(0x80);
				for(i=0; message5[i] != '\0'; i++)
				disassemble_data(message5[i]);
				 if(max_temp%100 == max_temp)
				 {
					 disassemble_data('0');
					 display_num(max_temp);
				 }
				 else 
						display_num(max_temp);
		
				disassemble_cmd(0xc0);
				for(i=0;message6[i]!='\0';i++)
				disassemble_data(message6[i]);
				 if(min_temp%10 == min_temp)
				 {
					 disassemble_data('0');
					 display_num(min_temp);
				 }
				 else 
						display_num(min_temp);

				 flag = 3;
				 flag1 = 2;
				 delay(50000);
			 }

				else if((flag1 == 0 && flag == 2))
				{
					disassemble_cmd(0x85);
					mark = 1;
					flag = 4;
					flag1 = 2;
					delay(5000);
				}
				else if((flag1 == 1 && flag == 2))
				{
					disassemble_cmd(0xC5);
					mark = 1;
					flag = 5;
					flag1 = 2;
					delay(5000);
				}
				else if((flag1 == 0 && flag == 3))
				{
					disassemble_cmd(0x85);
					mark = 1;
					flag = 6;
					flag1 = 2;
					delay(5000);
				}
				else if((flag1 == 1 && flag == 3))
				{
					disassemble_cmd(0xC5);
					mark = 1;
					flag = 7;
					flag1 = 2;
					delay(5000);
				} 
				
				delay(50000);
		}
	
		 
	if(switchStatus2 == 0)
	{
		delay(1000);
			if(flag == 1 || flag == 2 || flag == 3)
			{
				disassemble_cmd(0x80);
				flag1 = 0;
			}
			else if(flag == 4)
			{
				separate_rpm_digits(max_rpm); 
				switch(mark)
				{
					case 1: var1 = increment(0x85,var1);
									display_num(var1);
									disassemble_cmd(0x85);
									break;
					case 2: var2 = increment(0x86,var2);
									display_num(var2);
									disassemble_cmd(0x86);
									break;
					case 3: var3 = increment(0x87,var3);
									display_num(var3);
						      disassemble_cmd(0x87);
									break;
					case 4: var4 = increment(0x88,var4);
									display_num(var4);
									disassemble_cmd(0x88);
									break;
				}
				max_rpm = set_number();
				delay(500000);
			}
			
			else if(flag == 5)
			{
				separate_rpm_digits(min_rpm); 
				switch(mark)
				{
					case 1: var1 = increment(0xC5,var1);
									display_num(var1);
									disassemble_cmd(0xC5);
									break;
					case 2: var2 = increment(0xC6,var2);
									display_num(var2);
									disassemble_cmd(0xC6);
									break;
					case 3: var3 = increment(0xC7,var3);
									display_num(var3);
									disassemble_cmd(0xC7);
									break;
					case 4: var4 = increment(0xC8,var4);
									display_num(var4);
									disassemble_cmd(0xC8);
									break;
				}
				min_rpm = set_number();
				delay(500000);
			}
			
			else if(flag == 6)
			{
				separate_temp_digits(max_temp); 
				switch(mark)
				{
					case 1: var2 = increment(0x85,var2);
									display_num(var2);
									disassemble_cmd(0x85);
									break;
					case 2: var3 = increment(0x86,var3);
									display_num(var3);
									disassemble_cmd(0x86);
									break;
					case 3: var4 = increment(0x87,var4);
									display_num(var4);
									disassemble_cmd(0x87);
									break;
				}
				max_temp = set_number();
				delay(500000);
			}
			
			else if(flag == 7)
			{
				separate_temp_digits(min_temp); 
				switch(mark)
				{
					case 1: var3 = increment(0xC5,var3);
									display_num(var3);
									disassemble_cmd(0xC5);
									break;
					case 2: var4 = increment(0xC6,var4);
									display_num(var4);
									disassemble_cmd(0xC6);
									break;
				}
				min_temp = set_number();
				delay(500000);
			}
			delay(50000);
		}
		
		
		if(switchStatus3 == 0)
		{
			delay(500000);
			if(flag == 1 || flag == 10)
			{
				disassemble_cmd(0x01);
				
				disassemble_cmd(0x80);
				for(i=0; message1[i] != '\0'; i++)
				disassemble_data(message1[i]);
				count = timer_init();
				count = (count*59.73) + 243;
				ch = itoa(count, 10);
				for(;*ch!='\0'; ch++)
				disassemble_data1(*ch);
	
				disassemble_cmd(0xc0);
				for(i=0;message2[i]!='\0';i++)
				disassemble_data(message2[i]);
				display_num(temp);
				flag = 0;
				flag1 = 2;
				delay(50000);
			}
			
			else if(flag == 2 || flag == 3)
			{
				delay(500000);
				disassemble_cmd(0x01);
				
				disassemble_cmd(0x80);
				for(i=0; message3[i] != '\0'; i++)
				disassemble_data(message3[i]);
	
				disassemble_cmd(0xc0);
				for(i=0;message4[i]!='\0';i++)
				disassemble_data(message4[i]);
				flag = 1;
				flag1 = 2;
				delay(50000);
			}
			
			else if(flag == 4 || flag == 5 || flag == 8)
			{
				delay(5000000);
				disassemble_cmd(0x01);
				
				if((max_rpm > min_rpm)|| (flag == 8))
			  {
					disassemble_cmd(0x80);
					for(i=0; message5[i] != '\0'; i++)
					disassemble_data(message5[i]);
					if(max_rpm%1000 == max_rpm)
					{
						disassemble_data('0');
						display_num(max_rpm);
					}
					else
						display_num(max_rpm);
	
					disassemble_cmd(0xc0);
					for(i=0;message6[i]!='\0';i++)
					disassemble_data(message6[i]);
					if(min_rpm%1000 == min_rpm)
					{
						disassemble_data('0');
						display_num(min_rpm);
					}
					else
						display_num(min_rpm);
					flag = 2;
					mark = 0;
					flag1 = 2;
					delay(50000);
				}
				
				else
				{
					disassemble_cmd(0x80);
					for(i=0;warn3[i]!='\0';i++)
					disassemble_data(warn3[i]);
					flag = 8;
					mark = 0;
					flag1 = 2;
					delay(50000);
				}
			
			}
			
			else if(flag == 6 || flag == 7 || flag == 9)
			{
				delay(5000000);
				disassemble_cmd(0x01);
				
				if((max_temp > min_temp)|| (flag == 9))
			  {
					disassemble_cmd(0x80);
					for(i=0; message5[i] != '\0'; i++)
					disassemble_data(message5[i]);
					if(max_temp%100 == max_temp)
					{
						disassemble_data('0');
						display_num(max_temp);
					}
					else 
						display_num(max_temp);
	
				disassemble_cmd(0xc0);
				for(i=0;message6[i]!='\0';i++)
				disassemble_data(message6[i]);
				 if(min_temp%10 == min_temp)
				 {
					 disassemble_data('0');
					 display_num(min_temp);
				 }
				 else 
						display_num(min_temp);
				flag = 3;
				mark = 0;
				 flag1 = 2;
				 delay(5000);
			 }
				
			 else
				{
					disassemble_cmd(0x80);
					for(i=0;warn3[i]!='\0';i++)
					disassemble_data(warn3[i]);
					flag = 9;
					mark = 0;
					flag1 = 2;
					delay(50000);
				}
				 
			}	
			delay(50000);
	  }
	
		
		if(switchStatus4 == 0)
		{	
			delay(500000);
			if(flag == 4)
			{
				delay(5000);
				if(mark == 1)
				{
					disassemble_cmd1(0x86);
					mark = 2;
					delay(5000000);
				}
				else if(mark == 2)
				{
					disassemble_cmd1(0x87);
					mark = 3;
					delay(5000000);
				}
				else if(mark == 3)
				{
					disassemble_cmd1(0x88);
					mark = 4;
					delay(5000000);
				}
				else
				{
					disassemble_cmd1(0x85);
					mark = 1;
					delay(5000000);
				}
			}
			
			else if(flag == 6)
			{
				delay(50000);
				if(mark == 1)
				{
					disassemble_cmd1(0x86);
					mark = 2;
					delay(5000000);
				}
				else if(mark == 2)
				{
					disassemble_cmd1(0x87);
					mark = 3;
					delay(5000000);
				}
				else if(mark == 3)
				{
					disassemble_cmd1(0x85);
					mark = 1;
					delay(5000000);
				}
			}
			
			else if(flag == 5)
			{
				delay(50000);
				if(mark == 4)
				{
					disassemble_cmd1(0xC5);
					mark = 1;
					delay(5000000);
			  }
				else if(mark == 1)
				{
					disassemble_cmd1(0xC6);
					mark = 2;
					delay(5000000);
				}
				else if(mark == 2)
				{
					disassemble_cmd1(0xC7);
					mark = 3;
					delay(5000000);
				}
				else if(mark == 3)
				{
					disassemble_cmd1(0xC8);
					mark = 4;
					delay(5000000);
				}
			}
			
			else if(flag == 7)
			{
				delay(50000);
				if(mark == 1)
				{
					disassemble_cmd1(0xC6);
					mark = 2;
					delay(5000000);
				}
				else if(mark == 2)
				{
					disassemble_cmd1(0xC5);
					mark = 1;
					delay(5000000);
				}
			}
			
			else if(flag == 1 || flag == 2 || flag == 3)
			{
				disassemble_cmd(0xC0);
				flag1 = 1;
				delay(5000);
			}
			
			delay(50000);
	  }
		
		if(flag == 0)
		{
			//RPM Monitoring
			count = 0;
			count = timer_init();
			disassemble_cmd(0x8C);
			
			if(count == 0)
			{
				disassemble_data1('0');
				disassemble_data1(' ');
				disassemble_data1(' ');
				disassemble_data1(' ');
			}
			else
			{
				count = (count*59.73) + 243;
				ch = itoa(count, 10);
				if(count%100 == count)
				{
					for(;*ch!='\0'; ch++)
					disassemble_data1(*ch);
					disassemble_data1(' ');
					disassemble_data1(' ');
				}
				else if(count%1000 == count)
				{
					for(;*ch!='\0'; ch++)
					disassemble_data1(*ch);
					disassemble_data1(' ');
				}
				else
					for(;*ch!='\0'; ch++)
					disassemble_data1(*ch);
			}
			
			if(count > max_rpm)
			{
				count = timer_init1();
				if(count > max_rpm)
				{
					warn_max();
					display_num(count);
					flag = 11;
					continue;
				}
			}
			
			else if((count < min_rpm) && (count != 0))
			{
				count = timer_init1();
				if((count < min_rpm) && (count != 0))
				{
					warn_min();
					display_num(count);
					flag = 10;
					continue;
				}
			}
			
			else
				IOCLR1 |= 0x02000000;
			
		//Temperature Monitoring
		temp = temp_monitor();
		
			
			if(temp > max_temp)
			{
				warn_max();		
				display_num(temp);
				flag = 11;
				continue;
			}
			else if(temp < min_temp) 
				{
					warn_min();
					display_num(temp);
					flag = 10;
					continue;
				}
				
				else
				IOCLR1 |= 0x02000000;
			}
			
		}
	}

