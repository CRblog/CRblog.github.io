create database blogSystem;
use blogSystem;

drop table if exists blog_table;
create table blog_table(
  blog_id int not null primary key auto_increment,
  title varchar(50),
  content text,
  tag_id int,
  create_time varchar(50)
) DEFAULT CHARSET=utf8;

drop table if exists tag_table;
create table tag_table(
  tag_id int not null primary key auto_increment,
  tag_name varchar(50)
)DEFAULT CHARSET=utf8;
