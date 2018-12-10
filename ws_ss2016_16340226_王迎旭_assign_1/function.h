#include <string>
#include <iostream>

//测试输入铭文序列是否满足16位十六进制序列，如果数据数目大于16位 那么就提取前16位进行操作 如果数据数目小于16位  就在末尾补0 直到够16位
string Test(string test)
{
	//处理非十六进制字符
	for(int i = 0 ; i < test.size() ; i ++ )
	{
		if( ( '0' <= test[i] && test[i] <= '9' )  || ( 'A' <= test[i] && test[i] <= 'F' ) || ( 'a' <= test[i] && test[i] <= 'f' ) )
		{
			continue ;
		}
		else
		{
			test[i] = '0' ;
		}
	}
	int Charnumber = test.size() ;
	int Processamount = Charnumber / 16 ;
	int Blankamount = Charnumber % 16 ; 
	if( Blankamount != 0 )
	{
		if( Processamount == 0 )
		{
			for(int i = Blankamount ; i < 16 ; i++ )
				test = test + '0' ;
		}
		else
			test.erase(16) ;
	}
	return test ;
}

//小写转大写
char * toUpper(char * src)
{
    char * p = src;
    while( *p )
    {
        if(*p >= 'a' && *p <= 'z' ) 
            *p -= 32 ; 
        p++ ; 
    }
    return src ;  
}


//16进制数组转2进制数组
char * HexToBi(char *hexArray)
{
	toUpper(hexArray) ; 
	char * biArray = new char[65] ;
	int index = 0 ;

    char *p = hexArray ;
    while (*p )
    {
		string temp;
        if( *p >= '0' && *p <= '9' ) 
        {
			temp = HexBiTable[*p - 48] ;
		}
		else if(*p >= 'A' && *p <='F')
		{
			temp = HexBiTable[*p - 65 + 10] ;
		}
		for(int i = 0 ; i < 4 ; i++, index++)
		{
			biArray[index] = temp[i] ;
		}
        p++; 
    }
	biArray[index] = '\0' ;
	return biArray ;
}

//2进制转16进制，4位1组
char * BiToHex(char *biArray)
{
	char *result = new char[17] ; 
	result[16] = '\0' ;
	char temp[5] ; 
	temp[4] = '\0' ; //4位1组
	int index = 0 ;
	for( int i = 0 ; i < 16 ; i++ ) 
	{
		int j = 0 ;
		for( j = 0 ; j < 4 ; j++ ) 
		{
			temp[j] = biArray[i * 4 + j] ;
		}
		string str = temp ; 
		int mark ; 
		for( int k = 0; k < 16; k++ )
		{
			if(str.compare(HexBiTable[k]) == 0) 
			{
				mark = k ;
			}
		}

		if(mark >= 0 && mark <= 9)
		{
			result[i] = mark + 48 ;
		}
		else if(mark >= 10 && mark <=15)
		{	
			result[i] = mark - 10 + 65 ;
		}
	}
	return result;
}

//输出函数
void HexPrint(char *msg)
{
    cout << "The Hex sequence: " << msg << endl ;
    cout << "The Binary sequence: " << HexToBi(msg) << endl ; 
    cout << endl ;
    cout << endl ;
}
//输出函数
void BinaryPrint(char *msg)
{
    cout << "The Hex sequence: " << BiToHex(msg) << endl ;
    cout << "The Binary sequence: " << msg << endl ; 
    cout << endl ;
    cout << endl ;
}

//逆序排列
void Reverse(char *arr, int begin, int end)
{
	char temp;
	for( ; begin < end; begin++, end--)
	{
		temp = arr[end] ;
		arr[end] = arr[begin] ; 
		arr[begin] = temp ;
	}
}

//循环左移n位
void LeftShift(char * arr, int n)
{
	Reverse(arr, 0, n - 1) ; //逆序前n位
	Reverse(arr, n, 28 - 1) ; //逆序后所有位
	Reverse(arr, 0, 28 - 1) ; //逆序所有位
}

//初始置换
void InitialPermutation(char * BiMsg)
{
	char * BiMsgCopy = new char[65] ;
	memcpy(BiMsgCopy, BiMsg, 65 * sizeof(char));
	for(int i = 0; i < 64; i++)
	{
		int index  = IPTable[i] - 1 ; 
		BiMsg[i] = BiMsgCopy[index] ; 
	}
	delete[] BiMsgCopy ; 
}

//64位密钥产生16个48位子密钥
void generateSubKeys(char * BiKey, char subKey[16][49])
{
	char * realKey = new char[57]; realKey[56] = '\0'; 	

	//置换选择1
	for(int i = 0; i < 56; i++)
	{
		int index = PC1Table[i] - 1 ; 
		realKey[i] = BiKey[index] ;
	}

	//C和D
	char *C = new char[29] ; 
	C[28] = '\0' ;
	char *D = new char[29] ; 
	D[28] = '\0' ;
	for(int i = 0; i < 28; i++)
	{
		C[i] = realKey[i] ;
	}
	for(int i = 0, j = 28; i < 28; i++, j++)
	{
		D[i] = realKey[j];
	}
	//产生16个子密钥
	for(int k = 0; k < 16; k++)
	{
		//左移
		LeftShift(C, LeftShiftTable[k]) ;
		LeftShift(D, LeftShiftTable[k]) ;
		char * CDCombine = new char[57] ;  CDCombine[56] = '\0';
		for(int i = 0; i < 28; i++)
		{
			CDCombine[i] = C[i] ;
			CDCombine[i+28] = D[i] ;
		}

		//置换选择2
		for(int i = 0; i < 48; i++)
		{
			int index  = PC2Table[i] - 1 ; 
			subKey[k][i] = CDCombine[index] ; 
		}
		subKey[k][48] = '\0';
	}
}


