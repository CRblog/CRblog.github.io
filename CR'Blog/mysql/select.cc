#include<cstdio>
#include<cstdlib>
#include<mysql/mysql.h>

int main()
{
  //1.创建句柄
  MYSQL* connect_fd = mysql_init(NULL);
  //2.建立连接
  if(mysql_real_connect(connect_fd,"127.0.0.1","root","","blogSystem",3306,NULL,0)==NULL){
    printf("连接失败---%s\n",mysql_error(connect_fd));
    return 1;
  }
  printf("连接成功!\n");

  //3.设置编码集
  mysql_set_character_set(connect_fd,"utf8");
  //4.拼装SQL语句
  char sql[1024 * 4] = "select * from blog_table";
  //5.执行mysql语句
  int ret = mysql_query(connect_fd,sql);
  if(ret < 0){
    printf("mysql_query failed! %s\n",mysql_error(connect_fd));
    mysql_close(connect_fd);
    return 1;
  }
  //6.遍历结果集合，MYSQL_RES select得到的结果集合
  MYSQL_RES* result = mysql_store_result(connect_fd);
    //a.获取到结果集合中的行数和列数
    int rows = mysql_num_rows(result);
    int fields = mysql_num_fields(result);
    // b. 根据函数和列数遍历结果 
  for(int i = 0;i < rows;++i) 
    {
     // a. 一次获取到一行数据
      MYSQL_ROW row = mysql_fetch_row(result);
      for (int j = 0;j < fields;++j)
      {
        printf("%s\t",row[j]);
      }
      printf("\n");
    }
  //释放结果集合
  mysql_free_result(result);

  mysql_close(connect_fd);
  return 0;
}
