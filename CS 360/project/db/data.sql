-- Author: Zane Durkin <durk7832@vandals.uidaho.edu>
-- Class: CS 360, Database systems

USE `CS360`;

DELETE FROM `CareerPath`;
DELETE FROM `Careers`;
DELETE FROM `Takes`;
DELETE FROM `PreReq`;
DELETE FROM `Students`;
DELETE FROM `Offered`;
DELETE FROM `Professors`;
DELETE FROM `Courses`;


INSERT INTO `Courses` (`id`, `credits`, `name`) VALUES
("CS112", 3, "Computational Thinking and Problem Solving"),
("CS120", 4, "Computer Science I"),
("CS121", 3, "Computer Science II"),
("CS150", 3, "Computer Organization and Architecture"),
("CS270", 3, "System Software"),
("CS210", 3, "Programming Languages"),
("CS240", 3, "Computer Operating Systems"),
("CS360", 3, "Database Design"),
("CS385", 3, "Theory of Computation"),
("CS395", 3, "Analysis of Algorithms"),
("CS383", 3, "Software Engineering"),
("CS445", 4, "Compiler Design"),
("CS401", 3, "Computer Science 401"),
("CS480", 3, "CS Senior Capstone Design I"),
("CS481", 3, "CS Senior Capstone Design II"),
("M143", 3, "Pre-calculus Algebra and Analytic Geometry"),
("M176", 3, "Discrete Mathematics"),
("M144", 3, "Analytic Trigonometry"),
("M170", 3, "Analytic Geometry and Calculus I"),
("M175", 3, "Analytic Geometry and Calculus II"),
("M330", 3, "Linear Algebra"),
("STAT301", 3, "Probability and Statistics"),
("ENG317", 3, "Technical Writing"),
("ENG101", 3, "Introduction to College Writing"),
("ENG102", 3, "College Writing and Rhetoric"),
("COM101", 2, "Communications"),
("SCIENCE", 3, "Science Elective"),
("SCILAB1", 1, "Science Lab 1"),
("SCILAB2", 1, "Science Lab 2"),
("ISEM", 3, "Integrated Seminar")
;

INSERT INTO	`Professors` (`id`, `fName`, `lName`, `department`) VALUES 
(1, "Hasan", "Jamil", "Computer Science"),
(2, "Sheldon", "Fredrick", "Computer Science"),
(3, "Axel", "Krings", "Computer Science"),
(4, "Bruce", "Bolden", "Computer Science"),
(5, "James", "Alves-Foss", "Computer Science"),
(6, "Terence", "Soule", "Computer Science"),
(7, "Sharon", "Stole", "Integrated Seminar"),
(8, "Barbara", "Kirchmeier", "English"),
(9, "Shannon", "Dryden", "English"),
(10, "John", "Hill", "English"),
(11, "Ann", "Abbott", "Math"),
(12, "Manuel", "Welhan", "Math"),
(13, "Alexander","Woo", "Math"),
(14, "Lyudmyla", "Barannyk", "Math"),
(15, "Jacob", "Turner", "Science"),
(16, "Francesca", "Sammarruca", "Science")
;

INSERT INTO `Offered` (`id`, `courseId`, `room`, `time`, `semester`, `year`, `profId`) VALUES
(1, "CS112", "JEB212", "09:30", "Fall", "2019", 4),
(2, "CS112", "JEB212", "10:30", "Fall", "2019", 4),
(3, "CS120", "JEB205", "09:30", "Fall", "2019", 2),
(4, "CS121", "JEB323", "13:00", "Fall", "2018", 5),
(5, "CS121", "JEB323", "13:30", "Fall", "2019", 1),
(6, "CS150", "ADMIN312", "16:00", "Fall", "2019", 3),
(7, "CS150", "ADMIN313", "16:00", "Spring", "2019", 6),
(8, "CS270", "Physics 231", "08:00", "Fall", "2019", 3),
(9, "CS383", "Physics 233", "06:00", "Fall", "2019", 4)
;

INSERT INTO `PreReq` (	courseId, reqId, grade, optional) VALUES
("CS481", "CS480", "C", FALSE),
("CS480", "CS383", "C", FALSE),

