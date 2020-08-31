import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd, multilookup

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

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    portfolio_total = 0
    # sum all transactions by user
    stock_list = db.execute("SELECT stock_symbol AS symbol, SUM(number_of_shares) AS shares FROM transactions WHERE user_id = :user_id GROUP BY stock_symbol HAVING shares > 0", user_id=session["user_id"])
    if stock_list:
        # add the current stock price for each symbol to grouped list
        # get a list of just the owned stock symbols
        symbol_list = [stock['symbol'] for stock in stock_list]
        # get stock data for the symbols
        lookup_data = multilookup(symbol_list)        
        # Calculate value of each symbol
        for i, stock in enumerate(stock_list):
            stock_lookup = lookup_data[stock['symbol']]
            #Expand the stock list with lookup data
            stock_list[i]['price'] = stock_lookup['price']
            stock_list[i]['name'] = stock_lookup['name']
            symbol_value = stock_lookup['price'] * stock['shares']
            stock_list[i]['value'] = symbol_value
            portfolio_total += symbol_value
    # get current cash balance
    cash = db.execute("SELECT users.cash from users where users.id = :id", id=session["user_id"])[0]['cash']
    portfolio_total += cash
    return render_template("index.html", stock_list=stock_list, cash=cash, usd=usd, portfolio_total=portfolio_total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "GET":
        return render_template("buy.html")
    else:
        # check that symbol is not blank
        if not (symbol := request.form.get("symbol")):
            return apology("Please enter a stock symbol", code=403)
        # check that shares is not blank
        if not (number_of_shares := request.form.get("shares")):
            return apology("Please enter a positive number of shares", code=403)
        # check share is integer
        try:
            number_of_shares = int(number_of_shares)
            # check share is greater than 0
            if number_of_shares < 1:
                return apology("Please enter a positive number of shares", code=403)
        except ValueError:
            return apology("Please enter a positive number of shares", code=403)
        # check affordability
        # get current cash balance
        cash = db.execute("SELECT users.cash FROM users WHERE users.id = :id",
                          id=session["user_id"])[0]["cash"]
        # get share price and total cost
        if not (quote := lookup(symbol)):
            return apology("Please use a valid stock symbol", code=403)
        cost_per_share = quote["price"]
        total_cost = cost_per_share * number_of_shares
        if total_cost > cash:
            return apology("Can't afford this purchase", code=403)
        # update users table with new cash balance
        db.execute("UPDATE users SET cash = (cash - :total_cost) WHERE users.id = :id", total_cost=total_cost, id=session["user_id"])
        # update transactions table with user_id, stock_symbol,
        # number_of_shares, cost_per_share
        db.execute("INSERT INTO transactions (user_id, stock_symbol, number_of_shares, cost_per_share) VALUES (:user_id, :stock_symbol, :number_of_shares, :cost_per_share)", user_id=session["user_id"], stock_symbol=quote["symbol"], number_of_shares=number_of_shares, cost_per_share=cost_per_share)

        flash('You bought some shares!')
        return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    # get all user transactions from db
    transactions = db.execute("""SELECT stock_symbol AS symbol,
        number_of_shares AS shares,
        cost_per_share AS price,
        strftime('%Y-%m-%d %H:%M:%S', timestamp) AS time
        FROM transactions WHERE user_id = :user_id ORDER BY time DESC""", user_id=session["user_id"])
    return render_template("history.html", transactions=transactions, usd=usd)


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

        # Query database for username - add no case
        rows = db.execute("SELECT * FROM users WHERE username = :username COLLATE NOCASE",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]
        session["user_name"] = rows[0]["username"]

        flash('Login Successful')
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
    if request.method == "GET":
        return render_template("quote.html")
    else:
        # get symbol from form
        symbol = request.form.get("symbol")
        # check symbol was provided
        if not symbol:
            return apology("Please insert a symbol to get a quote", code=403)
        symbol_info = lookup(symbol)
        # check symbol is valid
        if not symbol_info:
            return apology("Please use a valid stock symbol", code=403)
        return render_template("quoted.html", symbol_info=symbol_info)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "GET":
        return render_template("register.html")
    else:
        ## check if username is blank
        username = request.form.get("username")
        if not username:
            ## message you must supply a username
            return apology("must provide username", 403)

        # Check that password and confirmation are provided
        password = request.form.get("password")
        confirm_password = request.form.get("confirmation")
        if not password or not confirm_password:
            return apology("You must supply a password and confirmation", 403)
        # check passwords match
        if str(password) != str(confirm_password):
            return apology("Passwords must match", 403)
        
        # Check if username already exists in db - case unsensitive
        rows = db.execute("SELECT * FROM users WHERE username = :username COLLATE NOCASE" , username=username)
        if len(rows) != 0:
            return apology("Username already taken", 403)

        # add user and hashed password to database and redirect to login page
        db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash);", username=username, hash=generate_password_hash(password))
        # Auto login on successful resistration
        # Query database for user_id
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=username)
        session["user_id"] = rows[0]["id"]
        session["user_name"] = rows[0]["username"]
        
        flash('Registration Successful, you are now logged in!')
        return redirect("/")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == 'GET':
        # get list of shares owned by user
        stock_list = db.execute("SELECT symbol FROM (SELECT stock_symbol AS symbol, SUM(number_of_shares) AS shares FROM transactions WHERE user_id = :user_id GROUP BY stock_symbol HAVING shares > 0)", user_id=session["user_id"])
        return render_template("sell.html", stock_list=stock_list)
    else:
        # check that symbol is not blank
        if not (symbol := request.form.get("symbol")):
            return apology("Please enter a stock symbol", code=403)
        # check that shares is not blank
        if not (shares_to_sell := request.form.get("shares")):
            return apology("Please enter a positive number of shares", code=403)
        # check share is integer
        try:
            shares_to_sell = int(shares_to_sell)
            # check share is greater than 0
            if shares_to_sell < 1:
                return apology("Please enter a positive number of shares", code=403)
        except ValueError:
            return apology("Please enter a positive number of shares", code=403)
        # get owned amount of requested symbol
        owned_stock = db.execute("SELECT stock_symbol AS symbol, SUM(number_of_shares) AS shares FROM transactions WHERE user_id = :user_id AND transactions.stock_symbol = :symbol COLLATE NOCASE GROUP BY symbol HAVING shares > 0", user_id=session["user_id"], symbol=symbol)
        # check its valid
        if not owned_stock:
            return apology("You don't own this stock", code=403)
        # check number to sell is currently owned
        if shares_to_sell > owned_stock[0]['shares']:
            return apology("You dont own that many shares!", code=403)
        # check current share price
        current_lookup = lookup(symbol)
        current_price = current_lookup['price']
        sale_value = current_price * shares_to_sell
        # update cash balance
        db.execute("UPDATE users SET cash = (cash + :sale_value) WHERE users.id = :id", sale_value=sale_value, id=session["user_id"])
        # update transaction 
        db.execute("INSERT INTO transactions (user_id, stock_symbol, number_of_shares, cost_per_share) VALUES (:user_id, :stock_symbol, :number_of_shares, :cost_per_share)", user_id=session["user_id"], stock_symbol=current_lookup["symbol"], number_of_shares=(shares_to_sell * -1), cost_per_share=current_price)
        flash('You sold some shares!')
        return redirect("/")

