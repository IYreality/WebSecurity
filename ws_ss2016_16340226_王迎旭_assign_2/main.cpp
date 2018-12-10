//main文件使用 参考RCF 1321中的待测main.c文件的代码  测试7组序列 与已知情况进行对比 
#include <iostream>
#include "MD5.h"
using namespace std ;
#define LEN 7

int main() {

    string input[LEN] = {
        "",
        "a",
        "abc",
        "message digest",
        "abcdefghijklmnopqrstuvwxyz",
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
        "12345678901234567890123456789012345678901234567890123456789012345678901234567890"};

    string expect[LEN] = {
        "d41d8cd98f00b204e9800998ecf8427e", "0cc175b9c0f1b6a831c399e269772661",
        "900150983cd24fb0d6963f7d28e17f72", "f96b697d7cb7938d525a2f31aaf161d0",
        "c3fcd3d76192e4007dfb496cca67e13b", "d174ab98d277d9f5a5611c2c9f419d9f",
        "57edf4a22be3c955ac49da2e2107b67a"};

    for (int i = 0; i < LEN; ++i) {
        MD5 md5(input[i]);
        cout << "--------------------------------" << endl;
        cout << "Test " << i << ":" << endl;
        cout << "Original Message: " << input[i] << endl;
        cout << "Expected Result:  " << expect[i] << endl;
        auto digest = md5.getMessage();
        cout << "Calculate Result: " << digest << endl;
        auto flag = (digest == expect[i]) ? true : false;
        if (flag) {
            cout << "PASS" << endl;
        } else {
            cout << "FAILED" << endl;
        }
    }

    return 0;
}