#include <fstream>
#include <iostream>
#include <random>

using namespace std;

char testo(int n) {
    char r;
    switch (n) {
        case 0:
            r = 'a';
            break;
        case 1:
            r = '1';
            break;
        case 2:
            r = ',';
            break;
        case 3:
            r = ' ';
            break;
        default:
            break;
    }
    return r;
}

int main() {
    long long int n, x;
    long long int text[4];

    cout << "caratteri = ";
    cin >> text[0];
    cout << endl;
    cout << "numeri = ";
    cin >> text[1];
    cout << endl;
    cout << "punteggiatura = ";
    cin >> text[2];
    cout << endl;
    cout << "spazi = ";
    cin >> text[3];
    cout << endl;

    n = text[0] + text[1] + text[2] + text[3];
    cout << n;

    ofstream test("test.txt");

    for (int i = 0; i < n; i++) {
        x = rand() % 4;
        if (text[x] > 0) {
            text[x]--;
            char k = testo(x);
            test << k;
        } else {
            i--;
        }
    }
    return 0;
}