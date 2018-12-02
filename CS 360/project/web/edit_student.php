<?php
define('ROOTPATH', './');
require_once('inc/common.php');

if(array_key_exists('Student', $_GET)){
	$id = $_GET['Student'];
	$student = new Student($conn, $id);
}else{
	header('Location: select_student.php?forward=edit_student.php');
}

if($_SERVER['REQUEST_METHOD'] == 'POST'){
	if(isset($_POST['fName'])){
		$fname = $_POST['fName'];
		$lname = $_POST['lName'];
	    $gradeLevel = $_POST['gradeLevel'];
		$department = $_POST['department'];
		$st = new Student($conn);
		echo($department);
	    $st->update($id, $fname, $lname, $gradeLevel, $department);
	    echo("Student updated:");
		echo($st->id);
	}else if(isset($_POST['action'])){
		$sessionId = $_POST['course'];
		$grade=$_POST['grade'];
		if($_POST['action'] == 'Add'){
			$student->add_taken($sessionId, $grade);
		}else{
			$student->remove_taken($sessionId, $grade);
		}
		
	}
}

heading();
?>
<br/>
<h1>Updating Student: <?php echo($student->fName.' '.$student->lName);?></h1>
<div class="row">
	<div class="col">
		<p>Update student data</p>
		<form action="edit_student.php?Student=<?php echo($id);?>" method="POST">
			<label for="fName">First Name</label>
			<input type="text" name="fName" value="<?php echo($student->fName);?>"/><br/>
			<label for="lName">Last Name</label>
			<input type="text" name="lName" value="<?php echo($student->lName);?>"/><br/>
			<label for="gradeLevel">Current GradeLevel</label>
			<select name="gradeLevel" required>
				<option value="<?php echo($student->gradeLevel);?>" selected="true"><?php echo($student->gradeLevel);?></option>
				<option value="Senior Spring">Senior Spring</option>
				<option value="Senior Fall">Senior Fall</option>
				<option value="Junior Spring">Junior Spring</option>
				<option value="Junior Fall">Junior Fall</option>
				<option value="Sophmore Spring">Sophmore Spring</option>
				<option value="Sophmore Fall">Sophmore Fall</option>
				<option value="Freshman Spring">Freshman Spring</option>
				<option value="Freshman Fall">Freshman Fall</option>
				<option value="Predegree">Predegree</option>
			</select><br/>
			<label for="department">department</label>
			<select name="department" required>
				<option value="<?php echo($student->department);?>" selected><?php echo($student->department);?></option>
				<option value="Computer Science">Computer Science</option>
				<option value="English">English</option>
				<option value="Math">Math</option>
				<option value="Science">Science</option>
			</select><br/>
			
			<input type="submit" value="update student"/>
		</form>
	</div>
	<div class="col">
		<p>Current Courses</p>
		<table>
			<thead>
				<tr>
					<th>Course</th>
					<th>Grade</th>
				</tr>
			</thead>
			<tbody>
			<?php 
			foreach($student->taken as $course){
			?>
				<tr>
					<td><?php echo($course['session']->courseId);?></td>
					<td><?php echo($course['grade']);?></td>
				</tr>
			<?php
			}
			?>
			</tbody>
		</table>
	</div>
	<div class="col">
		<p>Update student courses</p>
		<form action="edit_student.php?Student=<?php echo($id);?>" method="POST">
			<label for="action">Action</label>
			<select name="action" required>
				<option value="Add" selected>Add Course</option>
				<option value="Rem">Remove Course</option>
			</select><br/>
			<label for="course">Course</label>
			<select name="course" required>
				<?php
					$courses = $conn->query('SELECT * FROM Offered');
					while($course = $courses->fetch_assoc()){?>
					<option value="<?php echo($course['id']);?>"><?php echo($course['courseId'].'-'.$course['year'].'-'.$course['semester'].'-'.$course['room']);?></option>
					<?php } ?>
			</select><br/>
			<label for="grade">Grade</label>
			<input type="text" maxlength="1" name="grade" placeholder="A" required/><br/>
			<input type="submit" value="apply"/>
		</form>
	</div>
</div>
<?php
scripts();
?>
<?php
footing();
?>
