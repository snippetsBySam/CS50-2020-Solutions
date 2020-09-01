# CS50 Finance

The following personal touches were added:
* An Account page that allows you to change your username and password.
* A Cash page that allows the depositing and withdrawal of funds.
* A 'multi-lookup' function was added to helpers.py which speeds up lookups by sending a single request for multiple symbol lookups instead  of one request per symbol. Only affects the index page.
* Messages that appear each time an action completes (buying/selling stock, updating username/password, etc.).

## Transactions table
The syntax used to add the transactions database table and indexes are below.

### Create Table

```sql
CREATE TABLE IF NOT EXISTS 'transactions' ('transaction_id' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
	'user_id' INTEGER NOT NULL REFERENCES users(id),
	'stock_symbol' TEXT NOT NULL,
	'number_of_shares' INTEGER NOT NULL,
	'cost_per_share' NUMERIC NOT NULL,
	'timestamp' DATETIME NOT NULL DEFAULT (strftime('%Y-%m-%d %H:%M:%f')));
```

### Creating indices

```sql
CREATE INDEX 'user_id' ON "transactions" ("user_id");
```

```sql
CREATE INDEX 'stock_symbol' ON "transactions" ("stock_symbol");
```

```sql
CREATE INDEX "history" ON "transactions" (
	"user_id",
	"stock_symbol"
);
```
