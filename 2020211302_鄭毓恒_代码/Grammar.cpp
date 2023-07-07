#include "head.h"

//�����Ų���ʽ
void Grammar::removeEGenerate()
{
	string toE;
	vector<int> loc;
	vector<Generate> p1;
	int symbol, generate;

	algorithm1(toE, "");	//��ȡ���¿շ��ż���

	symbol = p.size();
	for (int i = 0; i < symbol; i++)
	{
		Generate newGen;
		newGen.left = p[i].left;
		p1.push_back(newGen);
		generate = p[i].right.size();
		for (int j = 0; j < generate; j++)
		{
			judgeToE(p[i].right[j], toE, loc);		//�жϵ�ǰ����ʽ�Ҳ���Щ����Ϊ���¿շ��ţ���¼�±�
			if (loc.size())							//�п��¿շ���
				addGenerate(p1[i].right, p[i].right[j], loc, 0);	//����µ�����ʽ
			else
			{		
				if (p[i].right[j].size())			//û�п��¿շ��ţ��Ҳ�Ϊ�գ�ֱ�����
					p1[i].right.push_back(p[i].right[j]);
			}
		}
	}
	p = p1;

	if (isBelong(start, toE))	//�����ʼ��S�ǿ��¿շ���
	{
		Generate newStart;

		//���S1->��|S
		newStart.left = 's';	//��Сдs����S1
		newStart.right.push_back("");
		newStart.right.push_back(string(1, start));

		//��ӵ���һλ
		p.insert(p.begin(), newStart);
		n.insert(n.begin(), 's');
		start = 's';
	}
}

