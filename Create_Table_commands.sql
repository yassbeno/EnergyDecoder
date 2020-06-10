create table [User](
UserId int, UserName varchar(max),
Primary Key(UserId)
)

create table Location(
LocationId Int,
Bâtiment varchar(25),
Campus varchar(25),
Etage int,
Salle varchar(10),
Primary Key(LocationId)
)

create table Outlet(
OutletId int,
SerialNumber int,
OutletName varchar(25),

Primary Key(OutletId)
)
select * from Outlet


create table Acquisition1(
AcquisitionId int identity(1,1),
Username Varchar(255),
ValidityTag Binary,
Comment varchar(255),
AcquisitionTimestamp datetime,
DbimportTimestamp datetime,


)
