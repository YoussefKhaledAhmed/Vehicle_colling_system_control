#include <stdio.h>

char Main_menu(void) ;//to display
void sensors_set_menu_ (void) ;//to display sensors set menu
int set_the_traffic_color(char) ;
int AC (int,int) ;//to return the AC state
int  engine_temp_controller (int,int) ;//to return the controller state
float set_the_room_temp (int , float , int) ;
float set_the_engine_temp(int,float,int) ;
void display_the_current_vehicle_state(int,int,int,float,int,float) ;

enum state {off,on} ;//off = 0 , on = 1

struct variables {
	char Main_menu_char , sensors_menu_2 , sensors_set_menu , index;
	int vehicle_speed  ,  AC_state ;
	int controller_state , engine_state ;
	float current_temp  , engine_temp  ;
}; //it contains all variables used in the main

#define Main_menu_char project.Main_menu_char
#define sensors_menu_2 project.sensors_menu_2
#define sensors_set_menu project.sensors_set_menu
#define index project.index
#define vehicle_speed project.vehicle_speed
#define AC_state (project.AC_state)
#define controller_state project.controller_state
#define engine_state project.engine_state
#define current_temp project.current_temp
#define engine_temp project.engine_temp
/*
 * these #define to exchange each variable used in the main by project.variable which is the elements of project struct which is of type
 * variables
 */


int main(void)
{
	setvbuf(stdout,NULL,_IONBF,0) ;
	setvbuf(stderr,NULL,_IONBF,0) ;// instead of using fflush(stdout) after each printf("")

	struct variables project ;
	vehicle_speed = 100 ;
	AC_state = off ;
	controller_state = off ;
	current_temp = 20 , engine_temp = 120 ;

	//arrays of pointer to function
	float (*ptr_room_engine_temp[2])(int,float,int) = {set_the_room_temp,set_the_engine_temp} ;
	int (*ptr_AC_controller_state[2])(int,int) = {AC,engine_temp_controller} ;

	while(1)
	{
		Main_menu_char = Main_menu() ;

		while(Main_menu_char == 'A' || Main_menu_char == 'a')
		{
			printf("\nthe engine is turned off\n") ;
			Main_menu_char = Main_menu() ;
			engine_state = 0 ;
		}
		switch(Main_menu_char)
		{
		case 'B':
		case 'b':
			engine_state = 1 ;
			printf("\nthe engine is turned on\n") ;
			break ;
		case 'C':
		case 'c':
			printf("you quit the system") ;
			return 0 ;
		}

		sensors_set_menu_() ;// to display the sensors set menu
		while(1)
		{
			printf("\nplease choose by letter: ") ;
			scanf(" %c",&sensors_set_menu) ;
			if(sensors_set_menu == 'a')
			{
				printf("\nthe engine is turned off.\n\n") ;
				index = 1 ;
				/*
				 * this index is initialized by 0 so if it becomes 1 there is a condition below which states that if index becomes 1
				 * it will break from this loop and goes to the beginning of the main loop that will display the main menu again
				 * and so on ...
				 */
			}
			else if(sensors_set_menu == 'b')
			{
				do{
					printf("please enter the traffic color 'G'for green 'O'for orange 'R'for red: ") ;
					scanf(" %c",&sensors_menu_2) ;
					vehicle_speed = set_the_traffic_color(sensors_menu_2) ;
					AC_state = (*ptr_AC_controller_state[0]) (current_temp,vehicle_speed) ;
					current_temp = (*ptr_room_engine_temp[0]) (AC_state , current_temp , vehicle_speed) ;
					controller_state =  (*ptr_AC_controller_state[1])(engine_temp,vehicle_speed) ;
					engine_temp = (*ptr_room_engine_temp[1])(controller_state , engine_temp , vehicle_speed) ;

				}
				while(vehicle_speed == -1);
			/*
			 * this do while loop to check if the user enters any letter other than 'G' 'O' 'R'  so if he does enter another on it
			 * will ask him again to enter and so on ...
			 */
			}
			else if (sensors_set_menu == 'c')
			{
				printf("please enter the current room temperature: ") ;
				scanf(" %f",&current_temp) ;
				AC_state = (*ptr_AC_controller_state[0]) (current_temp,vehicle_speed) ;
				current_temp = (*ptr_room_engine_temp[0]) (AC_state , current_temp , vehicle_speed) ;
				controller_state =  (*ptr_AC_controller_state[1])(engine_temp,vehicle_speed) ;
				engine_temp = (*ptr_room_engine_temp[1])(controller_state , engine_temp , vehicle_speed) ;
			}
			else if (sensors_set_menu == 'd')
			{
				printf("please enter the engine temperature: ") ;
				scanf(" %f",&engine_temp) ;
				controller_state =  (*ptr_AC_controller_state[1])(engine_temp,vehicle_speed) ;
				engine_temp = (*ptr_room_engine_temp[1])(controller_state , engine_temp , vehicle_speed) ;
			}
			if (index == 1)//this is the condition of 'index' that is explained above
			{
				break ;
			}
			index = 0 ;
			display_the_current_vehicle_state(engine_state,AC_state,vehicle_speed,current_temp,controller_state,engine_temp) ;
		}
	}
	return 0 ;
}