@app.route("/account", methods=["GET"])
@login_required
def account():
    """Account Page"""
    if request.method == 'GET':
        return render_template("account.html")


@app.route("/change_username", methods=["GET", "POST"])
@login_required
def change_username():
    """Change Username"""
    if request.method == 'GET':
        return redirect("/account")
    else:
        ## check if username is blank
        new_username = request.form.get("new_username")
        if not new_username:
            ## message you must supply a username
            return apology("must provide username", 403)
        # Check if username already exists in db - case unsensitive
        rows = db.execute("SELECT * FROM users WHERE username = :username COLLATE NOCASE" , username=new_username)
        if len(rows) != 0:
            return apology("Username already taken", 403)
        ## update username in db and sesston ID
        db.execute("UPDATE users SET username = :new_username WHERE users.id = :id", new_username=new_username, id=session["user_id"])
        session["user_name"] = new_username
        flash('Username updated!')
        return redirect("/account")


@app.route("/change_password", methods=["GET", "POST"])
@login_required
def change_password():
    """Change Password"""
    if request.method == 'GET':
        return redirect("/account")
    else:
        # Check that all password fields are provided
        current_password = request.form.get("password")
        new_password = request.form.get("new_password")
        confirm_new_password = request.form.get("new_confirmation")
        if not current_password or not new_password or not confirm_new_password:
            return apology("You must supply the current, new and confirmation passwords", 403)

        # check new passwords match
        if str(new_password) != str(confirm_new_password):
            return apology("New passwords must match", 403)
        
        # Check current password is correct
        rows = db.execute("SELECT * FROM users WHERE id = :id" , id=session["user_id"])[0]
        if not check_password_hash(rows["hash"], current_password):
            return apology("Incorrect password", 403)
        
        # TODO
        # enforce password strength

        # update database with new hashed password
        db.execute("UPDATE users SET hash = :new_password_hash WHERE users.id = :id", new_password_hash=generate_password_hash(new_password), id=session["user_id"])
        flash('Password updated!')
        return redirect("/account")

