#include<iostream>
#include<windows.h>
#include<cstdlib>
#include<ctime>
#include<conio.h>
#include<string>
using namespace std;

#define BLACK 0
#define NAVY 1
#define GREEN 2
#define TEAL 3
#define MAROON 4
#define PURPLE 5
#define OLIVE 6
#define SILVER 7
#define GRAY 8
#define BLUE 9
#define LIME 10
#define AQUA 11
#define RED 12
#define FUCHSIA 13
#define YELLOW 14
#define WHITE 15

#define MAX_LEN 50
#define MAX_HGT 50

// Lenght, Height, mines
// Facile: 10, 8, 10
// Medio: 18, 14, 40
// Difficile: 24, 20, 99

/** 
	0:uncovered
	1:covered
	2:flagged
**/
struct s_tile{
	bool hasMine;
	unsigned short tileState;
	unsigned short adiacent_mines_num;
};

struct s_data{
	int curs_x;
	int curs_y;
	int width;//lunghezza del campo minato
	int height;//altezza del campo minato
	int mine_tot;//mine totali
	int flag_rem;//bandiere rimanenti
	s_tile tile[MAX_HGT][MAX_LEN];
}minefield;

struct settings{
	bool oldGeneration = false;
	bool secondaryInfo = false;
	int Color_num_1 = BLUE;
	int Color_num_2 = GREEN;
	int Color_num_3 = RED;
	int Color_num_4 = NAVY;
	int Color_num_5 = MAROON;
	int Color_num_6 = AQUA;
	int Color_num_7 = WHITE;
	int Color_num_8 = GRAY;
	int BgColor = BLACK;
	int BorderColor = WHITE;
	int textColor = WHITE;
}settings;

int CurColorNum = 15;
int pressed = 1;
int one_dim_pos = 0;
bool Uncover = false;
bool game_ended = false;
string curMenu = "Main";

void imput_data();//Permette di dare in imput i dati del campo

/**FUNZIONI DI GENERAZIONE DEL CAMPO MINATO**/
void Render_minefield(int);//disegna sulla prompt dei comandi il campo minato
void Render_frame(int, int, int);//disegna la cornice del campo minato
void Render_mines(int, int, int);//disegna le mine sul campo minato
void Render_numbers(int, int);//disegna i numeri sul campo minato
void Random_mine_insert();//inserisce mine a caso nel campo minato
void Random_mine_insert2(int, int);//inserisce mine a caso nel campo minato evitando la posizione data
void Blank_minefield();//crea un campo minato vuoto

/**FUNZIONI DI GESTIONE CAMPO MINATO**/
bool isCurTileInState(int);
void setCurTileState(int);
bool adiacentExists(int);
bool isAdiacentState(int, int);
int getCurTileAdiacentMinesNum(); //get's you the amount of mines surrounding the tile where you are on.
void DetectPressMinefield(int, string);
void AutoPressMinefield(string);
void UncoverNear(int);
void fillUncover();

/**FUNZIONI DI GENERAZIONE DEI MENU**/
void GameIntro();
void Menu();
void Difficulty();
void Playfield();
void Settings();
void Credits();

/**FUNZIONI DI GESTIONE DEI MENU**/
void DetectPressMenu(int, int, string);
void optionHighlight(string, int, int);
void boolOptionHighlight(string, int, int, bool);
void MoveInMenu(int, int, int&, int, int);

/**MISCELLANEOUS**/
int changeGameState();
void setColor(int, int);
void setBackgroundColor(int);
void setTextColor(int);
void swapColors();
void ReloadOnButtonPress(int);
void WaitForPress();


int main(){
	srand(time(NULL));
	GameIntro();
	do{
		if(curMenu == "Main"){
			Menu();
		}
		
		if(curMenu == "Difficulty"){
			Difficulty();
		}
		
		if(curMenu == "Playfield"){
			Playfield();
		}
		
		if(curMenu == "Settings"){
			Settings();
		}
		
		if(curMenu == "Credits"){
			Credits();
		}
	}while(curMenu != "GameExit");
	
	return 0;
}//fine main

/********************************************/
/**FUNZIONI DI GENERAZIONE DEL CAMPO MINATO**/
/********************************************/

