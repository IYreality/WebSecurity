#include "MD5.h"
#include <stdio.h>
using namespace std ;
/*
 private:
        //输入的字符串
        string input ;
        //使用vector存放输入字符串的二进制信息
        vector<bool> binary_input ;
        //记录初始信息的长度
        int length ;
        //存放length的二进制码
        vector<bool> binary_length ;
        //使用 bitset 类型的 A B C D
        bit32 A , B , C , D ;
*/
MD5::MD5(const string &str)
{
    input = str ;
}
//初始化
void MD5::init()
{
    //先清除原先存在输入信息
    binary_input.clear();
    for(int i = 0 ; i < input.size() ; i ++ )
    {
        auto temp = bitset<8> (input[i]);
        for(int j = 7 ; j >= 0 ; j -- )
        {
            binary_input.push_back(temp[j]) ;
        } 
    }
    //清除原先存在的长度二进制信息
    binary_length.clear();
    length = binary_input.size();
    bitset<64> temp(length) ;
    for(int i = 63 ; i >= 0 ; i -- )
    {
        binary_length.push_back(temp[i]);
    }
}

//MD5算法操作核心
const string MD5::getMessage()
{
    /*
        步骤依次为：
            Ⅰ、初始化
            Ⅱ、补足448位
            Ⅲ、末尾加上64位长度
            Ⅳ、分组操作
            Ⅴ、返回
    */
    //Ⅰ 、
    init();
    //Ⅱ、
    Padding();
    //Ⅲ、
    appendLength();
    //给 private 中的 A B C D 初始化
    A = 0x67452301;
    B = 0xefcdab89;
    C = 0x98badcfe;
    D = 0x10325476;
    //Ⅳ、
    int number = binary_input.size() / 512 ;
    for(int i = 0 ; i < number ; i ++ )
    {
        transform(i) ;
    }
    //Ⅴ、
    return toStr() ;
}

//补位操作
void MD5::Padding()
{
    int difference = 0 ;
    if( length % 512 < 448)
    {
        difference = 448 - length%512 ; 
    }
    else
    {
        difference = 960 - length%512 ;
    }
    //末尾补1 再加0
    binary_input.push_back(1);
    vector<bool> pad(difference-1,0);
    binary_input.insert(binary_input.end(),pad.begin(),pad.end());
    binarySort();
}

//这里完成数据的大端存储
void MD5::binarySort()
{
    int number = binary_input.size() / 32 ;
    vector<bool> ret ;
    for(int i = 0 ; i < number ; i ++ )
    {
        vector<bool> word(binary_input.begin() + i * 32 , binary_input.begin() + (i + 1) * 32);
        ret.insert(ret.end(), word.begin() + 24, word.end());
        ret.insert(ret.end(), word.begin() + 16, word.begin() + 24);
        ret.insert(ret.end(), word.begin() + 8, word.begin() + 16);
        ret.insert(ret.end(), word.begin(), word.begin() + 8);
    } 
    binary_input.clear();
    binary_input.insert(binary_input.end(),ret.begin(),ret.end());
}

//增加长度
void MD5::appendLength() 
{
    binary_input.insert(binary_input.end(), binary_length.begin() + 32, binary_length.end());
    binary_input.insert(binary_input.end(), binary_length.begin(), binary_length.begin() + 32);
};

//解码
void MD5::Decode(int begin, bit32* x)
{
    vector<bool>::const_iterator start = binary_input.begin() + begin;
    vector<bool>::const_iterator end = binary_input.begin() + begin + 512;
    vector<bool> cv(start, end);

    for (int i = 0; i < 16; ++i) 
    {
        vector<bool>::const_iterator first = cv.begin() + 32 * i;
        vector<bool>::const_iterator last = cv.begin() + 32 * (i + 1);
        vector<bool> newvec(first, last);

        x[i] = convert(newvec);
    }
}

//转32位bool类型vector为 bit32类型
bit32 MD5::convert(const vector<bool>& a) 
{
    int partlen = 32;
    bit32 res = 0;
    for (int i = 0; i < partlen; ++i) 
    {
        res = res | (a[i] << (partlen - i - 1));
    }
    return res;
}

