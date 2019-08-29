#include"db.hpp"
/*
void TestBlogTable(){
	MYSQL* mysql = blog_system::MySQLInit();
	blog_system::BlogTable blog_table(mysql);
	Json::Value blog;

  blog["title"] = "标题";
	blog["content"] = "测试文本";
	blog["tag_id"] = 1;
	blog["create_time"] = "2019/01/01";

  bool ret = blog_table.Insert(blog);
	
  printf("insert: %d\n",ret);
	blog_system::MySQLRelease(mysql);
}


void TestBlogTable(){
	Json::StyledWriter writer;
	MYSQL* mysql = blog_system::MySQLInit();
	blog_system::BlogTable blog_table(mysql);
  bool ret = false;
	Json::Value blogs;
	
	
	ret = blog_table.SelectAll(&blogs);
	printf("select all %d\n",ret);
	printf("%s\n",writer.write(blogs).c_str());
	blog_system::MySQLRelease(mysql);
}

void TestBlogTable(){
	Json::StyledWriter writer;
	MYSQL* mysql = blog_system::MySQLInit();
	blog_system::BlogTable blog_table(mysql);
	bool ret = false;
	Json::Value blog;


	ret = blog_table.SelectOne(3,&blog);
	printf("select one %d\n",ret);
	printf("blog: %s\n",writer.write(blog).c_str());
	blog_system::MySQLRelease(mysql);
}

void TestBlogTable(){
	Json::StyledWriter writer;
	MYSQL* mysql = blog_system::MySQLInit();
	blog_system::BlogTable blog_table(mysql);
	bool ret = false;
	Json::Value blog;


	blog["blog_id"] = 3;
	blog["title"] = "我的";
	blog["content"] = "啊1231";
	ret = blog_table.Update(blog);
	blog_system::MySQLRelease(mysql);
}
 

void TestBlogTable(){
	MYSQL* mysql = blog_system::MySQLInit();
	blog_system::TagTable tag_table(mysql);
	bool ret = false;
	Json::Value tag;
  tag["tag_id"] = 3;
  tag["tag_name"] = "数据结构";

	ret = tag_table.Insert(tag);
	blog_system::MySQLRelease(mysql);
}
*/

void TestBlogTable(){
	Json::StyledWriter writer;
	MYSQL* mysql = blog_system::MySQLInit();
	blog_system::BlogTable blog_table(mysql);
	bool ret = false;
	Json::Value blog;


	ret = blog_table.SelectOne(1,&blog);
	printf("select one %d\n",ret);
	printf("blog: %s\n",writer.write(blog).c_str());
	blog_system::MySQLRelease(mysql);
}
int main(){
  TestBlogTable();	
	return 0;
}




