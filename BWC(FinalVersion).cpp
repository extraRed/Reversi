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
 * Author: ����   ������
 */
void print(bool);//��ӡ����
void operatehuman(int *, bool*);//��ͨ��������������
void operatecomputer(bool*);//��������
void operate(int);//���壬�������������Ӻ���
void refresh();//��ԭ������ĳЩ����
void count();//�������ϵ���������������print������
int recordout(int);//��¼ִ����ɫ���µ��˵ڼ�����������������
int recordin(int *);//��ִ����ɫ�Ͳ�����ֵ
int fileout(int);//��¼����״̬
int fileinn(int);//��ֵ����
void win();//�ж���Ӯ
void judge(int, int*);//�ж��Ƿ����
void reversi();//��ת����
int access(int);//���ۺ���

int chess[10][10], hardship[4]; //difficulyΪ�����Ѷȿ��Ƽ�Ȩ�����飬�ֱ������������������λ�á���ȫϵ�����Է��ж���
int dif = 1, ahint = 1; //�����Ѷ�(1\2\3\4)�������ʾ(1ΪҪ��2Ϊ��Ҫ)
int choice;//ѡ�����ɫ
int color; //�Լ�����ɫ��0Ϊ���壬1Ϊ���壬3Ϊ�����Ӵ�,4Ϊ���µĺ��壬5Ϊ���µİ���
int mark[2] = { 5, 4 }; //4Ϊ���µĺ���(1)��5Ϊ���µİ���(0)
int wintim = 0, losetim = 0, tietim = 0;//�ܵ�ս�����

