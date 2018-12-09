/*
|****************************************************************************
|****************************************************************************

* Project                                         :<Funny Snake #1>

* Programm name                                   :funny_snake11.c

* Author                                          :Shults1981

* Data create                                     :10/10/2018

* Purpose                                         :classcal game Snake;
 						   testing librery gtk 

|***************************************************************************
|***************************************************************************
*/




#include <stdio.h>
#include <stdlib.h>
#include<gtk/gtk.h>
#include <signal.h>
#include <sys/time.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>



//---------------------------------------------------------------
#define border_x_min (X_max-9*X_max/10)
#define border_x_max (X_max-1*X_max/10)
#define border_y_min (Y_max-9*Y_max/10)
#define border_y_max (Y_max-1*Y_max/10)

#define NumNextLevelJump 6

typedef struct _point
{
	int _x; // coord X
	int _y; // coord Y
	int _d; // moving direction of the point
} point;

typedef struct _unit
{
	point* cord;  
	point* tpa;
	int len;
	int num_tpa;
	
} Unit;


typedef enum _game_status {game_exit=0, game_menu, game_on, game_over,game_next_level} GameStatus;




//----------------------define global varablre -------------------

Unit *Snake, *Rabbit;
point *snake_body_frame;
static cairo_surface_t *surface = NULL;

//-- WINDOW *MainMenu, *tuneMenu;


static int Y_max,X_max;
static int move_flag;
static int rabbitInFild;
static int Score;
static char str_BUF1[5],str_BUF2[5];
static int RabbitWasEaten; 
static GameStatus GST;
static int Level;
static int GameImpuls=0;
static int ImpulsFront=0;
static int Watchdog=0;



//------------------ declaretion  handlers and functions -------------------------
void gti(int);
void snake_body_manage();
void SnakeMoveToOneStep(int mv_flag,int kill_self_flag);
void snake_control(int);
void RabbitFactory(void);
void addNewElementInBackOfSnakeBody(point** Arr, int* len);
void addNewElementInBackOfArr(point** Arr,int* len, point source);
void delElementFromBackOfArr( point** Arr, int* len);
void delElementFromBeginOfArr (point** Arr, int* len);
void copy_body_frame(point * original,point** copy, int len);

static void activate (GtkApplication* app, gpointer user_data);
static void close_window(void);
static gboolean render_(GtkWidget *widget,cairo_t *cr,gpointer udata);
static void key_mon(GtkWidget *widget,GdkEvent *event,gpointer udata);
static gboolean _GameTic_ (gpointer data);
int Game(int state);

void CreateGameFild();
int InitUnits();
int DestroyUnits();
void gameMenuOpen();
void gameMenuClose();



//------------------ define  handlers ----------------------------------
void gti (int signo)
{
	GameImpuls++;		
}


void snake_control (int ch)
{
	switch(ch)
	{
		case GDK_KEY_Left:
			if (Snake->len==1)
				move_flag=1;
			else
				if ((Snake->len>1)&&(!(Snake->cord[0]._d==2)))
					move_flag=1;
			g_print("Pressed key -Left key-\n");
			break;
			
		case GDK_KEY_Right:
			if (Snake->len==1)
				move_flag=2;
			else
				if ((Snake->len>1)&&(!(Snake->cord[0]._d==1)))
					move_flag=2;
			g_print("Pressed key -Right key-\n");
			break;

		case GDK_KEY_Up:
			if (Snake->len==1)
				move_flag=3;
			else			
				if ((Snake->len>1)&&(!(Snake->cord[0]._d==4)))
					move_flag=3;	
			g_print("Pressed key -Up key-\n");
			break;

		case GDK_KEY_Down:	
			if (Snake->len==1)
				move_flag=4;
			else			
				if ((Snake->len>1)&&(!(Snake->cord[0]._d==3)))
					move_flag=4;
			g_print("Pressed key -Down key-\n");
			break; 
		
		default : break;
	}
}




