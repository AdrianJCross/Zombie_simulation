//NOT THE FINAL VERSION!!!!!!
//Adrian Cross
//year 3 intro to C++ project 
//zombie outbreak simulation v2
//This code simulates a zombie outbreak
//this code has been written in quincy
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <windows.h>
using namespace std;

//global
enum character {human, zombie, infected, dead};


//character class which contains the information for each character in the simulation (entity, zombie etc.)
class entity {
    private:
		character state;
		bool checked;	   //temporary state to determine if the character has moved that turn
	    int skill;      //skill level of character
		int speed;		//speed of character
		int x;
		int y;
		int turnsinfected;
    public:
        // Constructor, with defaults
    	entity(character statein=dead, bool checkedin=false,int skillin=0,
		int speedin=0, int xin=0, int yin=0, int turnsinfectedin=0);
        // access functions
		character getstate();
		bool getchecked();
		int getskill();
		int getspeed();
		int getx();
		int gety();
		int getturnsinfected();
        // member functions
		void charactergen(character, int, int, vector<entity>&);
		void characterchange(int, int, character);
		void checkedreset();
		void turntozombie(int);
		void bordercheck(int,int, bool&, bool&, bool&, bool&);
		int action(vector<entity>&, character&, bool&, bool&, bool&, bool&, int);
		void simulateattack(vector<entity>&, int, int);
		void skillincrease(int);
		void adjacentcheck(bool&, bool&,bool&, bool&, vector<entity>&, int);
	  	void action(vector<entity>&, int, int, int);
		int directionreturn(bool, bool,bool, bool);
		void movement(int);
		void counter (int&, int&, int&, int&);
};

//blank check cycles through the vector to check if anything occupies that point
bool blankcheck(int x, int y, vector<entity> id){
	for (int i=0; i<id.size(); i++){
		if (id[i].getx()==x && id[i].gety()==y){
			return false;
		}
	}
	return true;
}
				
			

//input initial into the class
entity::entity(character statein, bool checkedin, int skillin, int speedin, int xin, int yin, int turnsinfectedin){
	state=statein;
	checked=checkedin;
	skill=skillin;
	speed=speedin;
	x=xin;
	y=yin;
	turnsinfected=turnsinfectedin;
}
//access functions
character entity::getstate()	{return state;}
bool entity::getchecked()		{return checked;}
int entity::getskill()			{return skill;}
int entity::getspeed()			{return speed;}
int entity::getx()				{return x;}
int entity::gety()				{return y;}
int entity::getturnsinfected()	{return turnsinfected;}

//member functions	     
//Character gen function checks if a coordinate is empty and will run character change function if it is
void entity::charactergen(character state, int gridsizex, int gridsizey,vector<entity>& id){
	int w=0;
	while (w==0){
		int xdummy=rand() % gridsizex;
		int ydummy=rand() % gridsizey;
		switch (blankcheck(xdummy, ydummy, id)){
			case true:
				characterchange(xdummy, ydummy, state);
				return;
				break;
			case false:
				break;
			default:
				cout<<"error in characterchange function"<<endl;
		}	    	
	} 
}	
//character change function will generate a character at an inputted coordinate and is used for when a human turns into a zombie etc.
void entity::characterchange(int i, int j, character a){
	state=a;
	x=i;
	y=j;
	switch (a){
		case human:
			checked=true;
			skill=(rand() % 50)+1;//creates random number from 1 to 70
			speed=1;
			break;
		case zombie:
			checked=true;
			speed=1;
			break;
		case infected:
			checked=true;
			speed=1;
			break;
		default:
		cout<<"error in characterchange function"<<endl;
	}
}
//resets the temporary grid but leaves dead true so that they no longer move
void entity::checkedreset(){
	if (state==dead){
	checked=true;
	}
	else checked=false;
}
//turn infected into zombie
void entity::turntozombie(int incubation){
	if (state!=infected){
	return;
	}else{
		if (turnsinfected>=incubation-1){
			characterchange(x, y, zombie);
			return;
		}else{
			turnsinfected++;
			return;
		}	 	 	 
	}
}
//increases the skill of a character
void entity::skillincrease(int skillrate){	  
	if (skill+skillrate>=100){
		skill=99;//skill is 99 so there is always a chance of being infected
		}
	else skill=skill+skillrate;
}

//will simulate the attack
void entity::simulateattack(vector<entity>& id, int i, int skillrate){
	int a=((rand() % 100)+1); //random number from 1-100
	if (checked==true){//checks to see if the zombie has attacked already
		return;
	}
	if (a==id[i].skill){//both human and zombie die
		state=dead;
		id[i].state=dead;
	}else if (id[i].skill>a+10){//just zombie dies, skill of human increases
		state=dead;
		id[i].skillincrease(skillrate);
	}else if (id[i].skill<=a+10 && id[i].skill>=a-5){//zombie dies, human infected
		state=dead;
		id[i].state=infected;
		id[i].skillincrease(skillrate);
	}else if (id[i].skill<a-5 && id[i].skill>a-40){//human is infected, zombie lives
		id[i].state=infected;
		id[i].skillincrease(skillrate);
	}else if (id[i].skill<=a-40){//human dies, zombie lives
		id[i].state=dead;
	}	 
	else cout<<"error in simulateattack function";	  	  
}