void Render_minefield(int state){
	for(int i = -1; i<=minefield.height; i++){
		for(int j = -1; j<=minefield.width; j++){
			if((j == -1 || j == minefield.width) || (i == -1 || i == minefield.height)){
				Render_frame(i, j, state);
			}else{
				Render_mines(i, j, state);
			}
		}
		cout<<endl;
	}
	if(state == 1){
		cout<<"You won"<<endl<<endl;
		setColor(GRAY, BLACK);
		cout<<"*press any key*";
	}else if(state == -1){
		cout<<"Game over"<<endl<<endl;
		setColor(GRAY, BLACK);
		cout<<"*press any key*";
	}else if(state == -2){
		cout<<"SPACE - uncover tiles"<<endl;
	}else if(state == 0){
		if (settings.secondaryInfo)cout<<"Minefield size ("<<minefield.width<<" x "<<minefield.height<<")"<<endl;
		cout<<"Flags: "<<minefield.flag_rem<<endl;
		cout<<"SPACE - uncover tiles"<<endl;
		cout<<"ENTER - places/removes flags"<<endl;
	}
	cout<<endl;
}//end Create_minefield

void Render_frame(int i, int j, int state){
	char LeftDown_angle = 200;
	char RightDown_angle = 188;
	char LeftUp_angle = 201;
	char RightUp_angle = 187;
	char Vertical_line = 186;
	char Horizontal_line = 205;
	if(state == -1){
		setColor(RED, BLACK);
	}else if(state == 1){
		setColor(GREEN, BLACK);
	}else{
		setColor(WHITE, BLACK);
	}
	
	if(i == -1){
		if(j == -1){
			cout<<LeftUp_angle;
		}else if(j == minefield.width){
			cout<<RightUp_angle;
		}else{
			cout<<Horizontal_line;
		}
	}else if(i == minefield.height){
		if(j == -1){
			cout<<LeftDown_angle;
		}else if(j == minefield.width){
			cout<<RightDown_angle;
		}else{
			cout<<Horizontal_line;
		}
	}else{
		cout<<Vertical_line;
	}
}//end Render_frame

void Render_mines(int i, int j, int gamestate){
	int state;
	char mine = 207;
	char flag = 158;
	state = minefield.tile[i][j].tileState;
	if(gamestate == 1){
		setColor(GREEN, BLACK);
		if (minefield.tile[i][j].hasMine){
			cout<<flag;
		}else{
			if(minefield.tile[i][j].adiacent_mines_num == 0){
				cout<<" ";
			}else{
				cout<<minefield.tile[i][j].adiacent_mines_num;
			}
		}
	}else if(gamestate == -1){
		if (minefield.tile[i][j].hasMine){
			cout<<mine;
		}else{
			if(minefield.tile[i][j].adiacent_mines_num == 0){
				cout<<" ";
			}else{
				cout<<minefield.tile[i][j].adiacent_mines_num;
			}
		}
	}else{
		if(Uncover == true){
			minefield.tile[i][j].tileState = 0;
		}
		setColor(WHITE, BLACK);
		state = minefield.tile[i][j].adiacent_mines_num;
		if(minefield.tile[i][j].tileState == 2){
			setColor(LIME, BLACK);
			if(i == minefield.curs_y && j == minefield.curs_x)swapColors();
			cout<<flag;
		}else if(minefield.tile[i][j].tileState == 1){
			if(i == minefield.curs_y && j == minefield.curs_x)swapColors();
			char dirt = 176;
			cout<<dirt;
		}else if(minefield.tile[i][j].tileState == 0){
			setBackgroundColor(BLACK);
			switch(state){
				case 1:
					setTextColor(settings.Color_num_1);
				break;
				case 2:
					setTextColor(settings.Color_num_2);
				break;
				case 3:
					setTextColor(settings.Color_num_3);
				break;
				case 4:
					setTextColor(settings.Color_num_4);
				break;
				case 5:
					setTextColor(settings.Color_num_5);
				break;
				case 6:
					setTextColor(settings.Color_num_6);
				break;
				case 7:
					setTextColor(settings.Color_num_7);
				break;
				case 8:
					setTextColor(settings.Color_num_8);
				break;
				case 9:
					setTextColor(RED);
				break;
			}
			if(i == minefield.curs_y && j == minefield.curs_x && state != 0){
				swapColors();
			}else if(i == minefield.curs_y && j == minefield.curs_x && state == 0){
				setColor(BLACK, WHITE);
			}
			if(state <= 0){
				cout<<" ";
			}else if(state >= 9){
				cout<<mine;
			}else{
				cout<<state;
			}
		}
		setColor(WHITE, BLACK);
	}
}//end Render_mines

