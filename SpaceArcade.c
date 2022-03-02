//include for keyboard monitoring
#include "windows.h"
#include "toolbox.h"
#include <cvirte.h>		
#include <userint.h>
#include "SpaceArcade.h"

//--------------Panels 
static int panelHandle,panelHandleMain, TopChartPanel,HelpPanel;

//--------------Global variables
int ship_img, backGround_img, MovingStars_img, won, loose ,extra_Ammo , extra_Life, Instraction ;  	//Images
int s_r_e ,s_g_e ,s_b_e ,s_o_e;															//Enemies images
int Boss_lvl1,Boss_lvl2,Boss_lvl3;														//Enemies images
int counter=0 ,S_shoot_Valid=0, load_discard=0, time_i=0, ENEMIES_PER_ATTACK;
int active_small_Enemies=0;
int	active_Boss=0;
int	active_Boss_Shoot=0; 
int	active_Ship_Shoots=0;
int	active_Enemie_Shoots=0;
int	active_Ammo_Life=0; 
int LEVEL=1, laser_Factor=1, Mile_Stone=0, delay_bitween_attacks=500 , flag_extraLife=0,flag_extraAmmo=0,winning=0;

FILE *fp;
char filename[50]="TopCharts.csv", lineBuffer[100];
int ID_Pic=0,HELP_Pic=0;

//--------------Prototypes
void Images_Load_Discard();
void Draw();
void ship_Creation();
void ship_Location();
void ship_Draw();
void shoot();			
void enemies_Creation();	
void hit_Validation();		 
void enemies_Draw();		
void GAME_DATA();
void Top_Chart();
void Loose();
void Win();
void extra_Life_Ammo();
void reset();

 //--------------OBJECTS 
typedef struct{
double x;
double y;
double vx;
double vy;
int hight;
int width;
int SPEED;
int life;
int BMP_index;
}OBJECT;

OBJECT ship_Laser[200];
OBJECT enemie_Laser[200];
OBJECT boss_Laser[200];
OBJECT enemie[60];
OBJECT Boss;
OBJECT Player_Ship;
OBJECT AmmoLife[5];

 //--------------PLAYER 
typedef struct{
	char NAME[40];
	int AMMO;
	int POINTS;
	int LIFE;
}PLAYER;

PLAYER Player;
PLAYER Record[11];

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

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~MAIN
int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "SpaceArcade.uir", PANEL)) < 0)
		return -1;
	if ((panelHandleMain = LoadPanel (0, "SpaceArcade.uir", MainPanel)) < 0)
		return -1;
	if ((TopChartPanel = LoadPanel (0, "SpaceArcade.uir", TOP_CHART)) < 0)
		return -1;
	if ((HelpPanel = LoadPanel (0, "SpaceArcade.uir", ID_PANEL)) < 0)
		return -1;
	
	
	
	srand(time(0));
	
	Connect_To_Windows_Events();  //asking windows to send keyboard press and release events to our software  
	
	
	DisplayPanel (panelHandleMain);
	RunUserInterface ();
	DiscardPanel (panelHandleMain);
	DiscardPanel (panelHandle);
	DiscardPanel (TopChartPanel); 
	return 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Images_Load_Discard
