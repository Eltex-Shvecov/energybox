<?php
	$conn = mysqli_connect("192.168.20.21", "energybox_user", "Ghjcnjnfr564nfr");
	if (!$conn)
	{
		echo mysqli_error($conn);
	}

	mysqli_set_charset($conn, "utf8");
	$sql = "SELECT * FROM energybox_db.Users";

	if($result = mysqli_query($conn, $sql))
	{
		foreach ($result as $row) {
			$name = $row["Name"];
			$point = $row["Point"];
			echo $name." - ".$point."<br>";
		}
	}
	else
	{
		echo mysqli_error($conn);
	}

	mysqli_close($conn);
?>