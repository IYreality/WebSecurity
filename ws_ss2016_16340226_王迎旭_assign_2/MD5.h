
/*
注：
Ⅰ、参考RCF 1321中已经实现的MD5的C代码  自己使用c++实现一遍并完成debug  实现思路大致相同但具体的方法会有所差异
Ⅱ、使用bitset存放A B C D,方便进行操作
Ⅲ、使用vector<bool>存储每一位的二进制数据方便进行删减
*/
#include <iostream>
#include <vector>
#include <bitset>
#include <stdio.h>
using namespace std ;
//使用RCF 1321中的宏定义
#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))
#define ROTATELEFT(num, n) (((num) << (n)) | ((num) >> (32 - (n))))
/*FF(a,b,c,d,x,s,ac）表示 a = b + ((a + F(b,c,d) + x + ac) << s)

  GG(a,b,c,d,x,s,ac）表示 a = b + ((a + G(b,c,d) + x + ac) << s)
  
  HH(a,b,c,d,x,s,ac）表示 a = b + ((a + H(b,c,d) + x + ac) << s)
  
  Ⅱ（a,b,c,d,x,s,ac）表示 a = b + ((a + I(b,c,d) + x + ac) << s)
*/
#define FF(a, b, c, d, x, s, ac)            \
    {                                       \
        (a) += F((b), (c), (d)) + (x) + ac; \
        (a) = ROTATELEFT((a), (s));         \
        (a) += (b);                         \
    }
#define GG(a, b, c, d, x, s, ac)            \
    {                                       \
        (a) += G((b), (c), (d)) + (x) + ac; \
        (a) = ROTATELEFT((a), (s));         \
        (a) += (b);                         \
    }
#define HH(a, b, c, d, x, s, ac)            \
    {                                       \
        (a) += H((b), (c), (d)) + (x) + ac; \
        (a) = ROTATELEFT((a), (s));         \
        (a) += (b);                         \
    }
#define II(a, b, c, d, x, s, ac)            \
    {                                       \
        (a) += I((b), (c), (d)) + (x) + ac; \
        (a) = ROTATELEFT((a), (s));         \
        (a) += (b);                         \
    }

typedef unsigned int bit32;

// 使用MD5类方法 
class MD5
{
    public:
        //声明构造函数
        MD5(const string &str);
        //初始化
        void init() ;
        //MD5算法操作核心
        const string getMessage();
        //补位操作
        void Padding();
        //这里完成数据的大端存储
        void binarySort();
        //增加长度
        void appendLength(); 
        //解码
        void Decode(int begin, bit32* x);
        //转32位bool类型vector为 bit32类型
        bit32 convert(const vector<bool>& a);
        //轮转操作函数
        void transform(int beginIndex);
        //转字符串输出
        const string toStr();
        
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
};
