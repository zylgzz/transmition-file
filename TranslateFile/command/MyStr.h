#ifndef MYSTRING_H
#define MYSTRING_H
#include<windows.h>

class MyString
{
public:
	MyString();
	static int myStrLen(const char* str)
	{
		int i = 0;
		while (*str++ != '\0')i++;
		return i;
	}
	static void myStrcat(char *str1, const char* str2)
	{
		char *temp = str1;
		while (*temp != '\0')temp++;
		while ((*temp++ = *str2++) != '\0');
	}
	static void myStrcp(char *str1, const char *str2)
	{
		char *temp = str1;
		while ((*temp++ = *str2++) != '\0');

	}
	static void myStrSplit(const char *all_info, char *first_part,
		const char split_char, char*  second_part)
		//²ÎÊý:all_info,first_part,split_char,second_part
	{
		while ((*first_part++ = *all_info++) != split_char);
		*(--first_part) = '\0';
		while ((*second_part++ = *all_info++) != '\0');
	}
	static void myStrReverse(char *str)
	{
		char temp[1024];
		myStrcp(temp, str);
		int len = myStrLen(str);
		while (--len >= 0)*str++ = temp[len];
		*str = '\0';
	}
	static void myStrSubSplitR(char *str,char* sub_str,char type)
	{
		char *temp=sub_str;
		char *temp2=str;
		myStrReverse(str);
		while((*temp++=*temp2++)!=type);
		*(--temp)='\0';
		myStrReverse(sub_str);
		myStrReverse(str);

	}
	static void intToChar(int i, char *str)
	{
		int temp = i;
		char t[1024];
		int loca = 0;
		int remainder = 0;
		while (temp)
		{
			remainder = temp % 10;
			temp = temp / 10;
			t[loca++] = (char)remainder + 48;
		}
		t[loca] = '\0';
		myStrReverse(t);
		myStrcp(str, t);
	}
	static void DwordlongToChar(DWORDLONG i, char *str)
	{
		DWORDLONG temp = i;
		char t[1024];
		int loca = 0;
		int remainder = 0;
		while (temp)
		{
			remainder = temp % 10;
			temp = temp / 10;
			t[loca++] = (char)remainder + 48;
		}
		t[loca] = '\0';
		myStrReverse(t);
		myStrcp(str, t);
	}
	
	static bool myStrSame(const char *str1, const char *str2)
	{
		if (myStrLen(str1) != myStrLen(str2))return false;
		for (int i = 0; i<myStrLen(str1)+1; i++)
		{
			if (*str1++ != *str2++)return false;
		}	
		return true;
	}
	
	static void myStrSubR(const char *str,char* sub_str,int len)
	{
		int str_len=myStrLen(str)-len;
		while(len--)*sub_str++=str[str_len++];
		*sub_str='\0';
	}
	
	static bool myStrContainS(const char *str,const char *sub_str)
	{
		if(myStrLen(str)<myStrLen(sub_str))return false;
		while(*str++==*sub_str++);
		if(*sub_str=='\0')return true;
		return false;
	}
	
	static DWORDLONG charToDwordlong(const char*str)
	{
		int str_len=MyString::myStrLen(str)-1;
		int i=1,num;
		num=str[0]-'0';
		DWORDLONG total=num;
		while(str_len--)
		{
			num=str[i++]-'0';
			total=total*10+num;
		}
		return total;
	}
	static long long myGetSysTimeMicros()
	{

		FILETIME ft;
		LARGE_INTEGER li;
		long long tt = 0;
		GetSystemTimeAsFileTime(&ft);
		li.LowPart = ft.dwLowDateTime;
		li.HighPart = ft.dwHighDateTime;
		tt = li.QuadPart;
		return tt;
	}
	static void getTranslateSpeech(long long start_time,double stander_speech,long long file_size)
	{
		double speech;
		long long now_time= myGetSysTimeMicros();
		speech=file_size*10*10*10*10*10*10*10/(now_time-start_time)/1024.0;
		if(speech>stander_speech)
		{
			Sleep(1);
		// cout<<"have pause :"<<speech<<endl;
			getTranslateSpeech(start_time,stander_speech,file_size);
		}
		else
		{
			//cout<<speech<<endl;
		}
	}
	static void myStrDeleteLen(char *str,long start,long len)
	{
		char *temp=&str[start+len-1];
		char *str2=&str[start-1];
		while((*str2++=*temp++)!='\0');
	}

};

#endif // MYSTRING_H
