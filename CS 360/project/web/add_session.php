<?php
define('ROOTPATH', './');
require_once('inc/common.php');

$courses = $conn->query('SELECT * FROM Courses');
$profs = $conn->query('SELECT * FROM Professors');

if($_SERVER['REQUEST_METHOD'] == 'POST'){
	$courseId = $_POST['courseId'];
	$room = $_POST['room'];
	$time = $_POST['time'];
	$semester = $_POST['semester'];
	$year = $_POST['year'];
	$profId = $_POST['profId'];
	$sess = new Session($conn);
    $sess->create($courseId, $room, $time, $semester, $year, $profId);
	header('Location: edit_session.php?Session='.$sess->id);
    echo("Session created:");
	echo($sess->id);
	
}

heading();
?>
<br/>
<h1>Adding Session</h1>
<p>Create new session</p>
<div class="row">
	<div class="col">
		<form action="add_session.php" method="POST">
			<label for="courseId">CourseId</label>
			<select name="courseId">
				<?php 
					while($c = $courses->fetch_assoc()){
					?>
					<option value="<?php echo($c['id']);?>"><?php echo($c['id']);?></option>
					<?php
					}
				?>
			</select><br/>
			<label for="room">Room</label>
			<input type="text" name="room" placeholder="JEB112"/><br/>
			<label for="time">Time</label>
			<input type="text" name="time" placeholder="09:00"/><br/>
			<label for="semester">Semester</label>
			<select name="semester">
				<option value="fall">Fall</option>
				<option value="spring">spring</option>
			</select><br/>
			<label for="year">Year</label>
			<input name="year" type="number" min="2000" max="2099" step="1" value="2018"/><br/>
			<label for="profid">Professor</label>
			<select name="profId">
				<?php
				while($p = $profs->fetch_assoc()){
				?>
				<option value="<?php echo($p['id']);?>"><?php echo($p['lName']); ?></option>
				<?php
				}
				?>
			</select><br/>
			<input type="submit" value="Add sesssion"/>
		</form>
	</div>
</div>
<?php
scripts();
?>
<?php
footing();
?>
