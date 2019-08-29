
//使用MySQLapi完成数据的插入功能

#include <cstdio>
#include <cstdlib>
// 编译器默认从 /usr/include目录中查找头文件,mysql.h
#include <mysql/mysql.h>


int main()
{
  //1.创建一个数据库连接的句柄
  MYSQL* connect_fd = mysql_init(NULL);
  //2.和数据库建立连接(应用层)
  if(mysql_real_connect(connect_fd,"127.0.0.1","root","","blogSystem",3306,NULL,0)==NULL){
    printf("connect error ! %s\n",mysql_error(connect_fd));
    return 1;
  }
    //参数：   句柄：connect_fd    
    //         数据库服务器的IP地址
    //         服务器的端口号
    //         用户名密码
    //         数据库名
    //         unix_sock   NULL 
    //         client_flag 0  
  printf("connect success!\n");
  //3.设置编码方式
  //      mysql server 部分最初安装的时候已经设置成了utf8
  //      客户端也要使用语句设置成utf8
  mysql_set_character_set(connect_fd,"utf8");
 // 4.拼接SQL语句
  char sql[1024 * 4] = {0};
  char title[] = "我的博客";
  char content[] = "大家好，我是练习时长两年半的实习生";
  int tag_id = 1;
  char date[] = "2019/07/25";
  sprintf(sql,"insert into blog_table values(null,'%s','%s',%d,'%s')",title,content,tag_id,date);
  printf("sql: %s\n",sql);
  //4.让数据库服务器执行SQL
  int ret = mysql_query(connect_fd,sql);
  if(ret<0)
  {
    printf("执行sql失败，%s\n",mysql_error(connect_fd));
    mysql_close(connect_fd);
  }
  mysql_close(connect_fd);
  return 0;
}
