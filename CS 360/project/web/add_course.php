<?php
define('ROOTPATH', './');
require_once('inc/common.php');

if($_SERVER['REQUEST_METHOD'] == 'POST'){
	$courseId = $_POST['courseId'];
	$credits = $_POST['credits'];
	$name = $_POST['name'];
	$cr = new Course($conn);
    $cr->create($courseId, $name, $credits);
	header('Location: edit_course.php?Course='.$cr->courseId);
    echo("Course created:");
	echo($cr->courseId);
	
}

heading();
?>
<br/>
<h1>Adding Course</h1>
<p>Creating Course</p>	
<div class="row">
	<div class="col">
		<form action="add_course.php" method="POST">
			<label for="name">Name</label>
			<input type="text" name="name"/><br/>
			<label for="courseId">CourseId</label>
			<input type="text" name="courseId"/><br/>
			<label for="credits">Credits</label>
			<input type="number" name="credits"/><br/>
			<input type="submit" value="Add course"/>
		</form>
	</div>
</div>
<?php
scripts();
?>
<?php
footing();
?>
