#include "head.h"

//消除ε产生式
void Grammar::removeEGenerate()
{
	string toE;
	vector<int> loc;
	vector<Generate> p1;
	int symbol, generate;

	algorithm1(toE, "");	//获取可致空符号集合

	symbol = p.size();
	for (int i = 0; i < symbol; i++)
	{
		Generate newGen;
		newGen.left = p[i].left;
		p1.push_back(newGen);
		generate = p[i].right.size();
		for (int j = 0; j < generate; j++)
		{
			judgeToE(p[i].right[j], toE, loc);		//判断当前生成式右侧哪些符号为可致空符号，记录下标
			if (loc.size())							//有可致空符号
				addGenerate(p1[i].right, p[i].right[j], loc, 0);	//添加新的生成式
			else
			{		
				if (p[i].right[j].size())			//没有可致空符号，且不为空，直接添加
					p1[i].right.push_back(p[i].right[j]);
			}
		}
	}
	p = p1;

	if (isBelong(start, toE))	//如果起始符S是可致空符号
	{
		Generate newStart;

		//添加S1->ε|S
		newStart.left = 's';	//用小写s代替S1
		newStart.right.push_back("");
		newStart.right.push_back(string(1, start));

		//添加到第一位
		p.insert(p.begin(), newStart);
		n.insert(n.begin(), 's');
		start = 's';
	}
}

//消除单产生式
void Grammar::removeSingleGenerate()
{
	vector<Generate> p1 = p;

	for (int i = 0; i < n.size(); i++)
	{
		//对每个非终结符，找出与其是单元偶对的集合，存在singleGenerate
		string singleGenerate;
		getSingleGenerate(singleGenerate, n[i]);

		//将集合中每一个符号的生成式加入当前符号的生成式中
		for (int j = 0; j < singleGenerate.size(); j++)
		{
			int num = getSymbolNum(singleGenerate[j]);
			for (int k = 0; k < p[num].right.size(); k++)
			{
				string tmp(p[num].right[k]);
				if (find(p1[i].right.begin(), p1[i].right.end(), tmp) == p1[i].right.end())
					p1[i].right.push_back(tmp);
			}
		}
	}

	//消去单生成式
	for (int i = 0; i < p1.size(); i++)
	{
		for (vector<string>::iterator it = p1[i].right.begin(); it != p1[i].right.end();)
		{
			string tmp(*it);
			if (tmp.size() == 1 && isBelong(tmp, n))
				it = p1[i].right.erase(it);
			else
				it++;
		}
	}
	
	p = p1;
}

//消除无用符号
void Grammar::removeUselessSymbol()
{
	//消除非生成符号
	string generate;		//可生成符号集合
	algorithm1(generate, t);//获取可生成符号集合
	n = generate;			
	deleteGenerate();		//非终结符集合变了，删去相关的生成式

	algorithm2();			//消除非可达符号

	//给集合排序
	sort(n.begin() + 1, n.end());	//起始符保持在第一位
	sort(t.begin(), t.end());
}

//输出文法
void Grammar::output() const
{
	cout << "非终结符集：" << n << endl;
	cout << "终结符集：" << t << endl;
	cout << "起始符：" << start << endl;
	cout << "生成式：" << endl;
	for (int i = 0; i < p.size(); i++)
	{
		cout << p[i].left << "—>";
		for (int j = 0; j < p[i].right.size(); j++)
		{
			if (!p[i].right[j].size())
				cout << "ε";
			else
				cout << p[i].right[j];
			if (j < p[i].right.size() - 1)
				cout << '|';
		}
		cout << endl;
	}
}

//输入文法
void Grammar::input()
{
	cout << "请输入非终结符集，起始符在第一位，其余符号按ASCII码升序输入" << endl;
	cin >> n;
	cout << "请输入终结符集" << endl;
	cin >> t;
	cout << "请输入起始符" << endl;
	cin >> start;

	cout << "请输入生成式" << endl;
	for (int i = 0; i < n.size(); i++)
	{
		int num;
		Generate newGen;
		newGen.left = n[i];
		cout << "请输入非终结符" << n[i] << "右部生成结果的数量:" << endl;
		cin >> num;
		cin.ignore();

		for (int j = 0; j < num; j++)
		{
			string right;
			cout << "请输入右部的第" << j + 1 << "个生成结果，ε串则直接换行" << endl;
			getline(cin, right);
			newGen.right.push_back(right);
		}

		p.push_back(newGen);
	}
}

//算法1，将可生成集合dest的符号存在set中
void Grammar::algorithm1(string& set, const string& dest) const
{
	string n0;				//书中的N0
	string tmpN;			//N’
	string terminal = dest; //(T U N0)*

	do
	{
		n0 = tmpN;
		terminal += n0;
		for (int i = 0; i < p.size(); i++)
		{
			for (int j = 0; j < p[i].right.size(); j++)
			{
				//生成式右侧是终结符和生成符号的集合，生成式左侧符号也是生成符号，加入集合
				if (isBelong(p[i].right[j], terminal) && !isBelong(p[i].left, tmpN))	
				{
					tmpN.push_back(p[i].left);
					break;
				}
			}
		}
	} while (n0.compare(tmpN));
	
	set = tmpN;
}

