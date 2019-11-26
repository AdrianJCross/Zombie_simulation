//NOT THE FINAL VERSION!!!!!!
//Adrian Cross
//year 3 intro to C++ project 
//zombie outbreak simulation v1
//This code simulates a zombie outbreak
//this code has been written in quincy
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <windows.h>//needed for sleep function


using namespace std;

int zombiesense=5;
const int gridsizex=40;
const int gridsizey=20;

int skillrate=15;

enum character {empty, human, zombie, infected};
enum attack {infection, zombiedeath, zombiedeathinf, humandeath};
enum quadrant {straightup, straightright, straightdown, straightleft, upperright, lowerright,lowerleft,upperleft};

character coord[gridsizex][gridsizey];
	
//function to create a random x coordinate based on grid size
int randgridx(){
	int xrand=rand() % gridsizex;
	return xrand;
}
//function to create a random y coordinate based on grid size
int randgridy(){
	int yrand=rand() % gridsizey;
	return yrand;
}
	
//function to error check if space is empty
bool emptycheck(character state){
	switch (state){
		case empty:
			return true;
	
		default:
			return false;
	}
}



//Random number functions (numbers returned are in the function name)
int rand13(){
	int a=(rand() % 2)+1;
	if (a==2){
		return 3;
	}
	else return a;
}

int rand24(){
	int a=(rand() % 2)+1;
	if (a==1){
		return 4;
	}
	else return a;
}
int rand23(){return ((rand() % 2)+2);}

int rand34(){return ((rand() % 2)+3);}

int rand12(){return ((rand() % 2)+1);}

int rand14(){
	int a=(rand() % 2)+1;
	if (a==2){
		return 4;
		}
	else return a;
}
	
int rand123(){return ((rand() % 3)+1);}

int rand234(){return ((rand() % 3)+2);}

int rand1234(){return ((rand() % 4)+1);}

int rand134(){
	int a=(rand() % 3)+2;
	if (a==2){
		return 1;
	}	 	 
	else return a;
}

int rand124(){
	int a=(rand() % 3)+1;
	if (a==3){	  	  	  
		return 4;
	}
	else return a;
}

//function to pick a direction of movement (from 1 to 4) depending on what spaces around it are free
int direction(int x, int y){
	bool up=true;
	bool right=true;
	bool down=true;
	bool left=true;
	//checks the spaces around the character for other characters
	if (emptycheck(coord[x][y-1])==false){
		up=false;
	} 
	if (emptycheck(coord[x+1][y])==false){
		right=false;
	}
	if (emptycheck(coord[x][y+1])==false){
		down=false;
	}
	if (emptycheck(coord[x-1][y])==false){
		left=false;
	}
	
	//checks the spaces around the character for corners
	if (x==0 and y==0){
		left=false;
		up=false;
	}
	if (y==0 and x==gridsizex-1){
		up=false;
		right=false;
	}	 
	if (x==0 and y==gridsizey-1){
		down=false;
		left=false;
	}
	if (y==gridsizey-1 and x==gridsizex-1){
		down=false;
		right=false;
	}
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
	//returns a random number dependant on which direction the character can travel
	if (up==true and right==true and down==true and left==true) {return rand1234();} 
	else if (up==false and right==false and down==false and left==false) {return 0;}
	else if (up==true and right==false and down==false and left==false) {return 1;}
	else if (up==false and right==true and down==false and left==false) {return 2;}
	else if (up==false and right==false and down==true and left==false) {return 3;}
	else if (up==false and right==false and down==false and left==true) {return 4;}
	else if (up==true and right==true and down==false and left==false) {return rand12();}
	else if (up==true and right==false and down==true and left==false) {return rand13();}
	else if (up==true and right==false and down==false and left==true) {return rand14();}
	else if (up==false and right==true and down==true and left==false) {return rand23();}
	else if (up==false and right==true and down==false and left==true) {return rand24();}
	else if (up==false and right==false and down==true and left==true) {return rand34();}
	else if (up==true and right==true and down==true and left==false) {return rand123();}
	else if (up==false and right==true and down==true and left==true) {return rand234();}
	else if (up==true and right==true and down==false and left==true) {return rand124();}
	else if (up==true and right==false and down==true and left==true) {return rand134();}	 	 	 
}