void Images_Load_Discard(){
static int load_discard=0; // 0 = Load, 1= Discard
	if(load_discard)
		{
				DiscardBitmap (ship_img);
				DiscardBitmap (backGround_img); 
				DiscardBitmap (MovingStars_img);
				DiscardBitmap (won); 
				DiscardBitmap (loose);
				DiscardBitmap (extra_Life); 
				DiscardBitmap (extra_Ammo);
				DiscardBitmap (Instraction);
				
			//enemies
				DiscardBitmap (s_r_e); 
				DiscardBitmap (s_g_e); 
				DiscardBitmap (s_b_e); 
				DiscardBitmap (s_o_e); 
				DiscardBitmap (Boss_lvl1);
				DiscardBitmap (Boss_lvl2);
				DiscardBitmap (Boss_lvl3);
				 
		}
	else {
		   		GetBitmapFromFile ("Pics\\spaceShip.png", &ship_img); 				 // 122X54 Total sprites, 61X54 per image
		   		GetBitmapFromFile ("Pics\\Backgroung.png", &backGround_img); 	     //500X500
		   		GetBitmapFromFile ("Pics\\MovingStars.png", &MovingStars_img);		 //500X500
				GetBitmapFromFile ("Pics\\won.png", &won);							 //500X70
				GetBitmapFromFile ("Pics\\loose.png", &loose); 						 //500X70
				GetBitmapFromFile ("Pics\\extra_Life.png", &extra_Life);			 //25X22
				GetBitmapFromFile ("Pics\\extra_Ammo.png", &extra_Ammo);			 //25X25
				GetBitmapFromFile ("Pics\\instraction.png", &Instraction);	 		 //380X178
				
		   //enemies
				GetBitmapFromFile ("Pics\\S_R_E.png", &s_r_e); //20X20  Red
				GetBitmapFromFile ("Pics\\S_G_E.png", &s_g_e); //20X20  Green
				GetBitmapFromFile ("Pics\\S_B_E.png", &s_b_e); //20X20  Blue
				GetBitmapFromFile ("Pics\\S_O_E.png", &s_o_e); //20X20  Orange
				
			//Boss
				GetBitmapFromFile ("Pics\\Boss_lvl1.png", &Boss_lvl1); // 92X77
				GetBitmapFromFile ("Pics\\Boss_lvl2.png", &Boss_lvl2); // 102X66 
				GetBitmapFromFile ("Pics\\Boss_lvl3.png", &Boss_lvl3); // 167X84 

		}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~GAME_DATA
void GAME_DATA(){
	
	if( counter<10 ){	//Initial game values
		
		Player.AMMO =500; 
		Player.LIFE = 50;
		Player.POINTS=10;
											 
	}
	
	SetCtrlVal (panelHandle, PANEL_AMMO, Player.AMMO );
	SetCtrlVal (panelHandle, PANEL_POINTS_BAR,Player.POINTS );
	SetCtrlVal (panelHandle, PANEL_LIFE_BAR,Player.LIFE );
	SetCtrlVal (panelHandle, PANEL_LEVEL,LEVEL);

	if(Player.LIFE<=0)												   	 // zero life 
		Loose();	  
	if (Player.POINTS <=0) 												 // zero points
		Loose();
	 					   
	if(winning==1 && Mile_Stone==9){
		Win(); 
		SetCtrlAttribute (panelHandle, PANEL_TIMER, ATTR_ENABLED, 0);    //stop timer
	}
	
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Top_Chart 
void Top_Chart()  {
   
int i=0, numOfScores;
//read from file and insert to arry-------------------------------
   fp=fopen(filename,"r");
   while(fgets(lineBuffer,99,fp)){
	sscanf(lineBuffer,"%[^,],%d\n",Record[i].NAME,&Record[i].POINTS);
	i++;
   }
   numOfScores=i;
   fclose(fp);
   
//insert the last score to the arry ------------------------------
if(counter>0) {											//insert only after a game
    for( i=numOfScores-1 ; i>=0 && Record[i].POINTS <= Player.POINTS ; i--){   					//run on arry
		Record[i+1] = Record[i];
	}	
	Record[i+1]=Player;			   //insert the new result to the new location
}
//save the file---------------------------------------------------
	fp=fopen(filename,"w");
	for( i=0; i< numOfScores; i++){
		 fprintf(fp,"%s,%d\n",Record[i].NAME,Record[i].POINTS); 
	}
	fclose(fp);	 
//display the results --------------------------------------------
    for( i=1; i<numOfScores+1; i++){ 
	
	SetTableCellVal (TopChartPanel, TOP_CHART_TABLE, MakePoint(1,i),Record[i-1].NAME );   		//insert names 
	SetTableCellVal (TopChartPanel, TOP_CHART_TABLE, MakePoint(2,i),Record[i-1].POINTS );	   	//insert scores 
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Loose
void Loose(){
	CanvasDrawBitmap (panelHandle, PANEL_CANVAS,loose , VAL_ENTIRE_OBJECT ,MakeRect (200, 30, 70, 500) );
	SetCtrlAttribute (panelHandle, PANEL_TIMER, ATTR_ENABLED, 0);    //stop timer 
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Win
void Win(){
	CanvasDrawBitmap (panelHandle, PANEL_CANVAS,won , VAL_ENTIRE_OBJECT ,MakeRect (200, 30, 70, 500) );
	SetCtrlAttribute (panelHandle, PANEL_TIMER, ATTR_ENABLED, 0);    //stop timer 
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~MAIN PANEL
int CVICALLBACK ExitFunc2 (int panel, int event, void *callbackData,
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

int CVICALLBACK START (int panel, int control, int event,
					   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (panelHandleMain, MainPanel_ENTER_NAME, Player.NAME);
			load_discard=0; // 0 = Load, 1= Discard  
			Images_Load_Discard();//Load bitmaps
			ship_Creation ();
			
			SetCtrlAttribute (panelHandle, PANEL_TIMER, ATTR_ENABLED, 1); 	//Start timer
			
			DisplayPanel (panelHandle); 
			break;
	}
	return 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~GAME PANEL

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
			
			SetCtrlAttribute (panelHandle, PANEL_TIMER, ATTR_ENABLED, 0);
			load_discard=1; // 0 = Load, 1= Discard  
			Images_Load_Discard();//Discard bitmaps 
			HidePanel (panelHandle);
			Top_Chart();
			reset();
			break;
	}
	return 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Pause_Continue
int CVICALLBACK Pause_Continue (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			SetCtrlAttribute (panelHandle, PANEL_TIMER, ATTR_ENABLED, time_i%2);
			time_i++;
			break;
	}
	return 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~TIMER FUNCTION
int CVICALLBACK Tick (int panel, int control, int event,
					  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
			counter++;
			
			Draw();
			
			if( active_small_Enemies==0 && active_Boss==0)
			delay_bitween_attacks--;
	}
	return 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Draw
void Draw(){
		
		//Start Batch Draw---------------------------------------------------
			CanvasStartBatchDraw (panelHandle, PANEL_CANVAS);
			CanvasClear (panelHandle, PANEL_CANVAS, VAL_ENTIRE_OBJECT);
			
		//Backgroung ---------------------------------------------------
		    CanvasDrawBitmap (panelHandle, PANEL_CANVAS, backGround_img , VAL_ENTIRE_OBJECT, VAL_ENTIRE_OBJECT);
		    CanvasDrawBitmap (panelHandle, PANEL_CANVAS, MovingStars_img , VAL_ENTIRE_OBJECT
							  						,MakeRect((counter)%500,0,500,500) );    //Moving stars 1
		    CanvasDrawBitmap (panelHandle, PANEL_CANVAS, MovingStars_img , VAL_ENTIRE_OBJECT
							  						,MakeRect((counter)%500-500,0,500,500) );//Moving stars 2
			
		//Instraction-------------------------------------------------------- 
		if (counter<500)
			CanvasDrawBitmap (panelHandle, PANEL_CANVAS, Instraction , VAL_ENTIRE_OBJECT
							  						,MakeRect(100,50,178,384) );
		//Laser Shoot--------------------------------------------------------
			if(Keys_Down[17] && Player.AMMO>0)
				S_shoot_Valid=1;
			shoot();
			
		//Ship --------------------------------------------------------------
			ship_Location();
			ship_Draw();
			
		//Enemies------------------------------------------------------------
			if(active_small_Enemies==0 && active_Boss==0 && delay_bitween_attacks==0 ){
				delay_bitween_attacks=500;
				enemies_Creation();
				Mile_Stone++;
			}
		
		//Hit Validation-----------------------------------------------------
			hit_Validation();
			enemies_Draw();
		 
		//GAME_DATA----------------------------------------------------------			
			GAME_DATA();
	
		//extra_Life_Ammo----------------------------------------------------------	
			extra_Life_Ammo();
			
		//End Batch Draw-----------------------------------------------------
			CanvasEndBatchDraw (panelHandle, PANEL_CANVAS);

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ship_Creation 
void ship_Creation(){

	Player_Ship.x= 220;
	Player_Ship.y= 450;
	Player_Ship.hight=54;
	Player_Ship.width= 61;
	Player_Ship.vx = 5;
	Player_Ship.vy = 2;
	Player_Ship.BMP_index= ship_img;
				  
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ship_Location
void ship_Location(){
	
	if(Keys_Down[37] && Player_Ship.x>=0) 					 //Left
		    Player_Ship.x= Player_Ship.x-Player_Ship.vx;
	if(Keys_Down[39] && Player_Ship.x<=500-Player_Ship.width)//Right
		    Player_Ship.x= Player_Ship.x+Player_Ship.vx;
	if(Keys_Down[38]  && Player_Ship.y>=0)					 //Up
		    Player_Ship.y= Player_Ship.y-Player_Ship.vy;
	if(Keys_Down[40] && Player_Ship.y<=500-Player_Ship.hight)//Down
		    Player_Ship.y= Player_Ship.y+Player_Ship.vy;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ship_Draw 
void ship_Draw() {
	if(Keys_Down[38]){
			CanvasDrawBitmap (panelHandle, PANEL_CANVAS, Player_Ship.BMP_index, MakeRect(0,60,61,54), MakeRect(Player_Ship.y,Player_Ship.x,Player_Ship.hight,Player_Ship.width));	
	}else
			CanvasDrawBitmap (panelHandle, PANEL_CANVAS, Player_Ship.BMP_index, MakeRect(0,0,61,54), MakeRect(Player_Ship.y,Player_Ship.x,Player_Ship.hight,Player_Ship.width));
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~extra_Life_Ammo // flag_extraLife=0,flag_extraAmmo=0 ,active_Ammo_Life
void extra_Life_Ammo(){
int	Start_X_Location;
	
	
 //exra Life creation---------------------------------------------------	
  if(Player.LIFE<20 && flag_extraLife==0){
	flag_extraLife=1;
	Start_X_Location=50+300*(double)rand()/RAND_MAX;
	
	//object 
		AmmoLife[active_Ammo_Life].x=Start_X_Location;
		AmmoLife[active_Ammo_Life].y=-30;
		AmmoLife[active_Ammo_Life].vy=1;
		AmmoLife[active_Ammo_Life].hight=22;
		AmmoLife[active_Ammo_Life].width=25;
		AmmoLife[active_Ammo_Life].BMP_index=extra_Life;
	active_Ammo_Life++;
  }
		
 //exra Ammo creation---------------------------------------------------	
  if(Player.AMMO<100 && flag_extraAmmo==0){
	flag_extraAmmo=1;
	Start_X_Location=50+300*(double)rand()/RAND_MAX;
	
	//object 
		AmmoLife[active_Ammo_Life].x=Start_X_Location;
		AmmoLife[active_Ammo_Life].y=-30;
		AmmoLife[active_Ammo_Life].vy=1;
		AmmoLife[active_Ammo_Life].hight=25;
		AmmoLife[active_Ammo_Life].width=25;
		AmmoLife[active_Ammo_Life].BMP_index=extra_Ammo;
	 active_Ammo_Life++;
  }
//draw all life and ammo objects------------------------------------------------
	 for( int i=0 ; i < active_Ammo_Life ; i++ ) { 
		CanvasDrawBitmap (panelHandle, PANEL_CANVAS, AmmoLife[i].BMP_index , VAL_ENTIRE_OBJECT, 
						  								MakeRect(AmmoLife[i].y ,AmmoLife[i].x , AmmoLife[i].hight, AmmoLife[i].width));
		AmmoLife[i].y = AmmoLife[i].y + AmmoLife[i].vy ;
  }
//Ship hit extra life and ammo
	for( int i=0 ; i<active_Ammo_Life ; i++ ){
		 if(RectIntersection(MakeRect (Player_Ship.y, Player_Ship.x, Player_Ship.hight-10, Player_Ship.width-10),
							MakeRect (AmmoLife[i].y, AmmoLife[i].x, AmmoLife[i].hight, AmmoLife[i].width),NULL)){
	
				AmmoLife[i]=AmmoLife[active_Ammo_Life-1]; 					//Takes the last object and replace with used one 
				active_Ammo_Life--; 										//Arry reduce (delete last object in the arry)
				if(AmmoLife[i].BMP_index==extra_Life)
				Player.LIFE=Player.LIFE+25*LEVEL;							//Increase life
				if(AmmoLife[i].BMP_index==extra_Ammo) {
				Player.AMMO=Player.AMMO+150*LEVEL;							//Increase Ammo
				laser_Factor++;
				}
			}	
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Shoots
void shoot(){

//SHIP SHOOT CREATION------------------------------------------------------- ship_Laser		

	//Shoot creation
	if(S_shoot_Valid && counter%(10/laser_Factor)==0){					   //Delay creation between shoots
	ship_Laser[active_Ship_Shoots].x= Player_Ship.x + 34;				   //X start loction by the ship
	ship_Laser[active_Ship_Shoots].y= Player_Ship.y + Player_Ship.hight/2; //Y start loction by the ship
	ship_Laser[active_Ship_Shoots].hight= 7*laser_Factor; 				   //If ship gain awards- increasing laser length 
	ship_Laser[active_Ship_Shoots].width= 1*laser_Factor; 				   //If ship gain awards- increasing laser width
	ship_Laser[active_Ship_Shoots].vy= 5*laser_Factor; 					   //If ship gain awards- increasing laser speed
	active_Ship_Shoots++;
	Player.AMMO--;
	S_shoot_Valid=0;
	} 
	//Shoots drawing     
		for(int i=0; i< active_Ship_Shoots ; i++) {

			//Attribute for Ship Shoots 
					SetCtrlAttribute (panelHandle, PANEL_CANVAS, ATTR_PEN_WIDTH, 2*laser_Factor);
					SetCtrlAttribute (panelHandle, PANEL_CANVAS, ATTR_PEN_COLOR, MakeColor(0, 255, 255));

			//Canvas limitaion
					if(ship_Laser[i].y + ship_Laser[i].hight <0){
						active_Ship_Shoots--; 								//Arry reduce (delete last object in the arry)  
						ship_Laser[i]=ship_Laser[active_Ship_Shoots];		//Takes the last object and replace with used one
					}
			//Forward location
					ship_Laser[i].y = ship_Laser[i].y - ship_Laser[i].vy;

			//Line drawing 
					CanvasDrawLine (panelHandle, PANEL_CANVAS,MakePoint(ship_Laser[i].x,ship_Laser[i].y) 
													 ,MakePoint(ship_Laser[i].x, ship_Laser[i].y + ship_Laser[i].hight) );
		}

//ENEMIE SHOOT CREATION------------------------------------------------	enemie_Laser	

	//Shoot creation
	for( int i=0; i< active_small_Enemies ; i++){ 							//Take one enemie position each iteration
		if(Player_Ship.x + Player_Ship.width/2 == enemie[i].x+ enemie[i].width/2
		  											&& Player_Ship.y > enemie[i].y + enemie[i].hight && counter%3==0) {  //check position of ship VS enemie  
			enemie_Laser[active_Enemie_Shoots].x= enemie[i].x+ enemie[i].width/2 ; //X start loction by the enemie position
			enemie_Laser[active_Enemie_Shoots].y= enemie[i].y+ enemie[i].hight/2 ; //Y start loction by the enemie position
			enemie_Laser[active_Enemie_Shoots].width= LEVEL;
			enemie_Laser[active_Enemie_Shoots].hight = 3 * LEVEL;
			enemie_Laser[active_Enemie_Shoots].vy=2 * LEVEL;
			active_Enemie_Shoots++; 
		}
	}
	//Shoots drawing  
	for( int i=0; i< active_Enemie_Shoots ; i++){	
	
		//Attribute for Enemie Shoots 
			SetCtrlAttribute (panelHandle, PANEL_CANVAS, ATTR_PEN_WIDTH, 3);
			SetCtrlAttribute (panelHandle, PANEL_CANVAS, ATTR_PEN_COLOR, MakeColor(255, 153, 0));
			
		//Canvas limitaion
			if(enemie_Laser[i].y >=500){
			active_Enemie_Shoots--;						       			//Arry reduce (delete last object in the arry)
			enemie_Laser[i] = enemie_Laser[active_Enemie_Shoots];		//Takes the last object and replace with used one
			} 
			
		//Forward location
			enemie_Laser[i].y = enemie_Laser[i].y + enemie_Laser[i].vy;
			
		//Line drawing 
			CanvasDrawLine (panelHandle, PANEL_CANVAS,MakePoint(enemie_Laser[i].x,enemie_Laser[i].y) 
														 ,MakePoint(enemie_Laser[i].x, enemie_Laser[i].y + enemie_Laser[i].hight) );	
	}
	
//BOSS SHOOT CREATION------------------------------------------------	boss_Laser  active_Boss_Shoot

	//Shoot creation
	if(counter%30==0 && Boss.life>0){					   //Delay creation between shoots
	boss_Laser[active_Boss_Shoot].x= Boss.x + Boss.width/2;		   //X start loction by the ship
	boss_Laser[active_Boss_Shoot].y= Boss.y + Boss.hight/2;		   //Y start loction by the ship
	boss_Laser[active_Boss_Shoot].hight= 7*LEVEL; 				   //If ship gain awards 
	boss_Laser[active_Boss_Shoot].width= 1*LEVEL; 				   //If ship gain awards
	boss_Laser[active_Boss_Shoot].vy= 5*LEVEL; 					   //If ship gain awards
	active_Boss_Shoot++;
	S_shoot_Valid=0;
	} 
	//Shoots drawing     
		for(int i=0; i< active_Boss_Shoot ; i++) {

			//Attribute for Ship Shoots 
					SetCtrlAttribute (panelHandle, PANEL_CANVAS, ATTR_PEN_WIDTH, 2*LEVEL);
					SetCtrlAttribute (panelHandle, PANEL_CANVAS, ATTR_PEN_COLOR, MakeColor(0, 0, 255));

			//Canvas limitaion
					if(boss_Laser[i].y > 500){
						active_Boss_Shoot--; 								//Arry reduce (delete last object in the arry)  
						boss_Laser[i]=boss_Laser[active_Boss_Shoot];		//Takes the last object and replace with used one
					}
			//Forward location
					boss_Laser[i].y = boss_Laser[i].y + boss_Laser[i].vy;

			//Line drawing 
					CanvasDrawLine (panelHandle, PANEL_CANVAS,MakePoint(boss_Laser[i].x,boss_Laser[i].y) 
													 ,MakePoint(boss_Laser[i].x, boss_Laser[i].y + boss_Laser[i].hight) );
		}




}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~hit_Validation
void hit_Validation(){

//Enemies Hits by shoots (increase point)
	for( int i=0 ; i<active_small_Enemies ; i++ ) {						//Take one enemie
		
		for(int j=0; j<active_Ship_Shoots ; j++){ 						//And scan all shoots VS the enemie
		
			if(RectIntersection (MakeRect(enemie[i].y, enemie[i].x, enemie[i].hight, enemie[i].width), 
								 MakeRect(ship_Laser[j].y, ship_Laser[j].x, ship_Laser[j].hight, ship_Laser[j].width),
								 NULL)){
									 
																		//Check live -> if life =0 - remove from arry -kill
																		//            -> if life >0 - decrease life value
				if(enemie[i].life)
					enemie[i].life--;									//Decrease enemies life
				else  													//Kill Enemie
				{
					enemie[i]=enemie[active_small_Enemies-1]; 			//Takes the last object and replace with used one 
					active_small_Enemies--; 							//Arry reduce (delete last object in the arry)
					Player.POINTS++ ;									//Increase points
				}
																		//Delete shoot		
				active_Ship_Shoots--; 									//Arry reduce (delete last object in the arry)  
				ship_Laser[j]=ship_Laser[active_Ship_Shoots];			//Takes the last object and replace with used one
			}
		}
	}
//Ship Hits by enemies shoots (decrease life)
	for (int i=0 ; i<active_Enemie_Shoots;i++){
		
		 if(RectIntersection(MakeRect (Player_Ship.y, Player_Ship.x, Player_Ship.hight, Player_Ship.width),
							MakeRect (enemie_Laser[i].y, enemie_Laser[i].x, enemie_Laser[i].hight, enemie_Laser[i].width),NULL)){
								
				Player.LIFE--;								 			//Takes the last object and replace with used one
				enemie_Laser[i]=enemie_Laser[active_Enemie_Shoots]; 	//Takes the last object and replace with used one
			}	 
	}
	
//Ship Hits by Boss shoots (decrease life) // boss_Laser  active_Boss_Shoot
	for (int i=0 ; i<active_Boss_Shoot;i++){
		
		 if(RectIntersection(MakeRect (Player_Ship.y, Player_Ship.x, Player_Ship.hight, Player_Ship.width),
							MakeRect (boss_Laser[i].y, boss_Laser[i].x, boss_Laser[i].hight, boss_Laser[i].width),NULL)){
								
				Player.LIFE=Player.LIFE-3;								 			//Takes the last object and replace with used one
				boss_Laser[i]=boss_Laser[active_Boss_Shoot]; 	//Takes the last object and replace with used one
			}	 
	}	
	
//Ship Hits by enemies ship (decrease life)
	for( int i=0 ; i<active_small_Enemies ; i++ ){
		 if(RectIntersection(MakeRect (Player_Ship.y, Player_Ship.x, Player_Ship.hight-10, Player_Ship.width-10),
							MakeRect (enemie[i].y, enemie[i].x, enemie[i].hight, enemie[i].width),NULL)){
	
				enemie[i]=enemie[active_small_Enemies-1]; 				//Takes the last object and replace with used one 
				active_small_Enemies--; 								//Arry reduce (delete last object in the arry)				
				Player.LIFE=Player.LIFE-2;								//Decrease life
			}	
	}
//Enemies Hits by wall (decrease points)
	for( int i=0 ; i<active_small_Enemies ; i++ ){
		 if( enemie[i].y>500 ){
	
				enemie[i]=enemie[active_small_Enemies-1]; 				//Takes the last object and replace with used one 
				active_small_Enemies--; 								//Arry reduce (delete last object in the arry)				
				Player.POINTS=Player.POINTS-3 ;							//Decrease points 			
			}	
	}
//Boss------------------------------------------------------------------------
	if(active_Boss)
	for(int i=0; i<active_Ship_Shoots; i++){
		 if(RectIntersection(MakeRect (Boss.y, Boss.x, Boss.hight, Boss.width),
							MakeRect (ship_Laser[i].y, ship_Laser[i].x, ship_Laser[i].hight, ship_Laser[i].width),NULL)){
				
			if(Boss.life)
				Boss.life--;
			else  														//Kill Boss
				{
					active_Boss=0; 										//Takes the last object and replace with used one 
					Player.POINTS++ ;									//Increase points
				}
				active_Ship_Shoots--; 									//Arry reduce (delete last object in the arry)  
				ship_Laser[i]=ship_Laser[active_Ship_Shoots];			//Takes the last object and replace with used one				
				}
		
		
	}
	
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~enemies_Creation
void enemies_Creation(){
	
int Start_Y_Location, BMP_number, i=0;
int Ci=0; //Continues interator
		 ENEMIES_PER_ATTACK=4*LEVEL;
		 flag_extraLife=1;
		 flag_extraAmmo=1;				
	//Small Enemies, creates 20 enemies at ones--------------------------------
		 if(Mile_Stone!=2 && Mile_Stone!=4 && Mile_Stone!=6){
			Start_Y_Location=50+20*(double)rand()/RAND_MAX; 			//Change randomly Y start location 
					   
			for(i; i<ENEMIES_PER_ATTACK ; i++){ 						//Left Side 10
				enemie[i].x=-30-i*30; //Left
				enemie[i].y=Start_Y_Location;
				enemie[i].hight=20;
				enemie[i].width= 20;
				enemie[i].vy=0;
				enemie[i].vx=LEVEL;
				
				BMP_number=rand()%4+1;
				
					switch (BMP_number){ 								//Enemies Variety of colors and life
						case 1:
							enemie[i].BMP_index= s_r_e ;
							enemie[i].life=LEVEL;
							break;
						case 2:
							enemie[i].BMP_index= s_g_e ;
							enemie[i].life=1+LEVEL;
							break;
						case 3:
							enemie[i].BMP_index= s_b_e ;
							enemie[i].life=2+LEVEL;
							break;
						case 4:
							enemie[i].BMP_index= s_o_e ;
							enemie[i].life=2+LEVEL;
							break;
					}
					active_small_Enemies++; 							//Increase per new enemie 
			}
			
			Start_Y_Location=50+20*(double)rand()/RAND_MAX; 			//Change randomly Y start location 
			
			for(i ; i<2*ENEMIES_PER_ATTACK ; i++){ 						//Right Side 10
				enemie[i].x=510+Ci*30;//Right
				enemie[i].y=Start_Y_Location;
				enemie[i].hight= 20;
				enemie[i].width= 20;
				enemie[i].vy= 0;
				enemie[i].vx=-LEVEL;
			
				BMP_number=rand()%4+1;
				
					switch (BMP_number){ 								//Enemies Variety of colors and life
						case 1:
							enemie[i].BMP_index= s_r_e ;
							enemie[i].life=LEVEL;
							break;
						case 2:
							enemie[i].BMP_index= s_g_e ;
							enemie[i].life=1+LEVEL;
							break;
						case 3:
							enemie[i].BMP_index= s_b_e ;
							enemie[i].life=2+LEVEL;
							break;
						case 4:
							enemie[i].BMP_index= s_o_e ;
							enemie[i].life=2+LEVEL;
							break;
					}
					active_small_Enemies++; 							//Increase per new enemie
					Ci++;
			}
		 }	
	//Boss -----------------------------------------------------------------------
		switch (Mile_Stone){
			case 2:							   //boss lvl1
				Boss.x=250-92/2;
				Boss.y=-80;
				Boss.hight=77; 
				Boss.width=92; 
				Boss.vy=0.3; 
				Boss.vx=0;
				Boss.life=40;
				Boss.BMP_index=Boss_lvl1;
				active_Boss++;
				LEVEL=2;
				flag_extraLife=0;
				flag_extraAmmo=0;
				break;
			case 6:							   //boss lvl2
				Boss.x=250-102/2;
				Boss.y=-70;
				Boss.hight=77; 
				Boss.width=102; 
				Boss.vy=0.2; 
				Boss.vx=0;
				Boss.life=60;
				Boss.BMP_index=Boss_lvl2;
				active_Boss++;
				LEVEL=3;
				flag_extraLife=0;
				flag_extraAmmo=0;
				break;						   
			case 8:							   //boss lvl3 
				Boss.x=250-167/2;
				Boss.y=-90;
				Boss.hight=84; 
				Boss.width=167; 
				Boss.vy=0.1; 
				Boss.vx=0;
				Boss.life=100;
				Boss.BMP_index=Boss_lvl3;
				active_Boss++;
				LEVEL=4;
				flag_extraLife=0;
				flag_extraAmmo=0;
				winning=1;
				break;	
			}
	
	
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~enemies_Draw  
void enemies_Draw(){
	
//Small Enemies drawing and movement calculation 
	for( int i=0 ; i < active_small_Enemies ; i++ ) { 
		
		CanvasDrawBitmap (panelHandle, PANEL_CANVAS, enemie[i].BMP_index , VAL_ENTIRE_OBJECT, 
						  								MakeRect(enemie[i].y ,enemie[i].x , enemie[i].hight, enemie[i].width));
		
		enemie[i].x = enemie[i].x + enemie[i].vx;		
																		//Start Y movement when enter canvas 	
		if(enemie[i].x + enemie[i].width >=0 && enemie[i].x<=500)
				enemie[i].vy = 0.2 * LEVEL ;	
				
		enemie[i].y = enemie[i].y + enemie[i].vy ;
	
																		//X direction change by hiting wall
	
																		//Left moving object hit left wall
		if(enemie[i].x <= 0 && enemie[i].vx<0)
		enemie[i].vx = LEVEL;
		
																		//Right moving object hit right wall
		if(enemie[i].x+enemie[i].width >= 500 && enemie[i].vx>0)
		enemie[i].vx = -LEVEL;
	}
//BOSS drawing and movement calculation 
	for( int i=0 ; i < active_Boss ; i++ ) {
		
		 CanvasDrawBitmap (panelHandle, PANEL_CANVAS, Boss.BMP_index , VAL_ENTIRE_OBJECT, 
						  								MakeRect(Boss.y ,Boss.x , Boss.hight, Boss.width));
		 
		 if(Boss.y <=50)
		 Boss.y = Boss.y + Boss.vy ;
		 
		 }
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~TopChartPanel 
int CVICALLBACK exitTOP_CHART (int panel, int event, void *callbackData,
							   int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			 HidePanel (TopChartPanel); 
			break;
	}
	return 0;
}

int CVICALLBACK TOP_CHARTS_BUTTON (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			Top_Chart();
			DisplayPanel(TopChartPanel);
			break;
	}
	return 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~reset
void reset(){
	
	counter=0; 	
	Player.AMMO=0;	
	Player.LIFE=0;	
	Player.POINTS=0;

	active_small_Enemies=0;
	active_Boss=0;
	active_Boss_Shoot=0; 
	active_Ship_Shoots=0;
	active_Enemie_Shoots=0;
	active_Ammo_Life=0; 
	LEVEL=1;
	laser_Factor=1;
	Mile_Stone=0;
	flag_extraLife=0;
	flag_extraAmmo=0;
	
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ABOUT  HELP  DEMO
void CVICALLBACK ABOUT (int menuBar, int menuItem, void *callbackData,
						int panel)
{
	DisplayPanel (HelpPanel);
	
}

void CVICALLBACK HELP (int menuBar, int menuItem, void *callbackData,
					   int panel)
{
	OpenDocumentInDefaultViewer ("companionFile.pdf", VAL_NO_ZOOM);
}

void CVICALLBACK DEMO (int menuBar, int menuItem, void *callbackData,
					   int panel)
{
	OpenDocumentInDefaultViewer ("demonstraion.wmv", VAL_NO_ZOOM);
}

int CVICALLBACK ExitId (int panel, int event, void *callbackData,
						int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			 HidePanel (HelpPanel); 
			break;
	}
	return 0;
}