//sets directions to false if they are on a border
void entity::bordercheck(int gridsizex,int gridsizey,bool &up, bool &right, bool &down, bool &left){	
	//checks the spaces around the character for sides
	if (y==0){
		up=false;
	}
	if (x==0){
		left=false;
	}
	if (x==gridsizex-1){
		right=false;
	}
	if (y==gridsizey-1){
		down=false;
	}
}
	
//checks adjacent spaces and will change directions accordingly
void entity::adjacentcheck(bool &up, bool &right,bool &down, bool &left, vector<entity>& id, int skillrate){

	for (int i=0; i<id.size(); i++){
		int xdiff=x-id[i].x, ydiff=y-id[i].y;
		switch (state){
			case zombie:
				if (xdiff<2 && xdiff>-2 && ydiff<2 && ydiff>-2 && id[i].state==human){//does a simulated attack if within 1 unit radius
					simulateattack(id, i, skillrate);
					checked=true;
					id[i].checked=true;
					return;//ends the loop if an attack has been simulated	  	  	  	  	  
				}
				break; 
			case human:;
				break;
		}
	}
	for (int i=0; i<id.size(); i++){//needs to run as two different for loops as all positions need to be checked for attack first then the movement needs to be checked	   	   	   	   	   	   
		int xdiff=x-id[i].x, ydiff=y-id[i].y;//will run adjacent character check if an attack hasn't taken place
		if (xdiff==0 && ydiff==1){
			up=false;
		}
		if (xdiff==-1 && ydiff==0){
			right=false;

		}
		if (xdiff==0 && ydiff==-1){
			down=false;
		}
		if (xdiff==1 && ydiff==0){
			left=false;
		}
	}
}	
// will return a random number based on the possible directions which can be moved to
int entity::directionreturn(bool up, bool right,bool down, bool left){
	if (up==true and right==true and down==true and left==true) {
		return ((rand() % 4)+1);
	}else if (up==false and right==false and down==false and left==false){
		return 0;
	}else if (up==true and right==false and down==false and left==false){
		return 1;
	}else if (up==false and right==true and down==false and left==false){
		return 2;
	}else if (up==false and right==false and down==true and left==false){
		return 3;
	}else if (up==false and right==false and down==false and left==true){	
		return 4;
	}else if (up==true and right==true and down==false and left==false)	{
		return (rand() % 2)+1;
	}else if (up==true and right==false and down==true and left==false)	{
		int a=(rand() % 2)+1;
		if (a==2){
			return 3;
		}else return a;
	}else if (up==true and right==false and down==false and left==true)	{
		int a=(rand() % 2)+1;
		if (a==2){
			return 4;
		}else return a;
	}else if (up==false and right==true and down==true and left==false)	{
		return (rand() % 2)+2;
	}else if (up==false and right==true and down==false and left==true)	{
		int a=(rand() % 2)+1;
		if (a==1){
			return 4;
		}else return a;
	}else if (up==false and right==false and down==true and left==true)	{
		return (rand() % 2)+3;
	}else if (up==true and right==true and down==true and left==false)	{
	return (rand() % 3)+1;
	}else if (up==false and right==true and down==true and left==true)	{
	return (rand() % 3)+2;
	}else if (up==true and right==true and down==false and left==true)	{
		int a=(rand() % 3)+1;
		if (a==3){	  	  	  
			return 4;
		}else return a;
	}else if (up==true and right==false and down==true and left==true)	{
		int a=(rand() % 3)+2;
		if (a==2){
			return 1;
		}else return a;
	}
	else cout<<"error in direction return function";	         
}





//will run through functions and will act depending on what surrounds the entity and what entity it is,
// either returning 0 if an attack has taken place or 1,2,3,4 indicating a direction
void entity::action(vector<entity>& id, int skillrate, int gridsizex, int gridsizey){
	bool up=true, right=true, down=true, left=true;
	adjacentcheck(up, right, down, left, id, skillrate);
	if (checked==true){
		return; //will end if this entity has already been checked (i.e. if an attack has taken place)
	}
	bordercheck(gridsizex,gridsizey,up,right,down,left);
	int a=	directionreturn(up, right, down, left);  
	movement(a);
	return;	   	   	   
}
//physically moves the entity depending on input
void entity:: movement(int a){
	checked=true;
	switch(a){
		case 0:
			return;
			break;
		case 1://up
			y--;
			return;
			break;
		case 2://right
			x++;
			return;
			break;
		case 3://down
			y++;
			return;
			break;
		case 4://left
			x--;
			return;
			break;
		default:
			cout<<"error in movement function"<<endl;
		
	}
}
	

