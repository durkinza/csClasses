<?php
define('ROOTPATH', './');
require_once('inc/common.php');

if(array_key_exists('Course', $_GET)){
	$id = $_GET['Course'];
	$Course = new Course($conn, $id);
}else{
	header('Location: select_course.php?forward=edit_course.php');
}

if($_SERVER['REQUEST_METHOD'] == 'POST'){
	if(isset($_POST['credits'])){
		$courseId = $_GET['Course'];
		$name = $_POST['name'];
		$credits = $_POST['credits'];
		$cr = new Course($conn, $courseId);
	    $cr->update($courseId, $name, $credits);
	    echo("Course updated:");
		echo($cr->courseId);
		$Course->build_course($cr->courseId);
	}else if(isset($_POST['action'])){
		$courseId = $_POST['course'];
		$optional = $_POST['optional'];
		$grade = $_POST['grade'];
		if($_POST['action'] == 'Add'){
			$Course->add_required($courseId, $grade, $optional);
		}else{
			$Course->remove_required($courseId, $grade, $optional);
		}
		
	}
}

heading();
?>
<br/>
<h1>Updating Course: <?php echo($Course->name);?></h1>
<div class="row">
	<div class="col">
		<p>Update course data</p>
		<form action="edit_course.php?Course=<?php echo($id);?>" method="POST">
			<label for="name">Name</label>
			<input type="text" name="name" value="<?php echo($Course->name);?>"/><br/>
			<label for="credits">credits</label>
			<input type="text" name="credits" value="<?php echo($Course->credits);?>"/><br/>
			<input type="submit" value="update Course"/>
		</form>
	</div>
	<div class="col">
		<p>Current Courses</p>
		<table>
			<thead>
				<tr>
					<th>Course</th>
					<th>Grade</th>
					<th>Required</th>
				</tr>
			</thead>
			<tbody>
			<?php 
			foreach($Course->preReq as $c){
			?>
				<tr>
					<td><?php echo($c['courseId']);?></td>
					<td><?php echo($c['grade']);?></td>
					<td><input type="checkbox" disabled <?php echo($c['optional']==0?'checked':'');?>/></td>
				</tr>
			<?php
			}
			?>
			</tbody>
		</table>
	</div>
	<div class="col">
		<p>Update required Courses</p>
		<form action="edit_course.php?Course=<?php echo($id);?>" method="POST">
			<label for="action">Action</label>
			<select name="action" required>
				<option value="Add" selected>Add Course</option>
				<option value="Rem">Remove Course</option>
			</select><br/>
			<label for="course">Course</label>
			<select name="course" required>
				<?php
					$query = 'SELECT * FROM Courses WHERE `id`!=?';
					$stmt = $conn->prepare($query);
					$stmt->bind_param('s', $_GET['Course']);
					$stmt->execute();
					$courses = $stmt->get_result();
					while($course = $courses->fetch_assoc()){?>
					<option value="<?php echo($course['id']);?>"><?php echo($course['id']);?></option>
					<?php } ?>
			</select><br/>
			<label for="grade">Grade</label>
			<input type="text" maxlength="1" name="grade" placeholder="A" required/><br/>
			<span>Required</span>
			<input type="radio" name="optional" value="0" checked/>
			<span>optional</span>
			<input type="radio" name="optional" value="1"/><br/>
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
