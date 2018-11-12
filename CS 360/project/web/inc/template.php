<?php
require_once('config.php');

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
	    <ul class="navbar-nav">
	      <li class="nav-item active">
	        <a class="nav-link" href="<?php echo(ROOTPATH);?>/">Home</a>
	      </li>
	      <li class="nav-item">
	        <a class="nav-link" href="<?php echo(ROOTPATH);?>/">Page2</a>
	      </li>
	      <li class="nav-item">
	        <a class="nav-link" href="<?php echo(ROOTPATH);?>/">Page3</a>
	      </li>
	      <li class="nav-item">
	        <a class="nav-link" href="<?php echo(ROOTPATH);?>/">Page4</a>
	      </li>
	    </ul>
	  </div>
	</nav>
	<div class="container">
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
	</body>
</html>
<?php
}
