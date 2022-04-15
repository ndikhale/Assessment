#include <stdio.h>
#include <iostream>
#include <mysql.h>
#include <string.h>

using namespace std;
int queryState;

int main() {

	MYSQL* conn;
	MYSQL_ROW row;
	MYSQL_RES* res;
	conn = mysql_init(0);

	conn = mysql_real_connect(conn, "localhost", "root", "", "companyassessment", 3306, NULL, 0);

	if (conn) {
		cout << "Successfull connection to the database." << endl;

		cout << "Rule 1 Result:" << endl;

		std::string queryRule1 = 
			"SELECT "
			"CONCAT(a.first_name, ' ', a.last_name) AS Name, "
			"t.account_number, "
			"t.transaction_number AS 'Transaction Number', "
			"merchant_description AS 'Merchant', "
			"ABS(transaction_amt) AS 'Transaction_Amount' "
			"FROM "
			"transactions t "
			"INNER JOIN "
			"account_info a ON a.account_number = t.account_number "
			"WHERE "
			"(t.account_number, t.merchant_number, "
				"ABS(t.transaction_amt)) IN(SELECT "
					"t1.account_number, "
					"t1.merchant_number, "
					"MAX(-t1.transaction_amt) "
					"FROM "
					"transactions t1 "
					"GROUP BY t1.account_number, t1.merchant_number "
					"HAVING((STDDEV(-t1.transaction_amt)) <> 0) "
					"&& (ABS((ABS(STDDEV(-t1.transaction_amt))) - ABS(MAX(-t1.transaction_amt))) > 500)) "
			"GROUP BY t.account_number, t.merchant_number;";

		const char* rule1Query = queryRule1.c_str();
		queryState = mysql_query(conn, rule1Query);

		if (!queryState) {
			res = mysql_store_result(conn);
			cout << "Name \t\t Account Number \t Transaction Number \t Merchant \t\t\t\t\t Transaction Amount \n";
			while (row = mysql_fetch_row(res)) {
				cout << row[0] << "\t\t" << row[1] << "\t\t\t" << row[2] << "\t\t" << row[3] << "\t\t" << row[4] << endl;
			}
		}
		else {
			cout << "Execution of Query failed: " << mysql_error(conn) << endl;
		}

		cout << endl;

		cout << "Rule 2 Result:" << endl;

		std::string queryRule2 =
			"SELECT DISTINCT "
				"a.account_number, "
				"CONCAT(a.first_name, ' ', a.last_name) AS Name, "
				"t.transaction_number AS 'Transaction Number', "
				"a.state AS 'Expected Transaction Location', "
				"t.st "
			"FROM "
				"account_info a "
					"INNER JOIN "
				"transactions t ON a.account_number = t.account_number "
			"WHERE "
				"t.st <> a.state;";

		const char* rule2Query = queryRule2.c_str();
		queryState = mysql_query(conn, rule2Query);

		if (!queryState) {
			res = mysql_store_result(conn);
			cout << "Account Number \t Name \t\t Transaction Number \t Expected Transaction Location \t Actual Transaction Location \n";
			while (row = mysql_fetch_row(res)) {
				cout << row[0] << "\t\t" << row[1] << "\t\t" << row[2] << "\t\t\t" << row[3] << "\t\t\t\t" << row[4] << endl;
			}
		}
		else {
			cout << "Execution of Query failed: " << mysql_error(conn) << endl;
		}
	}
	else {
		cout << "Connection to database failed.";
	}

	return 0;
}