<?php

require_once('course.php');

class Career {

	// database connection
	private $conn;

	// career path attributes
	public $id;
	public $name;
	public $department;
	public $required = array();

	public function __construct($db, $id=null){
		$this->conn = $db;
		if(isset($id)){
			$this->build_career($id);
		}
	}

	public function build_career($id=null){
		if(isset($id)){
			$this->id = $id;
		}
		if(!isset($this->id)){
			throw new Exception('Career id not set');
		}
		$query = "SELECT * FROM `Careers` WHERE `id`=?";
		$stmt = $this->conn->prepare($query);
		$stmt->bind_param('i', $this->id);
		$stmt->execute();
		$stmt = $stmt->get_result();
		$num = $stmt->num_rows;
		if($num > 0){
			$row = $stmt->fetch_assoc();
			$this->name = $row['name'];
			$this->department = $row['department'];
			$this->build_required();
		}else{
			throw new Exception('Career not found');
		}
	}
	
	public function build_required(){
		if(!isset($this->id)){
			throw new Exception('Career id not set');
		}
		$query = "SELECT * FROM `CareerPath` WHERE `careerId`=?";
		$stmt = $this->conn->prepare($query);
		$stmt->bind_param('i', $this->id);
		$stmt->execute();
		$stmt = $stmt->get_result();
		$num = $stmt->num_rows;
		if($num > 0){
			$i = 0;
			while($row = $stmt->fetch_assoc()){
				$this->required[$i] = new Course($this->conn, $row['reqId']);
				$i++;
			}
		}
	}

}