void Random_mine_insert(){
	int x;
	int y;
	srand(time(NULL));
	bool allow = true;
	int mines = minefield.mine_tot;
	minefield.flag_rem = minefield.mine_tot;
	for(int i = 0; i < minefield.height; i++){
		for(int j = 0; j < minefield.width; j++){
			minefield.tile[i][j].adiacent_mines_num = 0;
			minefield.tile[i][j].hasMine = false;
			minefield.tile[i][j].tileState = 1;
		}
	}
	
	for(int i = 0; i<mines; i++){
		do{
			allow = true;
			x = rand() % minefield.width + 0;
			y = rand() % minefield.height + 0;
			if(minefield.tile[y][x].hasMine == true){
				allow = false;
			}else{
				minefield.tile[y][x].hasMine = true;
			}
		}while(allow == false);
	}
	
	allow = false; // togliere un uguale
	for(int i = 0; i < minefield.height; i++){
		for(int j = 0; j < minefield.width; j++){
			if(minefield.tile[i][j].hasMine){
				Render_numbers(i, j);
			}
		}
	}
}//end Random_mine_insert

void Random_mine_insert2(int xPress, int yPress){
	int x;
	int y;
	srand(time(NULL));
	bool allow = true;
	int mines = minefield.mine_tot;
	minefield.flag_rem = minefield.mine_tot;
	for(int i = 0; i < minefield.height; i++){
		for(int j = 0; j < minefield.width; j++){
			minefield.tile[i][j].adiacent_mines_num = 0;
			minefield.tile[i][j].hasMine = false;
			minefield.tile[i][j].tileState = 1;
		}
	}
	
	for(int i = 0; i<mines; i++){
		do{
			allow = true;
			x = rand() % minefield.width + 0;
			y = rand() % minefield.height + 0;
			if(minefield.tile[y][x].hasMine || (x <= xPress+1 && x >= xPress-1 && y <= yPress+1 && y >= yPress-1)){
				allow = false;
			}else{
				minefield.tile[y][x].hasMine = true;
			}
		}while(allow == false);
	}
	
	allow = false;
	for(int i = 0; i < minefield.height; i++){
		for(int j = 0; j < minefield.width; j++){
			if(minefield.tile[i][j].hasMine){
				Render_numbers(i, j);
			}
		}
	}
}//end Random_mine_insert2

void Blank_minefield(){
	srand(time(NULL));
	bool allow = true;
	int mines = minefield.mine_tot;
	minefield.flag_rem = minefield.mine_tot;
	for(int i = 0; i < minefield.height; i++){
		for(int j = 0; j < minefield.width; j++){
			minefield.tile[i][j].adiacent_mines_num = 0;
			minefield.tile[i][j].tileState = 1;
		}
	}
}//end Blank_minefield

void Render_numbers(int y, int x){
    for(int i = -1; i < 2; i++){
        for(int j = -1; j < 2; j++){
            int ni = y + i; // Nuovo indice per la riga
            int nj = x + j; // Nuovo indice per la colonna
            
            //Controlli per evitare accessi fuori dai limiti dell'array
        
            if(ni >= 0 && ni < minefield.height && nj >= 0 && nj < minefield.width && minefield.tile[y][x].hasMine == true) {
                if(minefield.tile[ni][nj].adiacent_mines_num != 9){
                    minefield.tile[ni][nj].adiacent_mines_num++;
            		/*
					ni >= 0 verifica che l'indice di riga non sia inferiore a 0.
					ni < minefield.height verifica che l'indice di riga non sia maggiore o uguale all'altezza del campo minato.
					nj >= 0 verifica che l'indice di colonna non sia inferiore a 0.
					nj < minefield.width verifica che l'indice di colonna non sia maggiore o uguale alla larghezza del campo minato.
					*/
                }
            }
        }
    }
}//end Render_numbers

/*************************************/
/**FUNZIONI DI GESTIONE CAMPO MINATO**/
/*************************************/

bool isCurTileInState(int state){
	if(minefield.tile[minefield.curs_y][minefield.curs_x].tileState == state)return true;
	return false;
}//end isCurTileInState

void setCurTileState(int state){
	minefield.tile[minefield.curs_y][minefield.curs_x].tileState = state;
}//end setCurTileState

