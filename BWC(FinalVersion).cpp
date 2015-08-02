#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<cstring>
#include<windows.h>
using namespace std;
/*
 * Black and White Chess.cpp
 * Started on:2012-12-03
 * Completed on:2013-01-06
 * Author: 刘易   刘敏行
 */
void print(bool);//打印棋盘
void operatehuman(int *, bool*);//人通过输入坐标下棋
void operatecomputer(bool*);//电脑下棋
void operate(int);//下棋，以上两个是其子函数
void refresh();//还原棋盘上某些参数
void count();//数棋盘上的棋子数，包含在print函数里
int recordout(int);//记录执子颜色和下到了第几步，辅助载入棋盘
int recordin(int *);//把执子颜色和步数赋值
int fileout(int);//记录棋盘状态
int fileinn(int);//赋值棋盘
void win();//判断输赢
void judge(int, int*);//判断是否可下
void reversi();//翻转操作
int access(int);//估价函数

int chess[10][10], hardship[4]; //difficuly为根据难度控制加权的数组，分别控制棋子数量、特殊位置、安全系数、对方行动力
int dif = 1, ahint = 1; //定义难度(1\2\3\4)，落点提示(1为要，2为不要)
int choice;//选择的颜色
int color; //试剂的颜色，0为白棋，1为黑棋，3为可落子处,4为刚下的黑棋，5为刚下的白棋
int mark[2] = { 5, 4 }; //4为刚下的黑棋(1)，5为刚下的白棋(0)
int wintim = 0, losetim = 0, tietim = 0;//总的战绩情况

