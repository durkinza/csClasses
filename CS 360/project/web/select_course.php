<?php
define('ROOTPATH', '');
require_once('inc/common.php');

$forward = isset($_GET['forward'])?$_GET['forward']:'/';

$courses = $conn->query('SELECT * FROM Courses');

heading();
?>
<br/>
<h1>Select a course</h1>
<p>Select a course</p>
<div class="row">
	<div class="col">
		<form action="<?php echo($forward); ?>" method="get">
		
			<select name="Course" required onChange="getCourse(this);">
				<option value="" selected disabled>No Course</option>
				<?php while ($course = $courses->fetch_assoc()) { ?>
				<option value="<?php echo $course["id"];?>">
					<?php echo($course['id']);?>
				</option>
				<?php }	?>
			</select>
		
			<input type="submit" value="select course"/>
		</form>
	</div>
</div>
<div class="row">
	<div class="col-6">
		<table id="Course">
			<thead>
				<tr>
					<th>Course</th>
					<th>Name</th>
					<th>Credits</th>
				</tr>
			</thead>
			<tbody id="Courses">
			</tbody>
		</table>
	</div>
</div>

<?php
scripts();
?>
<script type="text/javascript">
function getCourse(el){
	var val = $(el).val();
	$.ajax({
		url: '<?php echo APIPATH;?>',
		type: 'POST',
		data: {
			call:'course',
			courseId: val
		}
	}).done(function(result){
		var res = JSON.parse(result);
		// clear table
		var sc = $('#Courses');
		sc.children().remove();
		// insert new rows
		sc.append('<tr><td>'+res.courseId+'</td><td>'+res.name+'</td><td>'+res.credits+'</td></tr>');
	});
}
</script>
<?php
footing();
?>
