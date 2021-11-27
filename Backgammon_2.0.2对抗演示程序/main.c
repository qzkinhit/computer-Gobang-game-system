#include<stdio.h>
#include<windows.h>
#include<time.h>
#include <conio.h>
#include<string.h>
#define M 30//棋盘的大小
#define Max_depth 4//最大的深度

int count = 0;
int Table[M][M];
//int Sub_Table[M][M];
void Initialize();
void Print();
struct Shape
{
    int score;
    char shape[10];
};
struct Shape Score_Shape[30] =
{
    {50,"01100"},
    {50,"00110"},
    {200,"11010"},
    {500,"00111"},
    {500,"11100"},
    {5000,"01110"},
    {5000,"010110"},
    {5000,"011010"},
    {5000,"11101"},
    {5000,"11011"},
    {5000,"10111"},
    {5000,"11110"},
    {5000,"01111"},
    {50000,"011110"},
    {99999999,"11111"},
    {-50,"02200"},
    {-50,"00220"},
    {-200,"22020"},
    {-500,"00222"},
    {-500,"22200"},
    {-5000,"02220"},
    {-5000,"020220"},
    {-5000,"022020"},
    {-5000,"22202"},
    {-5000,"22022"},
    {-5000,"20222"},
    {-5000,"22220"},
    {-5000,"02222"},
    {-50000,"022220"},
    {-99999999,"22222"}
};

//AIone 有关
void Function();
void Computer_Move();
void Intialize_for_Weight();
int Horizontal(int x,int y,int role);
int Longitudinal(int x,int y,int role);
int Left_Oblique(int x,int y,int role);
int Right_Oblique(int x,int y,int role);
int Computer[M][M][4];
int Player[M][M][4];
int Weight_Computer[M][M];
int Weight_Player[M][M];
int Max,Max_x,Max_y;//权重最大的位置

//与判定游戏是否结束有关的函数
int Judgement();
int is_Five(int i,int j,int role);
int is_Longitudinal(int x,int y,int role);
int is_Horizontal(int x,int y,int role);
int is_Left_Oblique(int x,int y,int role);
int is_Right_Oblique(int x,int y,int role);

//与玩家移动有关的函数
void Player_Move();
int Up();
int Down();
int Right();
int Left();
int User_Position_x,User_Position_y;


//与字符串处理有关的函数
int My_strstr(char *str, char *sub_str);//寻找字串的个数
char *Transform(int type,int x,int y);//将数组转换为字符串


//与AItwo有关

int MinMax(int depth);
int Score_evaluation(char *str);//返回值是在该条路上获得的分数，注意，x和y在上一层给出了
int Sum_evaluation();//返回值是当前棋局的评估值
void AI_move();
void legalmove(int *x_min,int *y_max,int *x_max,int *y_min);
int MaxFun(int depth);
int MinFun(int depth);
int alpha;
int beta;
int best_x;
int best_y;//只有在深度为0的时候才可以改变这两个值。

