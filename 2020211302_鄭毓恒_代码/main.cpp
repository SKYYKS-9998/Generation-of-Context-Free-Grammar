#include "head.h"

int main(void)
{
	Grammar g;

	cout << "请输入要简化的上下文无关文法" << endl;
	g.input();

	cout << endl << "简化前" << endl;
	g.output();
	cout << "开始简化" << endl;
	cout << "消除ε产生式" << endl;
	g.removeEGenerate();
	g.output();
	cout << "消除单生成式" << endl;
	g.removeSingleGenerate();
	g.output();
	cout << "消除无用符号" << endl;
	g.removeUselessSymbol();
	g.output();
	cout << "简化完毕" << endl;

	system("pause");
	return 0;
}