void RabbitFactory(void)
{
if (GST==game_on)
 {
	RabbitWasEaten=0;
	if (!rabbitInFild)
	{
		Rabbit->cord->_x=border_x_min+rand()%((border_x_max-border_x_min)-1)+1;
		Rabbit->cord->_y=border_y_min+rand()%((border_y_max-border_y_min)-1)+1;
		rabbitInFild=1;
	}
	else 
	{
		 if ((Snake->cord[0]._y==Rabbit->cord->_y)&&(Snake->cord[0]._x==Rabbit->cord->_x))
			{
				RabbitWasEaten=1;
				rabbitInFild=0;
	 			Score++;
			}
	}
 }

}


void snake_body_manage()
{
	if (RabbitWasEaten)
	{
		addNewElementInBackOfSnakeBody(&Snake->cord, &Snake->len);
	}

}

void SnakeMoveToOneStep(int mv_flag,int kill_self_flag)
{
 
 int i,j,k,turn_flag,self_closure_flag=0;
 unsigned int kill_self=0,border_crash=0;


if (GST==game_on)
{

	border_crash=!((Snake->cord[0]._x>border_x_min)&&(Snake->cord[0]._x<border_x_max)&&(Snake->cord[0]._y>border_y_min)&&(Snake->cord[0]._y<border_y_max));
	if(kill_self_flag){
		for (i=1;i<Snake->len;i++)
			if((Snake->cord[0]._x==Snake->cord[i]._x)&&(Snake->cord[0]._y==Snake->cord[i]._y))
				kill_self=1;
	}

	snake_body_manage();
	if (Snake->cord[0]._d!=mv_flag)
	{
		Snake->cord[0]._d=mv_flag;
		if ((Snake->len>1)/*&&((Snake->num_tpa)<=(Snake->len-1))*/)
			addNewElementInBackOfArr(&Snake->tpa,&Snake->num_tpa, Snake->cord[0]);
	}
	else
	{
		self_closure_flag=0;
		i=Snake->num_tpa-1;
		while(i>=0 && (!self_closure_flag)){
			if((Snake->cord[0]._x==Snake->tpa[i]._x)&& (Snake->cord[0]._y==Snake->tpa[i]._y)){
				addNewElementInBackOfArr(&Snake->tpa,&Snake->num_tpa,Snake->cord[0]);
				self_closure_flag=1;
			}
			i-=1;
		}
	}
	
	if (k=Snake->num_tpa)
		k-=1;
	
 	for (i=0;i<Snake->len;i++)
 	{
		turn_flag=0;
		if (Snake->num_tpa){
			j=k;
			while((j>=0)&&(!turn_flag))
			{
				if ((Snake->cord[i]._x==Snake->tpa[j]._x)&&(Snake->cord[i]._y==Snake->tpa[j]._y))
				{
					switch(Snake->tpa[j]._d)
					{
					case 1:
						Snake->cord[i]._x--;
						break;
					case 2:
						Snake->cord[i]._x++;
						break;
					case 3:
						Snake->cord[i]._y--;
						break;
					case 4:
						Snake->cord[i]._y++;
						break;
					default:break;
					}
				
					Snake->cord[i]._d=Snake->tpa[j]._d;
					if ((i==Snake->len-1)&&(j==0))
						delElementFromBeginOfArr(&Snake->tpa, &Snake->num_tpa);
					turn_flag=1;
					k-=1;
				}
				j-=1;
			}
		}
		
			if (!turn_flag)
			{
				switch(Snake->cord[i]._d)
				{
				case 1:
					Snake->cord[i]._x--;
					break;
				case 2:
					Snake->cord[i]._x++;
					break;
				case 3:
					Snake->cord[i]._y--;
					break;
				case 4:
					Snake->cord[i]._y++;
					break;
				default:break;
				}
			}	
 	}
/*	


*/

	if (border_crash||kill_self){
		GST=game_over;
	}
	else{	}

 }

}