bool adiacentExists(int tile){
	switch(tile){
		case 0:
			if(0<=(minefield.curs_x-1) && (minefield.curs_x-1)<minefield.width && 0<=(minefield.curs_y-1) && (minefield.curs_y-1)<minefield.width)return true;
		break;
		case 1:
			if(0<=minefield.curs_x && minefield.curs_x<minefield.width && 0<=(minefield.curs_y-1) && (minefield.curs_y-1)<minefield.width)return true;
		break;
		case 2:
			if(0<=(minefield.curs_x+1) && (minefield.curs_x+1)<minefield.width && 0<=(minefield.curs_y-1) && (minefield.curs_y-1)<minefield.width)return true;
		break;
		case 3:
			if(0<=(minefield.curs_x-1) && (minefield.curs_x-1)<minefield.width && 0<=minefield.curs_y && minefield.curs_y-1<minefield.width)return true;
		break;
		case 4:
			if(0<=(minefield.curs_x+1) && (minefield.curs_x+1)<minefield.width && 0<=minefield.curs_y && minefield.curs_y-1<minefield.width)return true;
		break;
		case 5:
			if(0<=(minefield.curs_x-1) && (minefield.curs_x-1)<minefield.width && 0<=(minefield.curs_y+1) && (minefield.curs_y+1)<minefield.width)return true;
		break;
		case 6:
			if(0<=minefield.curs_x && minefield.curs_x<minefield.width && 0<=(minefield.curs_y+1) && (minefield.curs_y+1)<minefield.width)return true;
		break;
		case 7:
			if(0<=(minefield.curs_x+1) && (minefield.curs_x+1)<minefield.width && 0<=(minefield.curs_y+1) && (minefield.curs_y+1)<minefield.width)return true;
		break;
	}
	return false;
}//end adiacentExists

bool isAdiacentState(int tile, int state){
	switch(tile){
		case 0:
			if(minefield.tile[minefield.curs_y-1][minefield.curs_x-1].tileState == state)return true;
		break;
		case 1:
			if(minefield.tile[minefield.curs_y-1][minefield.curs_x].tileState == state)return true;
		break;
		case 2:
			if(minefield.tile[minefield.curs_y-1][minefield.curs_x+1].tileState == state)return true;
		break;
		case 3:
			if(minefield.tile[minefield.curs_y][minefield.curs_x-1].tileState == state)return true;
		break;
		case 4:
			if(minefield.tile[minefield.curs_y][minefield.curs_x+1].tileState == state)return true;
		break;
		case 5:
			if(minefield.tile[minefield.curs_y+1][minefield.curs_x-1].tileState == state)return true;
		break;
		case 6:
			if(minefield.tile[minefield.curs_y+1][minefield.curs_x].tileState == state)return true;
		break;
		case 7:
			if(minefield.tile[minefield.curs_y+1][minefield.curs_x+1].tileState == state)return true;
		break;
	}
	return false;
}//end isAdiacentState

int getCurTileAdiacentMinesNum(){
	return minefield.tile[minefield.curs_y][minefield.curs_x].adiacent_mines_num;
}//end getCurTileAdiacentMinesNum

void DetectPressMinefield(int button, string action){
	if(button == pressed){
		if(isCurTileInState(1) && minefield.flag_rem > 0 && action == "changeFlag"){
			setCurTileState(2);
			minefield.flag_rem--;
		}else if(isCurTileInState(2) && action == "changeFlag"){
			setCurTileState(1);
			minefield.flag_rem++;
		}else if(isCurTileInState(1) && action == "uncover"){
			setCurTileState(0);
			if(getCurTileAdiacentMinesNum() == 0){
				fillUncover();
			}
		}
	}
}//end DetectPressMinefield

void AutoPressMinefield(string action){
	if(isCurTileInState(1) && minefield.flag_rem > 0 && action == "changeFlag"){
		setCurTileState(2);
		minefield.flag_rem--;
	}else if(isCurTileInState(2) && action == "changeFlag"){
		setCurTileState(1);
		minefield.flag_rem++;
	}else if(isCurTileInState(1) && action == "uncover"){
		setCurTileState(0);
		if(getCurTileAdiacentMinesNum() == 0){
			fillUncover();
		}
	}
}//end AutoPressMinefield

