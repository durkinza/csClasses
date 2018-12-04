<?php
require_once('config.php');
header("Expires: Tue, 01 Jan 2000 00:00:00 GMT");
header("Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT");
header("Cache-Control: no-store, no-cache, must-revalidate, max-age=0");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

function heading(){
?>
<!DOCTYPE html>
<html>
	<head>
		<title>CS360 - Zane Durkin</title>
		<link rel="stylesheet" href="<?php echo(ROOTPATH);?>/static/css/vendor/bootstrap.min.css"/>
		<link rel="stylesheet" href="<?php echo(ROOTPATH);?>/static/css/main.css"/>
	</head>
	<body>
	<nav class="navbar navbar-expand-lg navbar-light bg-light">
		<a class="navbar-brand" href="<?php echo(ROOTPATH);?>/">CS 360</a>
		<button class="navbar-toggler" type="button" data-toggle="collapse" data-target="#navbarNav" aria-controls="navbarNav" aria-expanded="false" aria-label="Toggle navigation">
			<span class="navbar-toggler-icon"></span>
		</button>
		<div class="collapse navbar-collapse" id="navbarNav">
			<ul class="nav navbar-nav">
				<li class="nav-item active">
					<a class="nav-link" href="/">Home</a>
				</li>
		        <li class="nav-item dropdown">
		          <a class="nav-link dropdown-toggle" href="#" id="navbarDropdown" role="button" data-toggle="dropdown" aria-haspopup="true" aria-expanded="false">
			       Students
				  </a>
		          <div class="dropdown-menu" aria-labelledby="navbarDropdown">
				    <a class="dropdown-item" href="<?php echo(ROOTPATH);?>add_student.php">Add Student</a>
				    <a class="dropdown-item" href="<?php echo(ROOTPATH);?>del_student.php">Remove Student</a>
				    <a class="dropdown-item" href="<?php echo(ROOTPATH);?>edit_student.php">Edit Student</a>
		          </div>
				</li>
		        <li class="nav-item dropdown">
		          <a class="nav-link dropdown-toggle" href="#" id="navbarDropdown" role="button" data-toggle="dropdown" aria-haspopup="true" aria-expanded="false">
			       Courses
				  </a>
		          <div class="dropdown-menu" aria-labelledby="navbarDropdown">
				    <a class="dropdown-item" href="<?php echo(ROOTPATH);?>add_course.php">Add Course</a>
				    <a class="dropdown-item" href="<?php echo(ROOTPATH);?>del_course.php">Remove Course</a>
				    <a class="dropdown-item" href="<?php echo(ROOTPATH);?>edit_course.php">Edit Course</a>
		          </div>
				</li>
		        <li class="nav-item dropdown">
		          <a class="nav-link dropdown-toggle" href="#" id="navbarDropdown" role="button" data-toggle="dropdown" aria-haspopup="true" aria-expanded="false">
			       Sessions
				  </a>
		          <div class="dropdown-menu" aria-labelledby="navbarDropdown">
				    <a class="dropdown-item" href="<?php echo(ROOTPATH);?>add_session.php">Add Session</a>
				    <a class="dropdown-item" href="<?php echo(ROOTPATH);?>del_session.php">Remove Session</a>
				    <a class="dropdown-item" href="<?php echo(ROOTPATH);?>edit_session.php">Edit Session</a>
		          </div>
				</li>
			</ul>
		</div>
	</nav>
	<div class="container" style="min-height:90vh;">
<?php
}

function scripts(){
?>
	</div>
	<script src="<?php echo(ROOTPATH);?>/static/js/vendor/jquery-3.3.1.min.js"></script>
	<script src="<?php echo(ROOTPATH);?>/static/js/vendor/bootstrap.min.js"></script>
	<script src="<?php echo(ROOTPATH);?>/static/js/main.js"></script>
<?php
}

function footing(){
?>
    <footer class=" text-muted">
	  <hr/>
	  <div>
        <p>Developed by Zane Durkin (durk7832)</p>
	  </div>
    </footer>
	</body>
</html>
<?php
}