void menu(int tim) //开始菜单-------by 刘易
{
	char x[10];
	if (tim)
		cout << "游戏暂停\n";
	cout << "菜单\n";
	cout << "请输入选择的序号\n1、载入（如果您是第一次玩儿或刚刚完成一局，请不要点这个，谢谢配合！）\n2、重新开始\n3、退出游戏\n";
	if (tim)
		cout << "4、调整难度\n5、调整提示\n"; //五大选项
	cin >> x;
	{
		if (tim) {
			while (x[0] != '1' && x[0] != '2' && x[0] != '3' && x[0] != '4'
					&& x[0] != '5') {
				cout << "请重新输入序号" << endl;
				cin >> x;
			}
		} else {
			while (x[0] != '1' && x[0] != '2' && x[0] != '3') {
				cout << "请重新输入序号" << endl;
				cin >> x;
			}
		}
	}

	if (x[0] == '1') {//用户选择载入
		char empty[2];
		cin.getline(empty, 2);//接收刚才的回车
		if (recordin(&tim) == -1 || fileinn(tim) == -1) {//防止一些无聊的人一上来就点载入！
			system("CLS");
			cout << "喂，说了不让点载入你还点！请重新输入！\n";
			menu(0);
		} else {
			recordin(&tim);//倒入用户刚才的选择与下棋步数
			fileinn(tim);//倒入棋盘
			refresh();//还原一些数据
			system("CLS");
			if (choice)//如果是用户是先手，则直接运行操作函数
				operate(tim);
			else {//如果用户是后手，需要让用户先下一步，然后再正常运行
				bool check = true;
				operatehuman(&tim, &check);
				operate(tim);
			}
			return;
		}
	} else if (x[0] == '2' || x[0] == '4' || x[0] == '5') {
		char temp = x[0]; //存储下x的值
		if (temp == '2') { //重新开始
			memset(chess, -1, sizeof(chess)); //棋盘初始化
			chess[4][4] = chess[5][5] = 0;
			chess[4][5] = chess[5][4] = 1;
			cout << "请选择颜色\n1、黑棋\n2、白棋" << endl; //选择颜色
			cin >> x;
			if (x[0] == '1' || x[0] == '2')
				choice = 2 - atoi(x);
			while (x[0] != '1' && x[0] != '2') {
				cout << "请重新输入序号" << endl;
				cin >> x;
			}
		}

		if (temp == '2' || temp == '4') { //重新开始或者调整难度，要选择难度
			cout << "请选择难度（输入序号）\n1、简单\n2、中级\n3、困难\n4、很困难" << endl;
			cin >> x;
			while (x[0] != '1' && x[0] != '2' && x[0] != '3' && x[0] != '4') {
				cout << "请重新输入序号" << endl;
				cin >> x;
			}
			if (x[0] == '1' || x[0] == '2' || x[0] == '3' || x[0] == '4') {
				dif = atoi(x);
				for (int i = 0; i < dif; i++)
					hardship[i] = 1; //给难度系数赋值
			}
			if (temp == '4') { //继续游戏
				char empty[2];
				cin.getline(empty, 2);
				fileinn(tim);
				refresh();
				system("CLS");
				if (choice)//如果是用户是先手，则直接运行操作函数
					operate(tim);
				else {//如果用户是后手，需要让用户先下一步，然后再正常运行
					bool check = true;
					operatehuman(&tim, &check);
					operate(tim);
				}
				return;
			}
		}

		if (temp == '2' || temp == '5') { //重新开始或者设置提示，要选择提示
			cout << "是否需要落点提示？（输入序号）\n1、需要\n2、不需要" << endl;
			cin >> x;
			while (x[0] != '1' && x[0] != '2' && x[0] != '3' && x[0] != '4') {
				cout << "请重新输入序号" << endl;
				cin >> x;
			}
			if (x[0] == '1' || x[0] == '2')
				ahint = atoi(x);
			if (temp == '5') { //继续游戏
				char empty[2];
				cin.getline(empty, 2);
				fileinn(tim);
				refresh();
				system("CLS");
				if (choice)//如果是用户是先手，则直接运行操作函数
					operate(tim);
				else {//如果用户是后手，需要让用户先下一步，然后再正常运行
					bool check = true;
					operatehuman(&tim, &check);
					operate(tim);
				}
				return;
			}
		}

		char empty[2];
		cin.getline(empty, 2);
		cout << "游戏开始~" << endl;
		operate(0);
	} else
		cout << "谢谢，下次再见！\n";
	exit(0);
}
void print(bool x) {//    参数x为0指电脑，不要显示提示-------by 刘易
	cout << "********************************************" << endl;
	cout << "提示：1、调出菜单请输入 'menu' " << endl;
	cout << "      2、悔棋请输入 'regret'" << endl;
	cout << "      3、撤销悔棋请输入 'forward' " << endl;
	cout << "********************************************" << endl; //游戏菜单提示
	cout << "      1    2   3   4   5   6   7   8 " << endl;
	cout << "    ┏━┯━┯━┯━┯━┯━┯━┯━┓ " << endl;
	for (int i = 1; i <= 7; i++) //前7行的输出
	{
		cout << i << "   ┃";
		for (int j = 1; j <= 7; j++) {

			if (chess[i][j] == -1 || (chess[i][j] == 3
					&& (ahint == 2 || x == 0))) //中间棋子的输出
				cout << "  │";
			else if (chess[i][j] == 0)
				cout << "○│";
			else if (chess[i][j] == 1)
				cout << "●│";
			else if (chess[i][j] == 4)
				cout << "◆│";
			else if (chess[i][j] == 5)
				cout << "◇│";
			else if (chess[i][j] == 3 && ahint == 1 && x == 1)
				cout << "※│";
		}
		if (chess[i][8] == -1 || (chess[i][8] == 3 && (ahint == 2 || x == 0))) //最右边棋子的输出（边框加粗）
			cout << "  ┃" << endl;
		else if (chess[i][8] == 0)
			cout << "○┃" << endl;
		else if (chess[i][8] == 1)
			cout << "●┃" << endl;
		else if (chess[i][8] == 4)
			cout << "◆┃" << endl;
		else if (chess[i][8] == 5)
			cout << "◇┃" << endl;
		else if (chess[i][8] == 3 && ahint == 1 && x == 1)
			cout << "※┃" << endl;
		cout << "    ┠─┼─┼─┼─┼─┼─┼─┼─┨ " << endl; //分隔行的输出
	}
	cout << 8 << "   ┃"; //第八行的输出
	for (int j = 1; j <= 7; j++) {
		if (chess[8][j] == -1 || (chess[8][j] == 3 && (ahint == 2 || x == 0)))
			cout << "  │";
		else if (chess[8][j] == 0)
			cout << "○│";
		else if (chess[8][j] == 1)
			cout << "●│";
		else if (chess[8][j] == 4)
			cout << "◆│";
		else if (chess[8][j] == 5)
			cout << "◇│";
		else if (chess[8][j] == 3 && ahint == 1 && x == 1)
			cout << "※│";

	}
	if (chess[8][8] == -1 || (chess[8][8] == 3 && (ahint == 2 || x == 0))) //第八行最右的输出
		cout << "  ┃" << endl;
	else if (chess[8][8] == 0)
		cout << "○┃" << endl;
	else if (chess[8][8] == 1)
		cout << "●┃" << endl;
	else if (chess[8][8] == 4)
		cout << "◆┃" << endl;
	else if (chess[8][8] == 5)
		cout << "◇┃" << endl;
	else if (chess[8][8] == 3 && ahint == 1 && x == 1)
		cout << "※┃" << endl;
	cout << "    ┗━┷━┷━┷━┷━┷━┷━┷━┛ " << endl;

	count();//最后一行显示当前棋子
}