//function to check for humans in nearby spaces
//need to fix as overuns at edges (thinks human is next to zombie when it is on the other side
int humansense(int x,int y){
	if (coord[x][y-1]==human and y!=0){return 1;}
	else if (coord[x+1][y]==human and x!=gridsizex){return 2;}
	else if (coord[x][y+1]==human and y!=gridsizey){return 3;}
	else if (coord[x-1][y]==human and x!=0){return 4;}
	else {return 0;}
}

//simulates attack between human and zombie
//will need to be alterable
attack attacksim(int skill){
int a=((rand() % 100)+1); //random number from 1-100
	if (a>skill){
	cout<<"infection"<<endl;
	return infection;
	}
	
	else if (a<skill){
	cout<<"zombie death"<<endl;
	return zombiedeath;
	}
	
	else if (a==skill){
	cout<<"zombie death and infection"<<endl;
	return zombiedeathinf;
	}
	else cout<<"error in attack simulation"<<endl<<"random number is: "<<a<<endl<<"Skill is: "<<skill;
}



//function to increase skillrate
//alter to increase skill less at higher levels
int skillincrease(int skill){
	if (skill+skillrate>=100){
		return 99;
		}
	else return (skill+skillrate);
}

	//function to return a quadrant based on value of x and y
quadrant quadrantcheck(int i, int j, int x, int y){

	//returns quadrant based on value of x and y
	if (coord[i][j]==human){
		if (i==0 and j>y) {return straightdown;}
		else if (i==0 and j<y) {return straightup;}
		else if (i<x and j==0) {return straightleft;}
		else if (i>x and j==0) {return straightright;}
			
		else if (i>x and j<y) {return upperright;}
		else if (i>x and j>y) {return lowerright;}
		else if (i<x and j>y) {return lowerleft;}
		else if (i<x and j<y) {return upperleft;}
		else cout<<"error in quadrant check function";
	
	}
}
	
//searches in a spiral for a human and stops when it finds one
quadrant spiral(int x, int y){
	int i=x, j=y;
	j--;
	if (coord[i][j]==human) {return quadrantcheck(i, j, x, y);}
	i++;
	if (coord[i][j]==human) {return quadrantcheck(i, j, x, y);}	
	for (int a=0; a<=zombiesense; a++){
		j--;
		if (coord[i][j]==human) {return quadrantcheck(i, j, x, y);}
		i++;
		if (coord[i][j]==human) {return quadrantcheck(i, j, x, y);}	 
		j++;
		if (coord[i][j]==human) {return quadrantcheck(i, j, x, y);}
		j++;
		if (coord[i][j]==human) {return quadrantcheck(i, j, x, y);}
		i--;
		if (coord[i][j]==human) {return quadrantcheck(i, j, x, y);}
		i--;
		if (coord[i][j]==human) {return quadrantcheck(i, j, x, y);}
		j--;
		if (coord[i][j]==human) {return quadrantcheck(i, j, x, y);}
		j--;
		if (coord[i][j]==human) {return quadrantcheck(i, j, x, y);}
		i++;
		if (coord[i][j]==human) {return quadrantcheck(i, j, x, y);}
		i++;
		if (coord[i][j]==human) {return quadrantcheck(i, j, x, y);}
	}
}