void  addNewElementInBackOfSnakeBody ( point** Arr, int* len)
{
	int i;
	point *tVar1,*tVar2;

	(*len)=(*len)+1;

	
	tVar1=(point*)malloc(sizeof(point)*(*len));
	
	for (i=0;i<(*len-1);i++)
	{
                tVar1[i]._x=(*Arr)[i]._x;
		tVar1[i]._y=(*Arr)[i]._y;
		tVar1[i]._d=(*Arr)[i]._d;
	}
	
	switch(tVar1[*len-2]._d)
	{
	case 1:
		tVar1[*len-1]._x=(tVar1[*len-2]._x)+1;
		tVar1[*len-1]._y=tVar1[*len-2]._y;
		tVar1[*len-1]._d=tVar1[*len-2]._d;
		break;
	case 2:
		tVar1[*len-1]._x=(tVar1[*len-2]._x)-1;
		tVar1[*len-1]._y=tVar1[*len-2]._y;
		tVar1[*len-1]._d=tVar1[*len-2]._d;
		break;
	case 3:
		tVar1[*len-1]._x=tVar1[*len-2]._x;
		tVar1[*len-1]._y=(tVar1[*len-2]._y)+1;
		tVar1[*len-1]._d=tVar1[*len-2]._d;
		break;
	case 4:
		tVar1[*len-1]._x=tVar1[*len-2]._x;
		tVar1[*len-1]._y=(tVar1[*len-2]._y)-1;
		tVar1[*len-1]._d=tVar1[*len-2]._d;
		break;
	default:
		break;
	}

	tVar2=*Arr;
	*Arr=tVar1;
	tVar1=tVar2;	

	free(tVar1);
}

void  addNewElementInBackOfArr ( point** Arr, int* len, point source)
{
	int i;
	point *tVar1,*tVar2;

	(*len)=(*len)+1;

	
	tVar1=(point*)malloc(sizeof(point)*(*len));

	if (Arr==NULL)
	{
		tVar1->_x=source._x;
		tVar1->_y=source._y;
		tVar1->_d=source._d;
	}
	else
	{
		for (i=0;i<(*len-1);i++)
		{
                	tVar1[i]._x=(*Arr)[i]._x;
			tVar1[i]._y=(*Arr)[i]._y;
			tVar1[i]._d=(*Arr)[i]._d;
		}
	
		tVar1[*len-1]._x=source._x;
		tVar1[*len-1]._y=source._y;
		tVar1[*len-1]._d=source._d;
	}
	tVar2=*Arr;
	*Arr=tVar1;
	tVar1=tVar2;

	free(tVar1);	
}


void delElementFromBackOfArr( point** Arr, int* len)
{
	int i;
	point *tVar1,*tVar2;

	(*len)=(*len)-1;

	if (*len!=0)
	{
		tVar1=(point*)malloc(sizeof(point)*(*len));
	
		for (i=0;i<(*len-1);i++)
		{
        	        tVar1[i]._x=(*Arr)[i]._x;
			tVar1[i]._y=(*Arr)[i]._y;
			tVar1[i]._d=(*Arr)[i]._d;
		}


		tVar2=*Arr;
		*Arr=tVar1;
		tVar1=tVar2;	
	}
	else 
	{
		tVar1=*Arr;
		*Arr=NULL;
	}

	free(tVar1);
}

void delElementFromBeginOfArr (point** Arr, int* len)
{
	int i;
	point *tVar1,*tVar2;

	if (*len!=1)
	{
		tVar1=(point*)malloc(sizeof(point)*(*len-1));
	
		for (i=1;i<(*len);i++)
		{
        	        tVar1[i-1]._x=(*Arr)[i]._x;
			tVar1[i-1]._y=(*Arr)[i]._y;
			tVar1[i-1]._d=(*Arr)[i]._d;
		}

		tVar2=*Arr;
		*Arr=tVar1;
		tVar1=tVar2;	
	}
	else 
	{
		tVar1=*Arr;
		*Arr=NULL;
	}

	free(tVar1);

	(*len)=(*len)-1;
}