bool jud(int x1, int y1, int dx, int dy, int color, bool temp)
//判断某个位置在某个方向下是否可以落子（judge函数的子函数）——By 刘易
{
	x1 = x1 + dx;
	y1 = y1 + dy; //向该方向移动
	if (x1 < 1 || y1 < 1 || x1 > 8 || y1 > 8)
		return 0; //越界返回
	else if (chess[x1][y1] == 1 - color || chess[x1][y1] == 4 + color) //不同颜色
	{
		temp = 1;
		return jud(x1, y1, dx, dy, color, temp);
	} else if (temp && (chess[x1][y1] == color || chess[x1][y1] == 5 - color)) //可以落子处，返回1
		return 1;
	return 0; //无法落子，返回0
}

void judge(int color, int*checkiflegal) //判断每个位置是否可以落子——By 刘易
{

	for (int x = 1; x <= 8; x++)
		for (int y = 1; y <= 8; y++) {
			if (chess[x][y] == -1) //寻找未下处
			{
				for (int dx = -1; dx <= 1; dx++) {
					for (int dy = -1; dy <= 1; dy++) //向8个方向检查
					{
						if (dx == 0 && dy == 0)
							continue;

						if (jud(x, y, dx, dy, color, false)) {
							chess[x][y] = 3; //可以落子处
							*checkiflegal = 1;
						}
					}
				}
			}
		}

}
void reversi(int i, int j, int k, int num, int color, bool findchess) {//--------by 刘敏行
	//六个参数，横坐标，纵坐标，方向标号，可吃掉的棋子个数，执子方颜色，有没有找到不同颜色的棋子
	int direct[8][2] = { { -1, -1 }, { -1, 0 }, { -1, 1 }, { 0, 1 }, { 1, 1 },
			{ 1, 0 }, { 1, -1 }, { 0, -1 } };//八个方向
	i += direct[k][0];//向该方向运动
	j += direct[k][1];
	if (i < 1 || j < 1 || i > 8 || j > 8)//越界了退出
		return;
	{
		if (chess[i][j] == 1 - color) {//找到了不同颜色的棋子
			findchess = true;
			num++;//统计个数的循环变量
			reversi(i, j, k, num, color, findchess);//递归

		} else if (findchess && chess[i][j] == color) {//又找到了同颜色的棋子，表明可以翻转
			while (num >= 0) {
				chess[i -= direct[k][0]][j -= direct[k][1]] = color;//依次翻转
				num--;
			}
			return;
		} else
			return;

	}
}
void refresh() //棋盘的3复原--------by 刘易
{
	for (int i = 1; i <= 8; i++)
		for (int j = 1; j <= 8; j++) {
			if (chess[i][j] == 3)
				chess[i][j] = -1;
		}
}

void clean45() //棋盘刚下处4、5的复原——by 刘易
{
	bool x = 0;
	for (int i = 1; i <= 8; i++) { //清除余下的一个4或5
		for (int j = 1; j <= 8; j++) {
			if (chess[i][j] == 4 || chess[i][j] == 5) {
				chess[i][j] = 5 - chess[i][j];
				x = 1;
				break;
			}
		}
		if (x)
			break; //只有一个4或5，找到后直接跳出
	}
}

