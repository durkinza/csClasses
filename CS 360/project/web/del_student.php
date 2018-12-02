<?php
define('ROOTPATH', './');
require_once('inc/common.php');

if($_SERVER['REQUEST_METHOD'] == 'POST'){
	$st = new Student($conn, $_POST['id']);
	$st->delete();
    echo("Student removed");
}

$students = $conn->query('SELECT id,fName,lName FROM Students');

heading();
?>
<br/>
<h1>Removing Student</h1>
<p>Select a student and career path</p>
<div class="row">
	<div class="col">
		<form action="del_student.php" method="POST">
			<select name="id" required>
				<?php while ($student = $students->fetch_assoc()) { ?>
				<option value="<?php echo $student["id"];?>">
					<?php echo($student['fName'].' '.$student['lName']);?>
				</option>
				<?php }	?>
			</select>
			<input type="submit" value="Delete student"/>
		</form>
	</div>
</div>
<?php
scripts();
?>
<?php
footing();
?>