void entity::counter(int &hno, int &zno, int &Ino, int &dno){
	switch(state){
		case human:
			hno++;
			break;
		case zombie:
			zno++;
			break;
		case infected:
			Ino++;
			break;
		case dead:
			dno++;
	}
}	 


//normal functions--------------------------------------------------------------------------------------------------------------------------
//character gen loop loops the vector values and generates a character in each one
void charactergenloop(int initialhno,int initialzno, vector<entity>& id, int gridsizex, int gridsizey){
	for (int i=0; i<initialhno; i++){ 
		id[i].charactergen(human, gridsizex, gridsizey, id);
	}
  	for (int i=initialhno; i<initialhno+initialzno; i++){ 
		id[i].charactergen(zombie, gridsizex, gridsizey, id);   	 	 
	}  
}	 
//entity display function will loop through the id vector until it finds the entity with the correct coords and then displays it
void displayentity(vector<entity> id, int x, int y){
	for (int i=0; i<id.size(); i++){
		if(id[i].getx()==x && id[i].gety()==y){
			switch(id[i].getstate()){
				case human:
					cout<<"H";
					return;
				case zombie:
					cout<<"Z";
					return;
				case infected:
					cout<<"I";
					return;
				case dead:
					cout<<" ";
					return;
				default:
					cout<<"error in displayentity function";
			}
		}
	}
}	  	        	     	   
//grid display function will cycle through coordinates displaying a gap until it reaches an entity at which point the entity display function will show it
void displaygrid(int gridsizex, int gridsizey, vector<entity> id){
	for (int j=0; j<=gridsizey-1; j++){
		for (int i=0; i<=gridsizex-1; i++){
			switch(blankcheck(i, j, id)){
				case true:
					cout<<" ";
					break;
				case false:
					displayentity(id,i, j);
					break;
				default:
					cout<<"error in displaygrid function";	   	   	   	   	   
			}
		}
		cout<<endl;
	}
}



//will return the victor (if there isn't one will return dead)
character endgame(int &hno, int &zno, int &Ino, int &dno){
	if (hno!=0 and zno!=0 or Ino!=0){ //ends the loop (means all coords don't need to be checked)
		return dead;
	}else if (hno==0){//all humans dead	  	  	  	  	  	    
		return zombie;
	}else if (zno==0){
		return human;
	}	 
}
				

	


//start of main ----------------------------------------------------------------------------------------------
int main(){
	srand(time(0));

	//Inputs
	int timedelay, skillrate, hno, zno, initialhno,
	initialzno,gridsizex, incubation, gridsizey;
	cout<<"starting number of humans: ";
	cin>>initialhno;
	cout<<"starting number of zombies: ";
	cin>>initialzno;
	cout<<"Time delay between movement: ";
	cin>>timedelay;
	cout<<"human skill increase rate: ";
	cin>>skillrate;
	cout<<"grid size in x direction: ";
	cin>>gridsizex;
	cout<<"grid size in y direction: ";
	cin>>gridsizey;
	cout<<"Incubation period: ";
	cin>>incubation;
	hno=initialhno;
	zno=initialzno;
	int Ino=0; //put this as a starting condition
	int dno=0;

	
	if (initialhno+initialzno>gridsizex*gridsizey)
		{cout<<"number of entities is too large for the grid!";
		return 0;
	}
	
	
	vector<entity> id(initialhno+initialzno);	 
	charactergenloop(initialhno,initialzno, id,gridsizex,gridsizey);

	//main loop for the program
	for (int t=0; t<1; t){
		displaygrid(gridsizex, gridsizey, id);
		Sleep(timedelay);//delays screen by time in brackets in milliseconds
		system("cls");
		random_shuffle (id.begin(), id.end()); //randomly shuffles the i values for each person which allows the attacks to be randomised	 	 
		hno=0;
		zno=0;
		Ino=0;
		dno=0;
		for (int i=0; i<id.size(); i++){	//loop to run the movement and interaction of entities
			id[i].checkedreset();
			id[i].counter(hno, zno, Ino, dno);
			if (id[i].getchecked()==false){
			id[i].action(id,skillrate, gridsizex, gridsizey);
			id[i].turntozombie(incubation);
			}
		}
		switch (endgame(hno, zno, Ino, dno)){
			case zombie:
				cout<<"all "<<initialhno<<" human(s) were killed or turned by "
				<< initialzno<<" zombie(s) with "<<zno<<" zombie(s) left in ";//put in timer
				t=1;
				break;
			case human:
				cout<<"all "<<initialzno<<" starting zombie(s) were killed by "
				<< initialhno<<" human(s) with "<<hno<<" human(s) left";
				t=1;
				break;
			case dead:;
		}	 
	}

	
	

	 	 	 

	return 0;
}