void win() {//-------by 刘敏行
	int blacknum = 0, whitenum = 0;
	for (int i = 1; i <= 8; i++)
		for (int j = 1; j <= 8; j++) {
			if (chess[i][j] == 0)
				whitenum++;
			else
				blacknum++;
		}
	ifstream infile("win.txt", ios::in);
	infile >> wintim >> losetim >> tietim;//调出战绩
	if (choice && whitenum < blacknum || !choice && whitenum > blacknum)//获胜
	{
		cout << "玩家获胜！\n";
		wintim++;
	} else if (choice && blacknum < whitenum || !choice && blacknum > whitenum)//输掉
	{
		cout << "电脑获胜！\n";
		losetim++;
	} else {
		cout << "这是个平局！\n";
		tietim++;
	}
	ofstream outfile("win.txt", ios::out);
	outfile << wintim << " " << losetim << " " << tietim;//存入战绩
	cout << "当前您的战绩是：赢： " << wintim << "输： " << losetim << "平局： " << tietim
			<< endl;
	cout << "爽不爽？再来一局？\n请输入选择的序号\n1、再来一局\n2、退出游戏\n";
	bool flag = true;
	while (flag) {
		int ch;
		cin >> ch;
		if (ch == 1) {
			flag = false;
			menu(0);
		} else if (ch == 2) {
			cout << "谢谢！下次再见！\n";
			Sleep(1000);
			exit(0);//直接全部退出
		} else
			cout << "请重新输入\n";
	}
}
void count() {//-------by 刘敏行
	int blacknum = 0, whitenum = 0;
	for (int i = 1; i <= 8; i++)
		for (int j = 1; j <= 8; j++) {
			if (chess[i][j] == 0 || chess[i][j] == 5)
				whitenum++;
			if (chess[i][j] == 1 || chess[i][j] == 4)
				blacknum++;
		}
	cout << "当前黑棋数：" << blacknum << "  当前白棋数：" << whitenum << endl;
	if (whitenum == 0 || blacknum == 0 || whitenum + blacknum == 64)//结束条件，一方无子或者棋盘已满
		win();
}
//以下进入文件输入输出部分
int recordout(int tim) {//---------by刘敏行
	ofstream outfile("record.txt", ios::out);
	if (!outfile) {
		cout << "选择文件打开失败!\n";
		return -1;
	}
	outfile << choice << " " << dif << " " << tim;
	outfile.close();
	return 0;
}
int recordin(int *tim) {//------------by刘敏行
	ifstream infile("record.txt", ios::in);
	if (!infile) {
		cout << "选择文件打开失败!\n";
		return -1;
	}
	infile >> choice >> dif >> *tim;
	return 0;
}
int fileout(int tim) {//---------by刘敏行
	char name[] = "chess?00.txt";//每次都要存，只能不断改变文件名
	char*p = name;//用指针不断改文件名
	if (choice)
		*(p + 5) = 'b';//区分黑白
	else
		*(p + 5) = 'w';
	*(p + 6) = char(tim / 10 + '0');//变动的字符
	*(p + 7) = char(tim % 10 + '0');
	ofstream outfile(name, ios::out);
	if (!outfile) {
		cout << "棋盘文件存入失败!\n";
		return -1;
	}
	for (int i = 1; i <= 8; i++) {
		for (int j = 1; j <= 8; j++) {
			outfile << chess[i][j] << " ";//存入棋盘
		}
		outfile << endl;
	}

	outfile.close();
	return 0;
}
int fileinn(int tim) {//-------by 刘敏行
	char name[] = "chess?00.txt";//同样每次读入的文件名是变量
	char*p = name;
	if (choice)
		*(p + 5) = 'b';
	else
		*(p + 5) = 'w';
	*(p + 6) = char(tim / 10 + '0');
	*(p + 7) = char(tim % 10 + '0');
	ifstream infile(name, ios::in);
	if (!infile) {
		cout << "棋盘文件打开失败!\n";
		return -1;
	}
	for (int i = 1; i <= 8; i++)
		for (int j = 1; j <= 8; j++)
			infile >> chess[i][j];
	return 0;
}
//以下进入游戏运行部分
void operatehuman(int *tim, bool*check1) {//------by 刘敏行
	int row, col;//行、列坐标
	int sum = 0;//一个中间量，判断是否可以前进（forward）
	color = choice;
	int checkiflegal = false;//是否有子可下
	judge(color, &checkiflegal);//运行判断函数
	if (checkiflegal) {//如果有
		bool flag = true;//输入是否合法的判断量
		while (flag) {
			fileout((*tim));//存该下时棋盘情况
			print(1);//打印棋盘
			clean45(); //还原一些棋盘参数
			cout << "轮到你下，请输入横纵坐标，中间用空格区分\n";
			char putin[10];
			cin.getline(putin, 10);//读入指令
			int len = strlen(putin);//判断读入
			if (putin[0] >= '1' && putin[0] <= '8' && putin[1] == ' '
					&& putin[2] >= '1' && putin[2] <= '8' && len == 3) {//是坐标
				row = int(putin[0] - '0');
				col = int(putin[2] - '0');
				sum = 0;//还原
				{
					if (chess[row][col] == 3) {//说明输入的坐标合法
						(*tim)++;//步数1
						recordout((*tim));
						flag = false;
						for (int k = 0; k < 8; k++)
							reversi(row, col, k, 0, color, false);//翻转
						chess[row][col] = mark[choice]; //标记刚下的棋，方便用户看清
						refresh();//还原棋盘上的一些参数
						system("CLS");//清屏

					} else {
						cout << "此处不能落子！请重新输入！\n";
						Sleep(1000);//延时函数，不然电脑算的太快了根本看不清！
						system("CLS");
					}
				}
			} else if (strcmp(putin, "menu") == 0)//调用菜单
				menu(*tim);
			else if (strcmp(putin, "regret") == 0 && *tim >= 1) {//悔棋
				sum = *tim;
				fileinn(--(*tim));
				system("CLS");
			} else if (strcmp(putin, "forward") == 0 && *tim <= sum) {//悔错了可以前进
				fileinn(++(*tim));
				system("CLS");
			} else {
				cout << "输入不符合规定，请重新输入！\n";
				Sleep(1000);
				system("CLS");
			}
		}
	} else {//无子可下的情况
		print(1);
		cout << "你无处可走，跳过\n";
		Sleep(1000);
		system("CLS");
		*check1 = false;
	}
}
void operatecomputer(bool*check2) {//------by 刘敏行
	int row, col;
	color = 1 - choice;
	int checkiflegal = false;
	judge(color, &checkiflegal);
	if (checkiflegal) {
		//fileout(color, *tim);
		print(0);//电脑打印的时候不显示提示
		clean45();
		cout << "AI努力思考中。。。\n";
		Sleep(1000);
		int best = access(color);//算出了最好的一步
		col = best % 10;
		row = best / 10;
		for (int k = 0; k < 8; k++)
			reversi(row, col, k, 0, color, false);
		chess[row][col] = mark[color]; //标记刚下的棋
		system("CLS");
		refresh();
	} else {
		print(0);
		clean45(); //清除4、5
		cout << "AI无处可走，跳过\n";
		Sleep(1000);
		system("CLS");
		*check2 = false;
	}
}
void operate(int tim) {//------by 刘敏行
	bool check1 = true, check2 = true;//用来判断是否同时无子可下
	while (1) {
		if (choice) {//根据选择的颜色判断先后手
			operatehuman(&tim, &check1);
			operatecomputer(&check2);
		} else {
			operatecomputer(&check2);
			operatehuman(&tim, &check1);
		}
		if (!check1 && !check2) {//注意一个双方都无处可走的情况
			cout << "双方无处可走!\n";
			win();
			return;
		}
	}
}
//以下进入估价函数部分
int chessvalue[10][10];
int evas(int i, int j, int dx, int dy, int color)
//对某一处的某一个方向进行安全性评估,evasafety的子函数         ——by  刘易
{
	i = i + dx;
	j = j + dy;
	if (i > 8 || i < 1 || j > 8 || j < 1)
		return 2; //到达边界，肯定安全
	if (chess[i][j] == 1 - color)
		return 1; //遇到异己颜色，返回1
	if (chess[i][j] == -1 || chess[i][j] == 3)
		return 0; //遇到空格，返回0
	return evas(i, j, dx, dy, color); //遇到己方颜色，继续
}

