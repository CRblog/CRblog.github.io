#include"httplib.h"

#include"db.hpp"
MYSQL* mysql =NULL;
int main(){
	using namespace httplib;
	using namespace blog_system;
	mysql = blog_system::MySQLInit();
	signal(SIGINT,[](int){blog_system::MySQLRelease(mysql);exit(0);});
	BlogTable blog_table(mysql);
	TagTable tag_table(mysql);
	
	Server server;
	//新增博客
	server.Post("/blog",[&blog_table](const Request& req,Response& resp){
		printf("新增博客!\n");	
		Json::Reader reader;
		Json::FastWriter writer;
		Json::Value req_json;
		Json::Value resp_json;
		bool ret = reader.parse(req.body,req_json);
		if(!ret){
			printf("解析请求出错！%s\n",req.body.c_str());
			resp_json["ok"] = false;
			resp_json["reason"] = "input data format error!";
			resp.set_content(writer.write(resp_json),"application/json");
			return;
		}
		if(req_json["title"].empty()
		 ||req_json["content"].empty()
		 ||req_json["tag_id"].empty()
		 ||req_json["create_time"].empty())
		 {
		 	printf("请求数据格式有错!%s\n",req.body.c_str());
		 	
		 	resp_json["ok"] = false;
		 	resp_json["reason"] = "input data format error!";
		 	resp.status = 400;
		 	resp.set_content(writer.write(resp_json),"application/json");
			return;
		 }
		 ret = blog_table.Insert(req_json,&resp_json);
		 if(!ret){
		 	printf("博客插入失败!");
		 	resp_json["ok"] = false;
		 	resp_json["reason"] = "blog insert failed";
		 	resp.status = 500;
		 	resp.set_content(writer.write(resp_json),"application/json");
		 	return ;
		 }
		 printf("博客插入成功\n");
     resp_json["ok"] = true;
     resp.set_content(writer.write(resp_json),"application/json");
     return ;
	});
	//查看所有博客列表
	server.Get("/blog",[&blog_table](const Request& req,Response& resp){
		printf("查看所有博客!\n");
	  const std::string& tag_id = req.get_param_value("tag_id");
    Json::FastWriter writer;
		Json::Value resp_json;
		bool ret = blog_table.SelectAll(&resp_json,tag_id);
		if(!ret){
			resp_json["ok"] = false;
		 	resp_json["reason"] = "blog select all failed";
		 	resp.status = 500;
		 	resp.set_content(writer.write(resp_json),"application/json");
		 	return ;
		}
		resp.set_content(writer.write(resp_json),"application/json");
		return;
	});
	//查看某个博客
	server.Get(R"(/blog/(\d+))",[&blog_table](const Request& req,Response& resp){
		int32_t blog_id = std::stoi(req.matches[1].str());
		printf("查看id为 %d 的博客!\n",blog_id);
		Json::Value resp_json;
    Json::FastWriter writer; 
    bool ret = blog_table.SelectOne(blog_id,&resp_json);
		if(!ret){
			resp_json["ok"] = false;
		 	resp_json["reason"] = "查看指定博客失败: "+std::to_string(blog_id);
		 	resp.status = 404;
		 	resp.set_content(writer.write(resp_json),"application/json");
		 	return ;
		}
		resp.set_content(writer.write(resp_json),"application/json");
		 	return ;
	});
    //修改某个博客
	server.Put(R"(/blog/(\d+))",[&blog_table](const Request& req,Response& resp){
		int32_t blog_id = std::stoi(req.matches[1].str());
		printf("修改id为 %d 的博客!\n",blog_id);
		Json::Reader reader;
		Json::FastWriter writer;
		Json::Value req_json;
		Json::Value resp_json;
		bool ret = reader.parse(req.body,req_json);
		if(!ret){
			resp_json["ok"] = false;
			resp_json["reason"] = "update blog parse request failed";
			resp.status = 400;
			resp.set_content(writer.write(resp_json),"application/json");
			return;
		}
		if(req_json["title"].empty()
		 ||req_json["content"].empty()
		 ||req_json["tag_id"].empty()){

		 	resp_json["ok"] = false;
		 	resp_json["reason"] = "update blog request format error!";
		 	resp.status = 400;
		 	resp.set_content(writer.write(resp_json),"application/json");
			return;
		 }
		 req_json["blog_id"] = blog_id;
		 ret = blog_table.Update(req_json);
		if(!ret){
			resp_json["ok"] = false;
			resp_json["reason"] = "update blog database failed";
			resp.status = 500;
			resp.set_content(writer.write(resp_json),"application/json");
			return;
		}
		resp.set_content(writer.write(resp_json),"application/json");
		return;
	});
	//删除某个博客
	server.Delete(R"(/blog/(\d+))",[&blog_table](const Request& req,Response& resp){
		Json::FastWriter writer;
		Json::Value resp_json;
		int32_t blog_id = std::stoi(req.matches[1].str());
		printf("删除id为 %d 的博客!\n",blog_id);
		
		bool ret = blog_table.Delete(blog_id);
		if(!ret){
			printf("执行删除失败!\n");
			resp_json["ok"] = false;
		 	resp_json["reason"] = "delete blog failed";
		 	resp.status = 500;
		 	resp.set_content(writer.write(resp_json),"application/json");
		 	return ;
		}
		resp_json["ok"] = true;
		resp.set_content(writer.write(resp_json),"application/json");
		return;
	});
	//新增标签
	server.Post("/tag",[&tag_table](const Request& req,Response& resp){
		Json::Reader reader;
		Json::FastWriter writer;
		Json::Value req_json;
		Json::Value resp_json;
		bool ret = reader.parse(req.body,req_json);
		if(!ret){
			printf("插入标签失败0\n");
			resp_json["ok"] = false;
		 	resp_json["reason"] = "insert tag req parse failed";
		 	resp.status = 400;
		 	resp.set_content(writer.write(resp_json),"application/json");
		 	return;
		}
		if(req_json["tag_name"].empty()){
			printf("插入标签失败1\n");
			resp_json["ok"] = false;
		 	resp_json["reason"] = "insert tag req format failed";
		 	resp.status = 400;
		 	resp.set_content(writer.write(resp_json),"application/json");
		 	return;
		}
		ret = tag_table.Insert(req_json);
		if(!ret){
			printf("插入标签失败2\n");
			resp_json["ok"] = false;
		 	resp_json["reason"] = "insert tag req database failed";
		 	resp.status = 500;
		 	resp.set_content(writer.write(resp_json),"application/json");
		 	return;
		}
		
	});
	//删除标签
	server.Delete(R"(/tag/(\d+))",[&tag_table](const Request& req,Response& resp){
		Json::FastWriter writer;
		Json::Value resp_json;
		int32_t tag_id = std::stoi(req.matches[1].str());
		printf("删除id为 %d 的标签!\n",tag_id);
		bool ret = tag_table.Delete(tag_id);
		if(!ret){
			printf("删除失败111\n");
			resp_json["ok"] = false;
		 	resp_json["reason"] = "delete tag database failed";
		 	resp.status = 500;
		 	resp.set_content(writer.write(resp_json),"application/json");
		 	return;
		}
		resp_json["ok"] = true;
		resp.set_content(writer.write(resp_json),"application/json");
		return;
		
	});
	//查看所有标签
	server.Get("/tag",[&tag_table](const Request& req,Response& resp){
		Json::FastWriter writer;
		Json::Value resp_json;
		bool ret = tag_table.SelectAll(&resp_json);
			
    if(!ret){
			printf("获取标签失败\n");
			resp_json["ok"] = false;
		 	resp_json["reason"] = "get tag database failed";
		 	resp.status = 500;
		 	resp.set_content(writer.write(resp_json),"application/json");
		 	return;
		}
		  resp.set_content(writer.write(resp_json),"application/json");

	});
	server.set_base_dir("./wwwroot");
	server.listen("0.0.0.0",9093);
	return 0;
}
