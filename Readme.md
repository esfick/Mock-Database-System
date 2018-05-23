A C++ REPL mimicking MySql. Reads from an input file for testing, but will read from con

To compile:

Commands so far:

create database [name];
show databases;
use [db name];
drop database [name];
show tables;
create table[name];
create table[name](init list);
describe [table name];
alter table [name] drop column [name];
alter table [name] add column [name][type];
alter table [name] change [old name][new name];
alter table [name] modify [name name][new type];
alter table [name] add unique [name];
alter table [name] drop index [name];
alter table [name] set key [name];
alter table [name] unset key [name];

Types supported so far: int, double, boolean, char (max 255)

Init list in table creation is inside parens delineated by commas. Each substring in list is in format
[fieldname][field type]
with additional options pkey (make primary key), !null (make not nullable), and unique (require unique values)

To do:
- Add more types
- Add more commands inc. SQL
- Expand mysql style table initialization
- Be able to create table values
- Change back to cin
- Saving and loading
