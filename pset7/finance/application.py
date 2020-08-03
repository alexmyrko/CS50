import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from datetime import datetime
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    rows = db.execute("SELECT SUM(quotes.shares), symbol FROM quotes WHERE id = :id GROUP BY symbol", id = session["user_id"])
    deposit = 0
    for row in rows:
        name = lookup(row["symbol"])['name']
        price = lookup(row["symbol"])['price']
        shares = row["SUM(quotes.shares)"]
        row.update({'name' : name} )
        row.update( {'price' : price } )
        row.update( {'sum' : price*shares} )
        deposit = deposit + price*shares
    usercash = db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"])
    cash = usercash[0]["cash"]
    total = deposit + cash

    return render_template("index.html", rows = rows, cash = cash, deposit = deposit, total = total)
#    return apology("TODO")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("must provide symbol", 403)
        elif not request.form.get("shares"):
            return apology("must provide shares", 403)
        shares = request.form.get("shares")

        if not shares.isdigit():
            return apology("not a number", 403)
        elif not int(shares) > 0:
            return apology("must provide positive number", 403)

        symbol = request.form.get("symbol")

        if lookup(symbol) != None:
            price = lookup(symbol)['price']
        else:
            return apology("must provide symbol", 403)

        total = price * int(shares)
        rest = db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"])
        import datetime
        now = datetime.datetime.utcnow()
        datetime = now.strftime('%Y-%m-%d %H:%M:%S')

        if float(price)*float(shares) <= rest[0]["cash"]:
            db.execute("INSERT INTO history (id, symbol, price, shares, datetime) VALUES (:id, :symbol, :price, :shares, :datetime)", id = session["user_id"],
            symbol = symbol, price = total, shares = shares, datetime = datetime)
            db.execute("INSERT INTO operations (datetime, shares) VALUES (:datetime, :shares)", datetime = datetime, shares = int(shares))
            db.execute("UPDATE users SET cash = :cash WHERE id = :id", cash = rest[0]["cash"] - float(price)*float(shares), id = session["user_id"])
            count = db.execute("SELECT shares FROM quotes WHERE id = :id AND symbol = :symbol", id = session["user_id"], symbol = symbol)

            if len(count) != 0:
                db.execute("UPDATE quotes SET shares = :shares WHERE id = :id AND symbol = :symbol", id = session["user_id"], symbol = symbol,
                shares = int(shares)+count[0]["shares"])
            else:
                db.execute("INSERT INTO quotes (id, symbol, shares) VALUES (:id, :symbol, :shares)", id = session["user_id"], symbol = symbol, shares = shares)

        else:
            return render_template("test.html", message = "not enough money")
        return redirect("/")

    else:
        return render_template("buy.html")



@app.route("/check", methods=["GET"])
def check():
    """Return true if username available, else false, in JSON format"""
    username = request.args.get("username")
    if not username:
        return render_template("check.html", message = "NOT")

    else:
        rows = db.execute("SELECT username FROM users")
        names = []
        for row in rows:
            names.append(row["username"])

        if username in names:
            return jsonify(True)
        else:
            return jsonify(False)


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    rows = db.execute("SELECT history.id, history.symbol, operations.shares, history.price, history.datetime FROM history JOIN operations ON history.datetime = operations.datetime")
    return render_template("history.html", rows = rows)
    return apology("TODO")


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""
    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":

        if not request.form.get("symbol"):
            return render_template("test.html", message = "Empty field for Quote")
        symbol = request.form.get("symbol")

        return render_template("quoted.html", name = lookup(symbol)['name'], price = lookup(symbol)['price'], symbol = lookup(symbol)['symbol'])

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    session.clear()

    if request.method == "POST":

        if not request.form.get("username"):
            return apology("must provide username", 403)
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        user = request.form.get("username")
        rows = db.execute("SELECT * FROM users WHERE username = :user",
                            user=request.form.get("username"))
        if len(rows) !=0:
            return apology("username exists", 403)

        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        hashpass = generate_password_hash(password, method='pbkdf2:sha256', salt_length=8)
        if password == confirmation:
            db.execute("INSERT INTO users (username, hash) VALUES (:user, :hashpass)", user=user, hashpass = hashpass)
        else:
            return render_template("test.html", message="password and confirmation are not the same")
        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("Must provide SYMBOL")


        symbol = request.form.get("symbol")
        shares = db.execute("SELECT SUM(quotes.shares) FROM quotes WHERE id = :id AND symbol = :symbol", id = session["user_id"], symbol = symbol)

        count = shares[0]["SUM(quotes.shares)"]

        if not request.form.get("shares"):
            return apology("Must provide number of shares")

        tosell = request.form.get("shares")
        if not tosell.isdigit():
            return apology("not a number", 403)
        if int(tosell) <=0 or int(tosell) > count :
            return apology("Provide correct number of shares")

        import datetime
        now = datetime.datetime.utcnow()
        datetime = now.strftime('%Y-%m-%d %H:%M:%S')
        price = int(tosell)*lookup(symbol)['price']
        db.execute("INSERT INTO history (id, symbol, shares, price, datetime) VALUES (:id, :symbol, :shares, :price, :datetime)",
        id = session["user_id"], symbol = symbol, shares = int(tosell), price = price, datetime = datetime)
        db.execute("INSERT INTO operations (datetime, shares) VALUES (:datetime, :shares)", datetime = datetime, shares = -int(tosell))
        if count > int(tosell):
            db.execute("UPDATE quotes SET shares = :shares WHERE id = :id AND symbol = :symbol", shares = count-int(tosell), id = session["user_id"],
            symbol = symbol)
        else:
            db.execute("DELETE FROM quotes WHERE id = :id AND symbol = :symbol", id = session["user_id"], symbol = symbol)
        rows = db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"])
        cash = rows[0]["cash"]
        db.execute("UPDATE users SET cash = :cash WHERE id = :id", id = session["user_id"], cash = cash + price)

        return redirect ("/")

    else:
        symbols = db.execute("SELECT symbol FROM quotes WHERE id = :id", id = session["user_id"])
        return render_template("sell.html", rows = symbols)

def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
