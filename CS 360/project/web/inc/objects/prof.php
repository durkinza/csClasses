<?php

class Prof {

	// database connection
	private $conn;

	// Professor attributes
	public $id;
	public $fName;
	public $lName;
	public $department;
	
	public function __construct($db, $id=null){
		$this->conn = $db;
		if(isset($id)){
			$this->build_prof($id);
		}
	}

	public function build_prof($id=null){
		if(isset($id)){
			$this->id = $id;
		}
		if(!isset($this->id)){
			throw new Exception('Professor id not set');
		}
		$query = "SELECT * FROM `Professors` WHERE `id`=?";
		$stmt = $this->conn->prepare($query);
		$stmt->bind_param('i', $this->id);
		$stmt->execute();
		$stmt = $stmt->get_result();
		$num = $stmt->num_rows;
		if($num > 0){
			$row = $stmt->fetch_assoc();
			$this->fName = $row['fName'];
			$this->lName = $row['lName'];
			$this->department = $row['department'];
		}else{
			throw new Exception('Professor not found');
		}
		
	}

}