void menu(int tim) //��ʼ�˵�-------by ����
{
	char x[10];
	if (tim)
		cout << "��Ϸ��ͣ\n";
	cout << "�˵�\n";
	cout << "������ѡ������\n1�����루������ǵ�һ�������ո����һ�֣��벻Ҫ�������лл��ϣ���\n2�����¿�ʼ\n3���˳���Ϸ\n";
	if (tim)
		cout << "4�������Ѷ�\n5��������ʾ\n"; //���ѡ��
	cin >> x;
	{
		if (tim) {
			while (x[0] != '1' && x[0] != '2' && x[0] != '3' && x[0] != '4'
					&& x[0] != '5') {
				cout << "�������������" << endl;
				cin >> x;
			}
		} else {
			while (x[0] != '1' && x[0] != '2' && x[0] != '3') {
				cout << "�������������" << endl;
				cin >> x;
			}
		}
	}

	if (x[0] == '1') {//�û�ѡ������
		char empty[2];
		cin.getline(empty, 2);//���ողŵĻس�
		if (recordin(&tim) == -1 || fileinn(tim) == -1) {//��ֹһЩ���ĵ���һ�����͵����룡
			system("CLS");
			cout << "ι��˵�˲��õ������㻹�㣡���������룡\n";
			menu(0);
		} else {
			recordin(&tim);//�����û��ղŵ�ѡ�������岽��
			fileinn(tim);//��������
			refresh();//��ԭһЩ����
			system("CLS");
			if (choice)//������û������֣���ֱ�����в�������
				operate(tim);
			else {//����û��Ǻ��֣���Ҫ���û�����һ����Ȼ������������
				bool check = true;
				operatehuman(&tim, &check);
				operate(tim);
			}
			return;
		}
	} else if (x[0] == '2' || x[0] == '4' || x[0] == '5') {
		char temp = x[0]; //�洢��x��ֵ
		if (temp == '2') { //���¿�ʼ
			memset(chess, -1, sizeof(chess)); //���̳�ʼ��
			chess[4][4] = chess[5][5] = 0;
			chess[4][5] = chess[5][4] = 1;
			cout << "��ѡ����ɫ\n1������\n2������" << endl; //ѡ����ɫ
			cin >> x;
			if (x[0] == '1' || x[0] == '2')
				choice = 2 - atoi(x);
			while (x[0] != '1' && x[0] != '2') {
				cout << "�������������" << endl;
				cin >> x;
			}
		}

		if (temp == '2' || temp == '4') { //���¿�ʼ���ߵ����Ѷȣ�Ҫѡ���Ѷ�
			cout << "��ѡ���Ѷȣ�������ţ�\n1����\n2���м�\n3������\n4��������" << endl;
			cin >> x;
			while (x[0] != '1' && x[0] != '2' && x[0] != '3' && x[0] != '4') {
				cout << "�������������" << endl;
				cin >> x;
			}
			if (x[0] == '1' || x[0] == '2' || x[0] == '3' || x[0] == '4') {
				dif = atoi(x);
				for (int i = 0; i < dif; i++)
					hardship[i] = 1; //���Ѷ�ϵ����ֵ
			}
			if (temp == '4') { //������Ϸ
				char empty[2];
				cin.getline(empty, 2);
				fileinn(tim);
				refresh();
				system("CLS");
				if (choice)//������û������֣���ֱ�����в�������
					operate(tim);
				else {//����û��Ǻ��֣���Ҫ���û�����һ����Ȼ������������
					bool check = true;
					operatehuman(&tim, &check);
					operate(tim);
				}
				return;
			}
		}

		if (temp == '2' || temp == '5') { //���¿�ʼ����������ʾ��Ҫѡ����ʾ
			cout << "�Ƿ���Ҫ�����ʾ����������ţ�\n1����Ҫ\n2������Ҫ" << endl;
			cin >> x;
			while (x[0] != '1' && x[0] != '2' && x[0] != '3' && x[0] != '4') {
				cout << "�������������" << endl;
				cin >> x;
			}
			if (x[0] == '1' || x[0] == '2')
				ahint = atoi(x);
			if (temp == '5') { //������Ϸ
				char empty[2];
				cin.getline(empty, 2);
				fileinn(tim);
				refresh();
				system("CLS");
				if (choice)//������û������֣���ֱ�����в�������
					operate(tim);
				else {//����û��Ǻ��֣���Ҫ���û�����һ����Ȼ������������
					bool check = true;
					operatehuman(&tim, &check);
					operate(tim);
				}
				return;
			}
		}

		char empty[2];
		cin.getline(empty, 2);
		cout << "��Ϸ��ʼ~" << endl;
		operate(0);
	} else
		cout << "лл���´��ټ���\n";
	exit(0);
}
void print(bool x) {//    ����xΪ0ָ���ԣ���Ҫ��ʾ��ʾ-------by ����
	cout << "********************************************" << endl;
	cout << "��ʾ��1�������˵������� 'menu' " << endl;
	cout << "      2������������ 'regret'" << endl;
	cout << "      3���������������� 'forward' " << endl;
	cout << "********************************************" << endl; //��Ϸ�˵���ʾ
	cout << "      1    2   3   4   5   6   7   8 " << endl;
	cout << "    �����ө��ө��ө��ө��ө��ө��ө��� " << endl;
	for (int i = 1; i <= 7; i++) //ǰ7�е����
	{
		cout << i << "   ��";
		for (int j = 1; j <= 7; j++) {

			if (chess[i][j] == -1 || (chess[i][j] == 3
					&& (ahint == 2 || x == 0))) //�м����ӵ����
				cout << "  ��";
			else if (chess[i][j] == 0)
				cout << "��";
			else if (chess[i][j] == 1)
				cout << "��";
			else if (chess[i][j] == 4)
				cout << "����";
			else if (chess[i][j] == 5)
				cout << "��";
			else if (chess[i][j] == 3 && ahint == 1 && x == 1)
				cout << "����";
		}
		if (chess[i][8] == -1 || (chess[i][8] == 3 && (ahint == 2 || x == 0))) //���ұ����ӵ�������߿�Ӵ֣�
			cout << "  ��" << endl;
		else if (chess[i][8] == 0)
			cout << "��" << endl;
		else if (chess[i][8] == 1)
			cout << "��" << endl;
		else if (chess[i][8] == 4)
			cout << "����" << endl;
		else if (chess[i][8] == 5)
			cout << "��" << endl;
		else if (chess[i][8] == 3 && ahint == 1 && x == 1)
			cout << "����" << endl;
		cout << "    �ĩ��੤�੤�੤�੤�੤�੤�੤�� " << endl; //�ָ��е����
	}
	cout << 8 << "   ��"; //�ڰ��е����
	for (int j = 1; j <= 7; j++) {
		if (chess[8][j] == -1 || (chess[8][j] == 3 && (ahint == 2 || x == 0)))
			cout << "  ��";
		else if (chess[8][j] == 0)
			cout << "��";
		else if (chess[8][j] == 1)
			cout << "��";
		else if (chess[8][j] == 4)
			cout << "����";
		else if (chess[8][j] == 5)
			cout << "��";
		else if (chess[8][j] == 3 && ahint == 1 && x == 1)
			cout << "����";

	}
	if (chess[8][8] == -1 || (chess[8][8] == 3 && (ahint == 2 || x == 0))) //�ڰ������ҵ����
		cout << "  ��" << endl;
	else if (chess[8][8] == 0)
		cout << "��" << endl;
	else if (chess[8][8] == 1)
		cout << "��" << endl;
	else if (chess[8][8] == 4)
		cout << "����" << endl;
	else if (chess[8][8] == 5)
		cout << "��" << endl;
	else if (chess[8][8] == 3 && ahint == 1 && x == 1)
		cout << "����" << endl;
	cout << "    �����۩��۩��۩��۩��۩��۩��۩��� " << endl;

	count();//���һ����ʾ��ǰ����
}

