<?php
require_once('course.php');
require_once('prof.php');
class Session{

	// database connection
	protected $conn;

	// session info
	public $id;
	public $courseId;
	public $room;
	public $time;
	public $semester;
	public $year;
	public $profId;

	public function __construct($db, $id=null){
		$this->conn = $db;
		if(isset($id)){
			$this->build_session($id);
		}
	}

	public function build_session($id=null){
		if(isset($id)){
			$this->id = $id;
		}
		if(!isset($this->id)){
			throw new Exception('Session id not set');
		}
		$query = "SELECT * FROM `Offered` WHERE `id`=?";
		$stmt = $this->conn->prepare($query);
		$stmt->bind_param('i', $this->id);
		$stmt->execute();
		$stmt = $stmt->get_result();
		$num = $stmt->num_rows;
		if($num > 0){
			$row = $stmt->fetch_assoc();
			$this->courseId = $row['courseId'];
			$this->room = $row['room'];
			$this->time = $row['time'];
			$this->semester = $row['semester'];
			$this->year = $row['year'];
			$this->profId = $row['profId'];
			//$this->prof = new Prof($this->conn, $row['profId']);
//			$this->build_session();
		}else{
			throw new Exception('Session not found');
		}
	}
	public function get_prof(){
		return new Prof($this->conn, $this->profId);
	}

	public function create($courseId, $room, $time, $sems, $year, $profId){
		$query = "INSERT INTO `Offered` (`courseId`, `room`, `time`, `semester`, `year`, `profId`) VALUES (?, ?, ?, ?, ?, ?)";
		if($stmt = $this->conn->prepare($query)){
			$stmt->bind_param('sssssi', $courseId, $room, $time, $sems, $year, $profId);
			$stmt->execute();
			$last_id = $stmt->insert_id;
			// update object to use latest data
			$this->build_session($last_id);
		}else{
			echo('Problem Creating Session: '.$this->conn->error);
		}
	}
	public function update($id, $courseId, $room, $time, $sems, $year, $profId){
		if(isset($id)){
			$this->id = $id;
		}
		if(!isset($this->id)){
			throw new Exception('Session id not set');
		}
		$query = "UPDATE`Offered` SET `courseId`=?, `room`=?, `time`=?, `semester`=?, `year`=?, `profId`=? WHERE `id`=? LIMIT 1";
		if($stmt = $this->conn->prepare($query)){
			$stmt->bind_param('sssssii', $courseId, $room, $time, $sems, $year, $profId, $this->id);
			$stmt->execute();
			// update object to use latest data
			$this->build_session($this->id);
		}else{
			echo('Problem Creating Session: '.$this->conn->error);
		}
	}
	public function delete($id=NULL){
		if(isset($id)){
			$this->id = $id;
		}
		if(!isset($this->id)){
			throw new Exception('Session id not set');
		}
		$query = 'DELETE FROM `Offered` WHERE `id`=? LIMIT 1';
		if($stmt = $this->conn->prepare($query)){
			$stmt->bind_param('i', $this->id);
			$stmt->execute();
			// update object to be empty
			$this->clear_session();
		}else{
			echo('Problem Deleteing Session: '.$this->conn->error);
		}
	}
	public function clear_session(){
		$this->id = NULL;
		$this->courseId = NULL;
		$this->room = NULL;
		$this->time = NULL;
		$this->semester = NULL;
		$this->year = NULL;
		$this->profId = NULL;
		
	}
}
