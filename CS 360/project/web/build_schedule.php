<?php
define('ROOTPATH', '');
require_once('inc/common.php');

define('USINGSQL', false);

if($_SERVER['REQUEST_METHOD'] == 'POST'){
	$student = New Student($conn, $_POST['Student']);
	$career = New Career($conn, $_POST['Career']);
}else{
	header('Location:/');
}

heading();
?>
<br/>
<h1>Schedule Builder</h1>
<p>Schedule for <?php echo($student->fName.' '.$student->lName);?> with career <?php echo($career->name);?></p>
<div class="row">
		<?php
		if(USINGSQL){
			$query = "";
		}else{
			$courses = $career->required;
			$nLen = count($courses);
			$arr = $courses;
			$arr2 = array();
			$listed = array();
			foreach($student->taken as $c){
				array_push($listed, $c['session']->courseId);
			}
			while(count($arr) > 0){
				?>
				<div class="col">
					<ul>
				<?php
				foreach($arr as $course){
					echo('<li>'.$course->courseId.'</li>');
					$course->build_required();
					foreach($course->required as $c){
						if(!in_array($c->courseId, $listed)){
							array_push($arr2, $c);
							array_push($listed, $c->courseId);
						}
					}
				}
				$arr = array_unique($arr2, SORT_REGULAR);
				$arr2 = array();
				?>	
					</ul>
				</div>
				<?php
			}
		}	
		?>
</div>
<?php
scripts();
?>	
<?php
footing();
?>
