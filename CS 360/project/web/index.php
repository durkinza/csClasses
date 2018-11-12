<?php
define('ROOTPATH', '');
require_once('inc/common.php');

$students = $conn->query('SELECT id,fName,lName FROM Students');

$careers = $conn->query('SELECT id,name FROM Careers');

heading();
?>
<br/>
<h1>Schedule Builder</h1>
<p>Select a student and career path</p>
<div class="row">
	<div class="col">
		<form action="build-schedule.php" method="POST">
		
			<select name="Student" required onChange="getStudent(this);">
				<option value="" selected disabled>No Student</option>
				<?php while ($student = $students->fetch_assoc()) { ?>
				<option value="<?php echo $student["id"];?>">
					<?php echo($student['fName'].' '.$student['lName']);?>
				</option>
				<?php }	?>
			</select>
		
			<select name="Career" required onChange="getCareer(this);">
				<option value="" selected disabled>No Career</option>
				<?php while ($career = $careers->fetch_assoc()) { ?>
				<option value="<?php echo $career["id"];?>">
					<?php echo $career['name'];?>
				</option>
				<?php }	?>
			</select>
		
			<input type="submit" value="Generate Schedule"/>
		</form>
	</div>
</div>
<div class="row">
	<div class="col-6">
		<table id="Student">
			<thead>
				<tr>
					<th>Courses Taken</th>
					<th>Grade</th>
				</tr>
			</thead>
			<tbody id="StudentCourses">
			</tbody>
		</table>
	</div>
	<div class="col-6">
		<table id="Career">
			<thead>
				<tr>
					<th>Courses needed</th>
				</tr>
			</thead>
			<tbody id="CareerCourses">
			</tbody>
		</table>
	</div>
</div>

<?php
scripts();
?>
<script type="text/javascript">
function getStudent(el){
	var val = $(el).val();
	$.ajax({
		url: '<?php echo APIPATH;?>',
		type: 'POST',
		data: {
			call:'coursesTaken',
			studentId: val
		}
	}).done(function(result){
		var res = JSON.parse(result);
		// clear table
		var sc = $('#StudentCourses');
		sc.children().remove();
		// insert new rows
		for(var i=0; i<res.length; i++){
			sc.append('<tr><td>'+res[i].session.courseId+'</td><td>'+res[i].grade+'</td></tr>');
		}
	});
}
function getCareer(el){
	var val = $(el).val();
	$.ajax({
		url: '<?php echo APIPATH;?>',
		type: 'POST',
		data: {
			call:'careerPath',
			careerId: val
		}
	}).done(function(result){
		var res = JSON.parse(result);
		// clear table
		var cc = $('#CareerCourses');
		cc.children().remove();
		// insert new rows
		for(var i=0; i<res.length; i++){
			cc.append('<tr><td>'+res[i].courseId+'</td></tr>');
		}
	});
}
</script>
<?php
footing();
?>
