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

	public function build_required(){
		if(isset($this->id)){
			throw new Exception('Course id not set');
		}
		foreach($this->preReq as $key => $req){
			$this->required[$key] = new Course($this->conn, $req['courseId']);
		}
	}
}
