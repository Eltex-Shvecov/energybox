<?php
	$_CardNumber = $_GET["card"];
	$_Point = $_GET["point"];

	$conn = mysqli_connect("192.168.20.21", "energybox_user", "Ghjcnjnfr564nfr");
	if(!$conn)
	{
		die("Connection failed: " . mysqli_connect_error());
	}
	mysqli_set_charset($conn, "utf8");
	mysqli_select_db($conn, "energybox_db");

	if($_Point)
	{
		// здесь зачисляем очки пользователю
		$getPoint = "SELECT * FROM Users WHERE Number = ".$_CardNumber;
		if($request = mysqli_query($conn, $getPoint))
		{
			foreach ($request as $row)
			{
				$Name = $row["Name"];
				$Surname = $row["Surname"];
				$curPoint = $row["Point"];
			}
			mysqli_free_result($request);
		}
		else
		{
			echo mysqli_error($conn);
		}

		$updatePoint = "UPDATE Users SET Point = ".($curPoint + $_Point)." WHERE Number = ".$_CardNumber;
		if(!mysqli_query($conn, $updatePoint))
		{
			echo mysqli_error($conn);
		}

		echo "Points accrued: ".$_Point;
	}
	else
	{
		// здесь возвращаем количество очков у переданной карты и юзера
		$sql = "SELECT * FROM Users WHERE Number = ".$_CardNumber;

		if($request = mysqli_query($conn, $sql))
		{
			foreach ($request as $row)
			{
				$Name = $row["Name"];
				$Surname = $row["Surname"];
				$Point = $row["Point"];
			}
			echo $Name." ".$Surname."\nYour points: ".$Point;
			mysqli_free_result($request);
		}
		else
		{
			echo mysqli_error($conn);
		}

		mysqli_close($conn);
	}
?>