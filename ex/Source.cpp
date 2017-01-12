#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <math.h>
#include <windows.h>
#include <wchar.h>
#include <io.h>
#include <fcntl.h>

#define length  25
#define width   80
#define true     1
#define false    0


void gotoxy    (int xpos, int ypos)

{

	COORD scrn;

	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);

	scrn.X = xpos + 8; scrn.Y = ypos + 1;       //調整起始位置

	SetConsoleCursorPosition(hOuput, scrn); //設定游標位置

}
void showCursor(int visible)
{
	CONSOLE_CURSOR_INFO ConCurInf;

	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	GetConsoleCursorInfo(hStdOut, &ConCurInf);

	ConCurInf.bVisible = visible;

	SetConsoleCursorInfo(hStdOut, &ConCurInf);

}


typedef struct snake
{
	int x;
	int y;
}Snake;

Snake sBody[2500]; // range = 20*50 = 1000


void printSnake(int i){

	gotoxy(sBody[i].x, sBody[i].y);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | 0X0e);
	_setmode(_fileno(stdout), _O_U16TEXT);   //使用Unicode
	wprintf(L"■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | 0X0f);
}



void printWall(){

	int i, j;
	

	for (i = 0; i<length; i++){

		for (j = 0; j<width+1; j += 2){

			if (i == 0 || i == length-1 || j == 0 || j == width)
			{
				gotoxy(j, i);
				_setmode(_fileno(stdout), _O_U16TEXT);   //使用Unicode
				wprintf(L"■");

			}

		}

	}

}



void printInformation(int food, int speed){

	gotoxy(width+4, 1);

	wprintf(L"Use + to increase the");

	gotoxy(width + 4, 2);

	wprintf(L"snake moving speed.");

	gotoxy(width + 4, 4);

	wprintf(L"Use - to decrease the");

	gotoxy(width + 4, 5);

	wprintf(L"snake moving speed.");

	gotoxy(width + 4, 7);

	wprintf(L"Use P to pause.");

	gotoxy(width + 4, 9);

	wprintf(L"Grade: %d", food * 100);

	gotoxy(width + 4, 11);

	if (speed == 10) wprintf(L"Speed: %3d (The slowest)", 310 - speed);

	else if (speed == 300) wprintf(L"Speed: %3d (The fastest)", 310 - speed);

	else wprintf(L"Speed: %3d              ", 310 - speed);

}



void setSite(int i, int site_x, int site_y){

	sBody[i].x = site_x;

	sBody[i].y = site_y;

}



void setStartSite(){

	int i;

	sBody[0].x = sBody[1].x = sBody[2].x = sBody[3].x = sBody[4].x = 2;

	for (i = 0; i<5; i++) sBody[i].y = abs(5 - i);

}



int main(int argc, char *argv[])