//����������ʽ
void Grammar::removeSingleGenerate()
{
	vector<Generate> p1 = p;

	for (int i = 0; i < n.size(); i++)
	{
		//��ÿ�����ս�����ҳ������ǵ�Ԫż�Եļ��ϣ�����singleGenerate
		string singleGenerate;
		getSingleGenerate(singleGenerate, n[i]);

		//��������ÿһ�����ŵ�����ʽ���뵱ǰ���ŵ�����ʽ��
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

	//��ȥ������ʽ
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

//�������÷���
void Grammar::removeUselessSymbol()
{
	//���������ɷ���
	string generate;		//�����ɷ��ż���
	algorithm1(generate, t);//��ȡ�����ɷ��ż���
	n = generate;			
	deleteGenerate();		//���ս�����ϱ��ˣ�ɾȥ��ص�����ʽ

	algorithm2();			//�����ǿɴ����

	//����������
	sort(n.begin() + 1, n.end());	//��ʼ�������ڵ�һλ
	sort(t.begin(), t.end());
}

//����ķ�
void Grammar::output() const
{
	cout << "���ս������" << n << endl;
	cout << "�ս������" << t << endl;
	cout << "��ʼ����" << start << endl;
	cout << "����ʽ��" << endl;
	for (int i = 0; i < p.size(); i++)
	{
		cout << p[i].left << "��>";
		for (int j = 0; j < p[i].right.size(); j++)
		{
			if (!p[i].right[j].size())
				cout << "��";
			else
				cout << p[i].right[j];
			if (j < p[i].right.size() - 1)
				cout << '|';
		}
		cout << endl;
	}
}

//�����ķ�
void Grammar::input()
{
	cout << "��������ս��������ʼ���ڵ�һλ��������Ű�ASCII����������" << endl;
	cin >> n;
	cout << "�������ս����" << endl;
	cin >> t;
	cout << "��������ʼ��" << endl;
	cin >> start;

	cout << "����������ʽ" << endl;
	for (int i = 0; i < n.size(); i++)
	{
		int num;
		Generate newGen;
		newGen.left = n[i];
		cout << "��������ս��" << n[i] << "�Ҳ����ɽ��������:" << endl;
		cin >> num;
		cin.ignore();

		for (int j = 0; j < num; j++)
		{
			string right;
			cout << "�������Ҳ��ĵ�" << j + 1 << "�����ɽ�����Ŵ���ֱ�ӻ���" << endl;
			getline(cin, right);
			newGen.right.push_back(right);
		}

		p.push_back(newGen);
	}
}

//�㷨1���������ɼ���dest�ķ��Ŵ���set��
void Grammar::algorithm1(string& set, const string& dest) const
{
	string n0;				//���е�N0
	string tmpN;			//N��
	string terminal = dest; //(T U N0)*

	do
	{
		n0 = tmpN;
		terminal += n0;
		for (int i = 0; i < p.size(); i++)
		{
			for (int j = 0; j < p[i].right.size(); j++)
			{
				//����ʽ�Ҳ����ս�������ɷ��ŵļ��ϣ�����ʽ������Ҳ�����ɷ��ţ����뼯��
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

//�㷨2���������ɴ����
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
					//�����пɴ���ż��뼯��
					if (!isBelong(p[i].right[j][k], tmpN))
						tmpN.push_back(p[i].right[j][k]);
				}
			}
		}
	} while (n0.compare(tmpN));

	string tmp;
	//��ȡ�ɴ�����еķ��ս��
	andOfSet(n0, n, tmp);	
	n = tmp;
	//��ȡ�ɴ�����е��ս��
	tmp.clear();
	andOfSet(n0, t, tmp);
	t = tmp;
	deleteGenerate();	//ɾȥ���ɴ������ص�����ʽ
}

//�ж�c�Ƿ����ڼ���set�����򷵻�1�����򷵻�0
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

//�жϼ���s�Ƿ����ڼ���set�����򷵻�1�����򷵻�0
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

//����s1�ͼ���s2�������㣬�������result
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

//�жϴ�s����Щ�ַ����ڿ��¿շ��ż���toE���±����loc
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

//�����Ų���ʽʱ�����ڵݹ�����µ�����ʽ
void Grammar::addGenerate(vector<string>& right, const string& s, const vector<int>& loc, int startLoc)
{
	if (startLoc == loc.size())	//�ݹ���ȫ�����¿շ��ţ���ǰ�Ĵ�s������һ�ֿ��ܵ����ɽ����������ظ������
	{
		if (find(right.begin(), right.end(), s) == right.end())
			right.push_back(s);
	}
	else if (startLoc < loc.size())	//��û�ݹ���ȫ�����¿շ���
	{
		string tmp = s;
		vector<int> newLoc = loc;

		//��ÿ�����¿շ��ţ�������Ϊ�ջ�Ϊ�ա��������ֿ����Խ��еݹ�
		addGenerate(right, s, loc, startLoc + 1);	//��ǰ���¿շ��Ų�Ϊ��
		tmp.erase(loc[startLoc]);	//�赱ǰ����Ϊ��
		for (int i = startLoc + 1; i < newLoc.size(); i++)	//�������ŵ��±���ֱ仯
			newLoc[i]--;
		addGenerate(right, tmp, newLoc, startLoc + 1);	//��ǰ���¿շ���Ϊ��
	}
}

//ɾ�����в����ڷ��ŵ�����ʽ
void Grammar::deleteGenerate()
{
	//ɾ�������ڷ���
	for (vector<Generate>::iterator it = p.begin(); it != p.end();)
	{
		if (!isBelong(it->left, n))
			it = p.erase(it);
		else
			it++;
	}

	int symbolNum = p.size();
	string allSymbol = n + t;
	
	//ɾ����������ʽ�Ҳ��в����ڷ��ŵ����ɽ��
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

//��ȡ����s�����
int Grammar::getSymbolNum(char s)
{
	for (int i = 0; i < n.size(); i++)
	{
		if (n[i] == s)
			return i;
	}
}

//�ݹ��ҳ�s�ĵ�Ԫż�ԣ�����set��
void Grammar::getSingleGenerate(string& set, char s)
{
	int symbolNum = getSymbolNum(s);
	for (int i = 0; i < p[symbolNum].right.size(); i++)
	{
		//������ʽ���Ҳ೤��Ϊ1��Ϊ���ս�����ڲ��ظ�������²ż���set
		if ((p[symbolNum].right[i].size() == 1) && isBelong(p[symbolNum].right[i][0], n) && !isBelong(p[symbolNum].right[i][0], set))
		{
			set += p[symbolNum].right[i];
			getSingleGenerate(set, p[symbolNum].right[i][0]);
		}
	}
}
