-- Create user for web
CREATE USER 'cs360'@'localhost' IDENTIFIED BY '<password_web>';
GRANT SELECT ON CS360.* To 'cs360'@'localhost' WITH GRANT OPTION;
CREATE USER 'cs360'@'%' IDENTIFIED BY '<password_web>';
GRANT SELECT ON CS360.* To 'cs360'@'%' WITH GRANT OPTION;

-- Create cs360 database
CREATE DATABASE IF NOT EXISTS `CS360`;