//算法2，消除不可达符号
void Grammar::algorithm2()
{
	string n0(1, start);
	string tmpN(1, start);

	do
	{
		n0 = tmpN;
		for (int i = 0; i < p.size(); i++)
		{
			for (int j = 0; j < p[i].right.size(); j++)
			{
				for (int k = 0; k < p[i].right[j].size(); k++)
				{
					//将所有可达符号加入集合
					if (!isBelong(p[i].right[j][k], tmpN))
						tmpN.push_back(p[i].right[j][k]);
				}
			}
		}
	} while (n0.compare(tmpN));

	string tmp;
	//获取可达符号中的非终结符
	andOfSet(n0, n, tmp);	
	n = tmp;
	//获取可达符号中的终结符
	tmp.clear();
	andOfSet(n0, t, tmp);
	t = tmp;
	deleteGenerate();	//删去不可达符号相关的生成式
}

//判断c是否属于集合set，是则返回1，否则返回0
int Grammar::isBelong(char c, const string& set) const
{
	int found = 0;

	for (int i = 0; i < set.size() && !found; i++)
	{
		if (c == set[i])
			found = 1;
	}

	return found;
}

//判断集合s是否属于集合set，是则返回1，否则返回0
int Grammar::isBelong(const string& s, const string& set) const
{
	for (int i = 0; i < s.size(); i++)
	{
		int found = 0;
		for (int j = 0; j < set.size() && !found; j++)
		{
			if (s[i] == set[j])
				found = 1;
		}
		if (!found)
			return 0;
	}

	return 1;
}

//集合s1和集合s2作与运算，结果存在result
void Grammar::andOfSet(const string& s1, const string& s2, string& result) const
{
	for (int i = 0; i < s1.size(); i++)
	{
		int found = 0;
		for (int j = 0; j < s2.size() && !found; j++)
		{
			if (s1[i] == s2[j])
				found = 1;
		}
		if (found)
			result.push_back(s1[i]);
	}
}

//判断串s中哪些字符属于可致空符号集合toE，下标存在loc
void Grammar::judgeToE(const string& s, const string& toE, vector<int>& loc)
{
	loc.clear();

	for (int i = 0; i < s.size(); i++)
	{
		if (isBelong(s[i], toE))
			loc.push_back(i);
	}

	sort(loc.begin(), loc.end());
}

//消除ε产生式时，用于递归产生新的生成式
void Grammar::addGenerate(vector<string>& right, const string& s, const vector<int>& loc, int startLoc)
{
	if (startLoc == loc.size())	//递归完全部可致空符号，当前的串s是其中一种可能的生成结果，如果不重复则添加
	{
		if (find(right.begin(), right.end(), s) == right.end())
			right.push_back(s);
	}
	else if (startLoc < loc.size())	//还没递归完全部可致空符号
	{
		string tmp = s;
		vector<int> newLoc = loc;

		//对每个可致空符号，它可能为空或不为空。根据两种可能性进行递归
		addGenerate(right, s, loc, startLoc + 1);	//当前可致空符号不为空
		tmp.erase(loc[startLoc]);	//设当前符号为空
		for (int i = startLoc + 1; i < newLoc.size(); i++)	//其他符号的下标出现变化
			newLoc[i]--;
		addGenerate(right, tmp, newLoc, startLoc + 1);	//当前可致空符号为空
	}
}

//删除含有不存在符号的生成式
void Grammar::deleteGenerate()
{
	//删除不存在符号
	for (vector<Generate>::iterator it = p.begin(); it != p.end();)
	{
		if (!isBelong(it->left, n))
			it = p.erase(it);
		else
			it++;
	}

	int symbolNum = p.size();
	string allSymbol = n + t;
	
	//删除所有生成式右侧有不存在符号的生成结果
	for (int i = 0; i < symbolNum; i++)
	{
		for (vector<string>::iterator it = p[i].right.begin(); it != p[i].right.end();)
		{
			if (!isBelong(*it, allSymbol))
				it = p[i].right.erase(it);
			else
				it++;
		}
	}
}

//获取符号s的序号
int Grammar::getSymbolNum(char s)
{
	for (int i = 0; i < n.size(); i++)
	{
		if (n[i] == s)
			return i;
	}
}

//递归找出s的单元偶对，存在set中
void Grammar::getSingleGenerate(string& set, char s)
{
	int symbolNum = getSymbolNum(s);
	for (int i = 0; i < p[symbolNum].right.size(); i++)
	{
		//单生成式的右侧长度为1，为非终结符。在不重复的情况下才加入set
		if ((p[symbolNum].right[i].size() == 1) && isBelong(p[symbolNum].right[i][0], n) && !isBelong(p[symbolNum].right[i][0], set))
		{
			set += p[symbolNum].right[i];
			getSingleGenerate(set, p[symbolNum].right[i][0]);
		}
	}
}
