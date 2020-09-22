#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>
#include<conio.h>

#define SPACE 32 //空格键 旋转 
#define LEFT  75 //左方向键 向左移动 
#define RIGHT 77 //右方向键 向右移动
#define DOWN 80 //下方向键 加速向下移动 
#define ESC 27 //Esc键 退出 
#define Wall 2 //围墙 
#define Box 1 //方块 
#define Kong 0 
#define FACE_X 30 //方块长度 
#define FACE_Y 20 //方块高度 

void start_game(); //游戏循环 
void gotoxy(int x, int y); //设置光标位置 
void hidden_cursor(); //隐藏光标 
int color(int c); //设置颜色 
void init_face(); //初始化界面 
void init_dia(); //初始化方块形状 
void draw_dia(int base, int space_t, int x, int y); //画方块 
void draw_space(int base, int space_t, int x, int y); //画空格 
int border(int n, int space_t, int x, int y); //边界检测 
int del(); //删除增加分数 
void read_file(); //读入最高纪录 
void write_file(); //写入最高纪录 

struct Face
{
	int data[FACE_X][FACE_Y + 10]; //为1是方块,为0是空格
	int color[FACE_X][FACE_Y + 10];
}face;
typedef struct Diamonds
{
	int space[4][4]; //4*4矩阵,为1是方块,为0是空格
}Dia;
Dia dia[7][4]; //一维表示7种形状,二维表示旋转次数

int grade = 0;
int max = 0;
int b = 0;

int main()
{
	system("cls");
	system("title 俄罗斯方块");
	color(7);
	system("mode con cols=60 lines=30");
	hidden_cursor();
	srand(time(NULL));
	read_file();
	grade = 0;
	init_face();
	init_dia();
	b = rand() % 7;
	while (1)
		start_game();
	return 0;
}

void start_game()
{
	int n, ch, t = 0, x = 0, y = FACE_Y / 2 - 2, i, j;
	int space_t = 0;

	draw_space(b, space_t, 4, FACE_Y + 3);
	n = b;
	b = rand() % 7;
	color(b);
	draw_dia(b, space_t, 4, FACE_Y + 3);

	while (1)
	{
		color(n);
		draw_dia(n, space_t, x, y);
		if (t == 0)
			t = 20000;
		while (--t)
			if (kbhit() != 0)
				break;
		if (t == 0)
		{
			if (border(n, space_t, x + 1, y) == 1)
			{
				draw_space(n, space_t, x, y);
				x++;
			}
			else
			{
				for (i = 0; i < 4; i++)
				{
					for (j = 0; j < 4; j++)
					{
						if (dia[n][space_t].space[i][j] == 1)
						{
							face.data[x + i][y + j] = 1;
							face.color[x + i][y + j] = n;
							while (del());
						}
					}
				}
				return;
			}
		}
		else
		{
			ch = getch();
			switch (ch)
			{
				case LEFT: {
					if (border(n, space_t, x, y - 1) == 1)
					{
						draw_space(n, space_t, x, y);
						y--;
					}
					break;
				}
				case RIGHT: {
					if (border(n, space_t, x, y + 1) == 1)
					{
						draw_space(n, space_t, x, y);
						y++;
					}
					break;
				}
				case DOWN: {
					if (border(n, space_t, x + 1, y) == 1)
					{
						draw_space(n, space_t, x, y);
						x++;
					}
					break;
				}
				case SPACE: {
					if (border(n, (space_t + 1) % 4, x + 1, y) == 1)
					{
						draw_space(n, space_t, x, y);
						space_t = (space_t + 1) % 4;
					}
					break;
				}
				case ESC: {
					system("cls");
					gotoxy(FACE_X / 2 - 2, FACE_Y);
					printf("游戏结束!\n");
					gotoxy(FACE_X / 2, FACE_Y);
					printf("按任意键退出!\n");
					getch();
					exit(0);
					break;
				}
				case 'R':
				case 'r': {
					main();
					exit(0);
				}
				case 'S':
				case 's': {
					while (1)
						if (kbhit() != 0)
							break;
					break;
				}
			}
		}
	}
}

int border(int n, int space_t, int x, int y)
{
	int i, j;
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			if (dia[n][space_t].space[i][j] == 0)
				continue;
			else if (face.data[x + i][y + j] == Wall || face.data[x + i][y + j] == Box)
				return 0;
		}
	}
	return 1;
}

int del()
{
	int i, j, k, sum;
	for (i = FACE_X - 2; i >= FACE_X - 6; i--)
	{
		sum = 0;
		for (j = 1; j < FACE_Y - 1; j++)
			sum += face.data[i][j];
		if (sum == 0)
			break;
		if (sum == FACE_Y - 2)
		{
			grade += 100;
			color(7);
			gotoxy(FACE_X - 4, 2 * FACE_Y + 2);
			printf("当前分数：%d", grade);
			for (j = 1; j < FACE_Y - 1; j++)
			{
				face.data[i][j] = Kong;
				gotoxy(i, 2 * j);
				printf("  ");
			}
			for (j = i; j > 1; j--)
			{
				sum = 0;
				for (k = 1; k < FACE_Y - 1; k++)
				{
					sum += face.data[j - 1][k] + face.data[j][k];
					face.data[j][k] = face.data[j - 1][k];
					if (face.data[j][k] == Kong)
					{
						gotoxy(j, 2 * k);
						printf("  ");
					}
					else
					{
						gotoxy(j, 2 * k);
						color(face.color[j][k]);
						printf("■");
					}
				}
				if (sum == 0)
					return 1;
			}
		}
	}
	for (i = 1; i < FACE_Y - 1; i++)
	{
		if (face.data[1][i] == Box)
		{
			char n;
			Sleep(800);
			system("cls");
			color(7);
			gotoxy(FACE_X / 2 - 2, 2 * (FACE_Y / 3));
			if (grade > max)
			{
				printf("恭喜您打破记录，目前最高纪录为：%d", grade);
				write_file();
			}
			else if (grade == max)
				printf("与纪录持平，请突破你的极限！");
			else
				printf("请继续努力，你与最高记录只差：%d", max - grade);
			gotoxy(FACE_X / 2, 2 * (FACE_Y / 3));
			printf("GAME OVER!\n");
			do
			{
				gotoxy(FACE_X / 2 + 2, 2 * (FACE_Y / 3));
				printf("是否重新开始游戏(y/n): ");
				scanf("%c", &n);
				gotoxy(FACE_X / 2 + 4, 2 * (FACE_Y / 3));
				if (n != 'n' && n != 'N' && n != 'y' && n != 'Y')
					printf("输入错误，请重新输入!");
				else
					break;
			} while (1);
			if (n == 'n' || n == 'N')
			{
				gotoxy(FACE_X / 2 + 4, 2 * (FACE_Y / 3));
				printf("按任意键退出游戏！");
				exit(0);
			}
			else if (n == 'y' || n == 'Y')
				main();
		}
	}
	return 0;
}

