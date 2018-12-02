<?php
define('ROOTPATH', '');
require_once('inc/common.php');

$forward = isset($_GET['forward'])?$_GET['forward']:'/';

$students = $conn->query('SELECT id,fName,lName FROM Students');

heading();
?>
<br/>
<h1>Select a student</h1>
<p>Select a student</p>
<div class="row">
	<div class="col">
		<form action="<?php echo($forward); ?>" method="get">
		
			<select name="Student" required onChange="getStudent(this);">
				<option value="" selected disabled>No Student</option>
				<?php while ($student = $students->fetch_assoc()) { ?>
				<option value="<?php echo $student["id"];?>">
					<?php echo($student['fName'].' '.$student['lName']);?>
				</option>
				<?php }	?>
			</select>
		
			<input type="submit" value="select student"/>
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
</script>
<?php
footing();
?>
