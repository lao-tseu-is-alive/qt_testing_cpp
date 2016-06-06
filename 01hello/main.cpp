#include <QCoreApplication>
#include <iostream>
#include <string>

using namespace std;
int main(int argc, char *argv[])
{
    cout << "Hello World !" <<endl;
    cout << "I did receive " << argc << " arguments" << endl;
    for (int i = 0; i < argc; i++){
        cout << " Argument " << i << " : " << argv[i] << endl;
    }

    string myName = "Carlos GIL";
    string yourName;
    cout << "My name is "  << myName << endl;
    cout << "what's yours ?";
    cin >> yourName;

    cout << "Hello " << yourName << " !!" << endl;



    //QCoreApplication a(argc, argv);

    //return a.exec();
}

