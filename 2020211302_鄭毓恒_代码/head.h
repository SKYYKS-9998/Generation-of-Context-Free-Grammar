#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

//生成式数据结构
typedef struct
{
	char left;				//左侧非终结符
	vector<string> right;	//右侧生成结果
}Generate;

//文法类
class Grammar
{
public:
	void removeEGenerate();		//消除ε产生式
	void removeSingleGenerate();//消除单产生式
	void removeUselessSymbol();	//消除无用符号
	void input();				//输入文法
	void output() const;		//输出文法

private:
	void algorithm1(string& set, const string& dest) const;	//算法1，将可生成集合dest的符号存在set中
	void algorithm2();										//算法2，消除不可达符号

	int isBelong(char c, const string& set) const;			//判断c是否属于集合set，是则返回1，否则返回0
	int isBelong(const string& s, const string& set) const;	//判断集合s是否属于集合set，是则返回1，否则返回0
	void andOfSet(const string& s1, const string& s2, string& result) const;	//集合s1和集合s2作与运算，结果存在result
	void deleteGenerate();														//删除含有不存在符号的生成式
	void judgeToE(const string& s, const string& toE, vector<int>& loc);		//判断串s中哪些字符属于可致空符号集合toE，下标存在loc
	void addGenerate(vector<string>& right, const string& s, const vector<int>& loc, int startLoc);	//消除ε产生式时，用于递归产生新的生成式
	int getSymbolNum(char s);							//获取符号s的序号
	void getSingleGenerate(string& set, char s);		//递归找出s的单元偶对，存在set中

public:
	char start;			//起始符
	string n, t;		//非终结符集合、终结符集合
	vector<Generate> p;	//生成式集合
};