<?php
define('ROOTPATH', './');
require_once('inc/common.php');

if($_SERVER['REQUEST_METHOD'] == 'POST'){
	$st = new Session($conn, $_POST['id']);
	$st->delete();
    echo("Session removed");
}

$sessions = $conn->query('SELECT * FROM Offered');

heading();
?>
<br/>
<h1>Removing Session</h1>
<p>Removing session</p>
<div class="row">
	<div class="col">
		<form action="del_session.php" method="POST">
			<select name="id" required>
				<?php while ($session = $sessions->fetch_assoc()) { ?>
				<option value="<?php echo $session["id"];?>">
					<?php echo($session['courseId'].' '.$session['year'].' '.$session['semester'].' '.$session['room']);?>
				</option>
				<?php }	?>
			</select>
			<input type="submit" value="Delete session"/>
		</form>
	</div>
</div>
<?php
scripts();
?>
<?php
footing();
?>