int evasafety(int i, int j, int color)
//给每个可能下棋的位置进行安全性估分         ——by  刘易
{
	int safety = 0;
	int dir[4][2] = { { 0, 1 }, { 1, 0 }, { 1, 1 }, { -1, 1 } }; //方位数组
	for (int k = 0; k < 4; k++) //4个方向检查安全性
	{
		if (evas(i, j, dir[k][0], dir[k][1], color) + evas(i, j, -dir[k][0],
				-dir[k][1], color) != 1)
			safety += 10; //一个方向安全，就加上10分
	}
	return safety;
}

int evanumber(int color) //计算落子后棋盘上的某一种棋子的数量   ——by  刘易
{
	int chessnum = 0;
	for (int i = 1; i <= 8; i++)
		for (int j = 1; j <= 8; j++)
			if (chess[i][j] == color)
				chessnum += 1;
	return chessnum;
}

int evaaction(int color, int i, int j) //color要下棋时，判断对方的行动力  ——by 刘易
{
	int action = 0;
	refresh(); //先清空所有的3
	chess[i][j] = color;
	int a;
	judge(1 - color, &a);
	for (int k = 1; k <= 8; k++)
		for (int t = 1; t <= 8; t++)
			if (chess[k][t] == 3)
				action += 1; //对方可以落子处
	chess[i][j] = -1;
	refresh();
	judge(color, &a); //原始状态的复原
	return action;
}