void read_file()
{
	FILE *fp;
	fp = fopen("俄罗斯方块记录.txt", "r+");
	if (fp == NULL)
	{
		fp = fopen("俄罗斯方块记录.txt", "w+");
		fwrite(&max, sizeof(int), 1, fp);
	}
	fseek(fp, 0, 0);
	fread(&max, sizeof(int), 1, fp);
	fclose(fp);
}

void write_file()
{
	FILE *fp;
	fp = fopen("俄罗斯方块记录.txt", "r+");
	fwrite(&grade, sizeof(int), 1, fp);
	fclose(fp);
}

void draw_space(int base, int space_t, int x, int y)
{
	int i, j;
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			gotoxy(x + i, 2 * (y + j));
			if (dia[base][space_t].space[i][j] == 1)
				printf("  ");
		}
	}
}

void draw_dia(int base, int space_t, int x, int y)
{
	int i, j;
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			gotoxy(x + i, 2 * (y + j));
			if (dia[base][space_t].space[i][j] == 1)
				printf("■");
		}
	}
}

void init_face()
{
	int i, j;

	for (i = 0; i < FACE_X; i++)
	{
		for (j = 0; j < FACE_Y + 10; j++)
		{
			if (j == 0 || j == FACE_Y - 1 || j == FACE_Y + 9)
			{
				face.data[i][j] = Wall;
				gotoxy(i, 2 * j);
				printf("■");
			}
			else if (i == FACE_X - 1)
			{
				face.data[i][j] = Box;
				gotoxy(i, 2 * j);
				printf("■");
			}
			else
				face.data[i][j] = Kong;
		}
	}

	gotoxy(FACE_X - 20, 2 * FACE_Y + 2);
	printf("左移：←");

	gotoxy(FACE_X - 18, 2 * FACE_Y + 2);
	printf("右移：→");

	gotoxy(FACE_X - 16, 2 * FACE_Y + 2);
	printf("下移：↓");

	gotoxy(FACE_X - 14, 2 * FACE_Y + 2);
	printf("旋转：SPACE");

	gotoxy(FACE_X - 12, 2 * FACE_Y + 2);
	printf("暂停: S");

	gotoxy(FACE_X - 10, 2 * FACE_Y + 2);
	printf("退出: ESC");

	gotoxy(FACE_X - 8, 2 * FACE_Y + 2);
	printf("重新开始:R");

	gotoxy(FACE_X - 6, 2 * FACE_Y + 2);
	printf("最高纪录:%d", max);

	gotoxy(FACE_X - 4, 2 * FACE_Y + 2);
	printf("当前分数：%d", grade);
}

void init_dia()
{
	int i, j, k, z;
	int b[4][4];
	for (i = 0; i < 3; i++)
		dia[0][0].space[1][i] = 1;
	dia[0][0].space[2][1] = 1; //凸形

	for (i = 1; i < 4; i++)
		dia[1][0].space[i][2] = 1;
	dia[1][0].space[1][1] = 1; //L形

	for (i = 1; i < 4; i++)
		dia[2][0].space[i][1] = 1;
	dia[2][0].space[1][2] = 1; //倒L形

	for (i = 0; i < 2; i++)
	{
		dia[3][0].space[1][i] = 1;
		dia[3][0].space[2][i + 1] = 1; //Z形

		dia[4][0].space[1][i + 1] = 1;
		dia[4][0].space[2][i] = 1; //倒Z形

		dia[5][0].space[1][i + 1] = 1;
		dia[5][0].space[2][i + 1] = 1; //方形
	}

	for (i = 0; i < 4; i++)
		dia[6][0].space[i][2] = 1; //直形 

	for (i = 0; i < 7; i++)
	{
		for (z = 0; z < 3; z++)
		{
			for (j = 0; j < 4; j++)
			{
				for (k = 0; k < 4; k++)
				{
					b[j][k] = dia[i][z].space[j][k];
				}
			}
			for (j = 0; j < 4; j++)
			{
				for (k = 0; k < 4; k++)
				{
					dia[i][z + 1].space[j][k] = b[3 - k][j];
				}
			}
		}
	}
}

void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = y;
	coord.Y = x;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hidden_cursor()
{
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
	cci.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
}

int color(int c)
{
	switch (c)
	{
	case 0: c = 9; break;
	case 1:
	case 2: c = 12; break;
	case 3:
	case 4: c = 14; break;
	case 5: c = 10; break;
	case 6: c = 13; break;
	default: c = 7; break;
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
	return 0;
}