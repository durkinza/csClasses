<?php
require_once('course.php');
require_once('prof.php');
class Session extends Course{


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
			$this->build_course();
		}else{
			throw new Exception('Session not found');
		}
	}
	public function get_prof(){
		return new Prof($this->conn, $this->profId);
	}
}
