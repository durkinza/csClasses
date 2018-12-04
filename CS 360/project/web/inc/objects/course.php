<?php

class Course{
 
    // database connection
    protected $conn;
 
    // object properties
    public $courseId;
    public $credits;
    public $name;
	public $num_preReqs;
	public $preReq = array();
	public $required = array();
 
    // constructor with $db as database connection
    public function __construct($db, $courseId=null){
        $this->conn = $db;
		if(isset($courseId)){
			$this->build_course($courseId);
		}
    }

	public function add_required($reqId, $grade, $optional ){
		if(!isset($this->courseId)){
			throw new Exception('Course id not set');
		}
		$query = "INSERT INTO `PreReq` (courseId, reqId, grade, optional) VALUES (?, ?, ?, ?)";
		if($stmt = $this->conn->prepare($query)){
			$stmt->bind_param('sssi', $this->courseId, $reqId, $grade, $optional);
			$stmt->execute();
			// update object
			$this->build_course();
		}else{
			echo('Problem creating Prereq: '.$this->conn->error);
		}
	}
	public function remove_required($reqId, $grade, $optional){
		if(!isset($this->courseId)){
			throw new Exception('Course id not set');
		}
		$query = "DELETE FROM `PreReq` WHERE `courseId`=? AND `reqId`=? AND `grade`=? AND `optional`=? LIMIT 1";
		if($stmt = $this->conn->prepare($query)){
			$stmt->bind_param('sssi', $this->courseId, $reqId, $grade, $optional);
			$stmt->execute();
			// update object
			$this->clear_preReq();
			$this->build_preReqs();
		}else{
			echo('Problem Removing PreReq: '.$this->conn->error);
		}
	}

	public function build_course($courseId=null){

		if(isset($courseId)){
			$this->courseId = $courseId;
		}
		if(!isset($this->courseId)){
			throw new Exception('Course id not set');
		}
		$query = "SELECT * FROM `Courses` WHERE `id`=?";
		$stmt = $this->conn->prepare($query);
		$stmt->bind_param('s', $this->courseId);
		$stmt->execute();
		$stmt = $stmt->get_result();
		$num = $stmt->num_rows;

		if($num > 0){
			$row = $stmt->fetch_assoc();
			$this->credits = $row['credits'];
			$this->name = $row['name'];
			$this->num_preReqs = $this->build_preReqs();
		}else{
			throw new Exception('Course not found');
		}
	}

	public function get_preReq($courseId){
		return new Course($courseId);
	}

	private function build_preReqs(){
		if(!isset($this->courseId)){
			throw new Exception('Course id not set');
		}
		$query = "SELECT * FROM `PreReq` Where `courseId`=?";
		$stmt = $this->conn->prepare($query);
		$stmt->bind_param('s', $this->courseId);
		$stmt->execute();
		$stmt = $stmt->get_result();
		$num = $stmt->num_rows;

		if($num > 0){
			$i = 0;
			// get retrieved row
			while( $row = $stmt->fetch_assoc()){
			    // set values to object properties
			    $this->preReq[$i]['courseId']	= $row['reqId'];
			    $this->preReq[$i]['grade']		= $row['grade'];
			    $this->preReq[$i]['optional']	= $row['optional'];
				// this will build the course tree recursively
				//$this->preReq[$i]['course']		= new course($this->conn, $row['reqId']);
				$i++;
			}
			// return the number of preReqs
			return $num;
		}
		// return zero preReqs
		return 0;
	}

	public function clear_course(){
		$this->courseId = NULL;
		$this->credits = NULL;
		$this->name = NULL;
		$this->clear_preReq();
	}

	public function clear_preReq(){
		$this->num_preReqs = NULL;
		$this->preReq = array();
		$this->required = array();
	}

	public function create($courseId, $name, $credits){
		if(isset($this->courseId)){
			throw new Exception('Course has already been created');
		}
		$query = "INSERT INTO `Courses` (`id`, `credits`, `name`) VALUES (?, ?, ?)";
		if($stmt = $this->conn->prepare($query)){
			$stmt->bind_param("sis", $courseId, $credits, $name);
			$stmt->execute();
			$last_id = $stmt->insert_id;
			// update object to use latest data
			$this->build_course($courseId);
		}else{
			echo('Problem Creating Course: '.$this->conn->error);
		}
	}

	public function update($courseId, $name, $credits){
		if(isset($courseId)){
			$this->courseId = $courseId;
		}
		if(!isset($this->courseId)){
			throw new Exception('Course id not set');
		}
		$query = "UPDATE `Courses` SET `id`=?, `credits`=?, `name`=? WHERE `id`=? LIMIT 1";
		if($stmt = $this->conn->prepare($query)){
			$stmt->bind_param("siss", $courseId, $credits, $name, $this->courseId);
			$stmt->execute();
			// update object to use latest data
			$this->build_course($this->courseId);
		}else{
			echo('Problem Updating Course: '.$this->conn->error);
		}
	}

	public function delete($id=NULL){
		if(isset($id)){
			$this->courseId = $id;
		}
		if(!isset($this->courseId)){
			throw new Exception('Course id not set');
		}
		$query = "DELETE FROM `Courses` WHERE `id`=? LIMIT 1";
		if($stmt = $this->conn->prepare($query)){
			$stmt->bind_param('s', $this->courseId);
			$stmt->execute();
			// update object to be empty
			$this->clear_course();
		}else{
			echo('Problem Deleting Course: '.$this->conn->error);
		}
	}

	public function build_required(){
		if(!isset($this->courseId)){
			throw new Exception('Course id not set');
		}
		foreach($this->preReq as $key => $req){
			$this->required[$key] = new Course($this->conn, $req['courseId']);
		}
	}
}
