#include "head.h"

int main(void)
{
	Grammar g;

	cout << "������Ҫ�򻯵��������޹��ķ�" << endl;
	g.input();

	cout << endl << "��ǰ" << endl;
	g.output();
	cout << "��ʼ��" << endl;
	cout << "�����Ų���ʽ" << endl;
	g.removeEGenerate();
	g.output();
	cout << "����������ʽ" << endl;
	g.removeSingleGenerate();
	g.output();
	cout << "�������÷���" << endl;
	g.removeUselessSymbol();
	g.output();
	cout << "�����" << endl;

	system("pause");
	return 0;
}
