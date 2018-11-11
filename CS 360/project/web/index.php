<?php
define('ROOTPATH', '.');
define('BACKENDPATH', './backend/main.php');
require_once('inc/common.php');

$students = $conn->query('SELECT id,fName,lName FROM Students');

$careers = $conn->query('SELECT id,name FROM Careers');

heading();
?>
<br/>
<h1>Schedule Builder</h1>
<p>Select a student and career path</p>
<form action="<?php echo(BACKENDPATH);?>" method="POST">

	<select name="Student" required>
		<option value="" selected disabled>No Student</option>
		<?php while ($student = $students->fetch_assoc()) { ?>
		<option value="<?php echo $student["id"];?>">
			<?php echo($student['fName'].' '.$student['lName']);?>
		</option>
		<?php }	?>
	</select>

	<select name="Career" required>
		<option value="" selected disabled>No Career</option>
		<?php while ($career = $careers->fetch_assoc()) { ?>
		<option value="<?php echo $career["id"];?>">
			<?php echo $career['name'];?>
		</option>
		<?php }	?>
	</select>

	<input type="submit" value="Generate Schedule"/>
</form>

<?php
footing();
?>
