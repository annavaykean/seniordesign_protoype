# SQL commands to create and populate MySQL database for Senior Design
# Group 7, Smart Chair, Spring-Summer 2019

# if database already exists, delete and create it again
# do this to reset for testing

drop database if exists seniordesign;

create database seniordesign;

# switch to seniordesign database
use seniordesign;

create table users (
	userID integer not null auto_increment,
	email varchar(100) not null,
	firstName varchar(50),
	lastName varchar(50),
	password varchar(100) not null,
	securityAnswer varchar(300),
	primary key (userID)
)Engine=InnoDB;

create table posture (
	postureID integer not null auto_increment,
	userID integer not null,
	created_at datetime default current_timestamp,
	cogX numeric(15, 10) not null,
	cogY numeric(15, 10) not null,
	primary key (postureID),
	foreign key (userID) references users(userID)
	on delete cascade
	on update cascade
)Engine=InnoDB;

# enter new users
insert into users (userID, email, firstName, lastName, password, securityAnswer)
values ('0', 'johnDoe@gmail.com', 'John', 'Doe', 'hashmelater', 'dog');

insert into users (email, firstName, lastName, password, securityAnswer)
values ('janeDoe@gmail.com', 'Jane', 'Doe', 'hashmelater', 'cat');

insert into users (email, firstName, lastName, password, securityAnswer)
values ('robertFrost@gmail.com', 'Robert', 'Frost', 'hashmelater', 'hamster');

insert into users (email, firstName, lastName, password, securityAnswer)
values ('janeAusten@gmail.com', 'Jane', 'Austen', 'hashmelater', 'bird');

# enter posture data
insert into posture (userID, cogX, cogY)
values ('1', -1.25, 1.25);

insert into posture (userID, cogX, cogY)
values ('1', 1.278, -1.2909);

insert into posture (userID, cogX, cogY)
values ('1', 0.567, -0.897);

insert into posture (userID, cogX, cogY)
values ('3', 0.001, -0.101)