void UncoverNear(int x, int y){
	for(int i = -1; i < 2; i++){
        for(int j = -1; j < 2; j++){
			if((j != 0 && i != 0) || y+i>=0 || y+i<minefield.height || x+j>=0 || x+j<minefield.width){
				if(minefield.tile[y+i][x+j].tileState == 1){
					minefield.tile[y+i][x+j].tileState = 0;
					if(minefield.tile[y+i][x+j].adiacent_mines_num == 0){
						
					}
				}
			}
		}
	}
}//end UncoverNear

void fillUncover(){
	int rappArr[minefield.height][minefield.width];
	
	//generare il campo iniziale
	for(int y = 0; y<minefield.height; y++){
		for(int x = 0; x<minefield.width; x++){
			rappArr[y][x]=0;
			if(y == minefield.curs_y && x == minefield.curs_x)rappArr[y][x]=1;
		}
	}
	
	//genera le caselle da scoprire
	bool finished = true;
	do{
		finished = true;
		for(int y = 0; y<minefield.height; y++){
			for(int x = 0; x<minefield.width; x++){
				if(rappArr[y][x] == 1){
					finished = false;
					for(int i = -1; i < 2; i++){
        				for(int j = -1; j < 2; j++){
							int ny = y + i; // Nuovo indice per la riga
							int nx = x + j; // Nuovo indice per la colonna
            				//Controlli per evitare accessi fuori dai limiti dell'array
            				if(ny >= 0 && ny < minefield.height && nx >= 0 && nx < minefield.width){
                				if((i != 0 || j != 0) && rappArr[ny][nx] == 0){
                					if(minefield.tile[ny][nx].adiacent_mines_num == 0){
										rappArr[ny][nx] = 1;
									}else{
										rappArr[ny][nx] = 2;
									}
								}
            					/*
								ny >= 0 verifica che l'indice di riga non sia inferiore a 0.
								ny < minefield.height verifica che l'indice di riga non sia maggiore o uguale all'altezza del campo minato.
								nx >= 0 verifica che l'indice di colonna non sia inferiore a 0.
								nx < minefield.width verifica che l'indice di colonna non sia maggiore o uguale alla larghezza del campo minato.
								*/
        					}
    					}
					}
					rappArr[y][x] = 2;
				}
			}
		}
	}while(finished == false);
	
	//scoprire gli spazi corretti
	for(int y = 0; y<minefield.height; y++){
		for(int x = 0; x<minefield.width; x++){
			if(rappArr[y][x]==2)minefield.tile[y][x].tileState = 0;
		}
	}
}//end fillUncover

/************************************/
/**FUNZIONI DI GENERAZIONE DEI MENU**/
/************************************/

void GameIntro(){
	setColor(WHITE, BLACK);
    cout<<"MINESWEEPER"<<endl;
    Sleep(500);
    system("cls");
    cout<<"MINESWEEPER IN"<<endl;
    Sleep(500);
    system("cls");
    cout<<"MINESWEEPER IN C"<<endl;
    Sleep(500);
    system("cls");
    cout<<"MINESWEEPER IN C+"<<endl;
    Sleep(500);
    system("cls");
}//end GameIntro

void Menu(){
	one_dim_pos = 0;
	do{
		setColor(WHITE, BLACK);
		cout<<"MINESWEEPER IN C++"<<endl<<endl<<endl;
		optionHighlight("Play", 0, one_dim_pos);
		cout<<endl;
		optionHighlight("Settings", 1, one_dim_pos);
		cout<<endl;
		optionHighlight("Credits", 2, one_dim_pos);
		cout<<endl<<endl;
		optionHighlight("exit", 3, one_dim_pos);
		cout<<endl;
		WaitForPress();
		
		MoveInMenu(119, 115, one_dim_pos, 0, 3);
		MoveInMenu(72, 80, one_dim_pos, 0, 3);
		
		DetectPressMenu(13, 0, "Difficulty");
		DetectPressMenu(32, 0, "Difficulty");
		
		DetectPressMenu(13, 1, "Settings");
		DetectPressMenu(32, 1, "Settings");
		
		DetectPressMenu(13, 2, "Credits");
		DetectPressMenu(32, 2, "Credits");

		DetectPressMenu(13, 3, "GameExit");
		DetectPressMenu(32, 3, "GameExit");
		
		system("cls");
	}while(curMenu == "Main");
}//end Menu

