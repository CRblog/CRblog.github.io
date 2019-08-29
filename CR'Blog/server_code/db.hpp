#include<cstdlib>
#include<cstdio>
#include<cstring>
#include<memory>
#include<mysql/mysql.h>
#include<jsoncpp/json/json.h>

namespace blog_system{
	static MYSQL* MySQLInit(){
		MYSQL* connect_fd = mysql_init(NULL);
		if(mysql_real_connect(connect_fd,"127.0.0.1","root","","blogSystem",3306,NULL,0)==NULL){
			printf("链接失败! %s\n",mysql_error(connect_fd));
			return NULL;
		}
		mysql_set_character_set(connect_fd,"utf8");
		return connect_fd;
	}
	
	static void MySQLRelease(MYSQL* mysql){
		mysql_close(mysql);
	}
class BlogTable{
	public:
		BlogTable(MYSQL* mysql):mysql_(mysql){}
		bool Insert(const Json::Value& blog,Json::Value* id){
      std::string s=blog["content"].asString();
      printf("This is string print: %s\n",s.c_str());
			const std::string& content =s;
			std::unique_ptr<char> to(new char[content.size() * 2 + 1]);	
      mysql_real_escape_string(mysql_,to.get(),content.c_str(),content.size());
			std::unique_ptr<char> sql(new char[content.size() * 2+4096]);
			
		  //mysql_set_character_set(mysql_,"utf8");
      sprintf(sql.get(),"insert into blog_table values(null,'%s','%s',%d,'%s')",
          blog["title"].asCString(),
          to.get(),
          blog["tag_id"].asInt(),
          blog["create_time"].asCString());

			int ret = mysql_query(mysql_,sql.get());
			if(ret!=0){
				printf("insert fail！%s\n",mysql_error(mysql_));
				return false;
			}
      char sqlto[1024] = {0};
      sprintf(sqlto,"select blog_id from blog_table order by blog_id desc limit 1");
      MYSQL* sq = mysql_;
      int ret2 = mysql_query(mysql_,sqlto);
      if(ret2!=0)
      {
        printf("获取博客id失败 %s\n",mysql_error(sq));
        return false;
      }

      printf("获取博客id成功\n"); 
			MYSQL_RES* result = mysql_store_result(mysql_); 
			MYSQL_ROW row = mysql_fetch_row(result);
      (*id)["blog_id"] = atoi(row[0]);
      printf("%d\n",(*id)["blog_id"].asInt());
			return true;
		}
		bool SelectAll(Json::Value* blogs,const std::string& tag_id = ""){
			char sql[1024*4] = {0};
			if(tag_id == ""){
				sprintf(sql,"select blog_id,title,tag_id,create_time from blog_table order by blog_id desc");
			}
			else{
				sprintf(sql,"select blog_id,title,tag_id,create_time from blog_table order by blog_id desc where tag_id = %d",std::stoi(tag_id));
			}
			int ret = mysql_query(mysql_,sql);
			if(ret!=0){
				printf("执行查找所有博客失败！%s\n",mysql_error(mysql_));
				return false;
			}
			printf("执行查找所有博客成功！\n");
			MYSQL_RES* result = mysql_store_result(mysql_);
			int rows = mysql_num_rows(result);
			for(int i = 0;i <rows;++i){
				MYSQL_ROW row = mysql_fetch_row(result);
				Json::Value blog;
				blog["blog_id"] = atoi(row[0]);
				blog["title"] = row[1];
				blog["tag_id"] = atoi(row[2]);
				blog["create_time"] = row[3];
				blogs->append(blog);
			}
			mysql_free_result(result);
			return true;
		}
		bool SelectOne(int32_t blog_id ,Json::Value* blog){
			char sql[1024] = {0};
			sprintf(sql,"select blog_id,title,content,tag_id,create_time from blog_table where blog_id = %d",blog_id);
			int ret = mysql_query(mysql_,sql);
			if(ret!=0){
				printf("执行查找博客失败！%s\n",mysql_error(mysql_));
				return false;
			}
			MYSQL_RES* result = mysql_store_result(mysql_);
			int rows = mysql_num_rows(result);
			if(rows!=1){
				printf("查找到的博客不是只有一条，实际有%d条\n",rows);
				return false;
			}
			MYSQL_ROW row = mysql_fetch_row(result);
				(*blog)["blog_id"] = atoi(row[0]);
				(*blog)["title"] = row[1];
				(*blog)["content"] = row[2];
				(*blog)["tag_id"] = atoi(row[3]);
				(*blog)["create_time"] = row[4];
				return true;
		}
		bool Update(const Json::Value& blog){
			const std::string& content = blog["content"].asString();
			std::unique_ptr<char> to(new char[content.size()*2+1]);
			mysql_real_escape_string(mysql_,to.get(),content.c_str(),content.size());
			std::unique_ptr<char> sql(new char[content.size()*2+4096]);
			
			sprintf(sql.get(),"update blog_table set title='%s',content='%s',tag_id=%d where blog_id = %d",
				blog["title"].asCString(),
				to.get(),
				blog["tag_id"].asInt(),
				blog["blog_id"].asInt());
			int ret = mysql_query(mysql_,sql.get());
			if(ret!=0){
				printf("更新博客失败！%s\n",mysql_error(mysql_));
				return false;
			}
				printf("更新博客成功！\n");
        return true;
		}
		bool Delete(int32_t blog_id){
			char sql[1024*4] = {0};
			sprintf(sql,"delete from blog_table where blog_id = %d",blog_id);
			int ret = mysql_query(mysql_,sql);
			if(ret !=0){
				printf("删除博客失败!%s\n",mysql_error(mysql_));
				return false;
			}
			return true;
		}
	private:
		MYSQL* mysql_;
};
class TagTable{
	public:
		TagTable(MYSQL* mysql) : mysql_(mysql){}
		
    bool Insert(const Json::Value& tag){
			char sql[1024*4];
			sprintf(sql,"insert into tag_table values(null,'%s')",tag["tag_name"].asCString());
			int ret = mysql_query(mysql_,sql);
			if(ret!=0){
				printf("插入标签失败%s\n",mysql_error(mysql_));
				return false;
			}
			printf("插入标签成功");
			return true;
		}
		bool Delete(int32_t tag_id){
			char sql[1024*4] = {0};
			sprintf(sql,"delete from tag_table where tag_id = %d",tag_id);
			int ret = mysql_query(mysql_,sql);
			if(ret!=0){
				printf("删除标签失败%s\n",mysql_error(mysql_));
				return false;
			}
			printf("删除标签成功\n");
			return true;
		}
		bool SelectAll(Json::Value* tags){
			char sql[1024*4] = {0};
			sprintf(sql,"select tag_id,tag_name from tag_table");
      printf("--------\n");
			int ret = mysql_query(mysql_,sql);
			if(ret!=0){
				printf("查找标签失败:db.hpp  %s\n",mysql_error(mysql_));
				return false;
			}
			MYSQL_RES* result = mysql_store_result(mysql_);
			int rows = mysql_num_rows(result);
			for(int i = 0;i < rows;++i){
				MYSQL_ROW row = mysql_fetch_row(result);
				Json::Value tag;
				tag["tag_id"] = atoi(row[0]);
				tag["tag_name"] = row[1];
				tags->append(tag);
			}
			printf("查找标签成功 共找到 %d 个 \n",rows);
			return true;
		}
	private:
    MYSQL* mysql_;
};
} //end
