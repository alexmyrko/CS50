import cs50
import csv

from flask import Flask, jsonify, redirect, render_template, request

# Configure application
app = Flask(__name__)

# Reload templates when they are changed
app.config["TEMPLATES_AUTO_RELOAD"] = True


@app.after_request
def after_request(response):
    """Disable caching"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET"])
def get_index():
    return redirect("/form")


@app.route("/form", methods=["GET"])
def get_form():
    return render_template("form.html")


@app.route("/form", methods=["POST"])
def post_form():
    roads = request.form.get("roads")
    quality = request.form.get("quality")
    descr = request.form.get("description")
    wind = request.form.get("wind")
    email = request.form.get("email")

    if not roads or not quality or not descr or not wind or not email:
        return render_template("error.html", message="Please fill out all fields !")

    with open("result.csv", "a") as file:
        writer = csv.DictWriter(file, fieldnames=["roads", "quality", "description", "wind", "email"])
        writer.writerow({"roads": roads, "quality": quality, "description": descr, "wind": wind, "email": email })
    return redirect("/sheet")
#    return render_template("success.html", message="MADE")


@app.route("/sheet", methods=["GET"])
def get_sheet():
    with open("result.csv", "r") as file:
        reader = csv.DictReader(file)
        conditions = list(reader)
    return render_template("sheet.html", conditions=conditions)