bool jud(int x1, int y1, int dx, int dy, int color, bool temp)
//�ж�ĳ��λ����ĳ���������Ƿ�������ӣ�judge�������Ӻ���������By ����
{
	x1 = x1 + dx;
	y1 = y1 + dy; //��÷����ƶ�
	if (x1 < 1 || y1 < 1 || x1 > 8 || y1 > 8)
		return 0; //Խ�緵��
	else if (chess[x1][y1] == 1 - color || chess[x1][y1] == 4 + color) //��ͬ��ɫ
	{
		temp = 1;
		return jud(x1, y1, dx, dy, color, temp);
	} else if (temp && (chess[x1][y1] == color || chess[x1][y1] == 5 - color)) //�������Ӵ�������1
		return 1;
	return 0; //�޷����ӣ�����0
}

void judge(int color, int*checkiflegal) //�ж�ÿ��λ���Ƿ�������ӡ���By ����
{

	for (int x = 1; x <= 8; x++)
		for (int y = 1; y <= 8; y++) {
			if (chess[x][y] == -1) //Ѱ��δ�´�
			{
				for (int dx = -1; dx <= 1; dx++) {
					for (int dy = -1; dy <= 1; dy++) //��8��������
					{
						if (dx == 0 && dy == 0)
							continue;

						if (jud(x, y, dx, dy, color, false)) {
							chess[x][y] = 3; //�������Ӵ�
							*checkiflegal = 1;
						}
					}
				}
			}
		}

}
void reversi(int i, int j, int k, int num, int color, bool findchess) {//--------by ������
	//���������������꣬�����꣬�����ţ��ɳԵ������Ӹ�����ִ�ӷ���ɫ����û���ҵ���ͬ��ɫ������
	int direct[8][2] = { { -1, -1 }, { -1, 0 }, { -1, 1 }, { 0, 1 }, { 1, 1 },
			{ 1, 0 }, { 1, -1 }, { 0, -1 } };//�˸�����
	i += direct[k][0];//��÷����˶�
	j += direct[k][1];
	if (i < 1 || j < 1 || i > 8 || j > 8)//Խ�����˳�
		return;
	{
		if (chess[i][j] == 1 - color) {//�ҵ��˲�ͬ��ɫ������
			findchess = true;
			num++;//ͳ�Ƹ�����ѭ������
			reversi(i, j, k, num, color, findchess);//�ݹ�

		} else if (findchess && chess[i][j] == color) {//���ҵ���ͬ��ɫ�����ӣ��������Է�ת
			while (num >= 0) {
				chess[i -= direct[k][0]][j -= direct[k][1]] = color;//���η�ת
				num--;
			}
			return;
		} else
			return;

	}
}
void refresh() //���̵�3��ԭ--------by ����
{
	for (int i = 1; i <= 8; i++)
		for (int j = 1; j <= 8; j++) {
			if (chess[i][j] == 3)
				chess[i][j] = -1;
		}
}

void clean45() //���̸��´�4��5�ĸ�ԭ����by ����
{
	bool x = 0;
	for (int i = 1; i <= 8; i++) { //������µ�һ��4��5
		for (int j = 1; j <= 8; j++) {
			if (chess[i][j] == 4 || chess[i][j] == 5) {
				chess[i][j] = 5 - chess[i][j];
				x = 1;
				break;
			}
		}
		if (x)
			break; //ֻ��һ��4��5���ҵ���ֱ������
	}
}