("CS445", "CS385", "C", FALSE),

("CS385", "CS240", "C", FALSE),
("CS385", "CS270", "C", FALSE),
("CS385", "CS210", "C", FALSE),
("CS385", "M170", "C", FALSE),
("CS385", "M175", "C", FALSE),

("CS383", "CS240", "C", FALSE),
("CS383", "CS270", "C", FALSE),
("CS383", "CS210", "C", FALSE),
("CS383", "M170", "C", FALSE),
("CS383", "M175", "C", FALSE),

("CS401", "CS240", "C", FALSE),
("CS401", "CS270", "C", FALSE),
("CS401", "CS210", "C", FALSE),
("CS401", "M170", "C", FALSE),
("CS401", "M175", "C", FALSE),

("CS395", "CS240", "C", FALSE),
("CS395", "CS270", "C", FALSE),
("CS395", "CS210", "C", FALSE),
("CS395", "M170", "C", FALSE),
("CS395", "M175", "C", FALSE),

("CS480", "ENG317", "C", FALSE),
("ENG317", "ENG102", "C", FALSE),
("ENG102", "ENG101", "C", FALSE),

("CS240", "CS270", "C", TRUE),

("CS270", "CS150", "C", FALSE),
("CS270", "CS121", "C", FALSE),
("CS270", "M176", "C", FALSE),
("CS270", "CS120", "C", FALSE),

("CS210", "CS150", "C", FALSE),
("CS210", "CS121", "C", FALSE),
("CS210", "M176", "C", FALSE),
("CS210", "CS120", "C", FALSE),

("M175", "M170", "C", FALSE),
("M170", "M144", "C", FALSE),
("M170", "M143", "C", FALSE),

("COM101", "ENG102", "C", FALSE),
("CS121", "CS120", "C", FALSE),
("CS121", "M176", "C", TRUE),
("M176", "M143", "B", FALSE),
("CS120", "CS112", "B", FALSE),
("CS120", "M143", "B", FALSE),
("CS150", "CS120", "C", FALSE)
;

INSERT INTO `Students` (`id`, `fName`, `lName`, `gradeLevel`, `department`) VALUES
(1, "Johnny", "Smith", "Senior Fall", "Computer Science"),
(2, "Sarah", "Lennet", "Junior Spring", "Computer Science"),
(3, "Tim", "Qually", "Freshman Fall", "Computer Science"),
(4, "Jess", "Porter", "Sophomore Spring", "Computer Science"),
(5, "Lenny", "Carter", "Predegree", "Computer Science"),
(6, "Otto", "Culer", "Predegree", "Computer Science"),
(7, "Eddie", "Tash", "Freshman Spring", "Computer Science")
; 

INSERT INTO `Takes` (`id`, `studentId`, `courseId`, `grade`) VALUES
(1, 1, 1, 'A'),
(2, 1, 2, 'C'),
(3, 1, 3, 'A'),
(4, 1, 4, 'B'),
(5, 1, 5, 'A'),
(6, 1, 6, 'C'),
(7, 1, 7, 'B'),
(8, 1, 8, 'A'),
(9, 2, 1, 'B'),
(10, 2, 2, 'A'),
(11, 2, 3, 'A'),
(12, 2, 5, 'B'),
(13, 2, 6, 'B'),
(14, 3, 1, 'B'),
(15, 3, 2, 'A'),
(16, 4, 1, 'C'),
(17, 4, 2, 'A'),
(18, 4, 3, 'B'),
(19, 4, 4, 'A'),
(20, 7, 1, 'B'),
(21, 7, 2, 'A'),
(22, 7, 3, 'C'),
(23, 7, 4, 'A')
;

INSERT INTO `Careers` (`id`, `name`, `department`) VALUES
(1, "Database Designer", "Computer Science"),
(2, "Network Security Engineer", "Computer Science")
;

INSERT INTO `CareerPath` (`careerId`, `reqId`) VALUES
(1, "CS385"),
(1, "CS360"),
(2, "CS383")
;
