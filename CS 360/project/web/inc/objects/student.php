<?php
require_once('session.php');
class Student {

	// database connection
	private $conn;

	// student attributes
	public $id;
	public $fName;
	public $lName;
	public $gradeLevel;
	public $department;
	public $taken = array();

	public function __construct($db, $id=null){
		$this->conn = $db;
		if(isset($id)){
			$this->build_student($id);
		}
	}
	
	public function build_student($id=null){
		if(isset($id)){
			$this->id = $id;
		}
		if(!isset($this->id)){
			throw new Exception('Student id not set');
		}
		$query = "SELECT * FROM `Students` WHERE `id`=?";
		$stmt = $this->conn->prepare($query);
		$stmt->bind_param('i', $this->id);
		$stmt->execute();
		$stmt = $stmt->get_result();
		$num = $stmt->num_rows;
		if($num > 0 ){
			$row = $stmt->fetch_assoc();
			$this->fName = $row['fName'];
			$this->lName = $row['lName'];
			$this->gradeLevel = $row['gradeLevel'];
			$this->department = $row['department'];
			$this->build_taken();
		}else{
			throw new Exception('Student not found');
		}
	}

	public function build_taken(){
		if(!isset($this->id)){
			throw new Exception('Student id not set');
		}
		$query = "SELECT * FROM `Takes` WHERE `studentId`=?";
		$stmt = $this->conn->prepare($query);
		$stmt->bind_param('i', $this->id);
		$stmt->execute();
		$stmt = $stmt->get_result();
		$num = $stmt->num_rows;
		if($num > 0){
			$i = 0;
			while($row = $stmt->fetch_assoc()){
				$this->taken[$i]['id'] = $row['id'];
				$this->taken[$i]['sessionId'] = $row['courseId'];
				$this->taken[$i]['grade'] = $row['grade'];
				$this->taken[$i]['session'] = new Session($this->conn, $row['courseId']);
				$i++;
			}
		}
	}

	public function get_taken($id){
		return new Session($this->conn, $this->taken[$id]['sessionId']);
	}
}