{

	int startBodyLenght = 5, startEatenFood = 0, bodyLenght, eatenFood;

	int keyinFirst, keyinSecond;

	int i, j, gameOver = false, isFoodEaten = false, xyChanged = false;

	int path = 2; // 方向

	int snakeSpeed = 100;

	char gameKey = 'y';

	Snake foodSite, coor, last;



	srand(time(NULL));

	showCursor(0);

	while (1)
	{

		if (gameKey == 'n' || gameKey == 'N') break;

		// 初始狀態

		setStartSite();

		printWall();

		bodyLenght = startBodyLenght;

		eatenFood = startEatenFood;

		coor.x = 2;

		coor.y = 4;

		path = 2;

		isFoodEaten = false;

		gameOver = false;

		xyChanged = false;

		snakeSpeed = 100;

		printInformation(startEatenFood, snakeSpeed);

		for (i = 0; i<bodyLenght; i++)

			printSnake(i);



		do{

			gotoxy(width + 4, 13);

			wprintf(L"Enter to start.");

			gameKey = _getch();

			gotoxy(width + 4, 13);

			wprintf(L"               ");

		} while (gameKey != 13);



		while (!gameOver)
		{

			printInformation(eatenFood, snakeSpeed);

			if (!isFoodEaten){ // 如果沒食物, 隨機算出一個食物的位置

				foodSite.x = (rand() % 38 * 2) + 2;

				foodSite.y = rand() % 17 + 2;

				isFoodEaten = true;

			}

			gotoxy(foodSite.x, foodSite.y);

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | 0X04);
			_setmode(_fileno(stdout), _O_U16TEXT);   //使用Unicode
			wprintf(L"■");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | 0X0f);

			xyChanged = false;



			if (_kbhit()){ // 鍵盤敲擊

				keyinFirst = _getch();

				if (keyinFirst == 224) {

					keyinSecond = _getch();

					switch (keyinSecond)
					{

					case  72: /* up, 1 */
					
						if (path != 2)
						{
							coor.y--;
							path = 1;
							xyChanged = true;
						}
						break;

					case  80: /* down, 2 */
					 
						if (path != 1)
						{
							coor.y++;
							path = 2;
							xyChanged = true;
						}
						break;

					case  75: /* left, 3 */
						
						if (path != 4)
						{
							coor.x -= 2;
							path = 3;
							xyChanged = true;
						}
						break;

					case  77: /* right, 4 */
						
						if (path != 3)
						{
							coor.x += 2;
							path = 4;
							xyChanged = true;
						}	
						break;

					}


				}

				if (keyinFirst == 'p' || keyinFirst == 'P'){

					do{

						gotoxy(55, 13);

						wprintf(L"Pause.");

						gameKey = _getch();

						gotoxy(55, 13);

						wprintf(L"          ");

					} while (gameKey != 'p' && gameKey != 'P');

				}

				

			}
			if(xyChanged == false) { // 沒有任何動作則順著path(方向)前進

				switch (path){

				case  1: /* up, 1 */

					if (path != 2)
					{
						coor.y--;
						path = 1;
					}
					break;

				case  2: /* down, 2 */

					if (path != 1)
					{
						coor.y++;
						path = 2;
					}
					break;

				case  3: /* left, 3 */

					if (path != 4)
					{
						coor.x -= 2;
						path = 3;
					}
					break;

				case  4: /* right, 4 */

					if (path != 3)
					{
						coor.x += 2;
						path = 4;
					}
					break;

				}

				xyChanged = true;

			}
			for (i = 1; i<bodyLenght; i++){ /* Did snake die? */

				if (coor.x == sBody[i].x && coor.y == sBody[i].y)

					gameOver = true;

			}

			if (gameOver == 0 && xyChanged == 1){ /* Snake moving */

				if (coor.x>0 && coor.x<width-1 && coor.y>0 && coor.y<length-1)
				{

					last.x = sBody[bodyLenght - 1].x;

					last.y = sBody[bodyLenght - 1].y;

					for (i = bodyLenght - 1; i >= 0; i--){

						if (i == 0)

							setSite(i, coor.x, coor.y);

						else

							setSite(i, sBody[i - 1].x, sBody[i - 1].y);

					}

					for (i = 0; i<bodyLenght; i++){

						printSnake(i);

					}

					snakeSpeed = -0.0002*bodyLenght*bodyLenght*bodyLenght + 0.040*bodyLenght*bodyLenght - 3.4088*bodyLenght + 113.52;
					Sleep(snakeSpeed);
					if (last.x != 0 && last.y != 0)
					{
						gotoxy(last.x, last.y); // 清尾巴

						wprintf(L"  ");
					}

				}
				else if (coor.x==0)
				{
					coor.x = width;
				}
				else if (coor.x == width)
				{
					coor.x = 0;
				}
				else if (coor.y == length-1)
				{
					coor.y = 0;
				}
				else if (coor.y == 0)
				{
					coor.y = length-1;
				}
				else gameOver = true;



				if (sBody[0].x == foodSite.x && sBody[0].y == foodSite.y){ /* Snake ate food */

					isFoodEaten = false;

					if (bodyLenght<1000) bodyLenght++; // 避免蛇身超出陣列長度

					eatenFood++;

				}

			}

		}

		/* Game over ? */

		while (1){

			system("CLS");

			wprintf(L"Your Grade: %d\n", eatenFood * 100);

			wprintf(L"Game over.\nPlay again? (y/n) \n");

			gameKey = _getch();

			if (gameKey == 'y' || gameKey == 'Y' || gameKey == 'n' || gameKey == 'N'){

				system("CLS");

				break;

			}

		}

	}

	wprintf(L"Bye!\n");

	system("pause");

	return 0;

}