@app.route("/cash", methods=["GET"])
@login_required
def cash():
    """Load Cash page"""
    if request.method == 'GET':
        cash = db.execute("SELECT users.cash FROM users WHERE users.id = :id",
                          id=session["user_id"])[0]["cash"]
        return render_template("cash.html", cash=usd(cash))

@app.route("/cash_deposit", methods=["GET", "POST"])
@login_required
def cash_deposit():
    """Change Password"""
    if request.method == 'GET':
        return redirect("/cash")
    else:
        ## check if deposit is blank
        deposit = request.form.get("deposit")
        if not deposit:
            return apology("must provide deposit amount", 403)
        try:
            deposit = float(deposit)
            # check deposit is greater than 0
            if deposit < 1:
                return apology("Deposit must be a positive number", code=403)
        except ValueError:
            return apology("Deposit must be a positive number", code=403)
        
        # Add deposit to cash in db
        db.execute("UPDATE users SET cash = (cash + :deposit) WHERE users.id = :id", deposit=deposit, id=session["user_id"])
        flash('Cash deposited!')
        return redirect("/cash")

@app.route("/cash_withdrawal", methods=["GET", "POST"])
@login_required
def cash_withdrawal():
    """Change Password"""
    if request.method == 'GET':
        return redirect("/cash")
    else:
        ## check if Withdrawal is blank
        withdrawal = request.form.get("withdrawal")
        if not withdrawal:
            return apology("must provide withdrawal amount", 403)
        try:
            withdrawal = float(withdrawal)
            # check Withdrawal is greater than 0
            if withdrawal < 1:
                return apology("Withdrawal must be a positive number", code=403)
        except ValueError:
            return apology("Withdrawal must be a positive number", code=403)
        # check there is enough cash for withdrawal
        cash = db.execute("SELECT users.cash FROM users WHERE users.id = :id",
                          id=session["user_id"])[0]["cash"]
        if withdrawal > cash:
            return apology("Not enough cash", code=403)

        # Add withdraw to cash in db
        db.execute("UPDATE users SET cash = (cash - :withdrawal) WHERE users.id = :id", withdrawal=withdrawal, id=session["user_id"])
        flash('Cash withdrawn!')
        return redirect("/cash")

def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
