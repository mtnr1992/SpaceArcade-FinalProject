//include for keyboard monitoring
#include "windows.h"
#include "toolbox.h"
#include <cvirte.h>		
#include <userint.h>
#include "SpaceArcade.h"
#define  SPEED 5
#define  ENEMIES_PER_ATTACK 10

static int panelHandle;

//global variables--
double x=250,y=250;
int ship_img, backGround_img, MovingStars_img ;//images
int smallEnemies=0, s_r_e ,s_g_e ,s_b_e ,s_o_e ,explotion;//enemies images
int counter=0,s=0,sc=0 ,ec=0,exp_c=0;
//prototypes--
void draw();
void shipLocation();
void getImages();
void shoot();
void enemies();
void drawExplotion(int i);
//bullets
typedef struct{
double x;
double y;
}OBJECT;

OBJECT laser[50];
OBJECT enemie[500];

 //keyboard~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ start
	
				//variables for keyboard monitoring
				int Callback_Data;	
				int Posting_Handle; 
				int Keys_Down[256]={0}; 

				//Receives information from windows regarding key presses
				int CVICALLBACK Key_DownFunc(int panel, int message, unsigned int* wParam, unsigned int* lParam, void* callbackData)
				{
					unsigned int Key = *wParam;
					Keys_Down[Key] = 1;
					return 0;
				}

				//Receives information from windows regarding key releases
				int CVICALLBACK Key_UpFunc(int panel, int message, unsigned int* wParam, unsigned int* lParam, void* callbackData)
				{
					unsigned int Key = *wParam;
					Keys_Down[Key] = 0;
					return 0;
				}

				//asking windows to send keyboard press and release events to our software 
				//Specifing 'Key_DownFunc' and 'Key_UpFunc' as the functions to call
				void Connect_To_Windows_Events(void)
				{
					InstallWinMsgCallback(panelHandle,WM_KEYDOWN,Key_DownFunc,VAL_MODE_IN_QUEUE,&Callback_Data,&Posting_Handle);
					InstallWinMsgCallback(panelHandle,WM_KEYUP,Key_UpFunc,VAL_MODE_IN_QUEUE,&Callback_Data,&Posting_Handle);
				}


//keyboard~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  end

void getImages(){
static int load_discard=0; // 0 = load, 1= discard
	if(load_discard)
		{
				DiscardBitmap (ship_img);
				DiscardBitmap (backGround_img); 
				DiscardBitmap (MovingStars_img);
				
			//enemies
				DiscardBitmap (s_r_e);
				DiscardBitmap (s_g_e);
				DiscardBitmap (s_b_e);
				DiscardBitmap (s_o_e);
				//explotion
				DiscardBitmap(explotion);
		}
	else {
		   		GetBitmapFromFile ("Pics\\spaceShip.png", &ship_img);
		   		GetBitmapFromFile ("Pics\\Backgroung.png", &backGround_img);
		   		GetBitmapFromFile ("Pics\\MovingStars.png", &MovingStars_img);
		   //enemies
				GetBitmapFromFile ("Pics\\S_R_E.png", &s_r_e);
				GetBitmapFromFile ("Pics\\S_G_E.png", &s_g_e);
				GetBitmapFromFile ("Pics\\S_B_E.png", &s_b_e);
				GetBitmapFromFile ("Pics\\S_O_E.png", &s_o_e);
			//explotion sprites
				GetBitmapFromFile ("Pics\\Explotion.png", &explotion); //140x18 _ Steps of 20 along x 
	
		}


}

int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "SpaceArcade.uir", PANEL)) < 0)
		return -1;
	
	
	Connect_To_Windows_Events();  //asking windows to send keyboard press and release events to our software  
	getImages();//load bitmaps
	
	SetCtrlAttribute (panelHandle, PANEL_CANVAS, ATTR_PEN_WIDTH, 2);
	SetCtrlAttribute (panelHandle, PANEL_CANVAS, ATTR_PEN_COLOR, MakeColor(0, 255, 255));
	
	
	
	DisplayPanel (panelHandle);
	RunUserInterface ();
	DiscardPanel (panelHandle);
	return 0;
}

int CVICALLBACK ExitFunc (int panel, int event, void *callbackData,
						  int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			QuitUserInterface (0);
			break;
	}
	return 0;
}

