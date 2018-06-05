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
display table [table name];
create index [index name] on [table name] ([column(s)]);
alter table [name] drop column [name];
alter table [name] add column [name][type];
alter table [name] change [old column name][new column name];
alter table [name] modify [column name][new type];
alter table [name] add unique [column name];
alter table [name] drop unique [column name];
alter table [name] set key [column name];
alter table [name] unset key [column name];
insert into [table] [(attrib names)] values [(values)];




Types supported so far: int, double, boolean, char (max 255)

Init list in table creation is inside parens delineated by commas. Each substring in list is in format
[fieldname][field type]
with additional options pkey (make primary key), !null (make not nullable), and unique (require unique values)

When inserting into a table, specify attribute names and corresponding values in
comma-separated lists inside parens. Attributes listed must match pairwise with
those in the specified table.


To do:
- Add more types
- Add more commands inc. SQL
- Expand mysql style table initialization
- Be able to create table values - process values and interpret!
- Change back to cin
- Saving and loading
