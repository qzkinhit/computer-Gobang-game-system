#include<stdio.h>
#include<windows.h>
#include<time.h>
#include <conio.h>
#define M 25//棋盘的大小

int Table[M][M];
int Computer[M][M][4];
int Player[M][M][4];
int Weight_Computer[M][M];
int Weight_Player[M][M];
int Max,Max_x,Max_y;//权重最大的位置
int User_Position_x,User_Position_y;

void Initialize();
void Print();
void Player_Move();
void Function();
void Computer_Move();
void Intialize_for_Weight();
int Judgement();

int Horizontal(int x,int y,int role);
int Longitudinal(int x,int y,int role);
int Left_Oblique(int x,int y,int role);
int Right_Oblique(int x,int y,int role);
int Up();
int Down();
int Right();
int Left();
int main()
{
    int flag = 0;
    srand((unsigned)time(NULL));//生成随机种子
	system("mode con cols=220 lines=100");//扩大控制台窗口
	system("color 0C");//改动字体的颜色
    Initialize();
    MessageBox(0, TEXT("键盘上的wasd移动，f是落子"), TEXT("提示"), NULL);
    while(1)
    {
        Print();
        Player_Move();
        Print();
        Function();
        Computer_Move();
        Print();
        flag = Judgement();
        if(flag == 1)
        {
            MessageBox(0, TEXT("Player Win"), TEXT("恭喜"), NULL);
            return 0;
        }
        else if(flag == 2)
        {
            MessageBox(0, TEXT("Computer Win"), TEXT("遗憾"), NULL);
            return 0;
        }
        Intialize_for_Weight();
    }
    return 0;
}
void Initialize()
{
    Max = 0;
    Max_x = 0;
    Max_y = 0;
    for(int i = 0;i < M;i++)
    {
        for(int j = 0;j < M;j++)
        {
            Table[i][j] = 0;
            Weight_Computer[i][j] = 0;
            Weight_Player[i][j] = 0;
            for(int k = 0;k < 4;k++)
            {
                Computer[i][j][k] = 0;
                Player[i][j][k] = 0;
            }
        }
    }
    return;
}
void Intialize_for_Weight()
{
    Max = 0;
    Max_x = 0;
    Max_y = 0;
    for(int i = 0;i < M;i++)
    {
        for(int j = 0;j < M;j++)
        {
            Weight_Computer[i][j] = 0;
            Weight_Player[i][j] = 0;
            for(int k = 0;k < 4;k++)
            {
                Computer[i][j][k] = 0;
                Player[i][j][k] = 0;
            }
        }
    }
    return;
}
void Print()
{
    char str[2 * (M+2) * (M+2)] = { '\0' };
	int i = 0, j = 0;
	str[0] = '\0';
	system("cls");
	for (i = 0;i < M;i++)
	{
		for (j = 0;j < M;j++)
		{
			if (Table[i][j] == 0)
			{
				strcat(str, "■");
			}
			else if (Table[i][j] == 1)
			{
				strcat(str, "⊙");
			}
			//	printf("⊙");
			else if (Table[i][j] == 2)
			{
				strcat(str, "◇");
			}
			//	printf("◇");
			else if (Table[i][j] == 3)
			{
				strcat(str, "☆");
			}
		}
		//if (Table[i][0] != -1)
        strcat(str, "\n");
		//printf("\n");
	}
	printf("%s", str);
	return;
}
void Player_Move()
{
    //int x,y;
    /*while(1)
    {
        printf("which row do you want to settle down?");
        scanf("%d",&x);
        printf("which line do you want to settle down?");
        scanf("%d",&y);
        if(Table[x-1][y-1] == 0) break;
        else printf("you can not settle down here\n");
    }*/
    int flag = 0;
    User_Position_x = M/2;
    User_Position_y = M/2;
    while(Table[User_Position_x][User_Position_y] != 0)
    {
        if(rand()%2 == 1)
            User_Position_x++;
        else
            User_Position_y++;
    }
    Table[User_Position_x][User_Position_y] = 3;
    Print();
	while (1)
	{
		flag = 0;
		Find_User();
		switch (getch())//接收按键输入，该函数的作用：从控制台读取一个字符但是不显示在控制台上
		{
		case 'w':flag = Up();break;
		case 'a':flag = Left();break;
		case 's':flag = Down();break;
		case 'd':flag = Right();break;
		case 'f':flag = 2;break;
		}
        if (flag == 1)
            Print();
        else if(flag == 2)
        {
            Table[User_Position_x][User_Position_y] = 1;
            return;
        }
	}
    return;
}
void Function()
{
    for(int i = 0;i < M;i++)
    {
        for(int j = 0;j < M;j++)
        {
            if(Table[i][j] != 0)
            {
                for(int k = 0;k < 4;k++)
                {
                    Computer[i][j][k] = 0;
                    Player[i][j][k] = 0;
                }
            }
            else if(Table[i][j] == 0)
            {
                Computer[i][j][0] = Horizontal(i,j,2);
                Computer[i][j][1] = Longitudinal(i,j,2);
                Computer[i][j][2] = Left_Oblique(i,j,2);
                Computer[i][j][3] = Right_Oblique(i,j,2);
                Player[i][j][0] = Horizontal(i,j,1);
                Player[i][j][1] = Longitudinal(i,j,1);
                Player[i][j][2] = Left_Oblique(i,j,1);
                Player[i][j][3] = Right_Oblique(i,j,1);
            }
        }
    }
    return;
}
int Horizontal(int x,int y,int role)
{
    int sum = 0;
    for(int i = x+1;i < M;i++)
    {
        if(Table[i][y] == role) sum += 1;
        else break;
    }
    for(int i = x-1;i >= 0;i--)
    {
        if(Table[i][y] == role) sum += 1;
        else break;
    }
    if(sum == 4) sum = 100;
    return sum;
}
int Longitudinal(int x,int y,int role)
{
    int sum = 0;
    for(int i = y+1;i < M;i++)
    {
        if(Table[x][i] == role) sum += 1;
        else break;
    }
    for(int i = y-1;i >= 0;i--)
    {
        if(Table[x][i] == role) sum += 1;
        else break;
    }
    if(sum == 4) sum = 100;
    return sum;
}
int Left_Oblique(int x,int y,int role)
{
    int sum = 0;
    for(int i = 1;(i < M - x&&i < M - y);i++)
    {
        if(Table[x+i][y+i] == role) sum += 1;
        else break;
    }
    for(int i = 1;(i <= x&&i <= y);i++)
    {
        if(Table[x-i][y-i] == role) sum += 1;
        else break;
    }
    if(sum == 4) sum = 100;
    return sum;
}
int Right_Oblique(int x,int y,int role)
{
    int sum = 0;
    for(int i = 1;(i < M - x&&i <= y);i++)
    {
        if(Table[x+i][y-i] == role) sum += 1;
        else break;
    }
    for(int i = 1;(i <= x&&i < M - y);i++)
    {
        if(Table[x-i][y+i] == role) sum += 1;
        else break;
    }
    if(sum == 4) sum = 100;
    return sum;
}
void Computer_Move()
{
    for(int i = 0;i < M;i++)
    {
        for(int j = 0;j < M;j++)
        {
            for(int k = 0;k < 4;k++)
            {
                Weight_Computer[i][j] += Computer[i][j][k];
                Weight_Player[i][j] += Player[i][j][k];
            }
            if(Weight_Computer[i][j] > Max)
            {
                Max_x = i;
                Max_y = j;
                Max = Weight_Computer[i][j];
            }
            if(Weight_Player[i][j] > Max)
            {
                Max_x = i;
                Max_y = j;
                Max = Weight_Player[i][j];
            }
        }
    }
    Table[Max_x][Max_y] = 2;
}
int Judgement()
{
    for(int i = 0;i < M;i++)
    {
        for(int j = 0;j < M;j++)
        {
            if((Horizontal(i,j,2) == 5||Longitudinal(i,j,2) == 5||Left_Oblique(i,j,2) == 5||Right_Oblique(i,j,2) == 5)&&i >= 5&&j >= 5&&i <= M-5&&j <= M-5)
                return 2;
            else if((Horizontal(i,j,1) == 5||Longitudinal(i,j,1) == 5||Left_Oblique(i,j,1) == 5||Right_Oblique(i,j,1) == 5)&&i >= 5&&j >= 5&&i <= M-5&&j <= M-5)
                return 1;
        }
    }
    return 0;
}
int Up()
{
    int i = 2;
	if (User_Position_x != 0)
	{
		if (Table[User_Position_x - 1][User_Position_y] == 0)//可走
		{
			Table[User_Position_x - 1][User_Position_y] = 3;
			Table[User_Position_x][User_Position_y] = 0;
			return 1;//行走成功
		}
		else if(Table[User_Position_x - 1][User_Position_y] == 1||Table[User_Position_x - 1][User_Position_y] == 2)
        {
            while((Table[User_Position_x - i][User_Position_y] == 1||Table[User_Position_x - i][User_Position_y] == 2)&&User_Position_x - i >= 0)
            {
                i++;
            }
            Table[User_Position_x - i][User_Position_y] = 3;
			Table[User_Position_x][User_Position_y] = 0;
			return 1;
        }
	}
	return 0;
}
int Down()
{
    int i = 2;
	if (User_Position_x != M - 1)
	{
		if (Table[User_Position_x + 1][User_Position_y] == 0)//可走
		{
			Table[User_Position_x + 1][User_Position_y] = 3;
			Table[User_Position_x][User_Position_y] = 0;
			return 1;//行走成功
		}
		else if(Table[User_Position_x + 1][User_Position_y] == 1||Table[User_Position_x + 1][User_Position_y] == 2)
        {
            while((Table[User_Position_x + i][User_Position_y] == 1||Table[User_Position_x + i][User_Position_y] == 2)&&User_Position_x + i < M)
            {
                i++;
            }
            Table[User_Position_x + i][User_Position_y] = 3;
			Table[User_Position_x][User_Position_y] = 0;
			return 1;
        }
	}
	return 0;
}
int Right()
{
    int i = 2;
	if (User_Position_y != M - 1)
	{
		if (Table[User_Position_x][User_Position_y + 1] == 0)//可走
		{
			Table[User_Position_x][User_Position_y + 1] = 3;
			Table[User_Position_x][User_Position_y] = 0;
			return 1;//行走成功
		}
		else if(Table[User_Position_x][User_Position_y + 1] == 1||Table[User_Position_x][User_Position_y + 1] == 2)
        {
            while((Table[User_Position_x][User_Position_y + i] == 1||Table[User_Position_x][User_Position_y + i] == 2)&&User_Position_y + i < M)
            {
                i++;
            }
            Table[User_Position_x][User_Position_y + i] = 3;
			Table[User_Position_x][User_Position_y] = 0;
			return 1;
        }
	}

	return 0;
}
int Left()
{
    int i = 2;
	if (User_Position_y != 0)
	{
		if (Table[User_Position_x][User_Position_y - 1] == 0)//可走
		{
			Table[User_Position_x][User_Position_y - 1] = 3;
			Table[User_Position_x][User_Position_y] = 0;
			return 1;//行走成功
		}
		else if(Table[User_Position_x][User_Position_y - 1] == 1||Table[User_Position_x][User_Position_y - 1] == 2)
        {
            while((Table[User_Position_x][User_Position_y - i] == 1||Table[User_Position_x][User_Position_y - i] == 2)&&User_Position_y - i >= 0)
            {
                i++;
            }
            Table[User_Position_x][User_Position_y - i] = 3;
			Table[User_Position_x][User_Position_y] = 0;
			return 1;
        }
	}
	return 0;
}
void Find_User()
{
	int i, j;
	for (i = 0;i < M;i++)
		for (j = 0;j < M;j++)
		{
			if (Table[i][j] == 3)
			{
				User_Position_x = i;
				User_Position_y = j;
			}
		}
}
