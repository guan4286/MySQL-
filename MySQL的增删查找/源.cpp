#define _CRT_SECURE_NO_WARNINGS
#include<iostream>

#include <stdio.h>
#include <WinSock.h>  //һ��Ҫ�������������winsock2.h
#include "mysql.h"    //����mysqlͷ�ļ�(һ�ַ�ʽ����vcĿ¼�������ã�һ�����ļ��п�������Ŀ¼��Ȼ����������)
#include <Windows.h>

//�������������Ҳ�����ڹ���--������������
#pragma comment(lib,"wsock32.lib")
#pragma comment(lib,"libmysql.lib")
MYSQL mysql; //mysql����
MYSQL_FIELD* fd;  //�ֶ�������
char field[32][32];  //���ֶ�����ά����
MYSQL_RES* res; //����ṹ�������е�һ����ѯ�����
MYSQL_ROW column; //һ�������ݵ����Ͱ�ȫ(type-safe)�ı�ʾ����ʾ�����е���
char query[150]; //��ѯ���

bool ConnectDatabase();     //��������
void FreeConnect();
bool QueryDatabase1();  //��ѯ1
bool QueryDatabase2();  //��ѯ2
bool InsertData();
bool ModifyData();
bool DeleteData();
int main(int argc, char** argv)
{
	ConnectDatabase();
	QueryDatabase1();
	InsertData();
	QueryDatabase2();
	ModifyData();
	QueryDatabase2();
	DeleteData();
	QueryDatabase2();
	FreeConnect();
	system("pause");
	return 0;
}
//�������ݿ�
bool ConnectDatabase()
{
	//��ʼ��mysql
	mysql_init(&mysql);  //����mysql�����ݿ�

	//����false������ʧ�ܣ�����true�����ӳɹ�
	//�������ݿ�
	//�ж��������ʧ�ܾͰ�����ʧ�ܵ���Ϣ��ʾ���������Ǻý��ж�Ӧ�޸ġ�
	// mysql_real_connect������2.���ص�ַ 3.���mysql�û��� 4.���mysql���� 5.���ݿ����� 6.�˿ں�
	if (!(mysql_real_connect(&mysql, "localhost", "root", "123456", "mysql", 0, NULL, 0))) //�м�ֱ����������û��������룬���ݿ������˿ںţ�����дĬ��0����3306�ȣ���������д�ɲ����ٴ���ȥ
	{
		printf("Error connecting to database:%s\n", mysql_error(&mysql));
		return false;
	}
	else
	{
		printf("Connected...\n");
		return true;
	}
}
//�ͷ���Դ
void FreeConnect()
{
	//�ͷ���Դ
	mysql_free_result(res);
	mysql_close(&mysql);
}
/***************************���ݿ����***********************************/
//��ʵ���е����ݿ����������д��sql��䣬Ȼ����mysql_query(&mysql,query)����ɣ������������ݿ�����ɾ�Ĳ�
//��ѯ����
bool QueryDatabase1()
{
	sprintf(query, "select * from runoob_tbl"); //ִ�в�ѯ��䣬�����ǲ�ѯ���У�user�Ǳ��������ü����ţ���strcpyҲ����
	mysql_query(&mysql, "set names gbk"); //���ñ����ʽ��SET NAMES GBKҲ�У�������cmd����������
	//����0 ��ѯ�ɹ�������1��ѯʧ��
	if (mysql_query(&mysql, query))        //ִ��SQL���
	{
		printf("Query failed (%s)\n", mysql_error(&mysql));
		return false;
	}
	else
	{
		printf("query success\n");
	}
	//��ȡ�����
	if (!(res = mysql_store_result(&mysql)))    //���sql�������󷵻صĽ����
	{
		printf("Couldn't get result from %s\n", mysql_error(&mysql));
		return false;
	}

	//��ӡ��������
	printf("number of dataline returned: %d\n", mysql_affected_rows(&mysql));

	//��ȡ�ֶε���Ϣ
	char* str_field[32];  //����һ���ַ�������洢�ֶ���Ϣ
	for (int i = 0; i < 4; i++)   //����֪�ֶ�����������»�ȡ�ֶ���
	{
		str_field[i] = mysql_fetch_field(res)->name;
	}
	for (int i = 0; i < 4; i++)   //��ӡ�ֶ�
		printf("%10s\t", str_field[i]);
	printf("\n");
	//��ӡ��ȡ������
	while (column = mysql_fetch_row(res))   //����֪�ֶ���������£���ȡ����ӡ��һ��
	{
		printf("%10s\t%10s\t%10s\t%10s\n", column[0], column[1], column[2], column[3]);  //column��������
	}
	return true;
}
bool QueryDatabase2()
{
	mysql_query(&mysql, "set names gbk");
	//����0 ��ѯ�ɹ�������1��ѯʧ��
	if (mysql_query(&mysql, "select * from tcount_tbl"))        //ִ��SQL���
	{
		printf("Query failed (%s)\n", mysql_error(&mysql));
		return false;
	}
	else
	{
		printf("query success\n");
	}
	res = mysql_store_result(&mysql);
	//��ӡ��������
	printf("number of dataline returned: %d\n", mysql_affected_rows(&mysql));
	for (int i = 0; fd = mysql_fetch_field(res); i++)  //��ȡ�ֶ���
		strcpy(field[i], fd->name);
	int j = mysql_num_fields(res);  // ��ȡ����
	for (int i = 0; i < j; i++)  //��ӡ�ֶ�
		printf("%10s\t", field[i]);
	printf("\n");
	while (column = mysql_fetch_row(res))
	{
		for (int i = 0; i < j; i++)
			printf("%10s\t", column[i]);
		printf("\n");
	}
	return true;
}
//��������
bool InsertData()
{
	sprintf(query, "insert into user values (NULL, 'Lilei', 'wyt2588zs','lilei23@sina.cn');");  //������취ʵ���ֶ��ڿ���̨�ֶ�����ָ��
	if (mysql_query(&mysql, query))        //ִ��SQL���
	{
		printf("Query failed (%s)\n", mysql_error(&mysql));
		return false;
	}
	else
	{
		printf("Insert success\n");
		return true;
	}
}
//�޸�����
bool ModifyData()
{
	sprintf(query, "update user set email='lilei325@163.com' where name='Lilei'");
	if (mysql_query(&mysql, query))        //ִ��SQL���
	{
		printf("Query failed (%s)\n", mysql_error(&mysql));
		return false;
	}
	else
	{
		printf("Insert success\n");
		return true;
	}
}
//ɾ������
bool DeleteData()
{
	/*sprintf(query, "delete from user where id=6");*/
	char query[100];
	printf("please input the sql:\n");
	gets_s(query);  //�����ֶ�����sql���
	if (mysql_query(&mysql, query))        //ִ��SQL���
	{
		printf("Query failed (%s)\n", mysql_error(&mysql));
		return false;
	}
	else
	{
		printf("Insert success\n");
		return true;
	}
}


INSERT INTO runoob_tbl(runoob_title, runoob_author, submission_date) VALUES ("ѧϰ PHP", "����̳�", NOW());

INSERT INTO runoob_tbl (runoob_title, runoob_author, submission_date) VALUES ("ѧϰ MySQL", "����̳�", NOW());

INSERT INTO runoob_tbl  (runoob_title, runoob_author, submission_date) VALUES  ("JAVA �̳�", "RUNOOB.COM", '2016-05-06');