void win() {//-------by ������
	int blacknum = 0, whitenum = 0;
	for (int i = 1; i <= 8; i++)
		for (int j = 1; j <= 8; j++) {
			if (chess[i][j] == 0)
				whitenum++;
			else
				blacknum++;
		}
	ifstream infile("win.txt", ios::in);
	infile >> wintim >> losetim >> tietim;//����ս��
	if (choice && whitenum < blacknum || !choice && whitenum > blacknum)//��ʤ
	{
		cout << "��һ�ʤ��\n";
		wintim++;
	} else if (choice && blacknum < whitenum || !choice && blacknum > whitenum)//���
	{
		cout << "���Ի�ʤ��\n";
		losetim++;
	} else {
		cout << "���Ǹ�ƽ�֣�\n";
		tietim++;
	}
	ofstream outfile("win.txt", ios::out);
	outfile << wintim << " " << losetim << " " << tietim;//����ս��
	cout << "��ǰ����ս���ǣ�Ӯ�� " << wintim << "�䣺 " << losetim << "ƽ�֣� " << tietim
			<< endl;
	cout << "ˬ��ˬ������һ�֣�\n������ѡ������\n1������һ��\n2���˳���Ϸ\n";
	bool flag = true;
	while (flag) {
		int ch;
		cin >> ch;
		if (ch == 1) {
			flag = false;
			menu(0);
		} else if (ch == 2) {
			cout << "лл���´��ټ���\n";
			Sleep(1000);
			exit(0);//ֱ��ȫ���˳�
		} else
			cout << "����������\n";
	}
}
void count() {//-------by ������
	int blacknum = 0, whitenum = 0;
	for (int i = 1; i <= 8; i++)
		for (int j = 1; j <= 8; j++) {
			if (chess[i][j] == 0 || chess[i][j] == 5)
				whitenum++;
			if (chess[i][j] == 1 || chess[i][j] == 4)
				blacknum++;
		}
	cout << "��ǰ��������" << blacknum << "  ��ǰ��������" << whitenum << endl;
	if (whitenum == 0 || blacknum == 0 || whitenum + blacknum == 64)//����������һ�����ӻ�����������
		win();
}
//���½����ļ������������
int recordout(int tim) {//---------by������
	ofstream outfile("record.txt", ios::out);
	if (!outfile) {
		cout << "ѡ���ļ���ʧ��!\n";
		return -1;
	}
	outfile << choice << " " << dif << " " << tim;
	outfile.close();
	return 0;
}
int recordin(int *tim) {//------------by������
	ifstream infile("record.txt", ios::in);
	if (!infile) {
		cout << "ѡ���ļ���ʧ��!\n";
		return -1;
	}
	infile >> choice >> dif >> *tim;
	return 0;
}
int fileout(int tim) {//---------by������
	char name[] = "chess?00.txt";//ÿ�ζ�Ҫ�棬ֻ�ܲ��ϸı��ļ���
	char*p = name;//��ָ�벻�ϸ��ļ���
	if (choice)
		*(p + 5) = 'b';//���ֺڰ�
	else
		*(p + 5) = 'w';
	*(p + 6) = char(tim / 10 + '0');//�䶯���ַ�
	*(p + 7) = char(tim % 10 + '0');
	ofstream outfile(name, ios::out);
	if (!outfile) {
		cout << "�����ļ�����ʧ��!\n";
		return -1;
	}
	for (int i = 1; i <= 8; i++) {
		for (int j = 1; j <= 8; j++) {
			outfile << chess[i][j] << " ";//��������
		}
		outfile << endl;
	}

	outfile.close();
	return 0;
}
int fileinn(int tim) {//-------by ������
	char name[] = "chess?00.txt";//ͬ��ÿ�ζ�����ļ����Ǳ���
	char*p = name;
	if (choice)
		*(p + 5) = 'b';
	else
		*(p + 5) = 'w';
	*(p + 6) = char(tim / 10 + '0');
	*(p + 7) = char(tim % 10 + '0');
	ifstream infile(name, ios::in);
	if (!infile) {
		cout << "�����ļ���ʧ��!\n";
		return -1;
	}
	for (int i = 1; i <= 8; i++)
		for (int j = 1; j <= 8; j++)
			infile >> chess[i][j];
	return 0;
}
//���½�����Ϸ���в���
void operatehuman(int *tim, bool*check1) {//------by ������
	int row, col;//�С�������
	int sum = 0;//һ���м������ж��Ƿ����ǰ����forward��
	color = choice;
	int checkiflegal = false;//�Ƿ����ӿ���
	judge(color, &checkiflegal);//�����жϺ���
	if (checkiflegal) {//�����
		bool flag = true;//�����Ƿ�Ϸ����ж���
		while (flag) {
			fileout((*tim));//�����ʱ�������
			print(1);//��ӡ����
			clean45(); //��ԭһЩ���̲���
			cout << "�ֵ����£�������������꣬�м��ÿո�����\n";
			char putin[10];
			cin.getline(putin, 10);//����ָ��
			int len = strlen(putin);//�ж϶���
			if (putin[0] >= '1' && putin[0] <= '8' && putin[1] == ' '
					&& putin[2] >= '1' && putin[2] <= '8' && len == 3) {//������
				row = int(putin[0] - '0');
				col = int(putin[2] - '0');
				sum = 0;//��ԭ
				{
					if (chess[row][col] == 3) {//˵�����������Ϸ�
						(*tim)++;//����1
						recordout((*tim));
						flag = false;
						for (int k = 0; k < 8; k++)
							reversi(row, col, k, 0, color, false);//��ת
						chess[row][col] = mark[choice]; //��Ǹ��µ��壬�����û�����
						refresh();//��ԭ�����ϵ�һЩ����
						system("CLS");//����

					} else {
						cout << "�˴��������ӣ����������룡\n";
						Sleep(1000);//��ʱ��������Ȼ�������̫���˸��������壡
						system("CLS");
					}
				}
			} else if (strcmp(putin, "menu") == 0)//���ò˵�
				menu(*tim);
			else if (strcmp(putin, "regret") == 0 && *tim >= 1) {//����
				sum = *tim;
				fileinn(--(*tim));
				system("CLS");
			} else if (strcmp(putin, "forward") == 0 && *tim <= sum) {//�ڴ��˿���ǰ��
				fileinn(++(*tim));
				system("CLS");
			} else {
				cout << "���벻���Ϲ涨�����������룡\n";
				Sleep(1000);
				system("CLS");
			}
		}
	} else {//���ӿ��µ����
		print(1);
		cout << "���޴����ߣ�����\n";
		Sleep(1000);
		system("CLS");
		*check1 = false;
	}
}
void operatecomputer(bool*check2) {//------by ������
	int row, col;
	color = 1 - choice;
	int checkiflegal = false;
	judge(color, &checkiflegal);
	if (checkiflegal) {
		//fileout(color, *tim);
		print(0);//���Դ�ӡ��ʱ����ʾ��ʾ
		clean45();
		cout << "AIŬ��˼���С�����\n";
		Sleep(1000);
		int best = access(color);//�������õ�һ��
		col = best % 10;
		row = best / 10;
		for (int k = 0; k < 8; k++)
			reversi(row, col, k, 0, color, false);
		chess[row][col] = mark[color]; //��Ǹ��µ���
		system("CLS");
		refresh();
	} else {
		print(0);
		clean45(); //���4��5
		cout << "AI�޴����ߣ�����\n";
		Sleep(1000);
		system("CLS");
		*check2 = false;
	}
}
void operate(int tim) {//------by ������
	bool check1 = true, check2 = true;//�����ж��Ƿ�ͬʱ���ӿ���
	while (1) {
		if (choice) {//����ѡ�����ɫ�ж��Ⱥ���
			operatehuman(&tim, &check1);
			operatecomputer(&check2);
		} else {
			operatecomputer(&check2);
			operatehuman(&tim, &check1);
		}
		if (!check1 && !check2) {//ע��һ��˫�����޴����ߵ����
			cout << "˫���޴�����!\n";
			win();
			return;
		}
	}
}
//���½�����ۺ�������
int chessvalue[10][10];
int evas(int i, int j, int dx, int dy, int color)
//��ĳһ����ĳһ��������а�ȫ������,evasafety���Ӻ���         ����by  ����
{
	i = i + dx;
	j = j + dy;
	if (i > 8 || i < 1 || j > 8 || j < 1)
		return 2; //����߽磬�϶���ȫ
	if (chess[i][j] == 1 - color)
		return 1; //�����켺��ɫ������1
	if (chess[i][j] == -1 || chess[i][j] == 3)
		return 0; //�����ո񣬷���0
	return evas(i, j, dx, dy, color); //����������ɫ������
}

