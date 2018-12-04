<?php
define('ROOTPATH', './');
require_once('inc/common.php');

if($_SERVER['REQUEST_METHOD'] == 'POST'){
	$cr = new Course($conn, $_POST['id']);
	$cr->delete();
    echo("Course removed");
}

$courses = $conn->query('SELECT * FROM Courses');

heading();
?>
<br/>
<h1>Removing Course</h1>
<p>Select a Course to remove</p>
<div class="row">
	<div class="col">
		<form action="del_course.php" method="POST">
			<select name="id" required>
				<?php while ($course = $courses->fetch_assoc()) { ?>
				<option value="<?php echo $course["id"];?>">
					<?php echo($course['id']);?>
				</option>
				<?php }	?>
			</select>
			<input type="submit" value="Delete course"/>
		</form>
	</div>
</div>
<?php
scripts();
?>
<?php
footing();
?>
