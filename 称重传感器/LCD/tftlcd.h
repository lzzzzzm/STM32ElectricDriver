#ifndef __LCD_H
#define __LCD_H		
//#include "sys.h"	 
#include "stdlib.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//YTCE STM32开发板
//2.8寸/3.5寸/4.3寸/7寸 TFT液晶驱动	  
//支持驱动IC型号包括:ILI9341/NT35310/NT35510/SSD1963等		    
//原野数码电子@YTCE
//技术论坛:www.ytce.com
//////////////////////////////////////////////////////////////////////////////////	 
#include "gpio.h"
#include "main.h"

//LCD重要参数集
typedef struct  
{		 	 
	u16 width;			//TFTLCD 宽度
	u16 height;			//TFTLCD 高度
	u16 id;				//TFTLCD ID
	u8  dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。	
	u16	wramcmd;		//开始写gram指令
	u16 setxcmd;		//设置x坐标指令
	u16 setycmd;		//设置y坐标指令 
}_lcd_dev; 	  

//TFTLCD参数
extern _lcd_dev lcddev;	//管理LCD重要参数
//LCD的画笔颜色和背景色	   
extern u32  POINT_COLOR;//默认红色    
extern u32  BACK_COLOR; //背景颜色.默认为白色

//////////////////////////////////////////////////////////////////////////////////	 
//-----------------MCU屏 TFTLCD端口定义----------------   
//LCD地址结构体
typedef struct
{
	vu16 LCD_REG;
	vu16 LCD_RAM;
} LCD_TypeDef;
//使用NOR/SRAM的 Bank1.sector1,地址位HADDR[27,26]=00 A18作为数据命令区分线 
//注意设置时STM32内部会右移一位对其!  			    
#define LCD_BASE        ((u32)(0x60000000 | 0x0007FFFE))
#define TFTLCD             ((LCD_TypeDef *) LCD_BASE)
//////////////////////////////////////////////////////////////////////////////////
	 
//扫描方向定义
#define L2R_U2D  0 		//从左到右,从上到下
#define L2R_D2U  1 		//从左到右,从下到上
#define R2L_U2D  2 		//从右到左,从上到下
#define R2L_D2U  3 		//从右到左,从下到上
#define U2D_L2R  4 		//从上到下,从左到右
#define U2D_R2L  5 		//从上到下,从右到左
#define D2U_L2R  6 		//从下到上,从左到右
#define D2U_R2L  7		//从下到上,从右到左	 

#define DFT_SCAN_DIR  L2R_U2D  //默认的扫描方向

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	   0x001F  
#define BRED             0XF81F
#define GRED 			       0XFFE0
#define GBLUE			       0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			     0XBC40 //棕色
#define BRRED 			     0XFC07 //棕红色
#define GRAY  			     0X8430 //灰色
//GUI颜色

#define DARKBLUE      	   0X01CF	//深蓝色
#define LIGHTBLUE      	   0X7D7C	//浅蓝色  
#define GRAYBLUE       	   0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	   0X841F //浅绿色 
#define LGRAY 			       0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE          0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE             0X2B12 //浅棕蓝色(选择条目的反色)
	    															  
void TFTLCD_Init(void);													   	//初始化
void TFTLCD_ON(void);													      //开显示
void TFTLCD_OFF(void);													    //关显示
void TFTLCD_Clear(u32 Color);	 											//清屏
void TFTLCD_SetCursor(u16 Xpos, u16 Ypos);					//设置光标
void TFTLCD_DrawPoint(u16 x,u16 y);									//画点
void TFTLCD_FDrawPoint(u16 x,u16 y,u32 color);			//快速画点
u32  TFTLCD_ReadPoint(u16 x,u16 y); 										//读点 
void TFTLCD_Draw_Circle(u16 x0,u16 y0,u8 r);						//画圆
void TFTLCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);							//画线
void TFTLCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);		   				//画矩形
void TFTLCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u32 color);		   				//填充单色
void TFTLCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color);				//填充指定颜色
void TFTLCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode);						//显示一个字符
void TFTLCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);  						//显示一个数字
void TFTLCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);				//显示 数字
void TFTLCD_ShowStr(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p);		//显示一个字符串,12/16字体
void TFTLCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue);
u16 TFTLCD_ReadReg(u16 LCD_Reg);
void TFTLCD_WriteRAM_Prepare(void);
void TFTLCD_WriteRAM(u16 RGB_Code);
void TFTLCD_SSD_BackLightSet(u8 pwm);							//SSD1963 背光控制
void TFTLCD_Scan_Dir(u8 dir);									//设置屏扫描方向
void TFTLCD_Display_Dir(u8 dir);								//设置屏幕显示方向
void TFTLCD_Set_Window(u16 sx,u16 sy,u16 width,u16 height);	//设置窗口					   						   																			 

#endif  
	 
	 