int main()
{
    int flag = 0;
    srand((unsigned)time(NULL));//生成随机种子
	system("mode con cols=220 lines=100");//扩大控制台窗口
	system("color 0C");//改动字体的颜色
    Initialize();
    while(1)
    {
        Print();
        AI_move();
        count++;
        Print();
        Sleep(500);
        Function();
        Computer_Move();
        count++;
        Print();
        Sleep(500);
        flag = Judgement();
        if(flag == 1)
        {
            MessageBox(0, TEXT("AI Upgraded version Win"), TEXT("恭喜"), NULL);
            return 0;
        }
        else if(flag == 2)
        {
            MessageBox(0, TEXT("AI Initial version Win"), TEXT("遗憾"), NULL);
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
            if(is_Five(i,j,2))
                return 2;
            else if(is_Five(i,j,1))
                return 1;
        }
    }
    return 0;
}
int is_Five(int i,int j,int role)
{
    if(is_Horizontal(i,j,role) >= 5) return 1;
    if(is_Longitudinal(i,j,role) >= 5) return 1;
    if(is_Left_Oblique(i,j,role) >= 5) return 1;
    if(is_Right_Oblique(i,j,role) >= 5) return 1;
    return 0;
}
int is_Longitudinal(int x,int y,int role)
{
    int sum = 0;
    for(int i = y;i < M;i++)
    {
        if(Table[x][i] == role) sum += 1;
        else break;
    }
    for(int i = y-1;i >= 0;i--)
    {
        if(Table[x][i] == role) sum += 1;
        else break;
    }
    return sum;
}
int is_Horizontal(int x,int y,int role)
{
    int sum = 0;
    for(int i = x;i < M;i++)
    {
        if(Table[i][y] == role) sum += 1;
        else break;
    }
    for(int i = x-1;i >= 0;i--)
    {
        if(Table[i][y] == role) sum += 1;
        else break;
    }
    return sum;
}
int is_Left_Oblique(int x,int y,int role)
{
    int sum = 0;
    for(int i = 0;(i < M - x&&i < M - y);i++)
    {
        if(Table[x+i][y+i] == role) sum += 1;
        else break;
    }
    for(int i = 1;(i <= x&&i <= y);i++)
    {
        if(Table[x-i][y-i] == role) sum += 1;
        else break;
    }
    return sum;
}
int is_Right_Oblique(int x,int y,int role)
{
    int sum = 0;
    for(int i = 0;(i < M - x&&i <= y);i++)
    {
        if(Table[x+i][y-i] == role) sum += 1;
        else break;
    }
    for(int i = 1;(i <= x&&i < M - y);i++)
    {
        if(Table[x-i][y+i] == role) sum += 1;
        else break;
    }
    return sum;
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
int My_strstr(char *str, char *sub_str)
{
    char *str_local = NULL;
    char *sub_str_local = NULL;
    int num = 0;
    if(!str || !sub_str)
    {
        printf("fun:%s param is error\n", __FUNCTION__);
        return 0;
    }

    while(*str)
    {
        str_local = str;
        sub_str_local = sub_str;
        do
        {
            if(*sub_str_local == '\0')
            {
                num++;
                break;
            }
        }while(*str_local++ == *sub_str_local++);
        str += 1;
    }
    return num;
}
char *Transform(int type,int x,int y)//注意这个函数 1 是从左向右查找， 2是从上到下查找。3是左上到右下查找，4是从右上到左下查找
{
    int i;
    static char result[M+1];
    if(type == 1)
    {
        for(i = 0;i < M;i++)
        {
            result[i] = Table[x][i] + '0';
        }
        result[i] = '\0';
        return result;
    }
    if(type == 2)
    {
        for(i = 0;i < M ;i++)
        {
            result[i] = Table[i][y] + '0';
        }
        result[i] = '\0';
        return result;
    }
    if(type == 3)//右斜线
    {
        for(i = 0;(i+x < M&&i+y < M);i++)
        {
            result[i] = Table[x+i][y+i] + '0';
        }
        result[i] = '\0';
        return result;
    }
    if(type == 4)//左斜线
    {
        for(i = 0; (x+i < M&&y - i >= 0); i++)
        {
            result[i] = Table[x+i][y-i] + '0';
        }
        result[i] = '\0';
        return result;
    }
}
int Score_evaluation(char *str)
{
    int sum  = 0;
    for(int i = 0;i < 30;i++)
    {
        sum += Score_Shape[i].score * My_strstr(str,Score_Shape[i].shape);
    }

    return sum;
}
int Sum_evaluation()
{
    int x,y;
    int sum = 0;
    for(int i = 0;i < M;i++)
    {
        x = i;
        y = 0;
        sum += Score_evaluation(Transform(1,x,y));
    }
    for(int i = 0;i < M;i++)
    {
        x = 0;
        y = i;
        sum += Score_evaluation(Transform(2,x,y));
    }
    for(int i = 0;i < M - 4;i++)
    {
        x = 0;
        y = i;
        sum += Score_evaluation(Transform(3,x,y));
    }
    for(int i = 1;i < M - 4;i++)
    {
        x = i;
        y = 0;
        sum += Score_evaluation(Transform(3,x,y));
    }
    for(int i = 4;i < M;i++)
    {
        x = 0;
        y = i;
        sum += Score_evaluation(Transform(4,x,y));
    }
    for(int i = 1;i < M - 4;i++)
    {
        x = i;
        y = M - 1;
        sum += Score_evaluation(Transform(4,x,y));
    }
    return sum;
}
void legalmove(int *x_min,int *y_max,int *x_max,int *y_min)
{
    int c;
    *x_min = M - 1;
    *y_max = 0;
    *x_max = 0;
    *y_min = M - 1;
    for(int i = 0;i < M;i++)
    {
        for(int j = 0;j < M;j++)
        {
            if(Table[i][j] != 0)
            {
                if(i >= *x_max)
                {
                    *x_max = i;
                }
                if(i <= *x_min)
                {
                    *x_min = i;
                }
                if(j >= *y_max)
                {
                    *y_max = j;
                }
                if(j <= *y_min)
                {
                    *y_min = j;
                }
            }
        }
    }
    if(*x_max + 1 < M) *x_max = *x_max + 1;
    if(*x_min - 1 >= 0) *x_min = *x_min - 1;
    if(*y_max + 1 < M) *y_max = *y_max + 1;
    if(*y_min - 1 >= 0) *y_min = *y_min - 1;
    if(*x_max < *x_min)
    {
        c = *x_max;
        *x_max = *x_min;
        *x_min = c;
    }
    if(*y_max < *y_min)
    {
        c = *y_max;
        *y_max = *y_min;
        *y_min = c;
    }
    return;
}
void AI_move()
{
    if(count == 0)
    {
        Table[M/2][M/2] = 1;
        return;
    }
    else
    {
        MaxFun(1);
    }
    Table[best_x][best_y] = 1;
}
int MaxFun(int depth)
{
    int i,j;
    int best = -9999999;
    int temp;
    int x_max;
    int x_min;
    int y_max;
    int y_min;
    if(depth == Max_depth)
    {
        return Sum_evaluation();
    }
    legalmove(&x_min,&y_max,&x_max,&y_min);
    for(i = x_min;i <= x_max;i++)
    {
        for(j = y_min;j <= y_max;j++)
        {
            if(Table[i][j] == 0)
            {
                Table[i][j] = 1;
                temp = MinFun(depth+1);
                Table[i][j] = 0;
                if(temp > best)
                {
                    best = temp;
                    if(depth == 1)
                    {
                        best_x = i;
                        best_y = j;
                    }
                }
            }
        }
    }
    return best;
}
int MinFun(int depth)
{
    int i,j;
    int best = 9999999;
    int temp;
    int x_max;
    int x_min;
    int y_max;
    int y_min;
    if(depth == Max_depth)
    {
        return Sum_evaluation();
    }
    legalmove(&x_min,&y_max,&x_max,&y_min);
    for(i = x_min;i <= x_max;i++)
    {
        for(j = y_min;j <= y_max;j++)
        {
            if(Table[i][j] == 0)
            {
                Table[i][j] = 2;
                temp = MaxFun(depth+1);
                Table[i][j] = 0;
                if(temp < best)
                {
                    best = temp;
                    /*if(depth == 1)
                    {
                        best_x = i;
                        best_y = j;
                    }*/
                }
            }
        }
    }
    return best;
}
