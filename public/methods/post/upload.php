<?php

error_reporting(E_ALL);

$target_dir = "public/methods/post/";
$target_file = $_FILES["file"]["tmp_name"];
$uploadOk = 1;

var_dump($_FILES);
echo "<br><br>";


if ($_FILES["file"]["size"] > 5000000)
{
    echo "Sorry, your file is too large.";
    fwrite(STDERR, "file too large");
    $uploadOk = 0;
}

if (!is_dir($to) || !is_writable($to))
{
    fwrite(STDERR, "file not a dir or writible");
    $uploadOk = 0;
}

if ($uploadOk == 0)
{
    fwrite(STDERR, "Sorry, your file was not uploaded.");
    echo "Sorry, your file was not uploaded.";
}
else
{
    echo $_FILES['file']['name']."<br />";
    if(move_uploaded_file($_FILES["file"]["tml_name"], $target_file))
    {
        echo "The file ". basename($_FILES["file"]["tml_name"], $target_file). " has been uploaded.";
    }
    else
    {
        echo "Sorry, there was an error uploading your file.";
    }
}

echo "FIN";
?>