void copy_body_frame(point * original , point **copy, int len)
{

	int i;
	point *tVar1,*tVar2;

	tVar1=(point*)malloc(sizeof(point)*len);
	for(i=0;i<len;i++)
	{
		tVar1[i]._x=original[i]._x;
		tVar1[i]._y=original[i]._y;
		tVar1[i]._d=original[i]._d;
	}

	tVar2=*copy;
	*copy=tVar1;	
	tVar1=tVar2;
	
	free (tVar1);
}

int InitUnits()
{
	int i;

	Rabbit=(Unit*)malloc(sizeof(Unit));
	Rabbit->len=1;
	Rabbit->cord=(point*)malloc(sizeof(point)*Rabbit->len);
	Rabbit->cord->_x=0;
	Rabbit->cord->_y=0;
	Rabbit->cord->_d=0;
	Rabbit->num_tpa=0;
	Rabbit->tpa=NULL;

	Snake=(Unit*)malloc(sizeof(Unit));
	Snake->len=1;
	Snake->cord=(point*)malloc(sizeof(point)*Snake->len);
	//for (i=0;i<Snake->len;i++)
	 //{
		Snake->cord[0]._y=Y_max/2;
		Snake->cord[0]._x=X_max/2;
		Snake->cord[0]._d=1;
	 //}
	Snake->num_tpa=0;
	Snake->tpa=NULL;

	return 0;
}

int DestroyUnits()
{
	if (Rabbit&&Snake)
	{
		free (Rabbit->cord);
		free (Rabbit->tpa);
		free (Rabbit);
	
		free (Snake->cord);
		free (Snake->tpa);
		free (Snake);

		free (snake_body_frame);
	}
	return 0;
}

//************************************* Game Face   ******************




static void activate(GtkApplication *app, gpointer userdata)
{

	GtkWidget *window;
	GtkWidget *frame;
	GtkWidget *drawing_area;


	window=gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(window),"FunnySnake11");
	gtk_window_set_default_size(GTK_WINDOW(window),300,300);
	gtk_window_set_resizable(GTK_WINDOW(window),FALSE);
	g_signal_connect(window,"destroy",G_CALLBACK(close_window),NULL);

	frame=gtk_frame_new(NULL);
	gtk_frame_set_shadow_type(GTK_FRAME(frame),GTK_SHADOW_IN);

	drawing_area=gtk_drawing_area_new();


	gtk_container_set_border_width(GTK_CONTAINER(window),8);
	gtk_container_add(GTK_CONTAINER(window),frame);
	gtk_container_add(GTK_CONTAINER(frame),drawing_area);

	g_signal_connect (drawing_area, "draw", G_CALLBACK (render_), NULL);

	g_signal_connect(window,"key_press_event",G_CALLBACK(key_mon),NULL);

	g_timeout_add(200, _GameTic_ ,drawing_area);


	gtk_widget_show_all(window);

}
static gboolean _GameTic_ (gpointer data)
{	
	GtkWidget *drawing_area;
	drawing_area=(GtkWidget*)data;
	gti(1);
	Game(1);
	gtk_widget_queue_draw(drawing_area);
	return TRUE;
}

int Game(int state)
{
	if (GST==game_on){
		RabbitFactory();
		SnakeMoveToOneStep(move_flag,0);
		if(Score>=NumNextLevelJump)
			GST=game_next_level;
	}

	g_print ("game step\n");
	return 0;
}