int main(){
	srand(time(0));
	bool temp[gridsizex][gridsizey];
	int skill[gridsizex][gridsizey];
	
	//inputs
	int starthumanno, startzombieno, skillrate, timedelay;

	cout<<"starting number of humans: ";
	cin>>starthumanno;
	cout<<endl<<"starting number of zombies: ";
	cin>>startzombieno;
	cout<<"Time delay between movement: ";
	cin>>timedelay;

	
	//checks the grid size to see if its large enough for the number of zombies and humans you have
	if (starthumanno+startzombieno>(gridsizex)*(gridsizey)){
	cout<<"Error grid size too small for characters to fit!"<<endl;
	return 0;
	}
		
	//loop to enter a blank grid for each point in the grid
	for (int j=0; j<=gridsizey-1; j++){
		for (int i=0; i<=gridsizex-1; i++){
			coord[i][j]=empty;
			}
	}
		
	
	//loop which gives each spot a random chance to have a human (1) 
	for (int i=0; i<starthumanno; i++){ 
		int xdummy=randgridx(), ydummy=randgridy();
		bool c=emptycheck(coord[xdummy][ydummy]);
		switch (c){
			case true:
				coord[xdummy][ydummy]=human;
				skill[xdummy][ydummy]=(rand() % 50)+1;//enters a random skill value from 1 to 50
			break;
			case false:
				i--;
			break;
		}
		
	}
	//loop which gives each spot a random chance to have a zombie(2) 
	for (int i=0; i<startzombieno; i++){
		int xdummy=randgridx(), ydummy=randgridy();
		bool c=emptycheck(coord[xdummy][ydummy]);
		switch (c){
			case true:
				coord[xdummy][ydummy]=zombie;
			break;
			case false:
				i--;
			break;
		}

	}	 
	//constants used in the loop	 
	int loopno=0;


	//main loop for this program
	for (int t=0; t<1; t){
		//displays the grid	
		for (int j=0; j<=gridsizey-1; j++){
			for (int i=0; i<=gridsizex-1; i++){
				switch (coord[i][j]){
					case empty:
					cout<<" ";
					break;
					case human:
					cout<<"H";
					break;
					case zombie:
					cout<<"Z";
					break;
					case infected:
					cout<<"I";
					break;
				}
			}
		cout<<endl;
		}	   
		//combine if statements to make more efficient
		
		//resets the temp grid which tells the program to skip a point if it has already been moved

		for (int j=0; j<=gridsizey-1; j++){
			for (int i=0; i<=gridsizex-1; i++){
				temp[i][j]=false;

				}
			}
		//turns infected into zombie 

		for (int j=0; j<=gridsizey-1; j++){
			for (int i=0; i<=gridsizex-1; i++){
				if (coord[i][j]==infected){
					coord[i][j]=zombie;
					skill[i][j]=0;

				}
			}
		}
		
		//checks the grid for adjacent human and zombie pair and then simulates the attack

		for (int j=0; j<=gridsizey-1; j++){
			for (int i=0; i<=gridsizex-1; i++){
				if (coord[i][j]==zombie){
					switch(humansense(i, j)){
						case 0:
						break;
						case 1:
							temp[i][j]=true;
							temp[i][j-1]=true;	  	  	  	  	  
							switch (attacksim(skill[i][j-1])){
								case infection: coord[i][j-1]=infected;
								break;
								case zombiedeath: coord[i][j]=empty; skill[i][j-1]=skillincrease(skill[i][j-1]);
								break;
								case zombiedeathinf: coord[i][j]=empty; coord[i][j-1]=infected;
								break;
								case humandeath: coord[i][j-1]=empty;
							
							}
												
						
						break;
						case 2:
							temp[i][j]=true;
							temp[i+1][j]=true;
							switch (attacksim(skill[i+1][j])){
								case infection: coord[i+1][j]=infected;
								break;
								case zombiedeath: coord[i][j]=empty; skill[i+1][j]=skillincrease(skill[i+1][j]);;
								break;
								case zombiedeathinf: coord[i][j]=empty; coord[i+1][j]=infected;
								break;
								case humandeath: coord[i+1][j]=empty;
							}
						
						break;
						case 3:
							temp[i][j]=true;
							temp[i][j+1]=true;
							switch (attacksim(skill[i][j+1])){
								case infection: coord[i][j+1]=infected;
								break;
								case zombiedeath: coord[i][j]=empty; skill[i][j+1]=skillincrease(skill[i][j+1]);
								break;
								case zombiedeathinf: coord[i][j]=empty; coord[i][j+1]=infected;
								break;
								case humandeath: coord[i][j+1]=empty;
							}
						
						break;
						case 4:
						temp[i][j]=true;
						temp[i-1][j]=true;
							switch (attacksim(skill[i-1][j])){
								case infection: coord[i-1][j]=infected;
								break;
								case zombiedeath: coord[i][j]=empty; skill[i-1][j]=skillincrease(skill[i-1][j]);
								break;
								case zombiedeathinf: coord[i][j]=empty; coord[i-1][j]=infected;
								break;
								case humandeath: coord[i-1][j]=empty;
								
							}
					
						break;
						
					}
				}
			}
		}

		
		
		

		for (int j=0; j<=gridsizey-1; j++){
			for (int i=0; i<=gridsizex-1; i++){
				if (temp[i][j]==false){ //needed so that if a character moves it doesn't move again in the same loop
					switch (coord[i][j]){
						case zombie: //changes the position of the zombie
							switch(direction(i, j)){ 
								case 0:cout<<0;
								break;
								case 1:coord[i][j]=empty; //case 1: coord [i][j-1]
									coord[i][j-1]=zombie;
									temp[i][j-1]=true;
								break;
								case 2:coord[i][j]=empty; //case 2: coord [i+1][j] 
									coord[i+1][j]=zombie;
									temp[i+1][j]=true;
								break;
								case 3:coord[i][j]=empty; //case 3: coord [i][j+1]
									coord[i][j+1]=zombie;
									temp[i][j+1]=true;
								break;
								case 4:coord[i][j]=empty; //case 4: coord [i-1][j] 
									coord[i-1][j]=zombie;
									temp[i-1][j]=true;
								break;
						}
							break;
						case human: //changes the position of the human
							switch(direction(i, j)){
								case 0: 
								break;
								case 1:coord[i][j]=empty; //case 1: coord [i][j-1]
									coord[i][j-1]=human;
									temp[i][j-1]=true;
									skill[i][j-1]=skill[i][j];
									skill[i][j]=0;
								break;
								case 2:coord[i][j]=empty; //case 2: coord [i+1][j] 
									coord[i+1][j]=human;
									temp[i+1][j]=true;
									skill[i+1][j]=skill[i][j];
									skill[i][j]=0;
								break;
								case 3:coord[i][j]=empty; //case 3: coord [i][j+1]
									coord[i][j+1]=human;
									temp[i][j+1]=true;
									skill[i][j+1]=skill[i][j];
									skill[i][j]=0;
								break;
								case 4:coord[i][j]=empty; //case 4: coord [i-1][j] 
									coord[i-1][j]=human;
									temp[i-1][j]=true;
									skill[i-1][j]=skill[i][j];
									skill[i][j]=0;
								break;
							}
						}
					}
				}
			}
	//delays screen by time in brackets in milliseconds
		Sleep(timedelay);

	
	//clears the screen
		system("cls");
	
	//if statement to end program when there are no more zombies or humans
		int h=0, z=0, I=0;
		for (int j=0; j<=gridsizey-1; j++){
			for (int i=0; i<=gridsizex-1; i++){
				switch(coord[i][j]){
					case human:h++;
					break;
					case zombie:z++;
					break;
					case infected:I++;
					default:;
				}
				if (h!=0 and z!=0){ //ends the loop (means all coords don't need to be checked)
					j=gridsizey-1;
					i=gridsizex-1;
				}
			}
		}
		//output if no humans or zombies left
		if (h+I==0){//infected count as humans
			t=1;
			cout<<"all "<<starthumanno<<" human(s) were killed or infected by "<< startzombieno<<" zombie(s) with "<<z<<" zombie(s) left";//put in timer????????
		}
		else if (z+I==0){
			t=1;
			cout<<"all "<<startzombieno<<" starting zombie(s) were killed by "<< starthumanno<<" human(s) with "<<h<<" human(s) left";//put in timer????????
		}

	loopno++;
			

	}
		
	




	return 0;
}

