#include <iostream>

using namespace std;

int main()
{
    int someD = 15;

	for (int i = 0; i < someD; i++)
	{
		cout << int(float(i) / someD * 4) << endl;
	}
}