int evaspecial(int i, int j, int color) //特殊位置有特殊的分值   ——by  刘易
{
	int special = 0;
	if ((i == 1 && j == 1) || (i == 1 && j == 8) || (i == 8 && j == 1) || (i
			== 8 && j == 8))
		special += 100; //四个角上位置最好了！

	if ((i == 1 && j == 2) || (i == 2 && j == 1) || (i == 1 && j == 7) || (i
			== 2 && j == 8) || (i == 7 && j == 1) || (i == 8 && j == 2) || (i
			== 7 && j == 8) || (i == 8 && j == 7) || (i == 2 && j == 2) || (i
			== 2 && j == 7) || (i == 7 && j == 2) || (i == 7 && j == 7))
		special += -30; //角落旁边的位置不要轻易下
	if ((i == 1 && j == 3) || (i == 3 && j == 1) || (i == 1 && j == 6) || (i
			== 3 && j == 8) || (i == 6 && j == 1) || (i == 8 && j == 3) || (i
			== 8 && j == 5) || (i == 6 && j == 8))
		special += 20; //角落旁边的旁边（贴边位置）有加分
	return special;
}

int rest() { //计算棋盘上还剩余多少空格 ——by  刘易
	int num = 0;
	for (int i = 1; i <= 8; i++)
		for (int j = 1; j <= 8; j++)
			if (chess[i][j] == -1 || chess[i][j] == 3)
				num++;
	return num;
}

bool change = 1; //标记是否改过权重的量
int access(int color) //分值评估系 ——by  刘易
{
	int bestrow, bestcol; //定义最佳选择的行列
	int max = -500; //max赋初值
	memset(chessvalue, 0, sizeof(chessvalue));//价值清零
	if (rest() <= 18 && change) {
		hardship[0] = 2; //如果棋盘上空余的格子已经小于18个了，要加大对于棋子数量评估的权重
		hardship[1] = hardship[1] * 2; //特殊位置同样重要，如果原来的权重不是0，则变为2
		change = 0;
	}
	for (int i = 1; i <= 8; i++)
		for (int j = 1; j <= 8; j++) {
			if (chess[i][j] == 3) {
				chessvalue[i][j] = hardship[0] * evanumber(color) + hardship[1]
						* evaspecial(i, j, color) + hardship[2] * evasafety(i,
						j, color) + hardship[3] * evaaction(color, i, j); //四种评估参数的分值的加权

				if (max < chessvalue[i][j]) {
					max = chessvalue[i][j];
					bestrow = i;
					bestcol = j; //产生最佳选择
				}

			}

		}
	return bestrow * 10 + bestcol;//哈希法存数据
}
int main() {
	system("color F2");//设置背景颜色
	menu(0);
	system("pause");
	return 0;
}
