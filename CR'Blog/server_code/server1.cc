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
	//��������
	server.Post("/blog",[&blog_table](const Request& req,Response& resp){
		printf("��������!\n");
		
		Json::Reader reader;
		Json::FastWriter writer;
		Json::Value req_json;
		Json::Value resp_json;
		bool ret = reader.parse(req.body,req_json);
		if(!ret){
			printf("�����������%s\n",req.body.c_str());
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
		 	printf("�������ݸ�ʽ�д�!%s\n",req.body.c_str());
		 	
		 	resp_json["ok"] = false;
		 	resp_json["reason"] = "input data format error!";
		 	resp.status = 400;
		 	resp.set_content(writer.write(resp_json),"application/json");
			return;
		 }
		 ret = blog_table.Insert(req_json);
		 if(!ret){
		 	printf("���Ͳ���ʧ��!");
		 	resp_json["ok"] = false;
		 	resp_json["reason"] = "blog insert failed";
		 	resp.status = 500;
		 	resp.set_content(writer.write(resp_json),"application/json");
		 	return ;
		 }
		 printf("���Ͳ���ɹ�\n");
     resp_json["ok"] = true;
     resp.set_content(writer.write(resp_json),"application/json");
     return ;
	});
	//�鿴���в����б�
	server.Get("/blog",[&blog_table](const Request& req,Response& resp){
		printf("�鿴���в���!\n");
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
	//�鿴ĳ������
	server.Get(R"(/blog/(\d+))",[&blog_table](const Request& req,Response& resp){
		int32_t blog_id = std::stoi(req.matches[1].str());
		printf("�鿴idΪ %d �Ĳ���!\n",blog_id);
		Json::Value resp_json;
    Json::FastWriter writer; 
    bool ret = blog_table.SelectOne(blog_id,&resp_json);
		if(!ret){
			resp_json["ok"] = false;
		 	resp_json["reason"] = "�鿴ָ������ʧ��: "+std::to_string(blog_id);
		 	resp.status = 404;
		 	resp.set_content(writer.write(resp_json),"application/json");
		 	return ;
		}
		resp.set_content(writer.write(resp_json),"application/json");
		 	return ;
	});
    //�޸�ĳ������
	server.Put(R"(/blog/(\d+))",[&blog_table](const Request& req,Response& resp){
		int32_t blog_id = std::stoi(req.matches[1].str());
		printf("�޸�idΪ %d �Ĳ���!\n",blog_id);
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
	//ɾ��ĳ������
	server.Delete(R"(/blog/(\d+))",[&blog_table](const Request& req,Response& resp){
		Json::FastWriter writer;
		Json::Value resp_json;
		int32_t blog_id = std::stoi(req.matches[1].str());
		printf("ɾ��idΪ %d �Ĳ���!\n",blog_id);
		
		bool ret = blog_table.Delete(blog_id);
		if(!ret){
			printf("ִ��ɾ��ʧ��!\n");
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
	//������ǩ
	server.Post("/tag",[&tag_table](const Request& req,Response& resp){
		Json::Reader reader;
		Json::FastWriter writer;
		Json::Value req_json;
		Json::Value resp_json;
		bool ret = reader.parse(req.body,req_json);
		if(!ret){
			printf("�����ǩʧ��0\n");
			resp_json["ok"] = false;
		 	resp_json["reason"] = "insert tag req parse failed";
		 	resp.status = 400;
		 	resp.set_content(writer.write(resp_json),"application/json");
		 	return;
		}
		if(req_json["tag_name"].empty()){
			printf("�����ǩʧ��1\n");
			resp_json["ok"] = false;
		 	resp_json["reason"] = "insert tag req format failed";
		 	resp.status = 400;
		 	resp.set_content(writer.write(resp_json),"application/json");
		 	return;
		}
		ret = tag_table.Insert(req_json);
		if(!ret){
			printf("�����ǩʧ��2\n");
			resp_json["ok"] = false;
		 	resp_json["reason"] = "insert tag req database failed";
		 	resp.status = 500;
		 	resp.set_content(writer.write(resp_json),"application/json");
		 	return;
		}
		
	});
	//ɾ����ǩ
	server.Delete(R"(/tag/(\d+))",[&tag_table](const Request& req,Response& resp){
		Json::FastWriter writer;
		Json::Value resp_json;
		int32_t tag_id = std::stoi(req.matches[1].str());
		printf("ɾ��idΪ %d �ı�ǩ!\n",tag_id);
		bool ret = tag_table.Delete(tag_id);
		if(!ret){
			printf("ɾ��ʧ��\n");
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
	//�鿴���б�ǩ
	server.Get("/tag",[&tag_table](const Request& req,Response& resp){
		Json::FastWriter writer;
		Json::Value resp_json;
		bool ret = tag_table.SelectAll(&resp_json);
			if(!ret){
			printf("ɾ��ʧ��\n");
			resp_json["ok"] = false;
		 	resp_json["reason"] = "get tag database failed";
		 	resp.status = 500;
		 	resp.set_content(writer.write(resp_json),"application/json");
		 	return;
		}
			resp_json["ok"] = true;
		resp.set_content(writer.write(resp_json),"application/json");
		return;

	});
	server.set_base_dir("./wwwroot");
	server.listen("0.0.0.0",9093);
	return 0;
}
