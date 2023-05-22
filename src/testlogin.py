from flask import Flask, render_template, redirect, request, session
# The Session instance is not used for direct access, you should always use flask.session
from flask_session import Session

app = Flask(__name__)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)


@app.route("/pbx")
def index():
	if not session.get("username"):
		return redirect("/login")
	return render_template('pbx.html')


@app.route("/login", methods=["POST", "GET"])
def login():
	if request.method == "POST":
		session["username"] = request.form.get("username")
		return redirect("/pbx")
	return render_template("login.html")


@app.route("/logout")
def logout():
	session["username"] = None
	return redirect("/")


if __name__ == "__main__":
	app.run(debug=True, host="0.0.0.0")