static gboolean  render_(GtkWidget *widget, cairo_t *cr,gpointer udata)
{
	guint width, height;
	GdkRGBA color;
	GtkStyleContext *context;

	int  x,y;
	int i,k,m,hStep=5,vStep=5;

	context=gtk_widget_get_style_context(widget);

	width=gtk_widget_get_allocated_width(widget);
	height=gtk_widget_get_allocated_height(widget);

	X_max=width;
	Y_max=height;

	gtk_render_background(context,cr,0,0,width,height);

	//---------- Make game fild ----------------------
	
	color.red=0.0;
	color.green=0.0;
	color.blue=0.0;
	color.alpha=1.0;
	gdk_cairo_set_source_rgba(cr,&color);
	cairo_move_to (cr,border_x_min,border_y_min);
	cairo_line_to(cr,border_x_max,border_y_min);
	cairo_line_to(cr,border_x_max,border_y_max);
	cairo_line_to(cr,border_x_min,border_y_max);
	cairo_line_to(cr,border_x_min,border_y_min);
	cairo_set_line_width(cr,1.0);
	cairo_stroke(cr);	
//	cairo_move_to(cr,border_x_min,border_y_min-10);
//	cairo_show_text(cr,"0123456789012345678901234567890123456789");

//	m=border_y_min;
	for (k=border_x_min;k<=border_x_max;k+=hStep)
	{
		cairo_move_to (cr,k,border_y_min);
		cairo_line_to(cr,k,border_y_max);
	}
	for (k=border_y_min;k<=border_y_max;k+=vStep)
	{
		cairo_move_to (cr,border_x_min,k);
		cairo_line_to(cr,border_x_max,k);
	}
	cairo_set_line_width(cr,1.0);
	cairo_stroke(cr);

	//--------------------------------------------------------

	if ((GameImpuls%2)==0){ 
		color.red=1.0;
		color.green=0.0;
		color.blue=0.0;
		color.alpha=1.0;
	}
	else{
		color.red=0.0;
		color.green=1.0;
		color.blue=0.0;
		color.alpha=1.0;
	}
	
 	gdk_cairo_set_source_rgba(cr,&color);

//*************************************************
	if (GST==game_on)
	{
		if (rabbitInFild)
			cairo_move_to(cr,Rabbit->cord->_y,Rabbit->cord->_x);
			cairo_show_text(cr,"*");

		for(i=0;i<Snake->len;i++ )
		{
				cairo_move_to(cr,Snake->cord[i]._x,Snake->cord[i]._y);
				cairo_show_text(cr,"O");
		}


		sprintf (str_BUF1,"%d",Score);
		cairo_move_to(cr,border_x_min,border_y_max+10);
		cairo_show_text(cr,"Score-");
		cairo_move_to(cr,border_x_min+60,border_y_max+10);
		cairo_show_text(cr,str_BUF1);

		sprintf (str_BUF2,"%d",Level);
		cairo_move_to(cr,border_x_min,border_y_max+20);
		cairo_show_text(cr,"Level-");
		cairo_move_to(cr,border_x_min+60,border_y_max+20);
		cairo_show_text(cr,str_BUF1);
	}

	if (GST==game_over)
	{
		cairo_move_to(cr,border_x_max/2-30,border_y_max/2);
		cairo_show_text(cr,"G A M E   O V E R !!!!!");
	}
//**********************************
	cairo_fill(cr);

	return FALSE; 
}



static void key_mon(GtkWidget *widget,GdkEvent *event,gpointer udata)
{
	GdkEventKey *key_event=(GdkEventKey*)event;

	if (key_event->keyval==GDK_KEY_m){
		GST=game_menu;
		g_print ("Pressed key -m-\n");

	}

	if (GST==game_menu||GST==game_over){
		switch (key_event->keyval)  {
			case GDK_KEY_e:
				GST=game_exit;
				g_print("Presed key -e-\n");
				break;
			case GDK_KEY_n:
				GST=game_on;
				g_print("Pressed key -n-\n");
				CreateGameFild();
				rabbitInFild=0;
				move_flag=rand()%4+1;
				RabbitWasEaten=0;
				Score=0;
				Level=1;
				InitUnits();
				break;
			case GDK_KEY_c:
				if(Snake && Rabbit)
				{
					GST=game_on;
					g_print("Pressed key -c-\n");
				}
				break;	
			default:
				break;

		}
	}

	if (GST==game_on)
		snake_control (key_event->keyval);	

}

static void close_window(void)
{
if (surface)
    cairo_surface_destroy (surface);
}
//---======================********************-------------------------


void CreateGameFild()
{

}


