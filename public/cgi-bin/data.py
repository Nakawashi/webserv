#!/usr/bin/python
import sys
print ("<!DOCTYPE html>")
print ("<html>")
print ("<head>")
print ("<meta charset=\"UTF-8\">")
print ("<link rel="'stylesheet'" href="'/public/css/main.css'">")
print ("<title>Données transmises</title>")
print ("</head>")
print ("<body>")
print ("<h1>Les données suivantes ont été transmises :</h1>")
print ("<ul>")
def append_to_file(filename, data):
    with open(filename, 'a') as f:
        f.write(data)
if __name__ == "__main__":
    for arg in sys.argv[1:]:
        append_to_file('data.txt', arg)
        print("<li>")
        print(arg)
        print("</li>")
print ("</ul>")
print ("data")
print ("sent")
print ("<p>Retour à l'accueil : <a href="'/public/index.html'">accueil</a></p>")
print ("</body>")
print ("</html>")