

  /*INSERT INTO [User] (UserId,UserName) VALUES ('101','Yassine Benomar')
  select top 1 * from [Raw] order by timestamp 

 
INSERT INTO [User] (UserId,UserName) VALUES ('102','Frédéric Drouhin')
INSERT INTO [User] (UserId,UserName) VALUES (103,'Eric Conte')
INSERT INTO [User] (UserId,UserName) VALUES (104,'Pranamika Solanki')
INSERT INTO [User] (UserId,UserName) VALUES (105,'Audrey')*/


select * from [User]


/*INSERT INTO [Location] (Locationid,Campus,Bâtiment,Etage,Salle) values (10,'ManoMano','M05',4,'E101')
INSERT INTO [Location] (Locationid,Campus,Bâtiment,Etage,Salle) values (11,'Sotel','S03',11,'C06')
INSERT INTO [Location] (Locationid,Campus,Bâtiment,Etage,Salle) values (12,'Rakuten','R08',15,'H110')
INSERT INTO [Location] (Locationid,Campus,Bâtiment,Etage,Salle) values (13,'Voltcraft','V01',4,'A055')
INSERT INTO [Location] (Locationid,Campus,Bâtiment,Etage,Salle) values (14,'Zaeel','Z09',5,'I020')*/

select * from [Location]

select distinct timestamp
into #outlet
from Raw

select * from #outlet

select distinct [timestamp] 
into outlet1
from Raw

select * from outlet1


select * from Location
INSERT INTO [Location](Locationid,Campus,Bâtiment,Etage,Salle) values (13,'Voltcraft','V01',4,'A055')





select * from Outlet where outletid is null order by timestamp desc

update outlet
set outletid = 1, outletname ='ManoMano',locationid = 10, serialnumber = 0001
where timestamp between '2018-03-21 00:01:00.0000000' and '2018-03-30 00:00:00.0000000'

update outlet
set outletid = 4, outletname ='Voltcraft',locationid = 13, serialnumber = 0004
where timestamp between '2018-04-29 00:01:00.0000000' and '2018-05-29 08:24:00.0000000'

select * from location
select * from outlet

insert into Acquisition1 (AcquisitionTimestamp,DbimportTimestamp)
select distinct timestamp as t1, timestamp as t2
from raw
select * from acquisition1



update acquisition1
set Username = 'Yassine Benomar',ValidityTag = 0, Comment = 'Not Valid'
where AcquisitionId between 62644 and 75117

update acquisition1
set Username = 'Pranamika Solanki',ValidityTag = 1, Comment = 'Valid'
where AcquisitionId between 75118 and 87591

update acquisition1
set Username = 'Frédéric Drouhin',ValidityTag = 0, Comment = 'Not Valid'
where AcquisitionId between 87592 and 100065

update acquisition1
set Username = 'Eric Conte',ValidityTag = 1, Comment = 'Valid'
where AcquisitionId between 100066 and 112539

update acquisition1
set Username = 'Audrey',ValidityTag = 0, Comment = 'Not Valid'
where AcquisitionId between 112540 and 127471

select * from Acquisition where Username is null 
select * from Location 
select * from [User]
select * from Outlet
select * from Acquisition


select R.timestamp as [Timestamp],R.Voltage as Voltage, R.[Current] as [Current],R.[power_factor] as [PowerFactor],
A.AcquisitionId as AcquisitionId,O.OutletId as OutletId,O.LocationId as LocationId,U.UserId as UserId
into Data1
from Raw as R Join Outlet as O on R.timestamp = O.timestamp
Join Acquisition as A on R.timestamp = A.AcquisitionTimestamp
join [User] as U on A.Username = U.Username

select * from Data


