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
	0-8: mines nearby
	
	9 has mine
	10 flagged
**/
struct s_tile{
	bool flagged;
	bool covered;
	int tile_state;
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
	int Color_num_1;
	int Color_num_2;
	int Color_num_3;
	int Color_num_4;
	int Color_num_5;
	int Color_num_6;
	int Color_num_7;
	int Color_num_8;
	int BgColor;
	int BorderColor;
	int textColor;
}settings;

int CurColorNum = 15;
bool Uncover = false;
bool game_ended = false;
string curMenu = "main";

void imput_data();//Permette di dare in imput i dati del campo

/**FUNZIONI DI GENERAZIONE DEL CAMPO MINATO**/
void Render_minefield(int);//disegna sulla prompt dei comandi il campo minato
void Render_frame(int, int, int);//disegna la cornice del campo minato
void Render_mines(int, int, int);//disegna le mine sul campo minato
void Render_numbers(int, int);//disegna i numeri sul campo minato
void Random_mine_insert();//inserisce mine a caso nel campo minato

/**FUNZIONI DI GENERAZIONE DEI MENU**/
void GameIntro();
void Menu();
void Difficulty();
void Playfield();
void Credits();

/**MISCELLANEOUS**/
void optionHighlight(string, string, string, int, int);
void MoveInMenuV2(int, int, int, int&, int, int);
void DetectPress(int, int, int, int, string);
int changeGameState();
void DetectPressMinefield(int, int, string);
void setColorV2(int, int);
void swapColors();
void UncoverAdiacent();
void ReloadLanguage(string);

int main(){
	srand(time(NULL));
	GameIntro();
	do{
	
		if(curMenu == "main"){
			Menu();
		}
		
		if(curMenu == "Difficulty"){
			Difficulty();
		}
		
		if(curMenu == "Playfield"){
			Playfield();
		}
		
		if(curMenu == "Credits"){
			Credits();
		}
	}while(curMenu != "gameExit");
	
	return 0;
}//fine main

void imput_data(){
	do{
		
	}while(minefield.width < 3);
	do{
		
	}while(minefield.height < 3);
}//end imput_data

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
	if(state > 0){
		cout<<"You won"<<endl<<endl;
		setColorV2(GRAY, BLACK);
		cout<<"*press any key*";
	}else if(state<0){
		cout<<"Game over"<<endl<<endl;
		setColorV2(GRAY, BLACK);
		cout<<"*press any key*";
	}else{
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
		setColorV2(RED, BLACK);
	}else if(state == 1){
		setColorV2(GREEN, BLACK);
	}else{
		setColorV2(WHITE, BLACK);
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
	state = minefield.tile[i][j].tile_state;
	if(gamestate > 0){
		minefield.tile[i][j].covered = false;
		setColorV2(GREEN, BLACK);
		if(state == 9){
			cout<<flag;
		}else if(state == 0){
			cout<<" ";
		}else{
			cout<<minefield.tile[i][j].tile_state;
		}
	}else if(gamestate < 0){
		minefield.tile[i][j].covered = false;
		setColorV2(RED, BLACK);
		if(state == 9){
			cout<<mine;
		}else if(state == 0){
			cout<<" ";
		}else{
			cout<<minefield.tile[i][j].tile_state;
		}
	}else{
		if(Uncover == true){
			minefield.tile[i][j].covered = false;
		}
		setColorV2(WHITE, BLACK);
		state = minefield.tile[i][j].tile_state;
		if(minefield.tile[i][j].covered == true && minefield.tile[i][j].flagged == true){
			setColorV2(LIME, BLACK);
			if(i == minefield.curs_y && j == minefield.curs_x)swapColors();
			cout<<flag;
		}else if(minefield.tile[i][j].covered == true && minefield.tile[i][j].flagged == false){
			if(i == minefield.curs_y && j == minefield.curs_x)swapColors();
			char dirt = 176;
			cout<<dirt;
		}else if(minefield.tile[i][j].covered == false && minefield.tile[i][j].flagged == false){
			switch(state){
				case 1:
					setColorV2(BLUE, BLACK);
				break;
				case 2:
					setColorV2(GREEN, BLACK);
				break;
				case 3:
					setColorV2(RED, BLACK);
				break;
				case 4:
					setColorV2(NAVY, BLACK);
				break;
				case 5:
					setColorV2(MAROON, BLACK);
				break;
				case 6:
					setColorV2(AQUA, BLACK);
				break;
				case 7:
					setColorV2(WHITE, BLACK);
				break;
				case 8:
					setColorV2(GRAY, BLACK);
				break;
				case 9:
					setColorV2(RED, BLACK);
				break;
			}
			if(i == minefield.curs_y && j == minefield.curs_x && state != 0){
				swapColors();
			}else if(i == minefield.curs_y && j == minefield.curs_x && state == 0){
				setColorV2(BLACK, WHITE);
			}
			if(state <= 0){
				cout<<" ";
			}else if(state >= 9){
				cout<<mine;
			}else{
				cout<<state;
			}
		}
		setColorV2(WHITE, BLACK);
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
			minefield.tile[i][j].tile_state = 0;
			minefield.tile[i][j].covered = true;
			minefield.tile[i][j].flagged = false;
		}
	}
	
	for(int i = 0; i<mines; i++){
		do{
			allow = true;
			x = rand() % minefield.width + 0;
			y = rand() % minefield.height + 0;
			if(minefield.tile[y][x].tile_state == 9){
				allow = false;
			}else{
				minefield.tile[y][x].tile_state = 9;
			}
		}while(allow == false);
	}
	
	allow = false; // togliere un uguale
	for(int i = 0; i < minefield.height; i++){
		for(int j = 0; j < minefield.width; j++){
			if(minefield.tile[i][j].tile_state == 9){
				Render_numbers(i, j);
			}
		}
	}
}//end Random_mine_insert

