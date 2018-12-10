#include <iostream>
#include <string>
#include "data_table.h"
#include "function.h"
using namespace std;


int main()
{
	//创建待加密明文序列
    char HexMsg[17] ; 
    memset(HexMsg,0,sizeof(char)*17);
    //创建密文序列
    char HexKey[17] ;
    memset(HexKey,0,sizeof(char)*17);

	//测试输入序列
    cout << "Input the pending sequence" << endl ;
    cout << "Sequence:";
    string Testinput ;
    //输入铭文序列
    cin >> Testinput ;
    Testinput = Test(Testinput);
    for(int i = 0 ; i < 16 ; i ++ )
    {
        HexMsg[i] = Testinput[i] ;
    }
    HexMsg[16] = '\0' ;
    //将十六进制序列转为二进制序列
    char *BiMsg = HexToBi(HexMsg);
    HexPrint(HexMsg);

    //输入密文序列
    cout << "Input the ciphertext sequence" << endl ;
    cout << "Sequence:";
    cin >> Testinput ;
    Testinput = Test(Testinput);
    for(int i = 0 ; i < 16 ; i ++ )
    {
        HexKey[i] = Testinput[i] ;
    }
    HexKey[16] = '\0' ;
    //将十六进制序列转为二进制序列
    char *BiKey = HexToBi(HexKey);
    HexPrint(HexKey);

    char *CipherMsg = DES(BiMsg, BiKey, 0) ;
    cout << "After process encryption " << endl ;
    BinaryPrint(CipherMsg);

    char *PlainMsg = DES(CipherMsg, BiKey, 1);
    cout << "After process decryption " << endl ;
    BinaryPrint(PlainMsg);	
}
