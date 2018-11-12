<?php
DEFINE("ROOTPATH", '../');
require_once(ROOTPATH.'inc/common.php');
require_once(ROOTPATH.'inc/objects/session.php');
require_once(ROOTPATH.'inc/objects/course.php');
require_once(ROOTPATH.'inc/objects/prof.php');
require_once(ROOTPATH.'inc/objects/student.php');
require_once(ROOTPATH.'inc/objects/career.php');

if(isset($_POST['call'])){

	switch($_POST['call']){

		case 'coursesTaken':
			if(!isset($_POST['studentId'])){
				die(json_encode(array('error'=>'studentId needed')));
			}
			$student = new Student($conn, $_POST['studentId']);
			echo json_encode($student->taken);
			break;

		case 'careerPath':
			if(!isset($_POST['careerId'])){
				die(json_encode(array('error'=>'careerId needed')));
			}
			$career = new Career($conn, $_POST['careerId']);
			echo json_encode($career->required);
			break;
		default:
			
	}
}

?>