int evasafety(int i, int j, int color)
//��ÿ�����������λ�ý��а�ȫ�Թ���         ����by  ����
{
	int safety = 0;
	int dir[4][2] = { { 0, 1 }, { 1, 0 }, { 1, 1 }, { -1, 1 } }; //��λ����
	for (int k = 0; k < 4; k++) //4�������鰲ȫ��
	{
		if (evas(i, j, dir[k][0], dir[k][1], color) + evas(i, j, -dir[k][0],
				-dir[k][1], color) != 1)
			safety += 10; //һ������ȫ���ͼ���10��
	}
	return safety;
}

int evanumber(int color) //�������Ӻ������ϵ�ĳһ�����ӵ�����   ����by  ����
{
	int chessnum = 0;
	for (int i = 1; i <= 8; i++)
		for (int j = 1; j <= 8; j++)
			if (chess[i][j] == color)
				chessnum += 1;
	return chessnum;
}

int evaaction(int color, int i, int j) //colorҪ����ʱ���ж϶Է����ж���  ����by ����
{
	int action = 0;
	refresh(); //��������е�3
	chess[i][j] = color;
	int a;
	judge(1 - color, &a);
	for (int k = 1; k <= 8; k++)
		for (int t = 1; t <= 8; t++)
			if (chess[k][t] == 3)
				action += 1; //�Է��������Ӵ�
	chess[i][j] = -1;
	refresh();
	judge(color, &a); //ԭʼ״̬�ĸ�ԭ
	return action;
}

