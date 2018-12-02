<?php
define('ROOTPATH', './');
require_once('inc/common.php');

if($_SERVER['REQUEST_METHOD'] == 'POST'){
	$fname = $_POST['fName'];
	$lname = $_POST['lName'];
    $gradeLevel = $_POST['gradeLevel'];
	$department = $_POST['department'];
	$st = new Student($conn);
    $st->create($fname, $lname, $gradeLevel, $department);
	header('Location: edit_student.php?Student='.$st->id);
    echo("Student created:");
	echo($st->id);
	
}

heading();
?>
<br/>
<h1>Adding Student</h1>
<p>Select a student and career path</p>
<div class="row">
	<div class="col">
		<form action="add_student.php" method="POST">
			<label for="fName">First Name</label>
			<input type="text" name="fName"/><br/>
			<label for="lName">Last Name</label>
			<input type="text" name="lName"/><br/>
			<label for="gradeLevel">Current GradeLevel</label>
			<select name="gradeLevel" required>
				<option value="Senior Spring">Senior Spring</option>
				<option value="Senior Fall">Senior Fall</option>
				<option value="Junior Spring">Junior Spring</option>
				<option value="Junior Fall">Junior Fall</option>
				<option value="Sophmore Spring">Sophmore Spring</option>
				<option value="Sophmore Fall">Sophmore Fall</option>
				<option value="Freshman Spring">Freshman Spring</option>
				<option value="Freshman Fall">Freshman Fall</option>
				<option value="Predegree" selected>Predegree</option>
			</select><br/>
			<label for="department">department</label>
			<select name="department" required>
				<option value="Computer Science" selected>Computer Science</option>
				<option value="English">English</option>
				<option value="Math">Math</option>
				<option value="Science">Science</option>
			</select><br/>
			<input type="submit" value="Add student"/>
		</form>
	</div>
</div>
<?php
scripts();
?>
<?php
footing();
?>
