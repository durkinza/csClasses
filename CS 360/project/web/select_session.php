<?php
define('ROOTPATH', '');
require_once('inc/common.php');

$forward = isset($_GET['forward'])?$_GET['forward']:'/';

$sessions = $conn->query('SELECT * FROM Offered');

heading();
?>
<br/>
<h1>Select a session</h1>
<p>Select a session</p>
<div class="row">
	<div class="col">
		<form action="<?php echo($forward); ?>" method="get">
		
			<select name="Session" required onChange="getSession(this);">
				<option value="" selected disabled>No Session</option>
				<?php while ($session = $sessions->fetch_assoc()) { ?>
				<option value="<?php echo $session["id"];?>">
					<?php echo($session['courseId'].' '.$session['year'].' '.$session['semester'].' '.$session['room']);?>
				</option>
				<?php }	?>
			</select>
		
			<input type="submit" value="select session"/>
		</form>
	</div>
</div>
<div class="row">
	<div class="col-6">
		<table id="Session">
			<thead>
				<tr>
					<th>CourseId</th>
					<th>Year</th>
					<th>semester</th>
					<th>room</th>
					<th>time</th>
					<th>Professor</th>
				</tr>
			</thead>
			<tbody id="Sessions">
			</tbody>
		</table>
	</div>
</div>

<?php
scripts();
?>
<script type="text/javascript">
function getSession(el){
	var val = $(el).val();
	$.ajax({
		url: '<?php echo APIPATH;?>',
		type: 'POST',
		data: {
			call:'session',
			courseId: val
		}
	}).done(function(result){
		var res = JSON.parse(result);
		// clear table
		var sc = $('#Sessions');
		sc.children().remove();
		// insert new rows
		sc.append('<tr><td>'+res.courseId+'</td><td>'+res.year+'</td><td>'+res.semester+'</td><td>'+res.room+'</td><td>'+res.time+'</td><td>'+res.profId+'</td></tr>');
	});
}
</script>
<?php
footing();
?>