int CVICALLBACK Tick (int panel, int control, int event,
					  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
			counter++;
			shipLocation();
			draw();
			//drawExplotion();
			CanvasEndBatchDraw (panelHandle, PANEL_CANVAS);	  				
			
			//ammo panel update--------------------** create pandel update function
			SetCtrlVal (panelHandle, PANEL_AMMO, 500-s);
			
	}
	return 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~DRAW
void draw(){


			CanvasStartBatchDraw (panelHandle, PANEL_CANVAS);
			CanvasClear (panelHandle, PANEL_CANVAS, VAL_ENTIRE_OBJECT);
		//draw backgroung ---------------------------------------------------
		    CanvasDrawBitmap (panelHandle, PANEL_CANVAS, backGround_img , VAL_ENTIRE_OBJECT, VAL_ENTIRE_OBJECT);
		    CanvasDrawBitmap (panelHandle, PANEL_CANVAS, MovingStars_img , VAL_ENTIRE_OBJECT ,MakeRect((counter)%500,0,500,500) );   //moving stars
		    CanvasDrawBitmap (panelHandle, PANEL_CANVAS, MovingStars_img , VAL_ENTIRE_OBJECT ,MakeRect((counter)%500-500,0,500,500) );//moving stars 
		//draw laser shoot---------------------------------------------------
			if(Keys_Down[17] )
			sc=1;
			
			shoot();
		//draw ship ---------------------------------------------------------
			if(Keys_Down[38]){
				CanvasDrawBitmap (panelHandle, PANEL_CANVAS, ship_img, MakeRect(0,60,61,54), MakeRect(y,x,61,54));	
			}else
				CanvasDrawBitmap (panelHandle, PANEL_CANVAS, ship_img, MakeRect(0,0,61,54), MakeRect(y,x,61,54));
		//enemies------------------------------------------------------------
		
			if(counter>100 && counter%(SPEED*10)==0)
				ec=1;
			enemies();
		//-------------------------------------------------------------------
		

}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~shipLocation
void shipLocation(){
	
	if(Keys_Down[37]) //left
		   x=x-SPEED;
	if(Keys_Down[39]) //right
		   x=x+SPEED;
	if(Keys_Down[38]) //up
		   y=y-SPEED;
	if(Keys_Down[40]) //down
		   y=y+SPEED;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~SHOOT
void shoot(){
		//shoot creation
		if(sc && counter%SPEED==0){
		laser[s].x= x+30;
		laser[s].y= y+27;
		s++;
		sc=0;
		} 
		//draw all shoots     
		for(int i=0;i<s ; i++) {
			if(laser[i].y>0){
			CanvasDrawLine (panelHandle, PANEL_CANVAS,MakePoint(laser[i].x,laser[i].y) ,MakePoint(laser[i].x,laser[i].y+10) );//line_ 2nd
			laser[i].y=laser[i].y-SPEED*2;
			}
		}
		if(s==50)
			s=0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Enemies
void enemies(){
//SMALL ENEMIES VAL NAMES	
	//s_r_e
	//s_g_e
	//s_b_e
	//s_o_e
	
	//small enemies creation
	if(ec && counter%(SPEED*10)==0 && smallEnemies<ENEMIES_PER_ATTACK){ 
		enemie[smallEnemies].x=0;
		enemie[smallEnemies].y=100;
		smallEnemies++; 
		ec=0;
	}
	//draw all small enemies
	for( int i=0 ; i<smallEnemies ; i++ ) {
		/*for(int j=0; j<50 ; j++) //scan enemie vs shoots
		if(enemie[i].y<laser[j].y && enemie[i].y+20 >laser[j].y && enemie[i].x <laser[j].x && enemie[i].x+20 >laser[j].x)//hit
			drawExplotion(i);
		else {*/  
			//}
			CanvasDrawBitmap (panelHandle, PANEL_CANVAS, s_r_e , VAL_ENTIRE_OBJECT, MakeRect(enemie[i].y ,enemie[i].x , 20,20));	
			
		enemie[i].y=enemie[i].y+0.2;
		enemie[i].x++;
	}
}

void drawExplotion(int i){
	
	if(counter%10==0)
	exp_c++;
	CanvasDrawBitmap (panelHandle, PANEL_CANVAS, explotion ,MakeRect((exp_c/4)*128,(exp_c%4)*128,128,128) ,MakeRect(enemie[i].y,enemie[i].x,64,64));
	
	if(exp_c==16)
		exp_c=0;
	
	
	
	}
	