//DES算法
//输入：64位2进制明文，64位2进制密钥，加密解密方式(mode=0表示加密，mode=1表示解密)
//输出：加密后密文或者解密后明文
char * DES(char * BiMsg, char *BiKey, int mode)
{
	InitialPermutation(BiMsg) ; 
	char subKey[16][49] ; 
	generateSubKeys(BiKey, subKey) ; //产生16个子密钥
	
	char *L = new char[33] ; L[32] = '\0';
	char *R = new char[33] ; R[32] = '\0';
	for(int i = 0; i < 32; i++)
	{
		L[i] = BiMsg[i] ; 
		R[i] = BiMsg[i + 32] ;
	}
	
	//16轮迭代
	for(int k = 0 ; k < 16 ; k++) 
	{
		char *RCopy = new char[33] ;
		memcpy(RCopy, R, 33) ;
		//扩展置换E
		char * ExtendedR = new char[49] ; ExtendedR[48] = '\0';
		for(int i = 0 ; i < 48 ; i++)
		{
			int index = ExtendedETable[i] - 1 ; 
			ExtendedR[i] = R[index] ;
		}

		//异或
		for(int i = 0; i < 48; i++)
		{
			char * temp = new char[65]; 
			if(mode == 0) 
			{
				memcpy(temp, subKey[k], 48);
			}
			else if(mode == 1) 
			{
				memcpy(temp, subKey[15 - k], 48); 
			}
			if(ExtendedR[i] == temp[i])
			{
				ExtendedR[i] = '0';
			}
			else
			{
				ExtendedR[i] = '1';
			}
		}

		//S盒变换
		int indexR = 0 ;
		for(int s = 0; s < 8; s++) 
		{
			char group[7] ; 
			group[6] = '\0' ; 
			for(int i = 0 ; i < 6; i++)
			{
				group[i] = ExtendedR[s * 6 + i] ; 
			}
			int x, y ; 
			char *line = new char[5] ; 
			line[4] = '\0' ;
			char *column = new char[5] ; 
			column[4] = '\0';
			line[0] = '0' ; 
			line[1] = '0' ; 
			line[2] = group[0] ; 
			line[3] = group[5] ; 
			column[0] = group[1] ; 
			column[1] = group[2] ; 
			column[2] = group[3] ; 
			column[3] = group[4] ;

			bool xflag = true , yflag = true; 
			 
			for(int i = 0; i < 16; i++)
			{
				xflag = true ; 
				yflag = true ; 
				
				for(int j = 0 ; j < 4 ; j++)
				{
					if(line[j] != HexBiTable[i][j]) 
					{
						xflag = false ;
					}
					if(column[j] != HexBiTable[i][j]) 
					{
						yflag = false ; 
					}
				}
				if(xflag == true)
				{
					x = i ;
				}
				if(yflag == true)
				{
					y = i ;
				}
			}

			int target = SBox[s][x][y] ;  
			char *biTarget = new char[5] ;
			for(int i = 3, index = 0 ; i >= 0 ; i--, index++)
			{  
				if(target & (1 << i))  
					biTarget[index] =  '1';  
				else  
					biTarget[index] =  '0';  
			}
			biTarget[4] = '\0' ;

			for(int i = 0 ; i < 4 ; i++)
			{
				R[indexR] = biTarget[i] ;
				indexR++ ;
			}
		}

		//P置换
		char *SR = new char[33] ;
		memcpy(SR, R, 33) ;
		for(int i = 0; i < 32; i++)
		{
			int index = PTable[i] - 1 ; 
			R[i] = SR[index] ;
		}
		for(int i = 0 ; i < 32 ; i++ )
		{
			if(R[i] == L[i])
			{
				R[i] = '0' ;
			}
			else
			{
				R[i] = '1' ;
			}
		}

		//L(i) = R(i-1)
		memcpy(L, RCopy, 33) ;
	}
	
	//32位互换
	char *RLChange = new char[65] ;
	for(int i = 0 ; i < 32 ; i++)
	{
		RLChange[i] = R[i] ;
		RLChange[i + 32] = L[i] ; 
	}
	RLChange[64] = '\0' ;

	//逆初始置换
	char *Cipher = new char[65] ; 
	
	Cipher[64] = '\0';
	for(int i = 0; i < 64; i++)
	{
		int index = RIPTable[i] - 1 ;
		Cipher[i] = RLChange[index] ;
	}
	return Cipher ;
}