void gameMenuOpen()
{
/*
	MainMenu=newwin(10,20,border_y_max/2-2,border_x_max/2-5);
	wbkgd(MainMenu,COLOR_PAIR(2));
	wattron(MainMenu,COLOR_PAIR(2));
	box(MainMenu,ACS_VLINE,ACS_HLINE);
	wmove(MainMenu,1,7);
	waddstr(MainMenu,"MENU:");
	wmove(MainMenu,3,1);
	waddstr(MainMenu,"NEW GAME....'n'");
	wmove(MainMenu,5,1);
	waddstr(MainMenu,"CONTINUE....'c'");
	wmove(MainMenu,7,1);
	waddstr(MainMenu,"EXIT........'e'");
	wrefresh(MainMenu);
*/
}

void gameMenuClose()
{
//	delwin(MainMenu);
//	MainMenu=NULL;
}




//============================= MAIN ======================================
		
int main (int argc, char** argv)
{	

	
	int i;
	int ret;
	struct itimerval tmr1, tmr2;
	char buf1[2]={'0',0x00};
	int PRG=1;	
	int status;

	// initialize  same  variable----------
	Snake=NULL;
	Rabbit=NULL;
	GST=game_menu;	

	srand(time(NULL));
	
	// ------initialize GTK 
	GtkApplication *app;
	app=gtk_application_new("org.gtk.funny_snake_11", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app,"activate",G_CALLBACK(activate),NULL);
	status=g_application_run(G_APPLICATION(app), argc, argv);// ---start main loop with 
	g_object_unref(app);


	
	//--------------------- main cicle---------------		
/*
	while (PRG)
	{
		ch=wgetch(stdscr);
		
		if (ch=='m')
		{
			GST=game_menu;
		}


		if ((GST==game_menu)||(GST==game_over))
		{
			if (!MainMenu)
				gameMenuOpen();
			switch (ch)
			{
			case 'e':
				GST=game_exit;
				gameMenuClose();
				PRG=0;
				break;
			case 'n':
				GST=game_on;
				gameMenuClose();
				CreateGameFild();
				rabbitInFild=0;
				move_flag=rand()%4+1;
				RabbitWasEaten=0;
				Score=0;
				Level=1;
				InitUnits();
				//timer setpoint value
				tmr1.it_value.tv_sec=0;
				tmr1.it_value.tv_usec=200000;
				tmr1.it_interval.tv_sec=0;
				tmr1.it_interval.tv_usec=200000;
				setitimer(ITIMER_REAL,&tmr1,NULL);// timer whith new setpoint by level
				break;
			case 'c':
				if(Snake && Rabbit)
				{
					GST=game_on;
					gameMenuClose();
					CreateGameFild();
				}
				break;
			
			}		
		}	

		
		if (GST==game_on){
			if (GameImpuls!=Watchdog){
				ImpulsFront=1;
				Watchdog=GameImpuls;
			}
			else{
				ImpulsFront=0;
			}

			snake_control(ch);
			
			if (ImpulsFront){
				rander(0);
				RabbitFactory();
				SnakeMoveToOneStep(move_flag,0);
				if(Score>=NumNextLevelJump)
					GST=game_next_level;
				rander(1);


				 
			}
		}

		if (GST==game_next_level){
				DestroyUnits();
				GST=game_on;
				CreateGameFild();
				rabbitInFild=0;
				move_flag=rand()%4+1;
				RabbitWasEaten=0;
				Score=0;
				Level++;
				InitUnits();
				//timer setpoint value
				tmr1.it_value.tv_sec=0;
				tmr1.it_value.tv_usec=200000-Level*10000;
				tmr1.it_interval.tv_sec=0;
				tmr1.it_interval.tv_usec=200000-Level*10000;
				setitimer(ITIMER_REAL,&tmr1,NULL);// timer whith new setpoint by level
		}
	}
	//-----------------------end of main cicle----------------
		
*/
	//----------- delete units  -----------
	DestroyUnits();
	g_print("exit app\n");


	
	return status;
}	