//转换
void MD5::transform(int beginIndex) 
{
    bit32 AA = A, BB = B, CC = C, DD = D;

    bit32 x[16];
    Decode(512 * beginIndex, x);

    // Round 1 
    FF(A, B, C, D, x[0], S11, 0xd76aa478);
    FF(D, A, B, C, x[1], S12, 0xe8c7b756);
    FF(C, D, A, B, x[2], S13, 0x242070db);
    FF(B, C, D, A, x[3], S14, 0xc1bdceee);
    FF(A, B, C, D, x[4], S11, 0xf57c0faf);
    FF(D, A, B, C, x[5], S12, 0x4787c62a);
    FF(C, D, A, B, x[6], S13, 0xa8304613);
    FF(B, C, D, A, x[7], S14, 0xfd469501);
    FF(A, B, C, D, x[8], S11, 0x698098d8);
    FF(D, A, B, C, x[9], S12, 0x8b44f7af);
    FF(C, D, A, B, x[10], S13, 0xffff5bb1);
    FF(B, C, D, A, x[11], S14, 0x895cd7be);
    FF(A, B, C, D, x[12], S11, 0x6b901122);
    FF(D, A, B, C, x[13], S12, 0xfd987193);
    FF(C, D, A, B, x[14], S13, 0xa679438e);
    FF(B, C, D, A, x[15], S14, 0x49b40821);

    // Round 2 
    GG(A, B, C, D, x[1], S21, 0xf61e2562);
    GG(D, A, B, C, x[6], S22, 0xc040b340);
    GG(C, D, A, B, x[11], S23, 0x265e5a51);
    GG(B, C, D, A, x[0], S24, 0xe9b6c7aa);
    GG(A, B, C, D, x[5], S21, 0xd62f105d);
    GG(D, A, B, C, x[10], S22, 0x2441453);
    GG(C, D, A, B, x[15], S23, 0xd8a1e681);
    GG(B, C, D, A, x[4], S24, 0xe7d3fbc8);
    GG(A, B, C, D, x[9], S21, 0x21e1cde6);
    GG(D, A, B, C, x[14], S22, 0xc33707d6);
    GG(C, D, A, B, x[3], S23, 0xf4d50d87);
    GG(B, C, D, A, x[8], S24, 0x455a14ed);
    GG(A, B, C, D, x[13], S21, 0xa9e3e905);
    GG(D, A, B, C, x[2], S22, 0xfcefa3f8);
    GG(C, D, A, B, x[7], S23, 0x676f02d9);
    GG(B, C, D, A, x[12], S24, 0x8d2a4c8a);

    // Round 3 
    HH(A, B, C, D, x[5], S31, 0xfffa3942);
    HH(D, A, B, C, x[8], S32, 0x8771f681);
    HH(C, D, A, B, x[11], S33, 0x6d9d6122);
    HH(B, C, D, A, x[14], S34, 0xfde5380c);
    HH(A, B, C, D, x[1], S31, 0xa4beea44);
    HH(D, A, B, C, x[4], S32, 0x4bdecfa9);
    HH(C, D, A, B, x[7], S33, 0xf6bb4b60);
    HH(B, C, D, A, x[10], S34, 0xbebfbc70);
    HH(A, B, C, D, x[13], S31, 0x289b7ec6);
    HH(D, A, B, C, x[0], S32, 0xeaa127fa);
    HH(C, D, A, B, x[3], S33, 0xd4ef3085);
    HH(B, C, D, A, x[6], S34, 0x4881d05);
    HH(A, B, C, D, x[9], S31, 0xd9d4d039);
    HH(D, A, B, C, x[12], S32, 0xe6db99e5);
    HH(C, D, A, B, x[15], S33, 0x1fa27cf8);
    HH(B, C, D, A, x[2], S34, 0xc4ac5665);

    // Round 4 
    II(A, B, C, D, x[0], S41, 0xf4292244);
    II(D, A, B, C, x[7], S42, 0x432aff97);
    II(C, D, A, B, x[14], S43, 0xab9423a7);
    II(B, C, D, A, x[5], S44, 0xfc93a039);
    II(A, B, C, D, x[12], S41, 0x655b59c3);
    II(D, A, B, C, x[3], S42, 0x8f0ccc92);
    II(C, D, A, B, x[10], S43, 0xffeff47d);
    II(B, C, D, A, x[1], S44, 0x85845dd1);
    II(A, B, C, D, x[8], S41, 0x6fa87e4f);
    II(D, A, B, C, x[15], S42, 0xfe2ce6e0);
    II(C, D, A, B, x[6], S43, 0xa3014314);
    II(B, C, D, A, x[13], S44, 0x4e0811a1);
    II(A, B, C, D, x[4], S41, 0xf7537e82);
    II(D, A, B, C, x[11], S42, 0xbd3af235);
    II(C, D, A, B, x[2], S43, 0x2ad7d2bb);
    II(B, C, D, A, x[9], S44, 0xeb86d391);

    A = A + AA;
    B = B + BB;
    C = C + CC;
    D = D + DD;
}

//转字符串输出
const string MD5::toStr() 
{
    bit32 input[4];
    input[0] = A;
    input[1] = B;
    input[2] = C;
    input[3] = D;

    string ret;
    char buffer[4];

    for (int i = 0; i < 4; ++i) 
    {
        for (int j = 0; j < 4; ++j) 
        {
            snprintf(buffer, 4, "%02x", input[i] >> j * 8 & 0xff);
            ret += buffer;
        }
    }
    return ret;
}