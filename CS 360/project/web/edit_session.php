<?php
define('ROOTPATH', './');
require_once('inc/common.php');

if(array_key_exists('Session', $_GET)){
	$id = $_GET['Session'];
	$Session = new Session($conn, $id);
}else{
	header('Location: select_session.php?forward=edit_session.php');
}

$courses = $conn->query('SELECT * FROM Courses');
$profs = $conn->query('SELECT * FROM Professors');

if($_SERVER['REQUEST_METHOD'] == 'POST'){
	$courseId = $_POST['courseId'];
	$room = $_POST['room'];
	$time = $_POST['time'];
	$semester = $_POST['semester'];
	$year = $_POST['year'];
	$profId = $_POST['profId'];
	$sess = new Session($conn, $id);
    $sess->update($id, $courseId, $room, $time, $semester, $year, $profId);
	header('Location: edit_session.php?Session='.$sess->id);
    echo("Session updated:");
	echo($sess->id);
	
}
heading();
?>
<br/>
<h1>Updating Session: <?php echo($Session->CourseId.' '.$Session->year.' '.$Session->semester.' '.$Session->room);?></h1>
<div class="row">
	<div class="col">
		<p>Update session data</p>
		<form action="edit_session.php?Session=<?php echo($id); ?>" method="POST">
			<label for="courseId">CourseId</label>
			<select name="courseId">
				<?php 
					while($c = $courses->fetch_assoc()){
						if($c['id'] == $Session->courseId){
							?>
							<option value="<?php echo($c['id']);?>" selected><?php echo($c['id']);?></option>
							<?php
						}else{
							?>
							<option value="<?php echo($c['id']);?>"><?php echo($c['id']);?></option>
							<?php
						}
					}
				?>
			</select><br/>
			<label for="room">Room</label>
			<input type="text" name="room" value="<?php echo($Session->room);?>"/><br/>
			<label for="time">Time</label>
			<input type="text" name="time" value="<?php echo($Session->time);?>"/><br/>
			<label for="semester">Semester</label>
			<select name="semester">
				<option value="<?php echo($Session->semester);?>" selected><?php echo($Session->semester);?><option>
				<option value="fall">Fall</option>
				<option value="spring">spring</option>
			</select><br/>
			<label for="year">Year</label>
			<input name="year" type="number" min="2000" max="2099" step="1" value="<?php echo($Session->year);?>"/><br/>
			<label for="profid">Professor</label>
			<select name="profId">
				<?php
				while($p = $profs->fetch_assoc()){
					if($p['id'] == $Session->profId){
						?>
						<option value="<?php echo($p['id']);?>" selected><?php echo($p['lName']); ?></option>
						<?php
					}else{
						?>
						<option value="<?php echo($p['id']);?>"><?php echo($p['lName']); ?></option>
						<?php
					}
				}
				?>
			</select><br/>
			<input type="submit" value="Edit sesssion"/>
		</form>
	</div>
</div>
<?php
scripts();
?>
<?php
footing();
?>