void sensors_set_menu_ (void)
{
	printf("\na. Turn off the engine") ;
	printf("\nb. Set the traffic light color.") ;
	printf("\nc. Set the room temperature (Temperature Sensor)") ;
	printf("\nd. Set the engine temperature (Engine Temperature Sensor)") ;
	return ;
}

char Main_menu(void) {
	char c ;
	printf("please choose one from the following:\n\n") ;
	printf("a.turn off the vehicle engine\n") ;
	printf("b.turn on the vehicle engine\n") ;
	printf("c.Quit the system\n") ;
	scanf(" %c",&c) ;
	return c ;
}

int set_the_traffic_color(char c)
{
	int speed ;
	if ('G' == c || 'g' == c)
	{
		speed = 100 ;
	}
	else if ('O' == c || 'o' == c)
	{
		speed = 30 ;
	}
	else if('R' == c || 'r' == c)
	{
		speed = 0 ;
	}
	else
	{
		speed = -1 ;
	}
	return speed ;
}

int AC (int temp , int speed)
{
	int state ;
	if ((temp < 10 || temp > 30)||(speed==30))
	{
		state = 1 ;
	}
	else
	{
		state = 0 ;
	}
	return state ;
}

float set_the_room_temp (int state , float temp , int speed)
{
	if (1 == state &&(temp<10 || temp>30))
	{
		temp = 20 ;
	}
	if (speed == 30 )
	{
		temp = temp*((float)5/4) +1 ;
	}
	return temp ;
}

int engine_temp_controller (int temp, int speed)
{
	int state ;
	if ((temp<100 || temp >150)||(speed==30))
	{
		state = 1 ;
	}
	else
	{
		state = 0 ;
	}
	return state ;
}

float set_the_engine_temp(int state,float temp,int speed)
{

	if (1 == state &&(temp<100 || temp >150))
	{
		temp = 125 ;
	}
	if (speed == 30 )
	{
		temp = temp*((float)5/4) +1 ;
	}
	return temp ;
}

void display_the_current_vehicle_state(int _engine_state,int _AC_state,int speed,float _room_temp,int _controller_state,float _engine_temp)
{
	printf("\n\n\n******************************************************\n") ;
	(_engine_state == 1)?(printf("the engine is ON.\n")):(printf("the engine is OFF.\n")) ;
	(_AC_state == 1)?(printf("the AC is ON.\n")):(printf("the AC is OFF.\n")) ;
	printf("vehicle_speed = %d\n", speed) ;
	printf("current room temperature = %f\n", _room_temp) ;
	(_controller_state == 1)?(printf("the engine controller is ON.\n")):(printf("the engine controller is OFF.\n")) ;
	printf("engine temperature = %f\n", _engine_temp) ;
	printf("\n******************************************************") ;
	return ;
}

