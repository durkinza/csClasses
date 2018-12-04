-- Author: Zane Durkin <durk7832@vandals.uidaho.edu>
-- Class: CS 360, Database systems

USE `CS360`;

DROP TABLE IF EXISTS `CareerPath`;
DROP TABLE IF EXISTS `Careers`;
DROP TABLE IF EXISTS `Takes`;
DROP TABLE IF EXISTS `PreReq`;
DROP TABLE IF EXISTS `Students`;
DROP TABLE IF EXISTS `Offered`;
DROP TABLE IF EXISTS `Professors`;
DROP TABLE IF EXISTS `Courses`;

--
-- Courses Table
-- Holds all courses that are available at university
--
CREATE TABLE `Courses` (
	`id` varchar(8) NOT NULL,
	`credits` int(4) unsigned NOT NULL,
	`name` varchar(100) NOT NULL,
	PRIMARY KEY (`id`)
);

--
-- Professors Table
-- Professors and their departments
--
CREATE TABLE `Professors`(
	`id` int(10) unsigned NOT NULL AUTO_INCREMENT,
	`fName` varchar(20) NOT NULL,
	`lName` varchar(20) NOT NULL,
	`department` varchar(20),
	PRIMARY KEY (`id`)
);
--
-- Offered Table
-- Will hold all courses the university offers, has offered, or plans to offer
-- courseId is seperate from primary key, because two of the same courses can be offered
--
CREATE TABLE `Offered` (
	`id` int(10) unsigned NOT NULL AUTO_INCREMENT,
	`courseId` varchar(8) NOT NULL,
	`room`	varchar(100) NOT NULL,
	`time` varchar(100) NOT NULL,
	`semester` varchar(7) NOT NULL,
	`year`	varchar(4) NOT NULL check(year > 1900),
	`profId` int(10) unsigned DEFAULT NULL,
	PRIMARY KEY (`id`),
	KEY `courseId` (`courseId`),
	KEY `profId` (`profId`),
	CONSTRAINT `offered_ibfk_1` FOREIGN KEY (`courseId`) REFERENCES `Courses` (`id`),
	CONSTRAINT `offered_ibfk_2` FOREIGN KEY (`profId`) REFERENCES `Professors` (`id`)
);

--
-- PreReq Table
-- holds all pre-required courses
--
CREATE TABLE `PreReq` (
	`courseId` varchar(8) NOT NULL,
	`reqId` varchar(8) NOT NULL,
	`grade` varchar(3) DEFAULT NULL,
	`optional` tinyint(1) DEFAULT FALSE,
	KEY `courseId` (`courseId`),	
	KEY `reqId` (`reqId`),	
	CONSTRAINT `prerec_ibfk_1` FOREIGN KEY (`courseId`) REFERENCES `Courses` (`id`),
	CONSTRAINT `prerec_ibfk_2` FOREIGN KEY (`reqId`) REFERENCES `Courses` (`id`)
);



--
-- Students Table
-- The students enrolled at the university
--
CREATE TABLE `Students` (
	`id` int(10) unsigned NOT NULL AUTO_INCREMENT,
	`fName` varchar(20) NOT NULL,
	`lName` varchar(20) NOT NULL,
	`gradeLevel` varchar(20),
	`department` varchar(20),
	PRIMARY KEY (`id`) 
);


--
-- Takes Table
-- holds the intersection between students and offered.
--
CREATE TABLE `Takes` (
	`id` int(10) unsigned NOT NULL AUTO_INCREMENT,
	`studentId` int(10) unsigned NOT NULL,
	`courseId` int(10) unsigned NOT NULL,
	`grade`	VARCHAR(3),
	PRIMARY KEY (`id`),
	KEY `courseId` (`courseId`),
	CONSTRAINT `takes_ibfk_1` FOREIGN KEY (`courseId`) REFERENCES `Offered` (`id`)
);



-- ------------------------------------------------------------------------------------ 


--
-- Careers Table
-- List of possible careers to reach for
-- 
CREATE TABLE `Careers` (
	`id` int(10) unsigned NOT NULL AUTO_INCREMENT,
	`name` VARCHAR(255) NOT NULL,
	`department` VARCHAR(20),
	PRIMARY KEY (`id`)
);

--
-- CareerPaths Table
-- List of courses required for given career
-- 
CREATE TABLE `CareerPath` (
	`careerId` int(10) unsigned NOT NULL,
	`reqId` varchar(8) NOT NULL,
	KEY `reqId` (`reqId`),	
	KEY `careerId` (`careerId`),	
	CONSTRAINT `careerpath_ibfk_1` FOREIGN KEY (`reqId`) REFERENCES `Courses` (`id`),
	CONSTRAINT `careerpath_ibfk_2` FOREIGN KEY (`careerId`) REFERENCES `Careers` (`id`)
);