void Difficulty(){
	one_dim_pos = 0;
	do{
		cout<<"CLASSIC"<<endl<<endl;
		optionHighlight("Easy", 0, one_dim_pos);
		cout<<endl;
		optionHighlight("Medium", 1, one_dim_pos);
		cout<<endl;
		optionHighlight("Hard", 2, one_dim_pos);
		cout<<endl;
		optionHighlight("Expert", 3, one_dim_pos);
		cout<<endl<<endl;
		cout<<"ENDLESS (WIP)"<<endl<<endl;
		cout<<"SHUFFLE (WIP)"<<endl<<endl;
		
		optionHighlight("exit", 4, one_dim_pos);
		cout<<endl;
		WaitForPress();

		MoveInMenu(119, 115, one_dim_pos, 0, 4);
		MoveInMenu(72, 80, one_dim_pos, 0, 4);
		
		DetectPressMenu(13, 4, "Main");
		DetectPressMenu(32, 4, "Main");
		if((pressed == 13 || pressed == 32) && one_dim_pos != 4){
			switch (one_dim_pos)
			{
				case 0:
					minefield.width = 10;
					minefield.height = 8;
					minefield.mine_tot = 10;
				break;
				case 1:
					minefield.width = 18;
					minefield.height = 14;
					minefield.mine_tot = 40;
				break;
				case 2:
					minefield.width = 24;
					minefield.height = 20;
					minefield.mine_tot = 99;
				break;
				case 3:
					minefield.width = 28;
					minefield.height = 24;
					minefield.mine_tot = 110;
				break;
				default:
				break;
			}
			minefield.flag_rem = minefield.mine_tot;
			curMenu = "Playfield";
		}
		system("cls");
	}while(curMenu == "Difficulty");
}//end Menu

void Playfield(){
	int gameState = 0;
	minefield.curs_x = minefield.width/2;
	minefield.curs_y = minefield.height/2;
	if(settings.oldGeneration == false){
		Blank_minefield();
		do{
			system("cls");
			Render_minefield(-2);
			WaitForPress();
		
			MoveInMenu(119, 115, minefield.curs_y, 0, minefield.height-1);
			MoveInMenu(72, 80, minefield.curs_y, 0, minefield.height-1);
		
			MoveInMenu(97, 100, minefield.curs_x, 0, minefield.width-1);
			MoveInMenu(75, 77, minefield.curs_x, 0, minefield.width-1);
		
			DetectPressMinefield(32, "uncover");
		}while(pressed != 32);
		Random_mine_insert2(minefield.curs_x, minefield.curs_y);
		Render_minefield(0);
		AutoPressMinefield("uncover");
	}else{
		Random_mine_insert();
	}
	
	do{
		system("cls");
		Render_minefield(0);
		WaitForPress();
		
		MoveInMenu(119, 115, minefield.curs_y, 0, minefield.height-1);
		MoveInMenu(72, 80, minefield.curs_y, 0, minefield.height-1);
		
		MoveInMenu(97, 100, minefield.curs_x, 0, minefield.width-1);
		MoveInMenu(75, 77, minefield.curs_x, 0, minefield.width-1);
		
		DetectPressMinefield(13, "changeFlag");
		DetectPressMinefield(32, "uncover");
		DetectPressMinefield(27, "exit");
		if(pressed == 32 || pressed == 13){
			gameState = changeGameState();
		}else if(pressed == 27){
			gameState = -2;
		}
	}while(gameState == 0);
	system("cls");
	if(gameState != -2){
		Render_minefield(gameState);
		WaitForPress();
		system("cls");
	}
	curMenu = "Main";
}//end Playfield

void Settings(){
	one_dim_pos = 0;
	do{
		system("cls");
		setColor(WHITE, BLACK);
		cout<<"SETTINGS"<<endl<<endl;
		boolOptionHighlight("old generation", 0, one_dim_pos, settings.oldGeneration);
		cout<<endl;
		boolOptionHighlight("show secondary info", 1, one_dim_pos, settings.secondaryInfo);
		cout<<endl<<endl;
		optionHighlight("exit", 2, one_dim_pos);
		cout<<endl;
		
		WaitForPress();
		
		MoveInMenu(119, 115, one_dim_pos, 0, 2);
		MoveInMenu(72, 80, one_dim_pos, 0, 2);
		
		DetectPressMenu(13, 2, "Main");
		DetectPressMenu(32, 2, "Main");
		if((pressed == 13 || pressed == 32) && one_dim_pos == 0){
			settings.oldGeneration = !settings.oldGeneration;
		}else if((pressed == 13 || pressed == 32) && one_dim_pos == 1){
			settings.secondaryInfo = !settings.secondaryInfo;
		}
	}while(curMenu == "Settings");
	curMenu = "Main";
	system("cls");
}

