import sys

print("<!DOCTYPE html>")
print("<html lang=\"en\">")
print("<head>")
print("    <meta charset=\"UTF-8\">")
print("    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">")
print("    <title>Home Page</title>")
print("    <link rel=\"stylesheet\" href=\"style0.css\">")
print("</head>")
print("<body>")
print("    <header>")
print("        <h1>Welcome to My Homepage</h1>")
print("        <nav>")
print("            <ul>")
print("                <li><a href=\"home.html\">Home</a></li>")
print("                <li><a href=\"about.html\">About</a></li>")
print("                <li><a href=\"service.html\">Services</a></li>")
print("                <li><a href=\"contact.html\">Contact</a></li>")
print("            </ul>")
print("        </nav>")
print("    </header>")


print("    <section>")
print("		<h1>Cette page est issue du script python </h1>")
print("     <p>")       
print(sys.argv)       
print("     </p>")       
print("    </section>")


print("	<footer>")
print("		<p>2023 My Website - All Rights Reserved</p>")
print("		<p>Contact us: contact@example.com</p>")
print("	</footer>")
	
print("</body>")
print("</html>")