void Render_numbers(int y, int x){
    for(int i = -1; i < 2; i++){
        for(int j = -1; j < 2; j++){
            int ni = y + i; // Nuovo indice per la riga
            int nj = x + j; // Nuovo indice per la colonna
            
            //Controlli per evitare accessi fuori dai limiti dell'array
        
            if(ni >= 0 && ni < minefield.height && nj >= 0 && nj < minefield.width && minefield.tile[y][x].tile_state == 9) {
                if(minefield.tile[ni][nj].tile_state != 9){
                    minefield.tile[ni][nj].tile_state++;
                    
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


/************************************/
/**FUNZIONI DI GENERAZIONE DEI MENU**/
/************************************/

void GameIntro(){
	setColorV2(WHITE, BLACK);
    cout<<"After an year of coding"<<endl;
    Sleep(1000);
    system("cls");
    cout<<"I present to you"<<endl;
    Sleep(1000);
    system("cls");
    cout<<"MINESWEEPER"<<endl;
    Sleep(1000);
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
}//GameIntro

void Menu(){
	int pressed = 1;
	int curOpt = 0;
	do{
		setColorV2(WHITE, BLACK);
		cout<<"MINESWEEPER IN C++"<<endl<<endl<<endl;
		optionHighlight(">", "Play", "<", 0, curOpt);
		cout<<endl;
		optionHighlight(">", "Settings(WIP)", "<", 1, curOpt);
		cout<<endl;
		optionHighlight(">", "Credits", "<", 2, curOpt);
		cout<<endl<<endl;
		optionHighlight(">", "exit", "<", 3, curOpt);
		cout<<endl;
		pressed = getch();
		
		MoveInMenuV2(pressed, 119, 115, curOpt, 0, 3);
		MoveInMenuV2(pressed, 72, 80, curOpt, 0, 3);
		
		DetectPress(13, pressed, 0, curOpt, "DifficultySelect");
		DetectPress(32, pressed, 0, curOpt, "DifficultySelect");
		
		DetectPress(13, pressed, 2, curOpt, "toCredits");
		DetectPress(32, pressed, 2, curOpt, "toCredits");

		DetectPress(13, pressed, 3, curOpt, "exit");
		DetectPress(32, pressed, 3, curOpt, "exit");
		
		system("cls");
		
	}while(curMenu == "main");
}//end Menu

void Difficulty(){
	int pressed = 1;
	int curOpt = 0;
	do{
		cout<<"Choose the difficulty"<<endl<<endl;
		optionHighlight(">", "Easy", "<", 0, curOpt);
		cout<<endl;
		optionHighlight(">", "Medium", "<", 1, curOpt);
		cout<<endl;
		optionHighlight(">", "Hard", "<", 2, curOpt);
		cout<<endl<<endl;
		optionHighlight(">", "exit", "<", 3, curOpt);
		cout<<endl;
		pressed = getch();

		MoveInMenuV2(pressed, 119, 115, curOpt, 0, 3);
		MoveInMenuV2(pressed, 72, 80, curOpt, 0, 3);
		
		DetectPress(13, pressed, 3, curOpt, "toMain");
		DetectPress(32, pressed, 3, curOpt, "toMain");
		if((pressed == 13 || pressed == 32) && curOpt != 3){
			if(curOpt == 0){
				minefield.width = 10;
				minefield.height = 8;
				minefield.mine_tot = 10;
			}else if(curOpt == 1){
				minefield.width = 18;
				minefield.height = 14;
				minefield.mine_tot = 40;
			}else if(curOpt == 2){
				minefield.width = 24;
				minefield.height = 20;
				minefield.mine_tot = 99;
			}
			minefield.flag_rem = minefield.mine_tot;
			curMenu = "Playfield";
		}
		system("cls");
	}while(curMenu == "Difficulty");
}//end Menu

void Playfield(){
	int gameState = 0;
	int pressed;
	Random_mine_insert();
	minefield.curs_x = minefield.width/2;
	minefield.curs_y = minefield.height/2;
	do{
		system("cls");
		Render_minefield(0);
		//cout<<endl<<"gamestate:"<<gameState<<endl;
		pressed = getch();
		
		MoveInMenuV2(pressed, 119, 115, minefield.curs_y, 0, minefield.height-1);
		MoveInMenuV2(pressed, 72, 80, minefield.curs_y, 0, minefield.height-1);
		
		MoveInMenuV2(pressed, 97, 100, minefield.curs_x, 0, minefield.width-1);
		MoveInMenuV2(pressed, 75, 77, minefield.curs_x, 0, minefield.width-1);
		
		DetectPressMinefield(13, pressed, "a");
		DetectPressMinefield(32, pressed, "a");
		if(pressed == 32 || pressed == 13){
			gameState = changeGameState();
		}
	}while(gameState == 0);
	system("cls");
	Render_minefield(gameState);
	pressed = getch();
	curMenu = "main";
	system("cls");
}//end Playfield

void Credits(){
	int press;
	setColorV2(PURPLE, BLACK);
	cout<<"An_enderman: Programmer and main developer"<<endl<<endl;
	setColorV2(WHITE, BLACK);
	cout<<"Prof. Massimo Danese: Programmer (made the code for the numbers that indicates the amounth of adiacent mines)"<<endl<<endl;
	cout<<"Betatester:"<<endl;
	setColorV2(FUCHSIA, BLACK);
	cout<<"Pierluigi Speranza"<<endl;
	setColorV2(GRAY, BLACK);
	cout<<"*press any key to exit credits*";
	press = getch();
	curMenu = "main";
	system("cls");
}//end Credits

/*****************/
/**MISCELLANEOUS**/
/*****************/

void optionHighlight(string h1, string text, string h2, int activationNum, int curNum){
	if(curNum == activationNum)cout<<h1;
	cout<<text;
	if(curNum == activationNum)cout<<h2;
}//end optionHighlight

void MoveInMenuV2(int buttonPr, int buttonDec, int buttonInc, int &pos, int dirmin, int dirmax){
	if(buttonDec == buttonPr){
		pos--;
	}else if(buttonInc == buttonPr)pos++;
	
	if(pos < dirmin){
		pos=dirmax;
	}else if(pos > dirmax)pos=dirmin;
}//end MoveInMenuV2

void DetectPress(int button, int buttonPr, int pos, int curPos, string action){
	if(pos == -1){
		if(button == buttonPr){
			
		}
	}else{
		if(button == buttonPr && pos == curPos){
			if(action == "DifficultySelect"){
				curMenu = "Difficulty";
			}else if(action == "play"){
				curMenu = "Playfield";
			}else if(action == "exit"){
				curMenu = "gameExit";
			}else if(action == "toMain"){
				curMenu = "main";
			}else if(action == "toCredits"){
				curMenu = "Credits";
			}
		}		
	}
}//end DetectPress

int changeGameState(){
	int flagged_mines = 0;
	int uncovered_tiles = 0; 
	int tot_safe_tiles = (minefield.height * minefield.width)-minefield.mine_tot;
	for(int y = 0; y<minefield.height; y++){
		for(int x = 0; x<minefield.width; x++){
			if(minefield.tile[y][x].tile_state == 9 && minefield.tile[y][x].covered == false){
				return -1;
			}else if(minefield.tile[y][x].tile_state == 9 && minefield.tile[y][x].flagged == true){
				flagged_mines++;
			}else if(minefield.tile[y][x].tile_state < 9 && minefield.tile[y][x].covered == false){
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

void DetectPressMinefield(int button, int buttonPr, string action){
	int tempX = minefield.curs_x;
	int tempY = minefield.curs_y;
	if(button == buttonPr){
		if(minefield.tile[tempY][tempX].covered == true && minefield.tile[tempY][tempX].flagged == false && minefield.flag_rem > 0 && button == 13){
			minefield.tile[tempY][tempX].flagged = true;
			minefield.flag_rem--;
		}else if(minefield.tile[tempY][tempX].covered == true && minefield.tile[tempY][tempX].flagged == true && button == 13){
			minefield.tile[tempY][tempX].flagged = false;
			minefield.flag_rem++;
		}else if(minefield.tile[tempY][tempX].covered == true && minefield.tile[tempY][tempX].flagged == false && button == 32){
			minefield.tile[tempY][tempX].covered = false;
			if(minefield.tile[tempY][tempX].tile_state == 0){
				UncoverAdiacent();
			}
		}
	}
}

void setColorV2(int colorText, int colorBackgr){
	CurColorNum = (colorBackgr * 16) + colorText;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, CurColorNum);
}//end setColorV2

void swapColors(){
	int bg, text;
	bg = CurColorNum/16;
	text = CurColorNum%16;
	setColorV2(bg, text);
}//end swapColors

void UncoverAdiacent(){
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
                					if(minefield.tile[ny][nx].tile_state == 0){
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
			if(rappArr[y][x]==2)minefield.tile[y][x].covered = false;
		}
	}
}//end UncoverAdiacent

void PreloadLanguage(string lang){
	
}