void Credits(){
	setColor(PURPLE, BLACK);
	cout<<"An_enderman: Programmer and main developer"<<endl<<endl;
	setColor(WHITE, BLACK);
	cout<<"Prof. Massimo Danese: Programmer (made the code for the numbers that indicates the amounth of adiacent mines)"<<endl<<endl;
	cout<<"Betatester:"<<endl;
	setColor(FUCHSIA, BLACK);
	cout<<"Pierluigi Speranza"<<endl;
	setColor(GRAY, BLACK);
	cout<<"*press any key to exit credits*";
	WaitForPress();
	curMenu = "Main";
	system("cls");
}//end Credits

/*********************************/
/**FUNZIONI DI GESTIONE DEI MENU**/
/*********************************/

void DetectPressMenu(int button, int pos, string action){
	if(pos == -1){
		if(button == pressed){
			
		}
	}else{
		if(button == pressed && pos == one_dim_pos){
			if(action == "Difficulty"){
				curMenu = "Difficulty";
			}else if(action == "Playfield"){
				curMenu = "Playfield";
			}else if(action == "Settings"){
				curMenu = "Settings";
			}else if(action == "GameExit"){
				curMenu = "GameExit";
			}else if(action == "Main"){
				curMenu = "Main";
			}else if(action == "Credits"){
				curMenu = "Credits";
			}
		}		
	}
}//end DetectPress

void optionHighlight(string text, int activationNum, int curNum){
	if(curNum == activationNum)cout<<">";
	cout<<text;
	if(curNum == activationNum)cout<<"<";
}//end optionHighlight

void boolOptionHighlight(string text, int activationNum, int curNum, bool var){
	if(curNum == activationNum)cout<<">";
	cout<<text<<" [";
	if(var){
		cout<<"X";
	}else{
		cout<<" ";
	}
	cout<<"]";
	if(curNum == activationNum)cout<<"<";
}//end boolOptionHighlight

void MoveInMenu(int buttonDec, int buttonInc, int &pos, int dirmin, int dirmax){
	if(buttonDec == pressed){
		pos--;
	}else if(buttonInc == pressed)
		pos++;
	if(pos < dirmin){
		pos=dirmax;
	}else if(pos > dirmax)pos=dirmin;
}//end MoveInMenu

/*****************/
/**MISCELLANEOUS**/
/*****************/

int changeGameState(){
	int flagged_mines = 0;
	int uncovered_tiles = 0; 
	int tot_safe_tiles = (minefield.height * minefield.width)-minefield.mine_tot;
	for(int y = 0; y<minefield.height; y++){
		for(int x = 0; x<minefield.width; x++){
			if(minefield.tile[y][x].hasMine && minefield.tile[y][x].tileState == 0){
				return -1;
			}else if(minefield.tile[y][x].hasMine && minefield.tile[y][x].tileState == 2){
				flagged_mines++;
			}else if(minefield.tile[y][x].hasMine == false && minefield.tile[y][x].tileState == 0){
				uncovered_tiles++;
			}
		}
	}
	if(minefield.mine_tot == flagged_mines && uncovered_tiles == tot_safe_tiles){
		return 1;
	}else{
		return 0;	
	}
}//end changeGameState

void setColor(int colorText, int colorBackgr){
	CurColorNum = (colorBackgr * 16) + colorText;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, CurColorNum);
}//end setColor

void setBackgroundColor(int colorBackgr){
	int prevTextColor=CurColorNum%16;
	CurColorNum = (colorBackgr * 16) + prevTextColor;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, CurColorNum);
}//end setBackgroundColor

void setTextColor(int colorText){
	int prevBackgroundColor=CurColorNum/16;
	CurColorNum = (prevBackgroundColor * 16) + colorText;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, CurColorNum);
}//end setBackgroundColor

void swapColors(){
	int bg, text;
	bg = CurColorNum/16;
	text = CurColorNum%16;
	setColor(bg, text);
}//end swapColors

void ReloadOnButtonPress(int button){
	if(pressed == button){
		system("cls");
		Render_minefield(0);
	}
}//end ReloadOnButtonPress

void WaitForPress(){
	pressed = getch();
}
