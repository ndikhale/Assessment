create database companyassessment;
use companyassessment;

-- make account_number as primary key
alter table account_info
add primary key (account_number);

-- adding foreign key in transactions table
ALTER TABLE transactions
ADD FOREIGN KEY (account_number) REFERENCES account_info(account_number);

-- add new column st in transactions table for storing the state mentioned in merchant_description
alter table transactions
add column st text;

-- update column st with the respective state mentioned in merchant_description in transactions table
update transactions t
inner join
account_info a
on a.account_number = t.account_number
set st = substring(substring_index(trim(t.merchant_description), ' ',-1), 1, 2);