int evaspecial(int i, int j, int color) //����λ��������ķ�ֵ   ����by  ����
{
	int special = 0;
	if ((i == 1 && j == 1) || (i == 1 && j == 8) || (i == 8 && j == 1) || (i
			== 8 && j == 8))
		special += 100; //�ĸ�����λ������ˣ�

	if ((i == 1 && j == 2) || (i == 2 && j == 1) || (i == 1 && j == 7) || (i
			== 2 && j == 8) || (i == 7 && j == 1) || (i == 8 && j == 2) || (i
			== 7 && j == 8) || (i == 8 && j == 7) || (i == 2 && j == 2) || (i
			== 2 && j == 7) || (i == 7 && j == 2) || (i == 7 && j == 7))
		special += -30; //�����Աߵ�λ�ò�Ҫ������
	if ((i == 1 && j == 3) || (i == 3 && j == 1) || (i == 1 && j == 6) || (i
			== 3 && j == 8) || (i == 6 && j == 1) || (i == 8 && j == 3) || (i
			== 8 && j == 5) || (i == 6 && j == 8))
		special += 20; //�����Աߵ��Աߣ�����λ�ã��мӷ�
	return special;
}

int rest() { //���������ϻ�ʣ����ٿո� ����by  ����
	int num = 0;
	for (int i = 1; i <= 8; i++)
		for (int j = 1; j <= 8; j++)
			if (chess[i][j] == -1 || chess[i][j] == 3)
				num++;
	return num;
}

bool change = 1; //����Ƿ�Ĺ�Ȩ�ص���
int access(int color) //��ֵ����ϵ ����by  ����
{
	int bestrow, bestcol; //�������ѡ�������
	int max = -500; //max����ֵ
	memset(chessvalue, 0, sizeof(chessvalue));//��ֵ����
	if (rest() <= 18 && change) {
		hardship[0] = 2; //��������Ͽ���ĸ����Ѿ�С��18���ˣ�Ҫ�Ӵ������������������Ȩ��
		hardship[1] = hardship[1] * 2; //����λ��ͬ����Ҫ�����ԭ����Ȩ�ز���0�����Ϊ2
		change = 0;
	}
	for (int i = 1; i <= 8; i++)
		for (int j = 1; j <= 8; j++) {
			if (chess[i][j] == 3) {
				chessvalue[i][j] = hardship[0] * evanumber(color) + hardship[1]
						* evaspecial(i, j, color) + hardship[2] * evasafety(i,
						j, color) + hardship[3] * evaaction(color, i, j); //�������������ķ�ֵ�ļ�Ȩ

				if (max < chessvalue[i][j]) {
					max = chessvalue[i][j];
					bestrow = i;
					bestcol = j; //�������ѡ��
				}

			}

		}
	return bestrow * 10 + bestcol;//��ϣ��������
}
int main() {
	system("color F2");//���ñ�����ɫ
	menu(0);
	system("pause");
	return 0;
}
