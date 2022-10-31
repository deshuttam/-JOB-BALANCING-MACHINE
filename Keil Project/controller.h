#include<lpc21xx.h>
#define LCD_RS 0x00000004
#define LCD_RW 0x00000000
#define LCD_EN 0x00000008

void lcd_cmd(unsigned char val);
void lcd_cmd1(unsigned char val);
void lcd_data(unsigned char val);
void lcd_data1(unsigned char val);
void gpio_init(void);
void delay(unsigned int);
void display_num(int);
void display_top(int);
void display_bottom(int);
void disassemble_cmd(unsigned char val);
void disassemble_cmd1(unsigned char val);
void disassemble_data(unsigned char val);
void disassemble_data1(unsigned char val);
void separate_rpm_digits(int val);
int set_number(void);
void separate_temp_digits(int val);
int increment(unsigned char val,int var);
char* itoa(int, int);
int timer_init1(void);
float temp_monitor(void);
void warn_max(void);
void Ext_ISR(void) __irq;

unsigned char warn1[] = "Err! Max limit:";
unsigned char warn2[] = "Err! Low limit:";
unsigned char nib = 0;
int var1 = 0, var2 = 0, var3 = 0, var4 = 0,count2 = 0;
int count1=0;
float temp;
unsigned int i;
char* pt, result[5];

void display_num(int x)
{
	char* r;
	r = itoa(x, 10);
	for(;*r!='\0'; r++)
	disassemble_data(*r);
}

void gpio_init()
{
	PINSEL0 = 0x20000000;
	PINSEL1 = 0x00040001;
	PINSEL2 = 0x0000000F;
	IODIR0 |= 0x000002FC;
	IODIR1 |= 0x02000000;
	IOCLR0 = 0x000002FC;
	IOCLR1 = 0x020F0000;
}

void disassemble_cmd(unsigned char val)
{
	nib = val & 0xf0;
  lcd_cmd(nib);
  nib = val & 0x0f;
  nib = nib << 4; 
  lcd_cmd(nib);
	delay(500);
}

void lcd_cmd(unsigned char val)
{
	IO0PIN = val;
	IOCLR0 |= LCD_RS;
	IOSET0 |= LCD_EN;
	delay(10);
	IOCLR0 |= LCD_EN;
	delay(50000);
}

void disassemble_cmd1(unsigned char val)
{
	nib = val & 0xf0;
  lcd_cmd1(nib);
  nib = val & 0x0f;
  nib = nib << 4; 
  lcd_cmd1(nib);
	delay(500);
}

void lcd_cmd1(unsigned char val)
{
	IO0PIN = val;
	IOCLR0 |= LCD_RS;
	IOSET0 |= LCD_EN;
	delay(50);
	IOCLR0 |= LCD_EN;
	delay(50000);
}

void disassemble_data(unsigned char val)
{
	nib = val & 0xf0;
  lcd_data(nib);
  nib = val & 0x0f;
  nib = nib << 4; 
  lcd_data(nib);
	delay(100);
}

void lcd_data(unsigned char val)
{
	IO0PIN = val;
	IOSET0 |= LCD_RS;
	IOSET0 |= LCD_EN;
	delay(10);
	IOCLR0 |= LCD_EN;
	delay(5000);
}

void disassemble_data1(unsigned char val)
{
	nib = val & 0xf0;
  lcd_data1(nib);
  nib = val & 0x0f;
  nib = nib << 4; 
  lcd_data1(nib);
	delay(100);
}

void lcd_data1(unsigned char val)
{
	IO0PIN = val;
	IOSET0 |= LCD_RS;
	IOSET0 |= LCD_EN;
	delay(10);
	IOCLR0 |= LCD_EN;
	delay(500);
}

void delay(unsigned int value)
{
	unsigned int j;
	for(j=0;j<value;j++);
}

void separate_rpm_digits(int val)
{
	var4 = val%10;
	val = val/10;
	var3 = val%10;
	val = val/10;
  var2 = val%10;
	val = val/10;
	var1 = val%10;
}	

void separate_temp_digits(int val)
{
	var4 = val%10;
	val = val/10;
	var3 = val%10;
	val = val/10;
	var2 = val%10;
	var1 = 0;
}

int increment(unsigned char val, int var)
{
	if(var == 9)
	{
		var = 0;
		disassemble_data('0');
		disassemble_cmd(val);
	}
	else
		var++;
	delay(5000000);
	return var;
}

int set_number()
{
	int num = (var1*1000)+(var2*100)+(var3*10)+(var4*1);
	return num;
}	

char* itoa(int val, int base)
{
	static char buf[32] = {0};
	int i = 30;
	for(; val && i ; --i, val /= base)
	buf[i] = "0123456789abcdef"[val % base];
	return &buf[i+1];
}

int timer_init()
{
	count1 = 0;
	T0PR = 11;
	T0MR0 = 1000000;
	T0TC = 0x0;
	T0TCR = 0x1;
	while(T0TC != T0MR0);
	T0TCR = 0x2;
	return count1;
}

void Ext_ISR(void)__irq
{
	EXTINT = 0x01;							//clear the interrupt
	count1++;
	VICVectAddr=0;                        	//Acknowledge Interrupt
}

int timer_init1(void)
{
	T1PR = 179;
	T1MR0 = 1000000;
	T1TC = 0x0;
	T1TCR = 0x1;
	while(T1TC < T1MR0)
	{
		count2 = 0;
		temp = temp_monitor();
			count2 = timer_init();
			disassemble_cmd(0x8C);
			if(count2 == 0)
			{
				disassemble_data1('0');
				disassemble_data1(' ');
				disassemble_data1(' ');
				disassemble_data1(' ');
			}
			else
			{
				count2 = (count2*59.73) + 243;
				pt = itoa(count2, 10);
				if(count2%100 == count2)
				{
					for(;*pt!='\0'; pt++)
					disassemble_data1(*pt);
					disassemble_data1(' ');
					disassemble_data1(' ');
				}
				else if(count2%1000 == count2)
				{
					for(;*pt!='\0'; pt++)
					disassemble_data1(*pt);
					disassemble_data1(' ');
				}
				else
					for(;*pt!='\0'; pt++)
						disassemble_data1(*pt);
		}
	}
	T1TCR = 0x2;
	return count2;
}

float temp_monitor(void)
{
	unsigned int adc_value = 0;
	AD0CR = 0x01200010;       																		//command register for ADC-AD0.4
		while(!(AD0GDR & (0x80000000))); 
		adc_value = AD0GDR;          																	//reading the ADC value
		adc_value >>=6;	
		adc_value &= 0x000003ff;
		temp = (((float)adc_value/1023.0)*3.3*100);
	disassemble_cmd(0xC6);
	
	
	
	
	
		delay(100000);
	  sprintf(result,"%4.1f",temp);
		for(i=0;result[i]!='\0';i++)
			disassemble_data(result[i]);
		delay(100);
		disassemble_cmd(0xCB);
		disassemble_data(0xDF);																		//for degree character
		disassemble_data('C');
	return temp;
}

void warn_min(void)
{
		//IOSET1 |= 0x02000000;
		disassemble_cmd(0x01);
		disassemble_cmd(0x80);
		for(i=0;warn2[i]!='\0';i++)
			disassemble_data(warn2[i]);
		disassemble_cmd(0xC0);
}

void warn_max(void)
{
	IOSET1 |= 0x02000000;
					disassemble_cmd(0x01);
					disassemble_cmd(0x80);
					for(i=0;warn1[i]!='\0';i++)
						disassemble_data(warn1[i]);
				disassemble_cmd(0xC0);
}


