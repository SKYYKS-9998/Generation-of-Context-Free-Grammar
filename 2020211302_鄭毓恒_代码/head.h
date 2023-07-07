#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

//����ʽ���ݽṹ
typedef struct
{
	char left;				//�����ս��
	vector<string> right;	//�Ҳ����ɽ��
}Generate;

//�ķ���
class Grammar
{
public:
	void removeEGenerate();		//�����Ų���ʽ
	void removeSingleGenerate();//����������ʽ
	void removeUselessSymbol();	//�������÷���
	void input();				//�����ķ�
	void output() const;		//����ķ�

private:
	void algorithm1(string& set, const string& dest) const;	//�㷨1���������ɼ���dest�ķ��Ŵ���set��
	void algorithm2();										//�㷨2���������ɴ����

	int isBelong(char c, const string& set) const;			//�ж�c�Ƿ����ڼ���set�����򷵻�1�����򷵻�0
	int isBelong(const string& s, const string& set) const;	//�жϼ���s�Ƿ����ڼ���set�����򷵻�1�����򷵻�0
	void andOfSet(const string& s1, const string& s2, string& result) const;	//����s1�ͼ���s2�������㣬�������result
	void deleteGenerate();														//ɾ�����в����ڷ��ŵ�����ʽ
	void judgeToE(const string& s, const string& toE, vector<int>& loc);		//�жϴ�s����Щ�ַ����ڿ��¿շ��ż���toE���±����loc
	void addGenerate(vector<string>& right, const string& s, const vector<int>& loc, int startLoc);	//�����Ų���ʽʱ�����ڵݹ�����µ�����ʽ
	int getSymbolNum(char s);							//��ȡ����s�����
	void getSingleGenerate(string& set, char s);		//�ݹ��ҳ�s�ĵ�Ԫż�ԣ�����set��

public:
	char start;			//��ʼ��
	string n, t;		//���ս�����ϡ��ս������
	vector<Generate> p;	//����ʽ����
};