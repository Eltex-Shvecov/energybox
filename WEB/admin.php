<?php
	$conn = mysqli_connect("192.168.0.177", "root", "");
	if (!$conn)
	{
		mysqli_error($conn);
	}

	$sql = "SELECT * FROM energybox.Users";

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
		mysqli_error($conn);
	}

	mysqli_close($conn);
?>