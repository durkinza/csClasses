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
			throw new Exception('No Student for id: '.$this->id);
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

	public function clear_student(){
		$this->id = Null;
		$this->fName = NULL;
		$this->lName = NULL;
		$this->gradeLevel = NULL;
		$this->department = NULL;
		$this->clear_taken();
	}

	public function clear_taken(){
		$this->taken = array();
	}

    public function create($firstName, $lastName, $gradeLevel, $department){
		if(isset($this->id)){
			throw new Exception('Student has alread been created');
		}
		$query = "INSERT INTO `Students` (`fName`, `lName`, `gradeLevel`, `department`) VALUES (?, ?, ?, ?)";
		if($stmt = $this->conn->prepare($query)){
			$stmt->bind_param("ssss", $firstName, $lastName, $gradeLevel, $department);
			$stmt->execute();
			$last_id = $stmt->insert_id;
			// update object to use latest data
			$this->build_student($last_id);
		}else{
			echo('Problem Creating Student: '.$this->conn->error);
		}
	}

    public function update($id, $firstName, $lastName, $gradeLevel, $department){
		if(isset($id)){
			$this->id = $id;
		}
		if(!isset($this->id)){
			throw new Exception('Student id not set');
		}
		$query = "UPDATE `Students` SET `fName`=?, `lName`=?, `gradelevel`=?, `department`=? WHERE `id`=? LIMIT 1";
		if($stmt = $this->conn->prepare($query)){
			$stmt->bind_param("ssssi", $firstName, $lastName, $gradeLevel, $department, $this->id);
			$stmt->execute();
			// update object to use latest data
			$this->build_student($this->id);
		}else{
			echo('Problem Updating Student: '.$this->conn->error);
		}
	}

    public function delete($id=NULL){
		if(isset($id)){
			$this->id = $id;
		}
		if(!isset($this->id)){
			throw new Exception('Student id not set');
		}
		$query = "DELETE FROM `Students` WHERE id=? LIMIT 1";
		if($stmt = $this->conn->prepare($query)){
			$stmt->bind_param('i', $this->id);
			$stmt->execute();
			// update object to be empty
			$this->clear_student();
		}else{
			echo('Problem Deleting Student: '.$this->conn->error);
		}
	}

	public function get_taken($id){
		return new Session($this->conn, $this->taken[$id]['sessionId']);
	}

	public function add_taken($sessionId, $grade){
		$query='INSERT INTO `Takes` (`studentId`, `courseId`, `grade`) VALUES (?, ?, ?)';
		if($stmt = $this->conn->prepare($query)){
			$stmt->bind_param('iis', $this->id, $sessionId, $grade);
			$stmt->execute();
			// update object
			$this->build_taken();
		}else{
			echo('Problem Adding Taken: '.$this->conn->error);	
		}
	}
	public function remove_taken($sessionId, $grade){
		$query="DELETE FROM `Takes` WHERE `studentId`=? AND `courseId`=? AND `grade`=? LIMIT 1";
		if($stmt = $this->conn->prepare($query)){
			$stmt->bind_param('iis', $this->id, $sessionId, $grade);
			$stmt->execute();
			// update object
			$this->clear_taken();
			$this->build_taken();
		}else{
			echo('Problem Removing Taken: '.$this->conn->error);	
		}
	}

}	
